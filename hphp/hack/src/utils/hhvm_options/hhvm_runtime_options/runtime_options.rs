// Copyright (c) Facebook, Inc. and its affiliates. All Rights Reserved.

use std::fmt;
use std::fs;

use anyhow::Result;
use byteorder::ByteOrder;
use byteorder::NetworkEndian;
use colored::Colorize;
use md5::Digest;
use md5::Md5;
use regex::Regex;

use crate::cxx_ffi;

/// A config.hdf rule that matched an input
// Temporarily allow this to compile, we're actually logging this for now.
#[allow(dead_code)]
#[derive(Debug)]
pub struct TracedRule {
    // Name of the overwrite rule that matched
    rule_name: String,
    // Name of the property we're tracing
    traced_property: String,
    // The body of the rule that matched
    rule_body: hdf::Value,
    // TODO: We also want to store what property/rule matched what input potentially?
}

impl fmt::Display for TracedRule {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(
            f,
            "\n{}: {},\n{}:\n{}",
            "Rule".magenta(),
            self.rule_name.yellow().bold(),
            "Body".purple(),
            if self.rule_name == "Root" {
                "Root {\n ... \n}".to_string()
            } else {
                format!("{:?}", self.rule_body)
            }
        )
    }
}

/// A machine can belong to a tier, which can overwrite
/// various settings, even if they are set in the same
/// hdf file. However, CLI overrides still win the day over
/// everything.
///
/// Based on getTierOverwrites() in runtime-option.cpp
pub fn apply_tier_overrides(config: hdf::Value) -> Result<hdf::Value> {
    // Machine metrics
    let hostname = config
        .get_str("Machine.name")?
        .unwrap_or_else(cxx_ffi::Process_GetHostName);

    let tier: String = config.get_str("Machine.tier")?.unwrap_or_default();
    let task: String = config.get_str("Machine.task")?.unwrap_or_default();

    let cpu: String = config
        .get_str("Machine.cpu")?
        .unwrap_or_else(cxx_ffi::Process_GetCPUModel);

    let tiers: String = config
        .get_str("Machine.tiers")?
        .and_then(|tiers| fs::read_to_string(tiers).ok())
        .unwrap_or_else(|| "".to_owned());

    let tags: String = config
        .get_str("Machine.tags")?
        .and_then(|tiers| fs::read_to_string(tiers).ok())
        .unwrap_or_else(|| "".to_owned());

    let results = apply_tier_overrides_with_params(
        config, &hostname, &tier, &task, &cpu, &tiers, &tags, None, None,
    )?;
    Ok(results.0)
}

pub fn apply_tier_overrides_with_params(
    mut config: hdf::Value,
    hostname: &String,
    tier: &String,
    task: &String,
    cpu: &String,
    tiers: &String,
    tags: &String,
    predefined_shard_value: Option<i64>,
    trace_property: Option<String>,
) -> Result<(hdf::Value, Vec<String>, Vec<TracedRule>)> {
    log::debug!(
        "Matching tiers using: machine='{}', tier='{}', task='{}', cpu='{}', tiers='{}', tags='{}'",
        hostname,
        tier,
        task,
        cpu,
        tiers,
        tags
    );
    let mut matched_tiers = vec![];

    // Setup property tracing state
    //let property_overrides: Vec<&hdf::Value> = vec![];

    let mut traced_rules: Vec<TracedRule> = vec![];
    if let Some(ref debug_prop_name) = trace_property {
        log::info!(
            "Property tracing enabled for property: {}",
            &debug_prop_name
        );
        if let Some(_config_body) = config.get(debug_prop_name)? {
            traced_rules.push(TracedRule {
                rule_name: "Root".to_string(),
                traced_property: debug_prop_name.clone(),
                rule_body: config.clone(),
            });
        }
    }

    let check_patterns = |hdf: &hdf::Value| -> Result<bool> {
        Ok(match_hdf_pattern(hostname, hdf, "machine", false)?
            && match_hdf_pattern(tier, hdf, "tier", false)?
            && match_hdf_pattern(task, hdf, "task", false)?
            && match_hdf_pattern(tiers, hdf, "tiers", true)?
            && match_hdf_pattern(tags, hdf, "tags", true)?
            && match_hdf_pattern_set(tags, hdf, "tagset")?
            && match_hdf_pattern(cpu, hdf, "cpu", false)?)
    };

    let mut enable_shards = true;

    if let Some(tiers) = config.get("Tiers")? {
        for tier_name in tiers.get_child_names()? {
            if let Some(tier) = tiers.get(&tier_name)? {
                if check_patterns(&tier)?
                    && (!tier.contains_key("exclude")?
                        || !tier
                            .get("exclude")?
                            .map_or(Ok(false), |v| check_patterns(&v))?)
                    && match_shard(enable_shards, hostname, &tier, predefined_shard_value)?
                {
                    log::info!("Matched tier: {}", tier.name()?);
                    matched_tiers.push(tier.name()?);

                    if enable_shards && tier.get_bool_or("DisableShards", false)? {
                        log::info!("Sharding is disabled.");
                        enable_shards = false;
                    }

                    if let Some(remove) = tier.get("clear")? {
                        for s in remove.values()? {
                            config.remove(&s)?;
                        }
                    }

                    if let Some(ref overwrite) = tier.get("overwrite")? {
                        log::info!(
                            "Applying overrides for tier: {}",
                            tier.name().unwrap_or_default()
                        );
                        config.copy(overwrite)?;

                        if let Some(ref debug_prop_name) = trace_property {
                            if overwrite.get(debug_prop_name)?.is_some() {
                                traced_rules.push(TracedRule {
                                    rule_name: tier.name()?,
                                    traced_property: debug_prop_name.clone(),
                                    rule_body: overwrite.clone(),
                                });
                                log::debug!(
                                    "Applied overrides from rule {:?}\n{:?}",
                                    overwrite.name()?,
                                    overwrite
                                );
                            }
                        }
                    } else {
                        log::info!(
                            "No overrides found for tier: {} {:?}",
                            tier.name().unwrap_or_default(),
                            tier,
                        );
                    }
                    // no break here, so we can continue to match more overwrites
                }

                // Avoid lint errors about unvisited nodes when the tier does not match.
                //-- tier["DisableShards"].setVisited();
                //-- tier["clear"].setVisited();
                //-- tier["overwrite"].setVisited();
            }
        }
    }
    Ok((config, matched_tiers, traced_rules))
}

fn match_shard(
    enable_shards: bool,
    hostname: &str,
    config: &hdf::Value,
    predefined_shard_value: Option<i64>,
) -> Result<bool> {
    if !enable_shards || !config.contains_key("Shard")? {
        return Ok(true);
    }
    let shard = config.get_int64_or("Shard", -1)?;
    let nshards = config.get_int64_or("ShardCount", 100)?;
    if shard < 0 || shard >= nshards {
        log::warn!(
            "Invalid shard number {}, must in the range [0..{})",
            shard,
            nshards
        );
        return Ok(true);
    }

    // This is a small variation from the standard logic to accommodate forced
    // evaluation of tier overrides for validation.  Reverse engineering
    // a md5 hash is ineffective for that purpose.
    if let Some(predefined_shard_value) = predefined_shard_value {
        return Ok(predefined_shard_value % nshards <= shard);
    }

    // This is close to the hash behavior in HHVM but not exact its a decent approximation however
    let mut input: String = hostname.to_string();
    if config.contains_key("ShardSalt")? {
        if let Some(salt) = config.get_str("ShardSalt")? {
            input.push_str(salt.as_str());
        }
    }
    let seed = create_seed(&input)?;
    log::info!(
        "Checking Shard = {shard}; input = {input}, seed = {seed}; ShardCount = {nshards}; Value = {}",
        seed % nshards
    );
    Ok(seed % nshards <= shard)
}

// This is to match the behavior of matchShard in runtime-option.cpp
fn create_seed(input: &str) -> Result<i64> {
    // This NetworkEndian and shift is to match the behavior of sharding in chef:
    //   seed = Digest::MD5.hexdigest(seed_input)[0...7].to_i(16)
    Ok((NetworkEndian::read_u32(&Md5::digest(input.as_bytes()).as_slice()[..4]) >> 4) as i64)
}

fn is_pattern_match(pattern: &String, value: &str, multiline: bool) -> Result<bool> {
    // In hhvm we would error withut a / prefix or suffix on the pattern
    // This is maintained here to ensure compatability with HHVM.
    let pattern = pattern
        .strip_prefix('/')
        .ok_or_else(|| anyhow::anyhow!("{pattern}: should have a / prefix"))?
        .strip_suffix('/')
        .ok_or_else(|| anyhow::anyhow!("{pattern}: should have a / suffix"))?;
    let re = if multiline {
        Regex::new(format!("(?m:{pattern})").as_str())?
    } else {
        Regex::new(pattern)?
    };
    Ok(re.is_match(value))
}

// Config::matchHdfPattern()
fn match_hdf_pattern(
    value: &str,
    config: &hdf::Value,
    name: &str,
    multiline: bool,
) -> Result<bool> {
    let pattern = config.get_str(name)?.unwrap_or_default();
    if !pattern.is_empty() {
        if !is_pattern_match(&pattern, value, multiline)? {
            return Ok(false);
        }
        log::info!("Matched {name} pattern: {pattern}");
    }
    Ok(true)
}

fn match_hdf_pattern_set(value: &str, config: &hdf::Value, name: &str) -> Result<bool> {
    let patterns = config.get_str_vec_or(name, vec![])?;
    for pattern in patterns {
        if !pattern.is_empty() {
            if !is_pattern_match(&pattern, value, true)? {
                return Ok(false);
            }
            log::info!("Matched {name} pattern: {pattern}");
        }
    }
    Ok(true)
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_match_shard() -> Result<()> {
        // From HHVM
        // Checking Shard = 4; Input = test_hostnamebespoke; Seed = 190005716; ShardCount = 100; Value = 16
        assert_eq!(create_seed("test_hostnamebespoke")?, 190005716);
        // From HHVM
        // Checking Shard = 0; Input = test_hostnamenoinline; Seed = 44676460; ShardCount = 1000; Value = 460
        assert_eq!(create_seed("test_hostnamenoinline")?, 44676460);

        Ok(())
    }

    #[test]
    fn test_get_seed() -> Result<()> {
        let hostname = "test_hostname";
        let enabled = true;
        let disabled = false;
        assert_eq!(create_seed(hostname)? % 100, 81);
        assert_eq!(create_seed("test_hostnamebespoke")? % 100, 16);

        let mut config = hdf::Value::default();

        // Config with no Shard should be true
        assert!(match_shard(enabled, hostname, &config, None)?);

        // With Shard

        // Invalid
        config.set_hdf("Shard = -1")?;
        assert!(match_shard(disabled, hostname, &config, None)?);
        config.set_hdf("Shard = 100")?;
        assert!(match_shard(disabled, hostname, &config, None)?);

        // Valid
        config.set_hdf("Shard = 80")?;
        assert!(match_shard(disabled, hostname, &config, None)?);
        assert!(!match_shard(enabled, hostname, &config, None)?);

        config.set_hdf("Shard = 81")?;
        assert!(match_shard(enabled, hostname, &config, None)?);

        // With salt
        config.set_hdf("Shard = 0\nShardSalt = bespoke")?;
        assert!(!match_shard(enabled, hostname, &config, None)?);
        config.set_hdf("Shard = 16")?;
        assert!(match_shard(enabled, hostname, &config, None)?);

        // With custom ShardCount and Salt
        config.set_hdf("Shard = 5\nShardCount = 10")?;
        assert!(!match_shard(enabled, hostname, &config, None)?);
        config.set_hdf("Shard = 6")?;
        assert!(match_shard(enabled, hostname, &config, None)?);

        // Precomputed shards cutoff 1 (non-prod behavior) and ShardCount 10
        config.set_hdf("Shard = 1")?;
        assert!(match_shard(enabled, hostname, &config, Some(0))?);
        assert!(match_shard(enabled, hostname, &config, Some(1))?);
        assert!(!match_shard(enabled, hostname, &config, Some(2))?);
        assert!(match_shard(enabled, hostname, &config, Some(11))?);

        Ok(())
    }

    #[test]
    fn test_match_hdf_pattern() -> Result<()> {
        let mut hdf = hdf::Value::default();
        // Selector pattern not in the config tree should return true
        assert!(match_hdf_pattern("", &hdf, "task", false)?);

        hdf.set_hdf("task = //")?;
        assert!(match_hdf_pattern("i/can/be/anything", &hdf, "task", false)?);

        hdf.set_hdf("task = /test\\/matches/")?;
        assert!(match_hdf_pattern("test/matches/true", &hdf, "task", false)?);
        assert!(!match_hdf_pattern("no/match/false", &hdf, "task", false)?);

        // Test multiline
        let tags = "hhvm\nmatch_me3\nfoo\nbaz";
        hdf.set_hdf("tags = /^match_me3$/")?;
        assert!(match_hdf_pattern(tags, &hdf, "tags", true)?);
        assert!(!match_hdf_pattern(tags, &hdf, "tags", false)?);

        Ok(())
    }

    #[test]
    fn test_match_hdf_bad_regex() -> Result<()> {
        let mut hdf = hdf::Value::default();

        let cases = [
            // HHVM specific requirements
            ("test\\/nobody/", "should have a / prefix"),
            ("/test\\/nobody", "should have a / suffix"),
            // Regex bad patterns
            ("/i_am(broken/", "unclosed group"),
            ("/i_am(broken\\)/", "unclosed group"),
            ("/i_am[broken/", "unclosed character class"),
            ("/i_am[broken\\]/", "unclosed character class"),
            ("/*/", "repetition operator missing expression"),
        ];

        for (pattern, err_substr) in cases.iter() {
            hdf.set_hdf(&format!("task = {pattern}"))?;
            let err_str = match_hdf_pattern("unused", &hdf, "task", false)
                .unwrap_err()
                .to_string();
            assert!(
                err_str.contains(err_substr),
                "Expected {err_str} to contain '{err_substr}' for pattern '{pattern}'",
            );
            let err_str = match_hdf_pattern("unused", &hdf, "task", true)
                .unwrap_err()
                .to_string();
            assert!(
                err_str.contains(err_substr),
                "Expected {err_str} to contain '{err_substr}' for pattern '{pattern}'",
            );
        }

        Ok(())
    }

    fn get_complex_hdf() -> hdf::Value {
        let hdf_raw_str = r#"
            Eval {
                strA = hello
                strB = world
                intA = 1
                intB = 2
            }

            Tiers {
                disable_shards {
                    tags = /no_shards/
                    DisableShards = true
                    overwrite { # don't blow up on empty overwrites
                    }
                }

                foo_tier {
                    tags = /^foo$/
                    clear {
                        * = Eval.strA
                    }
                    overwrite {
                        Eval.strB = foo
                    }
                }

                bar_tier {
                    task = /.*\/bar\/.*/
                    clear {
                        * = Eval.intA
                    }
                    overwrite {
                        Eval.strB = bar
                    }
                }

                baz_tier {
                    tags = /^baz$/
                    Shard = 5
                    overwrite {
                        Eval.strA = shard
                    }
                }

                foo_baz_tier {
                    tagset {
                        * = /^foo$/
                        * = /^baz$/
                    }
                    clear {
                        * = Eval.intA
                        * = Eval.intB
                    }
                    overwrite {
                        Eval.strA = foo
                        Eval.strB = baz
                    }
                }

            }
        "#;

        let mut hdf = hdf::Value::default();
        hdf.set_hdf(hdf_raw_str).unwrap();
        hdf
    }

    #[test]
    fn test_into_children_loop() -> Result<()> {
        let hdf = get_complex_hdf();
        let hdf2 = get_complex_hdf();

        // There currently is a bug with this unknown why so we
        // end up using names list to traverse the children nodes
        // Keeping this here to figure out the issue as other usages of
        // into_children may be vulnerable to this when using accessors
        // on the child.
        let tiers = hdf.get("Tiers")?.unwrap();
        for tier in tiers.into_children()? {
            let tier = tier?;
            // This is the buggy part where non-extant keys but directly accessed are fine
            // For some reason this mutates the tier node and sets the key while direct
            // access doesn't.
            assert!(tier.contains_key("i_dont_exist")?);
            assert!(tier.get("i_dont_exist")?.is_some());

            let tier2 = hdf2.get(&format!("Tiers.{}", tier.name()?))?.unwrap();
            assert!(!tier2.contains_key("i_dont_exist")?);
            assert!(tier2.get("i_dont_exist")?.is_none());
            assert!(!tier2.contains_key("i_dont_exist")?);
        }

        Ok(())
    }

    fn check_tier_overrides(
        task: Option<String>,
        tags: Option<String>,
        str_a: Option<String>,
        str_b: Option<String>,
        int_a: Option<u32>,
        int_b: Option<u32>,
        shard: Option<i64>,
        expected_matches: Vec<String>,
    ) -> Result<()> {
        let (hdf, matched_tiers, _) = apply_tier_overrides_with_params(
            get_complex_hdf(),
            &"hostname".into(),
            &"tier".into(),
            &task.unwrap_or("task".into()),
            &"cpu".into(),
            &"tiers".into(),
            &tags.unwrap_or("tags".into()),
            shard,
            None,
        )?;
        assert_eq!(hdf.get_str("Eval.strA")?, str_a);
        assert_eq!(hdf.get_str("Eval.strB")?, str_b);
        assert_eq!(hdf.get_uint32("Eval.intA")?, int_a);
        assert_eq!(hdf.get_uint32("Eval.intB")?, int_b);
        assert_eq!(matched_tiers, expected_matches);

        Ok(())
    }

    #[test]
    fn test_apply_tier_overrides() -> Result<()> {
        // No overrides
        check_tier_overrides(
            None,
            None,
            Some("hello".into()),
            Some("world".into()),
            Some(1),
            Some(2),
            None,
            vec![],
        )?;

        // Match on foo_tier
        // No overrides
        check_tier_overrides(
            None,
            Some("foo".into()),
            None,
            Some("foo".into()),
            Some(1),
            Some(2),
            None,
            vec!["foo_tier".into()],
        )?;

        // Match on bar_tier
        check_tier_overrides(
            Some("my/bar/task".into()),
            None,
            Some("hello".into()),
            Some("bar".into()),
            None,
            Some(2),
            None,
            vec!["bar_tier".into()],
        )?;

        // Match on baz in shard
        check_tier_overrides(
            None,
            Some("baz".into()),
            Some("shard".into()),
            Some("world".into()),
            Some(1),
            Some(2),
            Some(5),
            vec!["baz_tier".into()],
        )?;

        // Match on baz in not in shard
        check_tier_overrides(
            None,
            Some("baz".into()),
            Some("hello".into()),
            Some("world".into()),
            Some(1),
            Some(2),
            Some(6),
            vec![],
        )?;

        // Match on no_shard and baz in not in shard but applied
        check_tier_overrides(
            None,
            Some("no_shards\nbaz".into()),
            Some("shard".into()),
            Some("world".into()),
            Some(1),
            Some(2),
            Some(6),
            vec!["disable_shards".into(), "baz_tier".into()],
        )?;

        // Match on foo and baz in shard range, and foo_baz
        check_tier_overrides(
            None,
            Some("foo\nbaz".into()),
            Some("foo".into()),
            Some("baz".into()),
            None,
            None,
            Some(5),
            vec!["foo_tier".into(), "baz_tier".into(), "foo_baz_tier".into()],
        )?;
        Ok(())
    }
}
