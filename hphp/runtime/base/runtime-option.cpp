/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010-present Facebook, Inc. (http://www.facebook.com)  |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/

#include "hphp/runtime/base/runtime-option.h"

#include "hphp/hack/src/hackc/compile/options_gen.h"
#include "hphp/hack/src/hackc/ffi_bridge/compiler_ffi.rs.h"
#include "hphp/runtime/base/autoload-handler.h"
#include "hphp/runtime/base/bespoke-array.h"
#include "hphp/runtime/base/builtin-functions.h"
#include "hphp/runtime/base/coeffects-config.h"
#include "hphp/runtime/base/config.h"
#include "hphp/runtime/base/configs/server-loader.h"
#include "hphp/runtime/base/crash-reporter.h"
#include "hphp/runtime/base/execution-context.h"
#include "hphp/runtime/base/extended-logger.h"
#include "hphp/runtime/base/file-util-defs.h"
#include "hphp/runtime/base/file-util.h"
#include "hphp/runtime/base/hphp-system.h"
#include "hphp/runtime/base/ini-setting.h"
#include "hphp/runtime/base/init-fini-node.h"
#include "hphp/runtime/base/memory-manager.h"
#include "hphp/runtime/base/plain-file.h"
#include "hphp/runtime/base/recorder.h"
#include "hphp/runtime/base/replayer.h"
#include "hphp/runtime/base/req-heap-sanitizer.h"
#include "hphp/runtime/base/request-info.h"
#include "hphp/runtime/base/static-string-table.h"
#include "hphp/runtime/base/timestamp.h"
#include "hphp/runtime/base/tv-refcount.h"
#include "hphp/runtime/base/zend-string.h"
#include "hphp/runtime/base/zend-url.h"
#include "hphp/runtime/ext/extension-registry.h"
#include "hphp/runtime/ext/hash/hash_murmur.h"
#include "hphp/runtime/server/access-log.h"
#include "hphp/runtime/server/cli-server.h"
#include "hphp/runtime/server/files-match.h"
#include "hphp/runtime/server/satellite-server.h"
#include "hphp/runtime/server/virtual-host.h"
#include "hphp/runtime/server/server-stats.h"
#include "hphp/runtime/vm/jit/code-cache.h"
#include "hphp/runtime/vm/jit/mcgen-translate.h"
#include "hphp/runtime/vm/treadmill.h"
#include "hphp/util/arch.h"
#include "hphp/util/atomic-vector.h"
#include "hphp/util/build-info.h"
#include "hphp/util/bump-mapper.h"
#include "hphp/util/configs/server.h"
#include "hphp/util/current-executable.h" // @donotremove
#include "hphp/util/hardware-counter.h"
#include "hphp/util/hdf.h"
#include "hphp/util/hdf-extract.h"
#include "hphp/util/log-file-flusher.h"
#include "hphp/util/logger.h"
#include "hphp/util/network.h"
#include "hphp/util/numa.h"
#include "hphp/util/process.h"
#include "hphp/util/service-data.h"
#include "hphp/util/stack-trace.h"
#include "hphp/util/struct-log.h"
#include "hphp/util/text-util.h"
#include "hphp/zend/zend-string.h"

#include <cstdint>
#include <filesystem>
#include <limits>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <boost/algorithm/string/predicate.hpp>
#include <folly/CPortability.h>
#include <folly/Conv.h>
#include <folly/json/DynamicConverter.h>
#include <folly/FileUtil.h>
#include <folly/String.h>
#include <folly/portability/SysResource.h>
#include <folly/portability/SysTime.h>
#include <folly/portability/Unistd.h>

#ifdef __aarch64__
#include <sys/auxv.h>
#include <asm/hwcap.h>
#endif

#include "hphp/runtime/base/datetime.h"

namespace HPHP {
///////////////////////////////////////////////////////////////////////////////

RepoOptions RepoOptions::s_defaults;
RepoOptions RepoOptions::s_defaultsForSystemlib;

namespace {

std::vector<std::string> s_RelativeConfigs;

////////////////////////////////////////////////////////////////////////////////

void mangleForKey(bool b, std::string& s) { s += (b ? '1' : '0'); }
void mangleForKey(const Cfg::StringStringMap& map, std::string& s) {
  s += folly::to<std::string>(map.size());
  s += '\0';
  for (auto const& par : map) {
    s += par.first;
    s += '\0';
    s += par.second;
    s += '\0';
  }
}
void mangleForKey(const Cfg::StringVector& vec, std::string& s) {
  s += folly::to<std::string>(vec.size());
  s += '\0';
  for (auto const& val : vec) {
    s += val;
    s += '\0';
  }
}
void mangleForKey(const std::string& s1, std::string& s2) { s2 += s1; }

folly::dynamic toIniValue(bool b) {
  return b ? "1" : "0";
}

folly::dynamic toIniValue(const Cfg::StringStringMap& map) {
  folly::dynamic obj = folly::dynamic::object();
  for (auto& kv : map) {
    obj[kv.first] = kv.second;
  }
  return obj;
}

folly::dynamic toIniValue(const Cfg::StringVector& vec) {
  folly::dynamic obj = folly::dynamic::array();
  for (auto& val : vec) {
    obj.push_back(val);
  }
  return obj;
}

folly::dynamic toIniValue(const std::string& str) {
  return str;
}

struct CachedRepoOptions {
  CachedRepoOptions() = default;
  explicit CachedRepoOptions(RepoOptions&& opts)
    : options(new RepoOptions(std::move(opts)))
  {}
  CachedRepoOptions(const CachedRepoOptions& opts)
    : options(nullptr)
  {
    if (auto o = opts.options.load(std::memory_order_acquire)) {
      options.store(new RepoOptions(*o), std::memory_order_release);
    }
  }
  ~CachedRepoOptions() {
    Treadmill::enqueue([opt = options.exchange(nullptr)] { delete opt; });
  }

  CachedRepoOptions& operator=(const CachedRepoOptions& opts) {
    auto const o = opts.options.load(std::memory_order_acquire);
    auto const old = options.exchange(o ? new RepoOptions(*o) : nullptr);
    if (old) Treadmill::enqueue([old] { delete old; });
    return *this;
  }

  static bool isChanged(const RepoOptions* opts, const RepoOptionStats& st) {
    auto const compare = [&] (Optional<struct stat> o,
                              Optional<struct stat> s) {
      if (!o.has_value() && !s.has_value()) return false;
      if (o.has_value() != s.has_value()) return true;
      return
        s->st_mtim.tv_sec  != o->st_mtim.tv_sec ||
        s->st_mtim.tv_nsec != o->st_mtim.tv_nsec ||
        s->st_ctim.tv_sec  != o->st_ctim.tv_sec ||
        s->st_ctim.tv_nsec != o->st_ctim.tv_nsec ||
        s->st_dev != o->st_dev ||
        s->st_ino != o->st_ino;
    };
    return compare(opts->stat().m_configStat, st.m_configStat)
           || compare(opts->stat().m_packageStat, st.m_packageStat);
  }

  const RepoOptions* update(RepoOptions&& opts) const {
    auto const val = new RepoOptions(std::move(opts));
    auto const old = options.exchange(val);
    if (old) Treadmill::enqueue([old] { delete old; });
    return val;
  }

  const RepoOptions* fetch(const RepoOptionStats& st) const {
    auto const opts = options.load(std::memory_order_acquire);
    return opts && !isChanged(opts, st) ? opts : nullptr;
  }

  mutable std::atomic<RepoOptions*> options{nullptr};
};

struct RepoOptionCacheKey {
  Stream::Wrapper* wrapper;
  std::string filename;
};
struct RepoOptionCacheKeyCompare {
  bool equal(const RepoOptionCacheKey& k1, const RepoOptionCacheKey& k2) const {
    return k1.wrapper == k2.wrapper && k1.filename == k2.filename;
  }
  size_t hash(const RepoOptionCacheKey& k) const {
    return std::hash<Stream::Wrapper*>()(k.wrapper) ^
           hash_string_cs_unsafe(k.filename.c_str(), k.filename.size());
  }
};
using RepoOptionCache = tbb::concurrent_hash_map<
  RepoOptionCacheKey,
  CachedRepoOptions,
  RepoOptionCacheKeyCompare
>;
RepoOptionCache s_repoOptionCache;

constexpr const char* kHhvmConfigHdf = ".hhvmconfig.hdf";

template<class F>
bool walkDirTree(std::string fpath, F func) {
  do {
    auto const off = fpath.rfind('/');
    if (off == std::string::npos) return false;
    fpath.resize(off);
    fpath += '/';
    fpath += kHhvmConfigHdf;

    if (func(fpath)) return true;

    fpath.resize(off);
  } while (!fpath.empty() && fpath != "/");
  return false;
}

RDS_LOCAL(std::string, s_lastSeenRepoConfig);

}

RepoOptionStats::RepoOptionStats(const std::string& configPath,
                                 Stream::Wrapper* wrapper)
  : m_configStat(std::nullopt)
  , m_packageStat(std::nullopt)
{
  auto const wrapped_stat = [&](const char* path, struct stat* st) {
    if (wrapper) return wrapper->stat(path, st);
    return ::stat(path, st);
  };
  struct stat config;
  if (wrapped_stat(configPath.data(), &config) == 0) m_configStat = config;
  // Same directory
  auto const repo =
    configPath.empty() ? "" : std::filesystem::path(configPath).parent_path();
  auto const packagePath = repo / kPackagesToml;
  if (std::filesystem::exists(packagePath) || RO::EvalRecordReplay) {
    struct stat package;
    if (wrapped_stat(packagePath.string().data(), &package) == 0) {
      m_packageStat = package;
    }
  }
}

ParserEnv RepoOptionsFlags::getParserEnvironment() const {
  return ParserEnv {
      true // codegen
    , true  // hhvm_compat_mode
    , true  // php5_compat_mode
    , EnableXHPClassModifier
    , DisableXHPElementMangling
    , false // disable_xhp_children_declarations
    , true  // interpret_soft_types_as_like_types
    };
}

void RepoOptionsFlags::initAliasedNamespaces(hackc::NativeEnv& env) const {
  for (auto& [k, v] : AliasedNamespaces) {
    env.aliased_namespaces.emplace_back(hackc::StringMapEntry{k, v});
  }
}

void RepoOptionsFlags::initDeclConfig(hackc::DeclParserConfig& config) const {
  for (auto& [k, v] : AliasedNamespaces) {
    config.aliased_namespaces.emplace_back(hackc::StringMapEntry{k, v});
  }
  config.disable_xhp_element_mangling = DisableXHPElementMangling;
  config.interpret_soft_types_as_like_types = true;
  config.enable_xhp_class_modifier = EnableXHPClassModifier;
  config.php5_compat_mode = true;
  config.hhvm_compat_mode = true;
}

void RepoOptionsFlags::initHhbcFlags(hackc::HhbcFlags& flags) const {
  Cfg::InitHackcHHBCFlags(*this, flags);
}

void RepoOptionsFlags::initParserFlags(hackc::ParserFlags& flags) const {
  flags.abstract_static_props = AbstractStaticProps;
  flags.allow_unstable_features = AllowUnstableFeatures;
  flags.const_default_func_args = ConstDefaultFuncArgs;
  flags.const_static_props = ConstStaticProps;
  flags.disable_lval_as_an_expression = DisableLvalAsAnExpression;
  flags.disable_xhp_element_mangling = DisableXHPElementMangling;
  flags.disallow_func_ptrs_in_constants = DisallowFuncPtrsInConstants;
  flags.enable_xhp_class_modifier = EnableXHPClassModifier;
  flags.disallow_direct_superglobals_refs = DisallowDirectSuperglobalsRefs;
}

void RepoOptionsFlags::calcCachedQuery() {
  if (Query.empty()) return;
  try {
    m_cachedQuery = folly::parseJson(Query);
  } catch (const folly::json::parse_error& ) { /* swallow error */ }
}

const RepoOptions& RepoOptions::forFile(const std::string& path) {
  tracing::BlockNoTrace _{"repo-options"};

  if (boost::starts_with(path, "/:")) return defaults();

  // Fast path: we have an active request and it has cached a RepoOptions
  // which has not been modified. It can cause us to miss out on
  // configs that were added between the current directory and the source file.
  // (Loading these configs would result in a fatal anyway with this option)
  if (!g_context.isNull()) {
    if (auto const opts = g_context->getRepoOptionsForRequest()) {
      // If path() is empty we have the default() options, which means we have
      // negatively cached the existence of a .hhvmconfig.hdf for this request.
      if (opts->path().empty()) return *opts;

      // Don't bother checking if the file is changed. This cache is request
      // local and within any given request we want to use a consistent version
      // of the RepoOptions anyway.
      if (boost::starts_with(std::filesystem::path{path}, opts->dir())) {
        return *opts;
      }
    }
  }

  auto const isParentOf = [] (const std::filesystem::path& p1, const std::string& p2) {
    return boost::starts_with(std::filesystem::path{p2}, p1.parent_path());
  };

  // Wrap filesystem accesses if needed to proxy info from cli server client.
  Stream::Wrapper* wrapper = nullptr;
  if (is_cli_server_mode() || RO::EvalRecordReplay) {
    wrapper = Stream::getWrapperFromURI(path, nullptr, !RO::EvalRecordReplay);
    if (wrapper && !wrapper->isNormalFileStream()) wrapper = nullptr;
  }
  auto const wrapped_open = [&](const char* path) -> Optional<String> {
    if (wrapper) {
      if (auto const file = wrapper->open(path, "r", 0, nullptr)) {
        return file->read();
      }
      return std::nullopt;
    }

    auto const fd = open(path, O_RDONLY);
    if (fd < 0) return std::nullopt;
    auto file = req::make<PlainFile>(fd);
    return file->read();
  };

  auto const set = [&] (
    RepoOptionCache::const_accessor& rpathAcc,
    const std::string& path,
    const RepoOptionStats& st
  ) -> const RepoOptions* {
    *s_lastSeenRepoConfig = path;
    if (auto const opts = rpathAcc->second.fetch(st)) {
      return opts;
    }
    auto const contents = wrapped_open(path.data());
    if (!contents) return nullptr;
    RepoOptions newOpts{ contents->data(), path.data()};
    newOpts.m_stat = st;
    return rpathAcc->second.update(std::move(newOpts));
  };

  auto const test = [&] (const std::string& path) -> const RepoOptions* {
    RepoOptionCache::const_accessor rpathAcc;
    const RepoOptionCacheKey key {wrapper, path};
    if (!s_repoOptionCache.find(rpathAcc, key)) return nullptr;
    RepoOptionStats st(path, wrapper);
    if (st.missing()) {
      s_repoOptionCache.erase(rpathAcc);
      return nullptr;
    }
    return set(rpathAcc, path, st);
  };

  const RepoOptions* ret{nullptr};

  // WARNING: when Eval.CachePerRepoOptionsPath we cache the last used path for
  //          RepoOptions per thread, and while we will detect changes to this
  //          file, and do a rescan in the event that it is deleted or doesn't
  //          match the current file being loaded, we will miss out on new
  //          configs that may be added. Since we expect to see a single config
  //          per repository we expect that this will be a reasonably safe,
  //          optimization.
  if (RuntimeOption::EvalCachePerRepoOptionsPath) {
    if (!s_lastSeenRepoConfig->empty() &&
        isParentOf(*s_lastSeenRepoConfig, path)) {
      if (auto const r = test(*s_lastSeenRepoConfig)) return *r;
      s_lastSeenRepoConfig->clear();
    }

    // If the last seen path isn't set yet or is no longer accurate try checking
    // other cached paths before falling back to the filesystem.
    walkDirTree(path, [&] (const std::string& path) {
      return (ret = test(path)) != nullptr;
    });
  }

  if (ret) return *ret;

  walkDirTree(path, [&] (const std::string& path) {
    RepoOptionStats st(path, wrapper);
    if (st.missing()) return false;
    RepoOptionCache::const_accessor rpathAcc;
    const RepoOptionCacheKey key {wrapper, path};
    s_repoOptionCache.insert(rpathAcc, key);
    ret = set(rpathAcc, path, st);
    return true;
  });

  return ret ? *ret : defaults();
}

void RepoOptions::calcCacheKey() {
  std::string raw;
#define C(_, n) mangleForKey(m_flags.n, raw);
CONFIGS_FOR_REPOOPTIONSFLAGS()
#undef C

  mangleForKey(packageInfo().mangleForCacheKey(), raw);
  m_flags.m_sha1 = SHA1{string_sha1(raw)};
}

namespace {
std::string getCacheBreakerSchemaHash(std::string_view root,
                                      const RepoOptionsFlags& flags) {
  std::string optsHash = RO::EvalIncludeReopOptionsInFactsCacheBreaker
      ? flags.cacheKeySha1().toString()
      : flags.getFactsCacheBreaker();

  auto const logStr = folly::sformat(
    "Native Facts DB cache breaker:\n"
    " Version: {}\n"
    " Root: {}\n"
    " RepoOpts hash: {}",
    Facts::kSchemaVersion,
    root,
    optsHash
  );
  if (RO::ServerExecutionMode() && !is_cli_server_mode()) {
    Logger::FInfo(logStr);
  } else {
    Logger::FVerbose(logStr);
  }

  std::string rootHash = string_sha1(root);
  optsHash.resize(10);
  rootHash.resize(10);
  return folly::to<std::string>(Facts::kSchemaVersion, '_', optsHash, '_',
                                rootHash);
}
}

constexpr std::string_view kEUIDPlaceholder = "%{euid}";
constexpr std::string_view kSchemaPlaceholder = "%{schema}";
void RepoOptions::calcAutoloadDB() {
  namespace fs = std::filesystem;

  if (Cfg::Autoload::DBPath.empty()) return;
  std::string pathTemplate{Cfg::Autoload::DBPath};

  auto const euidIdx = pathTemplate.find(kEUIDPlaceholder);
  if (euidIdx != std::string::npos) {
    pathTemplate.replace(
        euidIdx, kEUIDPlaceholder.size(), folly::to<std::string>(geteuid()));
  }

  auto const schemaIdx = pathTemplate.find(kSchemaPlaceholder);
  if (schemaIdx != std::string::npos) {
    pathTemplate.replace(
        schemaIdx,
        kSchemaPlaceholder.size(),
        getCacheBreakerSchemaHash(m_repo.native(), m_flags));
  }

  fs::path dbPath = pathTemplate;
  if (dbPath.is_relative()) dbPath = m_repo / dbPath;
  m_autoloadDB = fs::absolute(dbPath);
}

const RepoOptions& RepoOptions::defaults() {
  always_assert(s_defaults.m_init);
  return s_defaults;
}

const RepoOptions& RepoOptions::defaultsForSystemlib() {
  if (!s_defaultsForSystemlib.m_init) {
    s_defaultsForSystemlib.initDefaultsForSystemlib();
  }
  return s_defaultsForSystemlib;
}

void RepoOptions::filterNamespaces() {
  for (auto it = m_flags.AliasedNamespaces.begin();
       it != m_flags.AliasedNamespaces.end(); ) {
    if (!is_valid_class_name(it->second)) {
      Logger::Warning("Skipping invalid AliasedNamespace %s\n",
                      it->second.c_str());
      it = m_flags.AliasedNamespaces.erase(it);
      continue;
    }

    while (it->second.size() && it->second[0] == '\\') {
      it->second = it->second.substr(1);
    }

    ++it;
  }
}

RepoOptions::RepoOptions(const char* str, const char* file) : m_path(file), m_init(true) {
  always_assert(s_defaults.m_init);
  Hdf config{};
  config.fromString(str);

  Cfg::GetRepoOptionsFlagsFromConfig(m_flags, config, s_defaults.m_flags);

  hdfExtract(config, "Autoload.CacheBreaker", m_flags.m_factsCacheBreaker,
             "d6ede7d391");

  filterNamespaces();
  if (!m_path.empty()) {
    if (UNLIKELY(RO::EvalRecordReplay)) {
      const String path{m_path.parent_path().c_str()};
      m_repo = Stream::getWrapperFromURI(path)->realpath(path).toCppString();
    } else {
      m_repo = std::filesystem::canonical(m_path.parent_path());
    }
  }
  m_flags.m_packageInfo = PackageInfo::fromFile(m_repo / kPackagesToml);
  calcCacheKey();
  calcAutoloadDB();
  m_flags.calcCachedQuery();
}

void RepoOptions::initDefaults(const Hdf& hdf, const IniSettingMap& ini) {
  Cfg::GetRepoOptionsFlags(m_flags, ini, hdf);

  filterNamespaces();
  m_path.clear();
  m_flags.m_packageInfo = PackageInfo::defaults();
  calcCacheKey();
  m_init = true;
}

void RepoOptions::initDefaultsForSystemlib() {
  Cfg::GetRepoOptionsFlagsForSystemlib(m_flags);

  filterNamespaces();
  m_path.clear();
  m_flags.m_packageInfo = PackageInfo::defaults();
  calcCacheKey();
  m_init = true;
}

void RepoOptions::setDefaults(const Hdf& hdf, const IniSettingMap& ini) {
  always_assert(!s_defaults.m_init);
  s_defaults.initDefaults(hdf, ini);
}

///////////////////////////////////////////////////////////////////////////////

std::map<std::string, std::string> RuntimeOption::TierOverwriteInputs;

std::string RuntimeOption::BuildId;
std::string RuntimeOption::InstanceId;
std::string RuntimeOption::DeploymentId;
int64_t RuntimeOption::ConfigId = 0;
std::string RuntimeOption::PidFile = "www.pid";

bool RuntimeOption::EnableXHP = true;
bool RuntimeOption::CheckSymLink = true;
bool RuntimeOption::TrustAutoloaderPath = false;
bool RuntimeOption::EnableArgsInBacktraces = true;
bool RuntimeOption::EnableZendIniCompat = true;
bool RuntimeOption::TimeoutsUseWallTime = true;
bool RuntimeOption::EvalAuthoritativeMode = false;
bool RuntimeOption::DumpPreciseProfData = true;
JitSerdesMode RuntimeOption::EvalJitSerdesMode{};
int RuntimeOption::ProfDataTTLHours = 24;
std::string RuntimeOption::ProfDataTag;
std::string RuntimeOption::EvalJitSerdesFile;

std::map<std::string, ErrorLogFileData> RuntimeOption::ErrorLogs = {
  {Logger::DEFAULT, ErrorLogFileData()},
};
// these hold the DEFAULT logger
std::string RuntimeOption::LogFile;
std::string RuntimeOption::LogFileSymLink;
uint16_t RuntimeOption::LogFilePeriodMultiplier;

int RuntimeOption::LogHeaderMangle = 0;
bool RuntimeOption::AlwaysLogUnhandledExceptions = true;
bool RuntimeOption::AlwaysEscapeLog = true;
bool RuntimeOption::NoSilencer = false;
int RuntimeOption::RuntimeErrorReportingLevel =
  static_cast<int>(ErrorMode::HPHP_ALL);
int RuntimeOption::ForceErrorReportingLevel = 0;

std::vector<std::string> RuntimeOption::TzdataSearchPaths;
hphp_fast_string_set RuntimeOption::ActiveExperiments;
hphp_fast_string_set RuntimeOption::InactiveExperiments;

int RuntimeOption::RaiseDebuggingFrequency = 1;
int64_t RuntimeOption::SerializationSizeLimit = StringData::MaxSize;

std::string RuntimeOption::AccessLogDefaultFormat = "%h %l %u %t \"%r\" %>s %b";
std::map<std::string, AccessLogFileData> RuntimeOption::AccessLogs;

std::string RuntimeOption::AdminLogFormat = "%h %t %s %U";
std::string RuntimeOption::AdminLogFile;
std::string RuntimeOption::AdminLogSymLink;

std::map<std::string, AccessLogFileData> RuntimeOption::RPCLogs;

std::vector<std::shared_ptr<VirtualHost>> RuntimeOption::VirtualHosts;
std::shared_ptr<IpBlockMap> RuntimeOption::IpBlocks;
std::vector<std::shared_ptr<SatelliteServerInfo>>
  RuntimeOption::SatelliteServerInfos;

std::map<std::string, std::string> RuntimeOption::IncludeRoots;

hphp_string_imap<std::string> RuntimeOption::StaticFileExtensions;
hphp_string_imap<std::string> RuntimeOption::PhpFileExtensions;
std::vector<std::shared_ptr<FilesMatch>> RuntimeOption::FilesMatches;

std::string RuntimeOption::AdminServerIP;
int RuntimeOption::AdminServerPort = 0;
int RuntimeOption::AdminThreadCount = 1;
bool RuntimeOption::AdminServerEnableSSLWithPlainText = false;
bool RuntimeOption::AdminServerStatsNeedPassword = true;
std::string RuntimeOption::AdminPassword;
std::set<std::string> RuntimeOption::AdminPasswords;
std::set<std::string> RuntimeOption::HashedAdminPasswords;
std::string RuntimeOption::AdminDumpPath;

int RuntimeOption::HttpDefaultTimeout = 30;
int RuntimeOption::HttpSlowQueryThreshold = 5000; // ms

bool RuntimeOption::NativeStackTrace = false;
bool RuntimeOption::ServerErrorMessage = false;
bool RuntimeOption::RecordInput = false;
bool RuntimeOption::ClearInputOnSuccess = true;
std::string RuntimeOption::ProfilerOutputDir = "/tmp";
std::string RuntimeOption::CoreDumpEmail;
bool RuntimeOption::CoreDumpReport = true;
std::string RuntimeOption::CoreDumpReportDirectory =
#if defined(HPHP_OSS)
  "/tmp";
#else
  "/var/tmp/cores";
#endif
std::string RuntimeOption::StackTraceFilename;
int RuntimeOption::StackTraceTimeout = 0; // seconds; 0 means unlimited
std::string RuntimeOption::RemoteTraceOutputDir = "/tmp";
std::set<std::string, stdltistr> RuntimeOption::TraceFunctions;

bool RuntimeOption::EnableStats = false;
bool RuntimeOption::EnableAPCStats = false;
bool RuntimeOption::EnableWebStats = false;
bool RuntimeOption::EnableMemoryStats = false;
bool RuntimeOption::EnableSQLStats = false;
bool RuntimeOption::EnableSQLTableStats = false;
bool RuntimeOption::EnableNetworkIOStatus = false;
std::string RuntimeOption::StatsXSL;
std::string RuntimeOption::StatsXSLProxy;
uint32_t RuntimeOption::StatsSlotDuration = 10 * 60; // 10 minutes
uint32_t RuntimeOption::StatsMaxSlot = 12 * 6; // 12 hours
std::vector<std::string> RuntimeOption::StatsTrackedKeys;

int64_t RuntimeOption::MaxSQLRowCount = 0;
int64_t RuntimeOption::SocketDefaultTimeout = 60;

#if FOLLY_SANITIZE
bool RuntimeOption::DisableSmallAllocator = true;
#else
bool RuntimeOption::DisableSmallAllocator = false;
#endif

std::map<std::string, std::string> RuntimeOption::ServerVariables;
std::map<std::string, std::string> RuntimeOption::EnvVariables;

std::string RuntimeOption::WatchmanRootSocket;
std::string RuntimeOption::WatchmanDefaultSocket;

int RuntimeOption::CheckCLIClientCommands = 0;

const std::string& RuntimeOption::GetServerPrimaryIPv4() {
   static std::string serverPrimaryIPv4 = GetPrimaryIPv4();
   return serverPrimaryIPv4;
}

const std::string& RuntimeOption::GetServerPrimaryIPv6() {
   static std::string serverPrimaryIPv6 = GetPrimaryIPv6();
   return serverPrimaryIPv6;
}

static inline std::string regionSelectorDefault() {
  return "tracelet";
}

static inline bool pgoDefault() {
#ifdef HHVM_NO_DEFAULT_PGO
  return false;
#else
  return true;
#endif
}

static inline bool eagerGcDefault() {
#ifdef HHVM_EAGER_GC
  return true;
#else
  return false;
#endif
}

static inline bool enableGcDefault() {
  return RuntimeOption::EvalEagerGC;
}

static inline uint64_t pgoThresholdDefault() {
  return debug ? 2 : 2000;
}

static inline bool reuseTCDefault() {
  return hhvm_reuse_tc && !RuntimeOption::RepoAuthoritative;
}

static inline bool useFileBackedArenaDefault() {
  return RuntimeOption::RepoAuthoritative &&
    RuntimeOption::ServerExecutionMode();
}

static inline bool hugePagesSoundNice() {
  return RuntimeOption::ServerExecutionMode();
}

static inline uint32_t hotTextHugePagesDefault() {
  if (!hugePagesSoundNice()) return 0;
  return arch() == Arch::ARM ? 12 : 8;
}

static inline std::string reorderPropsDefault() {
  if (isJitDeserializing()) {
    return "countedness-hotness";
  }
  return debug ? "alphabetical" : "countedness";
}

Optional<std::filesystem::path> RuntimeOption::GetHomePath(
    const folly::StringPiece user) {
  namespace fs = std::filesystem;

  auto homePath = fs::path{RO::SandboxHome} / fs::path{std::string{user}};
  if (fs::is_directory(homePath)) {
    return make_optional(std::move(homePath));
  }

  if (!RuntimeOption::SandboxFallback.empty()) {
    homePath = fs::path{RO::SandboxFallback} / fs::path{std::string{user}};
    if (fs::is_directory(homePath)) {
      return make_optional(std::move(homePath));
    }
  }

  return std::nullopt;
}

bool RuntimeOption::funcIsRenamable(const StringData* name) {
  if (HPHP::is_generated(name)) return false;
  if (Cfg::Jit::EnableRenameFunction == 0) return false;
  if (Cfg::Jit::EnableRenameFunction == 2) {
    return Cfg::Eval::RenamableFunctions.find(name->data()) !=
      Cfg::Eval::RenamableFunctions.end();
  } else {
    return true;
  }
}

std::string RuntimeOption::GetDefaultUser() {
  if (SandboxDefaultUserFile.empty()) return {};

  std::filesystem::path file{RO::SandboxDefaultUserFile};
  if (!std::filesystem::is_regular_file(file)) return {};

  std::string user;
  if (!folly::readFile(file.c_str(), user) || user.empty()) return {};

  return user;
}

bool RuntimeOption::ReadPerUserSettings(const std::filesystem::path& confFileName,
                                        IniSettingMap& ini, Hdf& config) {
  try {
    Config::ParseConfigFile(confFileName.native(), ini, config, false);
    return true;
  } catch (HdfException& e) {
    Logger::Error("%s ignored: %s", confFileName.native().c_str(),
                  e.getMessage().c_str());
    return false;
  }
}

std::string RuntimeOption::getTraceOutputFile() {
  return folly::sformat("{}/hphp.{}.log",
                        RuntimeOption::RemoteTraceOutputDir, (int64_t)getpid());
}

const uint64_t kEvalVMStackElmsDefault =
#if defined(VALGRIND) && !FOLLY_SANITIZE
 0x800
#else
 0x4000
#endif
 ;

constexpr uint32_t kEvalVMInitialGlobalTableSizeDefault = 512;

using std::string;
#define F(type, name, def) \
  type RuntimeOption::Eval ## name = type(def);
EVALFLAGS();
#undef F
hphp_string_map<TypedValue> RuntimeOption::ConstantFunctions;

bool RuntimeOption::RecordCodeCoverage = false;
std::string RuntimeOption::CodeCoverageOutputFile;

std::string RuntimeOption::RepoPath;
RepoMode RuntimeOption::RepoLocalMode = RepoMode::ReadOnly;
std::string RuntimeOption::RepoLocalPath;
RepoMode RuntimeOption::RepoCentralMode = RepoMode::ReadWrite;
std::string RuntimeOption::RepoCentralPath;
int32_t RuntimeOption::RepoCentralFileMode;
std::string RuntimeOption::RepoCentralFileUser;
std::string RuntimeOption::RepoCentralFileGroup;
std::string RuntimeOption::RepoJournal = "delete";
bool RuntimeOption::RepoAllowFallbackPath = true;
bool RuntimeOption::RepoCommit = true;
bool RuntimeOption::RepoDebugInfo = true;
bool RuntimeOption::RepoLitstrLazyLoad = true;
// Missing: RuntimeOption::RepoAuthoritative's physical location is
// perf-sensitive.
uint32_t RuntimeOption::RepoBusyTimeoutMS = 15000;

bool RuntimeOption::HHProfEnabled = false;
bool RuntimeOption::HHProfActive = false;
bool RuntimeOption::HHProfAccum = false;
bool RuntimeOption::HHProfRequest = false;

bool RuntimeOption::SandboxMode = false;
std::string RuntimeOption::SandboxPattern;
std::string RuntimeOption::SandboxHome;
std::string RuntimeOption::SandboxFallback;
std::string RuntimeOption::SandboxConfFile;
std::map<std::string, std::string> RuntimeOption::SandboxServerVariables;
bool RuntimeOption::SandboxFromCommonRoot = false;
std::string RuntimeOption::SandboxDirectoriesRoot;
std::string RuntimeOption::SandboxLogsRoot;
std::string RuntimeOption::SandboxDefaultUserFile;
std::string RuntimeOption::SandboxHostAlias;

std::string RuntimeOption::SendmailPath = "sendmail -t -i";
std::string RuntimeOption::MailForceExtraParameters;

bool RuntimeOption::SimpleXMLEmptyNamespaceMatchesAll = false;

int RuntimeOption::ProfilerTraceBuffer = 2000000;
double RuntimeOption::ProfilerTraceExpansion = 1.2;
int RuntimeOption::ProfilerMaxTraceBuffer = 0;

#ifdef HHVM_FACEBOOK

int RuntimeOption::ThriftFBServerWorkerThreads = 1;
int RuntimeOption::ThriftFBServerPoolThreads = 1;

bool RuntimeOption::EnableFb303Server = false;
int RuntimeOption::Fb303ServerPort = 0;
std::string RuntimeOption::Fb303ServerIP;
int RuntimeOption::Fb303ServerWorkerThreads = 1;
int RuntimeOption::Fb303ServerPoolThreads = 1;
bool RuntimeOption::Fb303ServerExposeSensitiveMethods = false;

bool RuntimeOption::ThreadTuneDebug = false;
bool RuntimeOption::ThreadTuneSkipWarmup = false;
double RuntimeOption::ThreadTuneAdjustmentPct = 0;
double RuntimeOption::ThreadTuneAdjustmentDownPct = 0;
double RuntimeOption::ThreadTuneStepPct = 5;
double RuntimeOption::ThreadTuneCPUThreshold = 95.0;
double RuntimeOption::ThreadTuneThreadUtilizationThreshold = 90.0;
#endif

double RuntimeOption::XenonPeriodSeconds = 0.0;
uint32_t RuntimeOption::XenonRequestFreq = 1;
bool RuntimeOption::XenonForceAlwaysOn = false;
bool RuntimeOption::XenonTrackActiveWorkers = false;

bool RuntimeOption::StrobelightEnabled = false;

bool RuntimeOption::TrackPerUnitMemory = false;

bool RuntimeOption::SetProfileNullThisObject = true;

std::map<std::string, std::string> RuntimeOption::CustomSettings;

#ifdef NDEBUG
  #ifdef ALWAYS_ASSERT
    const StaticString s_hhvm_build_type("Release with asserts");
  #else
    const StaticString s_hhvm_build_type("Release");
  #endif
#else
  const StaticString s_hhvm_build_type("Debug");
#endif

///////////////////////////////////////////////////////////////////////////////

static void setResourceLimit(int resource, const IniSetting::Map& ini,
                             const Hdf& rlimit, const char* nodeName) {
  if (UNLIKELY(RO::EvalRecordReplay && RO::EvalReplay)) {
    return;
  }
  if (!Config::GetString(ini, rlimit, nodeName).empty()) {
    struct rlimit rl;
    getrlimit(resource, &rl);
    rl.rlim_cur = Config::GetInt64(ini, rlimit, nodeName);
    if (rl.rlim_max < rl.rlim_cur) {
      rl.rlim_max = rl.rlim_cur;
    }
    int ret = setrlimit(resource, &rl);
    if (ret) {
      Logger::Error("Unable to set %s to %" PRId64 ": %s (%d)",
                    nodeName, (int64_t)rl.rlim_cur,
                    folly::errnoStr(errno).c_str(), errno);
    }
  }
}

static void normalizePath(std::string &path) {
  if (!path.empty()) {
    if (path[path.length() - 1] == '/') {
      path = path.substr(0, path.length() - 1);
    }
    if (path[0] != '/') {
      path = std::string("/") + path;
    }
  }
}

static bool matchShard(
  bool enableShards,
  const std::string& hostname,
  const IniSetting::Map& ini, Hdf hdfPattern,
  std::vector<std::string>& messages
) {
  if (!hdfPattern.exists("Shard")) return true;

  if (!enableShards) {
    hdfPattern["Shard"].setVisited();
    hdfPattern["ShardCount"].setVisited();
    hdfPattern["ShardSalt"].setVisited();
    return false;
  }

  auto const shard = Config::GetInt64(ini, hdfPattern, "Shard", -1, false);

  auto const nshards =
    Config::GetInt64(ini, hdfPattern, "ShardCount", 100, false);

  if (shard < 0 || shard >= nshards) {
    messages.push_back(folly::sformat("Invalid value for Shard: {}", shard));
    return true;
  }

  auto input = hostname;
  if (hdfPattern.exists("ShardSalt")) {
    input += Config::GetString(ini, hdfPattern, "ShardSalt", "", false);
  }

  auto const md5 = Md5Digest(input.data(), input.size());
  uint32_t seed{0};
  memcpy(&seed, &md5.digest[0], 4);

  // This shift is to match the behavior of sharding in chef which appears to
  // have an off-by-one bug:
  //   seed = Digest::MD5.hexdigest(seed_input)[0...7].to_i(16)
  seed = ntohl(seed) >> 4;

  messages.push_back(folly::sformat(
    "Checking Shard = {}; Input = {}; Seed = {}; ShardCount = {}; Value = {}",
    shard, input, seed, nshards, seed % nshards
  ));

  return seed % nshards <= shard;
}

static Optional<uint64_t> hostNumber(const std::string& hostname) {
  auto const pos = hostname.find_first_of("0123456789");
  if (pos == std::string::npos) return {};

  try {
    return std::stoll(hostname.substr(pos));
  } catch (const std::invalid_argument&) {
  } catch (const std::out_of_range&) {
  }

  return {};
}

static bool matchExperiment(
  bool enableShards,
  const std::string& hostname,
  const IniSetting::Map& ini, Hdf& config,
  std::vector<std::string>& messages
) {
  if (!config.exists("Experiment")) return true;
  if (!config.exists("Experiment.name")) {
    config["Experiment"].setVisited();
    messages.push_back(
      "Detected misconfigured experiment with no name, skipping"
    );
    return false;
  }

  if (!enableShards) {
    config["Experiment"].setVisited();
    return false;
  }

  auto const rate = Config::GetInt64(ini, config, "Experiment.rate", 2);
  auto const flip = Config::GetBool(ini, config, "Experiment.flip", false);
  auto const name = Config::GetString(ini, config, "Experiment.name");

  if (rate == 0) return flip;
  if (rate == 1) return !flip;

  auto const num  = hostNumber(hostname).value_or(12345678);
  auto const hash = murmur_hash_64A(name.data(), name.size(), num);
  auto const res  = hash % rate == 0;

  messages.push_back(folly::sformat(
    "Checking Experiment `{}'; hostname = {}; Seed = {}; Hash = {}; Flip = {}",
    name, hostname, num, hash, flip ? "true" : "false"
  ));

  return flip ? !res : res;
}

// A machine can belong to a tier, which can overwrite
// various settings, even if they are set in the same
// hdf file. However, CLI overrides still win the day over
// everything.
static std::vector<std::string> getTierOverwrites(
  IniSetting::Map& ini,
  Hdf& config,
  hphp_fast_string_set& active_exps,
  hphp_fast_string_set& inactive_exps
) {

  // Machine metrics
  string hostname, tier, task, cpu, tiers, tags;
  {
    hostname = Config::GetString(ini, config, "Machine.name");
    if (hostname.empty()) {
      hostname = Process::GetHostName();
    }

    tier = Config::GetString(ini, config, "Machine.tier");

    task = Config::GetString(ini, config, "Machine.task");

    cpu = Config::GetString(ini, config, "Machine.cpu");
    if (cpu.empty()) {
      cpu = Process::GetCPUModel();
    }

    tiers = Config::GetString(ini, config, "Machine.tiers");
    if (!tiers.empty()) {
      if (!folly::readFile(tiers.c_str(), tiers)) {
        tiers.clear();
      }
    }

    tags = Config::GetString(ini, config, "Machine.tags");
    if (!tags.empty()) {
      if (!folly::readFile(tags.c_str(), tags)) {
        tags.clear();
      }
    }
  }

  auto const checkPatterns = [&] (Hdf hdf) {
    // Check the patterns one by one so they all get evaluated; otherwise, when
    // using "&&" in a single expression with multiple patterns, if an earlier
    // one fails to match, the later one would be reported as unused.
    auto matched = true;
    matched &= Config::matchHdfPattern(hostname, ini, hdf, "machine");
    matched &= Config::matchHdfPattern(tier, ini, hdf, "tier");
    matched &= Config::matchHdfPattern(task, ini, hdf, "task");
    matched &= Config::matchHdfPattern(tiers, ini, hdf, "tiers", "m");
    matched &= Config::matchHdfPattern(tags, ini, hdf, "tags", "m");
    matched &= Config::matchHdfPatternSet(tags, ini, hdf, "tagset");
    matched &= Config::matchHdfPattern(cpu, ini, hdf, "cpu");
    return matched;
  };

  std::vector<std::string> messages;
  auto enableShards = true;

  auto const matchesTier = [&] (Hdf hdf) {
    // Check the patterns one by one so they all get evaluated; otherwise, when
    // using "&&" in a single expression with multiple patterns, if an earlier
    // one fails to match, the later one would be reported as unused.
    auto matched = true;
    matched &= checkPatterns(hdf);
    matched &= !hdf.exists("exclude") || !checkPatterns(hdf["exclude"]);
    matched &= matchShard(enableShards, hostname, ini, hdf, messages);
    return matched;
  };

  // Tier overwrites
  {
    // Required to get log entries actually written to scuba
    for (Hdf hdf = config["Tiers"].firstChild(); hdf.exists();
         hdf = hdf.next()) {
      if (messages.empty()) {
        messages.emplace_back(folly::sformat(
                                "Matching tiers using: "
                                "machine='{}', tier='{}', task='{}', "
                                "cpu='{}', tiers='{}', tags='{}'",
                                hostname, tier, task, cpu, tiers, tags));

        RuntimeOption::StoreTierOverwriteInputs(hostname, tier, task, cpu, tiers, tags);
      }
      auto const matches = matchesTier(hdf);
      auto const matches_exp = matchExperiment(
        enableShards, hostname, ini, hdf, messages
      );
      if (matches && matches_exp) {
        messages.emplace_back(folly::sformat(
                                "Matched tier: {}", hdf.getName()));
        if (enableShards && hdf["DisableShards"].configGetBool()) {
          messages.emplace_back("Sharding is disabled.");
          enableShards = false;
        }
        if (hdf.exists("clear")) {
          std::vector<std::string> list;
          hdf["clear"].configGet(list);
          for (auto const& s : list) {
            config.remove(s);
          }
        }
        if (hdf.exists("Experiment.name")) {
          active_exps.emplace(Config::GetString(ini, hdf, "Experiment.name"));
        }
        config.copy(hdf["overwrite"]);
        // no break here, so we can continue to match more overwrites
      } else if (matches && !matches_exp && hdf.exists("Experiment.name")) {
        inactive_exps.emplace(Config::GetString(ini, hdf, "Experiment.name"));
      }
      // Avoid lint errors about unvisited nodes when the tier does not match.
      hdf["DisableShards"].setVisited();
      hdf["clear"].setVisited();
      hdf["overwrite"].setVisited();
    }
  }
  return messages;
}

// Log the inputs used for calculating tier overwrites for this machine
void RuntimeOption::StoreTierOverwriteInputs(const std::string &machine, const std::string &tier,
  const std::string &task, const std::string &cpu, const std::string &tiers, const std::string &tags) {
    // Store the tier inputs as part of the RuntimeOption singleton, so that we can access it later
    RuntimeOption::TierOverwriteInputs["machine"] = machine;
    RuntimeOption::TierOverwriteInputs["tier"] = tier;
    RuntimeOption::TierOverwriteInputs["task"] = task;
    RuntimeOption::TierOverwriteInputs["cpu"] = cpu;
    RuntimeOption::TierOverwriteInputs["tiers"] = tiers;
    RuntimeOption::TierOverwriteInputs["tags"] = tags;
}

void logTierOverwriteInputs() {
  if(!Cfg::Server::LogTierOverwriteInputs) return;

  StructuredLogEntry log_entry;
  log_entry.force_init = true;
  log_entry.setStr("machine", RuntimeOption::TierOverwriteInputs["machine"]);
  log_entry.setStr("tier", RuntimeOption::TierOverwriteInputs["tier"]);
  log_entry.setStr("task", RuntimeOption::TierOverwriteInputs["task"]);
  log_entry.setStr("cpu", RuntimeOption::TierOverwriteInputs["cpu"]);
  log_entry.setStr("tiers", RuntimeOption::TierOverwriteInputs["tiers"]);
  log_entry.setStr("tags", RuntimeOption::TierOverwriteInputs["tags"]);
  StructuredLog::log("hhvm_config_hdf_logs", log_entry);
}

InitFiniNode s_logTierOverwrites(logTierOverwriteInputs, InitFiniNode::When::ServerInit);

void RuntimeOption::ReadSatelliteInfo(
    const IniSettingMap& ini,
    const Hdf& hdf,
    std::vector<std::shared_ptr<SatelliteServerInfo>>& infos) {
  auto ss_callback = [&] (const IniSettingMap &ini_ss, const Hdf &hdf_ss,
                         const std::string &ini_ss_key) {
    auto satellite = std::make_shared<SatelliteServerInfo>(ini_ss, hdf_ss,
                                                           ini_ss_key);
    infos.push_back(satellite);
  };
  Config::Iterate(ss_callback, ini, hdf, "Satellites");
}

extern void initialize_apc();
void RuntimeOption::Load(
  IniSetting::Map& ini, Hdf& config,
  const std::vector<std::string>& iniClis /* = std::vector<std::string>() */,
  const std::vector<std::string>& hdfClis /* = std::vector<std::string>() */,
  std::vector<std::string>* messages /* = nullptr */,
  std::string cmd /* = "" */) {

  // Intialize the memory manager here because various settings and
  // initializations that we do here need it
  tl_heap.getCheck();

  // Get the ini (-d) and hdf (-v) strings, which may override some
  // of options that were set from config files. We also do these
  // now so we can override Tier.*.[machine | tier | cpu] on the
  // command line, along with any fields within a Tier (e.g.,
  // CoreFileSize)
  for (auto& istr : iniClis) {
    Config::ParseIniString(istr, ini);
  }
  for (auto& hstr : hdfClis) {
    Config::ParseHdfString(hstr, config);
  }

  // See if there are any Tier-based overrides
  auto m = getTierOverwrites(ini, config, ActiveExperiments,
                             InactiveExperiments);
  if (messages) *messages = std::move(m);

  // RelativeConfigs can be set by commandline flags and tier overwrites, they
  // may also contain tier overwrites. They are, however, only included once, so
  // relative configs may not specify other relative configs which must to be
  // loaded. If RelativeConfigs is modified while loading configs an error is
  // raised, but we defer doing so until the logger is initialized below. If a
  // relative config cannot be found it is silently skipped (this is to allow
  // configs to be conditionally applied to scripts based on their location). By
  // reading the "hhvm.relative_configs" ini setting at runtime it is possible
  // to determine which configs were actually loaded.
  std::string relConfigsError;
  Config::Bind(s_RelativeConfigs, ini, config, "RelativeConfigs");
  if (!cmd.empty() && !s_RelativeConfigs.empty()) {
    String strcmd(cmd, CopyString);
    Process::InitProcessStatics();
    auto const currentDir = Process::CurrentWorkingDirectory.data();
    std::vector<std::string> newConfigs;
    auto const original = s_RelativeConfigs;
    for (auto& str : original) {
      if (str.empty()) continue;

      std::string fullpath;
      auto const found = FileUtil::runRelative(
        str, strcmd, currentDir,
        [&] (const String& f) {
          if (access(f.data(), R_OK) == 0) {
            fullpath = f.toCppString();
            FTRACE_MOD(Trace::facts, 3, "Parsing {}\n", fullpath);
            Config::ParseConfigFile(fullpath, ini, config);
            return true;
          }
          return false;
        }
      );
      if (found) newConfigs.emplace_back(std::move(fullpath));
    }
    if (!newConfigs.empty()) {
      auto m2 = getTierOverwrites(ini, config, ActiveExperiments,
                                  InactiveExperiments);
      if (messages) *messages = std::move(m2);
      if (s_RelativeConfigs != original) {
        relConfigsError = folly::sformat(
          "RelativeConfigs node was modified while loading configs from [{}] "
          "to [{}]",
          folly::join(", ", original),
          folly::join(", ", s_RelativeConfigs)
        );
      }
    }
    s_RelativeConfigs.swap(newConfigs);
  } else {
    s_RelativeConfigs.clear();
  }

  // Then get the ini and hdf cli strings again, in case the tier overwrites
  // overrode any non-tier based command line option we set. The tier-based
  // command line overwrites will already have been set in the call above.
  // This extra call is for the other command line options that may have been
  // overridden by a tier, but shouldn't have been.
  for (auto& istr : iniClis) {
    Config::ParseIniString(istr, ini);
  }
  for (auto& hstr : hdfClis) {
    Config::ParseHdfString(hstr, config);
  }

  Config::Bind(PidFile, ini, config, "PidFile", "www.pid");
  Config::Bind(DeploymentId, ini, config, "DeploymentId");

  {
    static std::string deploymentIdOverride;
    Config::Bind(deploymentIdOverride, ini, config, "DeploymentIdOverride");
    if (!deploymentIdOverride.empty()) {
      RuntimeOption::DeploymentId = deploymentIdOverride;
    }
  }

  {
    // Config ID
    Config::Bind(ConfigId, ini, config, "ConfigId", 0);
    auto configIdCounter = ServiceData::createCounter("vm.config.id");
    configIdCounter->setValue(ConfigId);
  }

  {
    // Logging
    auto setLogLevel = [](const std::string& value) {
      // ini parsing treats "None" as ""
      if (value == "None" || value == "") {
        Logger::LogLevel = Logger::LogNone;
      } else if (value == "Error") {
        Logger::LogLevel = Logger::LogError;
      } else if (value == "Warning") {
        Logger::LogLevel = Logger::LogWarning;
      } else if (value == "Info") {
        Logger::LogLevel = Logger::LogInfo;
      } else if (value == "Verbose") {
        Logger::LogLevel = Logger::LogVerbose;
      } else {
        return false;
      }
      return true;
    };
    auto str = Config::GetString(ini, config, "Log.Level");
    if (!str.empty()) {
      setLogLevel(str);
    }
    IniSetting::Bind(IniSetting::CORE, IniSetting::Mode::Config,
                     "hhvm.log.level", IniSetting::SetAndGet<std::string>(
      setLogLevel,
      []() {
        switch (Logger::LogLevel) {
          case Logger::LogNone:
            return "None";
          case Logger::LogError:
            return "Error";
          case Logger::LogWarning:
            return "Warning";
          case Logger::LogInfo:
            return "Info";
          case Logger::LogVerbose:
            return "Verbose";
        }
        return "";
      }
    ));

    Config::Bind(Logger::UseLogFile, ini, config, "Log.UseLogFile", true);
    Config::Bind(LogFile, ini, config, "Log.File");
    Config::Bind(LogFileSymLink, ini, config, "Log.SymLink");
    Config::Bind(LogFilePeriodMultiplier, ini,
                 config, "Log.PeriodMultiplier", 0);
    if (Logger::UseLogFile && RuntimeOption::ServerExecutionMode()) {
      RuntimeOption::ErrorLogs[Logger::DEFAULT] =
        ErrorLogFileData(LogFile, LogFileSymLink, LogFilePeriodMultiplier);
    }
    if (Config::GetBool(ini, config, "Log.AlwaysPrintStackTraces")) {
      Logger::SetTheLogger(Logger::DEFAULT, std::make_unique<ExtendedLogger>());
      ExtendedLogger::EnabledByDefault = true;
    }

    Config::Bind(Logger::LogHeader, ini, config, "Log.Header");
    Config::Bind(Logger::LogNativeStackTrace, ini, config,
                 "Log.NativeStackTrace", true);
    Config::Bind(Logger::UseSyslog, ini, config, "Log.UseSyslog", false);
    Config::Bind(Logger::UseRequestLog, ini, config, "Log.UseRequestLog",
                 false);
    Config::Bind(Logger::AlwaysEscapeLog, ini, config, "Log.AlwaysEscapeLog",
                 true);
    Config::Bind(Logger::UseCronolog, ini, config, "Log.UseCronolog", false);
    Config::Bind(Logger::MaxMessagesPerRequest, ini,
                 config, "Log.MaxMessagesPerRequest", -1);
    Config::Bind(LogFileFlusher::DropCacheChunkSize, ini,
                 config, "Log.DropCacheChunkSize", 1 << 20);
    Config::Bind(RuntimeOption::LogHeaderMangle, ini, config,
                 "Log.HeaderMangle", 0);
    Config::Bind(AlwaysLogUnhandledExceptions, ini,
                 config, "Log.AlwaysLogUnhandledExceptions",
                 true);
    Config::Bind(NoSilencer, ini, config, "Log.NoSilencer");
    Config::Bind(RuntimeErrorReportingLevel, ini,
                 config, "Log.RuntimeErrorReportingLevel",
                 static_cast<int>(ErrorMode::HPHP_ALL));
    Config::Bind(ForceErrorReportingLevel, ini,
                 config, "Log.ForceErrorReportingLevel", 0);
    Config::Bind(AccessLogDefaultFormat, ini, config,
                 "Log.AccessLogDefaultFormat", "%h %l %u %t \"%r\" %>s %b");

    auto parseLogs = [] (const Hdf &config, const IniSetting::Map& ini,
                         const std::string &name,
                         std::map<std::string, AccessLogFileData> &logs) {
      auto parse_logs_callback = [&] (const IniSetting::Map &ini_pl,
                                      const Hdf &hdf_pl,
                                      const std::string &ini_pl_key) {
        string logName = hdf_pl.exists() && !hdf_pl.isEmpty()
                       ? hdf_pl.getName()
                       : ini_pl_key;
        string fname = Config::GetString(ini_pl, hdf_pl, "File", "", false);
        if (!fname.empty()) {
          string symlink = Config::GetString(ini_pl, hdf_pl, "SymLink", "",
                                             false);
          string format = Config::GetString(ini_pl, hdf_pl, "Format",
                                            AccessLogDefaultFormat, false);
          auto periodMultiplier = Config::GetUInt16(ini_pl, hdf_pl,
                                                    "PeriodMultiplier",
                                                    0, false);
          logs[logName] = AccessLogFileData(fname, symlink,
                                            format, periodMultiplier);


        }
      };
      Config::Iterate(parse_logs_callback, ini, config, name);
    };

    parseLogs(config, ini, "Log.Access", AccessLogs);
    RPCLogs = AccessLogs;
    parseLogs(config, ini, "Log.RPC", RPCLogs);

    Config::Bind(AdminLogFormat, ini, config, "Log.AdminLog.Format",
                 "%h %t %s %U");
    Config::Bind(AdminLogFile, ini, config, "Log.AdminLog.File");
    Config::Bind(AdminLogSymLink, ini, config, "Log.AdminLog.SymLink");
  }

  // If we generated errors while loading RelativeConfigs report those now that
  // error reporting is initialized
  if (!relConfigsError.empty()) Logger::Error(relConfigsError);

  {
    if (Config::GetInt64(ini, config, "ResourceLimit.CoreFileSizeOverride")) {
      setResourceLimit(RLIMIT_CORE, ini,  config,
                       "ResourceLimit.CoreFileSizeOverride");
    } else {
      setResourceLimit(RLIMIT_CORE, ini, config, "ResourceLimit.CoreFileSize");
    }
    setResourceLimit(RLIMIT_NOFILE, ini, config, "ResourceLimit.MaxSocket");
    setResourceLimit(RLIMIT_DATA, ini, config, "ResourceLimit.RSS");
    // These don't have RuntimeOption::xxx bindings, but we still want to be
    // able to use ini_xxx functionality on them; so directly bind to a local
    // static via Config::Bind.
    static int64_t s_core_file_size_override, s_core_file_size, s_rss = 0;
    static int32_t s_max_socket = 0;
    Config::Bind(s_core_file_size_override, ini, config,
                 "ResourceLimit.CoreFileSizeOverride", 0);
    Config::Bind(s_core_file_size, ini, config, "ResourceLimit.CoreFileSize",
                 0);
    Config::Bind(s_max_socket, ini, config, "ResourceLimit.MaxSocket", 0);
    Config::Bind(s_rss, ini, config, "ResourceLimit.RSS", 0);

    Config::Bind(SocketDefaultTimeout, ini, config,
                 "ResourceLimit.SocketDefaultTimeout", 60);
    Config::Bind(MaxSQLRowCount, ini, config, "ResourceLimit.MaxSQLRowCount",
                 0);
    Config::Bind(SerializationSizeLimit, ini, config,
                 "ResourceLimit.SerializationSizeLimit", StringData::MaxSize);
  }
  {
    // watchman
    Config::Bind(WatchmanRootSocket, ini, config, "watchman.socket.root", "");
    Config::Bind(WatchmanDefaultSocket, ini, config,
                 "watchman.socket.default", "");
  }
  {
    // Repo
    auto repoModeToStr = [](RepoMode mode) {
      switch (mode) {
        case RepoMode::Closed:
          return "--";
        case RepoMode::ReadOnly:
          return "r-";
        case RepoMode::ReadWrite:
          return "rw";
      }

      always_assert(false);
      return "";
    };

    auto parseRepoMode = [&](const std::string& repoModeStr, const char* type, RepoMode defaultMode) {
      if (repoModeStr.empty()) {
        return defaultMode;
      }
      if (repoModeStr == "--") {
        return RepoMode::Closed;
      }
      if (repoModeStr == "r-") {
        return RepoMode::ReadOnly;
      }
      if (repoModeStr == "rw") {
        return RepoMode::ReadWrite;
      }

      Logger::Error("Bad config setting: Repo.%s.Mode=%s",
                    type, repoModeStr.c_str());
      return RepoMode::ReadWrite;
    };

    // Local Repo
    static std::string repoLocalMode;
    Config::Bind(repoLocalMode, ini, config, "Repo.Local.Mode", repoModeToStr(RepoLocalMode));
    RepoLocalMode = parseRepoMode(repoLocalMode, "Local", RepoMode::ReadOnly);

    // Repo.Path
    Config::Bind(RepoPath, ini, config, "Repo.Path", RepoPath);

    // Repo.Local.Path
    Config::Bind(RepoLocalPath, ini, config, "Repo.Local.Path");
    if (RepoLocalPath.empty()) {
      const char* HHVM_REPO_LOCAL_PATH = getenv("HHVM_REPO_LOCAL_PATH");
      if (HHVM_REPO_LOCAL_PATH != nullptr) {
        RepoLocalPath = HHVM_REPO_LOCAL_PATH;
      }
    }

    // Central Repo
    static std::string repoCentralMode;
    Config::Bind(repoCentralMode, ini, config, "Repo.Central.Mode", repoModeToStr(RepoCentralMode));
    RepoCentralMode = parseRepoMode(repoCentralMode, "Central", RepoMode::ReadWrite);

    // Repo.Central.Path
    Config::Bind(RepoCentralPath, ini, config, "Repo.Central.Path");
    Config::Bind(RepoCentralFileMode, ini, config, "Repo.Central.FileMode");
    Config::Bind(RepoCentralFileUser, ini, config, "Repo.Central.FileUser");
    Config::Bind(RepoCentralFileGroup, ini, config, "Repo.Central.FileGroup");

    Config::Bind(RepoAllowFallbackPath, ini, config, "Repo.AllowFallbackPath",
                 RepoAllowFallbackPath);

    replacePlaceholders(RepoLocalPath);
    replacePlaceholders(RepoCentralPath);
    replacePlaceholders(RepoPath);

    Config::Bind(RepoJournal, ini, config, "Repo.Journal", RepoJournal);
    Config::Bind(RepoCommit, ini, config, "Repo.Commit",
                 RepoCommit);
    Config::Bind(RepoDebugInfo, ini, config, "Repo.DebugInfo", RepoDebugInfo);
    Config::Bind(RepoLitstrLazyLoad, ini, config, "Repo.LitstrLazyLoad",
                 RepoLitstrLazyLoad);
    Config::Bind(RepoAuthoritative, ini, config, "Repo.Authoritative",
                 RepoAuthoritative);
    Config::Bind(RepoBusyTimeoutMS, ini, config,
                 "Repo.BusyTimeoutMS", RepoBusyTimeoutMS);

    if (RepoPath.empty()) {
      if (!RepoLocalPath.empty()) {
        RepoPath = RepoLocalPath;
      } else if (!RepoCentralPath.empty()) {
        RepoPath = RepoCentralPath;
      } else if (auto const env = getenv("HHVM_REPO_CENTRAL_PATH")) {
        RepoPath = env;
        replacePlaceholders(RepoPath);
      } else {
        always_assert_flog(
          !RepoAuthoritative,
          "Either Repo.Path, Repo.LocalPath, or Repo.CentralPath "
          "must be set in RepoAuthoritative mode"
        );
      }
    }
  }

  if (use_jemalloc) {
    // HHProf
    Config::Bind(HHProfEnabled, ini, config, "HHProf.Enabled", false);
    Config::Bind(HHProfActive, ini, config, "HHProf.Active", false);
    Config::Bind(HHProfAccum, ini, config, "HHProf.Accum", false);
    Config::Bind(HHProfRequest, ini, config, "HHProf.Request", false);
  }

  Cfg::Load(ini, config);

  {
    // Eval
    Config::Bind(EnableXHP, ini, config, "Eval.EnableXHP", EnableXHP);
    Config::Bind(TimeoutsUseWallTime, ini, config, "Eval.TimeoutsUseWallTime",
                 true);
    static std::string jitSerdesMode;
    Config::Bind(jitSerdesMode, ini, config, "Eval.JitSerdesMode", "Off");

    EvalJitSerdesMode = [&] {
      #define X(x) if (jitSerdesMode == #x) return JitSerdesMode::x
      X(Serialize);
      X(SerializeAndExit);
      X(Deserialize);
      X(DeserializeOrFail);
      X(DeserializeOrGenerate);
      X(DeserializeAndDelete);
      X(DeserializeAndExit);
      #undef X
      return JitSerdesMode::Off;
    }();
    Config::Bind(EvalJitSerdesFile, ini, config,
                 "Eval.JitSerdesFile", EvalJitSerdesFile);
    replacePlaceholders(EvalJitSerdesFile);

    // DumpPreciseProfileData defaults to true only when we can possibly write
    // profile data to disk.  It can be set to false to avoid the performance
    // penalty of only running the interpreter during retranslateAll.  We will
    // assume that DumpPreciseProfileData implies (JitSerdesMode::Serialize ||
    // JitSerdesMode::SerializeAndExit), to avoid checking too many flags in a
    // few places.  The config file should never need to explicitly set
    // DumpPreciseProfileData to true.
    auto const couldDump = !EvalJitSerdesFile.empty() &&
      (isJitSerializing() ||
       (EvalJitSerdesMode == JitSerdesMode::DeserializeOrGenerate));
    Config::Bind(DumpPreciseProfData, ini, config,
                 "Eval.DumpPreciseProfData", couldDump);
    Config::Bind(ProfDataTTLHours, ini, config,
                 "Eval.ProfDataTTLHours", ProfDataTTLHours);
    Config::Bind(ProfDataTag, ini, config, "Eval.ProfDataTag", ProfDataTag);

    Config::Bind(CheckSymLink, ini, config, "Eval.CheckSymLink", true);
    Config::Bind(TrustAutoloaderPath, ini, config,
                 "Eval.TrustAutoloaderPath", false);

#define F(type, name, defaultVal) \
    Config::Bind(Eval ## name, ini, config, "Eval."#name, defaultVal);
    EVALFLAGS()
#undef F

    if (UNLIKELY(RO::EvalRecordReplay && RO::EvalReplay)) {
      return Replayer::onRuntimeOptionLoad(ini, config, cmd);
    }

    if (Cfg::Jit::SerdesModeForceOff) EvalJitSerdesMode = JitSerdesMode::Off;
    if (!EvalEnableReusableTC) EvalReusableTCPadding = 0;
    if (numa_num_nodes <= 1) {
      EvalEnableNuma = false;
    }

    if (!Cfg::Server::ForkingEnabled && ServerExecutionMode()) {
      // Only use hugetlb pages when we don't fork().
      low_2m_pages(EvalMaxLowMemHugePages);
      high_2m_pages(EvalMaxHighArenaHugePages);
    }
#if USE_JEMALLOC_EXTENT_HOOKS
    g_useTHPUponHugeTLBFailure =
      Config::GetBool(ini, config, "Eval.UseTHPUponHugeTLBFailure",
                      g_useTHPUponHugeTLBFailure);
#endif
    s_enable_static_arena =
      Config::GetBool(ini, config, "Eval.UseTLStaticArena", true);

    replacePlaceholders(EvalEmbeddedDataExtractPath);
    replacePlaceholders(EvalEmbeddedDataFallbackPath);

    if (!jit::mcgen::retranslateAllEnabled()) {
      Cfg::Jit::WorkerThreads = 0;
      if (EvalJitSerdesMode != JitSerdesMode::Off) {
        if (Cfg::Server::Mode) {
          Logger::Warning("Eval.JitSerdesMode reset from " + jitSerdesMode +
                          " to off, becasue JitRetranslateAll isn't enabled.");
        }
        EvalJitSerdesMode = JitSerdesMode::Off;
      }
      EvalJitSerdesFile.clear();
      DumpPreciseProfData = false;
    }
    Cfg::Jit::PGOUseAddrCountedCheck &= addr_encodes_persistency;
    if (EvalSanitizeReqHeap) {
      HeapObjectSanitizer::install_signal_handler();
    }

    HardwareCounter::Init(EvalProfileHWEnable,
                          url_decode(EvalProfileHWEvents.data(),
                                     EvalProfileHWEvents.size()).toCppString(),
                          false,
                          EvalProfileHWExcludeKernel,
                          EvalProfileHWFastReads,
                          EvalProfileHWExportInterval);

    Config::Bind(RecordCodeCoverage, ini, config, "Eval.RecordCodeCoverage");
    if (Cfg::Jit::Enabled && RecordCodeCoverage) {
      throw std::runtime_error("Code coverage is not supported with "
        "Eval.Jit=true");
    }
    Config::Bind(DisableSmallAllocator, ini, config,
                 "Eval.DisableSmallAllocator", DisableSmallAllocator);
    SetArenaSlabAllocBypass(DisableSmallAllocator);
    EvalSlabAllocAlign = folly::nextPowTwo(EvalSlabAllocAlign);
    EvalSlabAllocAlign = std::min(EvalSlabAllocAlign,
                                  decltype(EvalSlabAllocAlign){4096});

    if (RecordCodeCoverage) CheckSymLink = true;
    Config::Bind(CodeCoverageOutputFile, ini, config,
                 "Eval.CodeCoverageOutputFile");
    // NB: after we know the value of RepoAuthoritative.
    Config::Bind(EnableArgsInBacktraces, ini, config,
                 "Eval.EnableArgsInBacktraces", !RepoAuthoritative);
    Config::Bind(EvalAuthoritativeMode, ini, config, "Eval.AuthoritativeMode",
                 false);

    Config::Bind(CheckCLIClientCommands, ini, config, "Eval.CheckCLIClientCommands", 1);
    if (RepoAuthoritative) {
      EvalAuthoritativeMode = true;
    }
  }
  {
    // CodeCache
    using jit::CodeCache;
    Config::Bind(CodeCache::ASize, ini, config, "Eval.JitASize", 60 << 20);
    Config::Bind(CodeCache::AColdSize, ini, config, "Eval.JitAColdSize",
                 24 << 20);
    Config::Bind(CodeCache::AFrozenSize, ini, config, "Eval.JitAFrozenSize",
                 40 << 20);
    Config::Bind(CodeCache::ABytecodeSize, ini, config,
                 "Eval.JitABytecodeSize", 0);
    Config::Bind(CodeCache::GlobalDataSize, ini, config,
                 "Eval.JitGlobalDataSize", CodeCache::ASize >> 2);

    Config::Bind(CodeCache::MapTCHuge, ini, config, "Eval.MapTCHuge",
                 hugePagesSoundNice());

    Config::Bind(CodeCache::TCNumHugeHotMB, ini, config,
                 "Eval.TCNumHugeHotMB", 64);
    Config::Bind(CodeCache::TCNumHugeMainMB, ini, config,
                 "Eval.TCNumHugeMainMB", 16);
    Config::Bind(CodeCache::TCNumHugeColdMB, ini, config,
                 "Eval.TCNumHugeColdMB", 4);

    Config::Bind(CodeCache::AutoTCShift, ini, config, "Eval.JitAutoTCShift", 1);
  }
  {
    // Server
    if (GetServerPrimaryIPv4().empty() && GetServerPrimaryIPv6().empty()) {
      throw std::runtime_error("Unable to resolve the server's IPv4 or IPv6 address");
    }

    // These things should be in Cfg PostProcess methods. But because they use
    // normalizeDir they can't
    {
      Cfg::Server::SourceRoot = FileUtil::normalizeDir(Cfg::Server::SourceRoot);
      if (Cfg::Server::SourceRoot.empty()) {
        Cfg::Server::SourceRoot = Cfg::ServerLoader::SourceRootDefault();
      }

      for (unsigned int i = 0; i < Cfg::Server::IncludeSearchPaths.size(); i++) {
        Cfg::Server::IncludeSearchPaths[i] = FileUtil::normalizeDir(Cfg::Server::IncludeSearchPaths[i]);
      }
      Cfg::Server::IncludeSearchPaths.insert(Cfg::Server::IncludeSearchPaths.begin(), ".");

      Cfg::Server::FontPath = FileUtil::normalizeDir(Cfg::Server::FontPath);
    }
  }

  VirtualHost::SortAllowedDirectories(Cfg::Server::AllowedDirectories);
  {
    auto vh_callback = [] (const IniSettingMap &ini_vh, const Hdf &hdf_vh,
                           const std::string &ini_vh_key) {
      if (VirtualHost::IsDefault(ini_vh, hdf_vh, ini_vh_key)) {
        VirtualHost::GetDefault().init(ini_vh, hdf_vh, ini_vh_key);
        VirtualHost::GetDefault().addAllowedDirectories(Cfg::Server::AllowedDirectories);
      } else {
        auto host = std::make_shared<VirtualHost>(ini_vh, hdf_vh, ini_vh_key);
        host->addAllowedDirectories(Cfg::Server::AllowedDirectories);
        VirtualHosts.push_back(host);
      }
    };
    // Virtual Hosts have to be iterated in order. Because only the first
    // one that matches in the VirtualHosts vector gets applied and used.
    // Hdf's and ini (via Variant arrays) internal storage handles ordering
    // naturally (as specified top to bottom in the file and left to right on
    // the command line.
    Config::Iterate(vh_callback, ini, config, "VirtualHost");
    Cfg::Server::LowestMaxPostSize = VirtualHost::GetLowestMaxPostSize();
  }
  {
    // IpBlocks
    IpBlocks = std::make_shared<IpBlockMap>(ini, config);
  }
  {
    ReadSatelliteInfo(ini, config, SatelliteServerInfos);
  }
  {
    // Static File

    hphp_string_imap<std::string> staticFileDefault;
    staticFileDefault["css"] = "text/css";
    staticFileDefault["gif"] = "image/gif";
    staticFileDefault["html"] = "text/html";
    staticFileDefault["jpeg"] = "image/jpeg";
    staticFileDefault["jpg"] = "image/jpeg";
    staticFileDefault["mp3"] = "audio/mpeg";
    staticFileDefault["png"] = "image/png";
    staticFileDefault["tif"] = "image/tiff";
    staticFileDefault["tiff"] = "image/tiff";
    staticFileDefault["txt"] = "text/plain";
    staticFileDefault["zip"] = "application/zip";

    Config::Bind(StaticFileExtensions, ini, config, "StaticFile.Extensions",
                 staticFileDefault);

    auto matches_callback = [](const IniSettingMap& ini_m, const Hdf& hdf_m,
                               const std::string& /*ini_m_key*/) {
      FilesMatches.push_back(std::make_shared<FilesMatch>(ini_m, hdf_m));
    };
    Config::Iterate(matches_callback, ini, config, "StaticFile.FilesMatch");
  }
  {
    // PhpFile
    Config::Bind(PhpFileExtensions, ini, config, "PhpFile.Extensions");
  }
  {
    // Admin Server
    Config::Bind(AdminServerIP, ini, config, "AdminServer.IP", Cfg::Server::IP);
    Config::Bind(AdminServerPort, ini, config, "AdminServer.Port", 0);
    Config::Bind(AdminThreadCount, ini, config, "AdminServer.ThreadCount", 1);
    Config::Bind(AdminServerEnableSSLWithPlainText, ini, config,
                 "AdminServer.EnableSSLWithPlainText", false);
    Config::Bind(AdminServerStatsNeedPassword, ini, config,
                 "AdminServer.StatsNeedPassword", AdminServerStatsNeedPassword);
    AdminPassword = Config::GetString(ini, config, "AdminServer.Password");
    AdminPasswords = Config::GetSet(ini, config, "AdminServer.Passwords");
    HashedAdminPasswords =
      Config::GetSet(ini, config, "AdminServer.HashedPasswords");
    Config::Bind(AdminDumpPath, ini, config,
                 "AdminServer.DumpPath", "/tmp/hhvm_admin_dump");
  }
  {
    // Http
    Config::Bind(HttpDefaultTimeout, ini, config, "Http.DefaultTimeout", 30);
    Config::Bind(HttpSlowQueryThreshold, ini, config, "Http.SlowQueryThreshold",
                 5000);
  }
  {
    // Debug

    Config::Bind(NativeStackTrace, ini, config, "Debug.NativeStackTrace");
    StackTrace::Enabled = NativeStackTrace;
    Config::Bind(ServerErrorMessage, ini, config, "Debug.ServerErrorMessage");
    Config::Bind(RecordInput, ini, config, "Debug.RecordInput");
    Config::Bind(ClearInputOnSuccess, ini, config, "Debug.ClearInputOnSuccess",
                 true);
    Config::Bind(ProfilerOutputDir, ini, config, "Debug.ProfilerOutputDir",
                 "/tmp");
    Config::Bind(CoreDumpEmail, ini, config, "Debug.CoreDumpEmail");
    Config::Bind(CoreDumpReport, ini, config, "Debug.CoreDumpReport", true);
    if (CoreDumpReport) {
      install_crash_reporter();
    }
    // Binding default dependent on whether we are using an OSS build or
    // not, and that is set at initialization time of CoreDumpReportDirectory.
    Config::Bind(CoreDumpReportDirectory, ini, config,
                 "Debug.CoreDumpReportDirectory", CoreDumpReportDirectory);
    std::ostringstream stack_trace_stream;
    stack_trace_stream << CoreDumpReportDirectory << "/stacktrace."
                       << (int64_t)getpid() << ".log";
    StackTraceFilename = stack_trace_stream.str();

    Config::Bind(StackTraceTimeout, ini, config, "Debug.StackTraceTimeout", 0);
    Config::Bind(RemoteTraceOutputDir, ini, config,
                 "Debug.RemoteTraceOutputDir", "/tmp");
    Config::Bind(TraceFunctions, ini, config,
                 "Debug.TraceFunctions", TraceFunctions);
  }
  {
    // Stats
    Config::Bind(EnableStats, ini, config, "Stats.Enable",
                 false); // main switch
    Config::Bind(EnableAPCStats, ini, config, "Stats.APC", false);
    Config::Bind(EnableWebStats, ini, config, "Stats.Web");
    Config::Bind(EnableMemoryStats, ini, config, "Stats.Memory");
    Config::Bind(EnableSQLStats, ini, config, "Stats.SQL");
    Config::Bind(EnableSQLTableStats, ini, config, "Stats.SQLTable");
    Config::Bind(EnableNetworkIOStatus, ini, config, "Stats.NetworkIO");
    Config::Bind(StatsXSL, ini, config, "Stats.XSL");
    Config::Bind(StatsXSLProxy, ini, config, "Stats.XSLProxy");
    Config::Bind(StatsSlotDuration, ini, config, "Stats.SlotDuration", 10 * 60);
    Config::Bind(StatsMaxSlot, ini, config, "Stats.MaxSlot",
                 12 * 6); // 12 hours
    StatsSlotDuration = std::max(1u, StatsSlotDuration);
    StatsMaxSlot = std::max(2u, StatsMaxSlot);
    Config::Bind(StatsTrackedKeys, ini, config, "Stats.TrackedKeys");
    Config::Bind(ProfilerTraceBuffer, ini, config, "Stats.ProfilerTraceBuffer",
                 2000000);
    Config::Bind(ProfilerTraceExpansion, ini, config,
                 "Stats.ProfilerTraceExpansion", 1.2);
    Config::Bind(ProfilerMaxTraceBuffer, ini, config,
                 "Stats.ProfilerMaxTraceBuffer", 0);
    Config::Bind(TrackPerUnitMemory, ini, config,
                 "Stats.TrackPerUnitMemory", false);
  }
  {
    Config::Bind(ServerVariables, ini, config, "ServerVariables");
    Config::Bind(EnvVariables, ini, config, "EnvVariables");
  }
  {
    // Sandbox
    Config::Bind(SandboxMode, ini, config, "Sandbox.SandboxMode");
    Config::Bind(SandboxPattern, ini, config, "Sandbox.Pattern");
    SandboxPattern = format_pattern(SandboxPattern, true);
    Config::Bind(SandboxHome, ini, config, "Sandbox.Home");
    Config::Bind(SandboxFallback, ini, config, "Sandbox.Fallback");
    Config::Bind(SandboxConfFile, ini, config, "Sandbox.ConfFile");
    Config::Bind(SandboxFromCommonRoot, ini, config, "Sandbox.FromCommonRoot");
    Config::Bind(SandboxDirectoriesRoot, ini, config,
                 "Sandbox.DirectoriesRoot");
    Config::Bind(SandboxLogsRoot, ini, config, "Sandbox.LogsRoot");
    Config::Bind(SandboxServerVariables, ini, config,
                 "Sandbox.ServerVariables");
    Config::Bind(SandboxDefaultUserFile, ini, config,
                 "Sandbox.DefaultUserFile");
    Config::Bind(SandboxHostAlias, ini, config, "Sandbox.HostAlias");
  }
  {
    // Mail
    Config::Bind(SendmailPath, ini, config, "Mail.SendmailPath",
                 "/usr/lib/sendmail -t -i");
    Config::Bind(MailForceExtraParameters, ini, config,
                 "Mail.ForceExtraParameters");
  }
  {
    // SimpleXML
    Config::Bind(SimpleXMLEmptyNamespaceMatchesAll, ini, config,
                 "SimpleXML.EmptyNamespaceMatchesAll", false);
  }
#ifdef HHVM_FACEBOOK
  {
    // ThriftFBServer
    Config::Bind(ThriftFBServerWorkerThreads, ini, config,
                 "ThriftFBServer.WorkerThreads", 1);
    Config::Bind(ThriftFBServerPoolThreads, ini, config, "ThriftFBServer.PoolThreads",
                 1);
                 
    // Fb303Server
    Config::Bind(EnableFb303Server, ini, config, "Fb303Server.Enable",
                 EnableFb303Server);
    Config::Bind(Fb303ServerPort, ini, config, "Fb303Server.Port", 0);
    Config::Bind(Fb303ServerIP, ini, config, "Fb303Server.IP");
    Config::Bind(Fb303ServerWorkerThreads, ini, config,
                 "Fb303Server.WorkerThreads", 1);
    Config::Bind(Fb303ServerPoolThreads, ini, config, "Fb303Server.PoolThreads",
                 1);
    Config::Bind(Fb303ServerExposeSensitiveMethods, ini, config,
                 "Fb303Server.ExposeSensitiveMethods", Fb303ServerExposeSensitiveMethods);

    Config::Bind(ThreadTuneDebug, ini, config,
                 "ThreadTuneDebug", ThreadTuneDebug);
    Config::Bind(ThreadTuneSkipWarmup, ini, config,
                 "ThreadTuneSkipWarmup", ThreadTuneSkipWarmup);
    Config::Bind(ThreadTuneAdjustmentPct, ini, config,
                 "ThreadTuneAdjustmentPct", ThreadTuneAdjustmentPct);
    Config::Bind(ThreadTuneAdjustmentDownPct, ini, config,
                 "ThreadTuneAdjustmentDownPct", ThreadTuneAdjustmentDownPct);
    Config::Bind(ThreadTuneStepPct, ini, config,
                 "ThreadTuneStepPct", ThreadTuneStepPct);
    Config::Bind(ThreadTuneCPUThreshold, ini, config,
                 "ThreadTuneCPUThreshold", ThreadTuneCPUThreshold);
    Config::Bind(ThreadTuneThreadUtilizationThreshold, ini, config,
                 "ThreadTuneThreadUtilizationThreshold", ThreadTuneThreadUtilizationThreshold);
  }
#endif

  {
    // Xenon
    Config::Bind(XenonPeriodSeconds, ini, config, "Xenon.Period", 0.0);
    Config::Bind(XenonRequestFreq, ini, config, "Xenon.RequestFreq", 1);
    Config::Bind(XenonForceAlwaysOn, ini, config, "Xenon.ForceAlwaysOn", false);
    Config::Bind(XenonTrackActiveWorkers, ini, config, "Xenon.TrackActiveWorkers", false);
  }
  {
    // Strobelight
    Config::Bind(StrobelightEnabled, ini, config, "Strobelight.Enabled", false);
  }
  {
    // Profiling
    Config::Bind(SetProfileNullThisObject, ini, config,
                 "SetProfile.NullThisObject", true);
  }

  Config::Bind(TzdataSearchPaths, ini, config, "TzdataSearchPaths");

  Config::Bind(CustomSettings, ini, config, "CustomSettings");

  // Run initializers dependent on options, e.g., resizing atomic maps/vectors.
  refineStaticStringTableSize();
  InitFiniNode::ProcessPostRuntimeOptions();

  // **************************************************************************
  //                                  DANGER
  //
  // Only bind Mode::Config here!
  // These settings are process-wide, while those need to be thread-local since
  // they are per-request. They should go into RequestInjectionData. Getting
  // this wrong will cause subtle breakage -- in particular, it probably will
  // not show up in CLI mode, since everything there tends to be single
  // theaded.
  // **************************************************************************
  // Language and Misc Configuration Options
  IniSetting::Bind(IniSetting::CORE, IniSetting::Mode::Config, "expose_php",
                   &Cfg::Server::ExposeHPHP);

  // Data Handling
  IniSetting::Bind(IniSetting::CORE, IniSetting::Mode::Config,
                   "post_max_size",
                   IniSetting::SetAndGet<int64_t>(
                     nullptr,
                     []() {
                       return VirtualHost::GetMaxPostSize();
                     },
                     &Cfg::Server::MaxPostSize
                   ));
  IniSetting::Bind(IniSetting::CORE, IniSetting::Mode::Config,
                   "always_populate_raw_post_data",
                   &Cfg::Server::AlwaysPopulateRawPostData);

  // Paths and Directories
  IniSetting::Bind(IniSetting::CORE, IniSetting::Mode::Config,
                   "doc_root", &Cfg::Server::SourceRoot);
  IniSetting::Bind(IniSetting::CORE, IniSetting::Mode::Config,
                   "sendmail_path", &RuntimeOption::SendmailPath);

  // FastCGI
  IniSetting::Bind(IniSetting::CORE, IniSetting::Mode::Config,
                   "pid", &RuntimeOption::PidFile);

  // File Uploads
  IniSetting::Bind(IniSetting::CORE, IniSetting::Mode::Config,
                   "file_uploads", "true",
                   &Cfg::Server::UploadEnableFileUploads);
  IniSetting::Bind(IniSetting::CORE, IniSetting::Mode::Config,
                   "upload_tmp_dir", &Cfg::Server::UploadTmpDir);
  IniSetting::Bind(IniSetting::CORE, IniSetting::Mode::Config,
                   "upload_max_filesize",
                   IniSetting::SetAndGet<std::string>(
                     [](const std::string& value) {
                       return ini_on_update(
                         value, Cfg::Server::UploadMaxFileSize);
                     },
                     []() {
                       return convert_long_to_bytes(
                         VirtualHost::GetUploadMaxFileSize());
                     }
                   ));
  // Filesystem and Streams Configuration Options
  IniSetting::Bind(IniSetting::CORE, IniSetting::Mode::Config,
                   "allow_url_fopen",
                   IniSetting::SetAndGet<std::string>(
                     [](const std::string& /*value*/) { return false; },
                     []() { return "1"; }));

  // HPHP specific
  IniSetting::Bind(IniSetting::CORE, IniSetting::Mode::Constant,
                   "hphp.compiler_id",
                   IniSetting::SetAndGet<std::string>(
                     [](const std::string& /*value*/) { return false; },
                     []() { return compilerId().begin(); }));
  IniSetting::Bind(IniSetting::CORE, IniSetting::Mode::Constant,
                   "hphp.compiler_timestamp",
                   IniSetting::SetAndGet<int64_t>(
                     [](const int64_t& /*value*/) { return false; },
                     []() { return compilerTimestamp(); }));
  IniSetting::Bind(IniSetting::CORE, IniSetting::Mode::Constant,
                   "hphp.compiler_version",
                   IniSetting::SetAndGet<std::string>(
                     [](const std::string& /*value*/) { return false; },
                     []() { return getHphpCompilerVersion(); }));
  IniSetting::Bind(IniSetting::CORE, IniSetting::Mode::Constant,
                   "hphp.cli_server_api_version",
                   IniSetting::SetAndGet<uint64_t>(
                     [](const uint64_t /*value*/) { return false; },
                     []() { return cli_server_api_version(); }));
  IniSetting::Bind(
    IniSetting::CORE, IniSetting::Mode::Constant, "hphp.build_id",
    IniSetting::SetAndGet<std::string>(
      [](const std::string& /*value*/) { return false; },
      nullptr,
      &RuntimeOption::BuildId)
  );
  IniSetting::Bind(IniSetting::CORE, IniSetting::Mode::Config,
                   "notice_frequency",
                   &Cfg::ErrorHandling::NoticeFrequency);
  IniSetting::Bind(IniSetting::CORE, IniSetting::Mode::Config,
                   "warning_frequency",
                   &Cfg::ErrorHandling::WarningFrequency);
  IniSetting::Bind(IniSetting::CORE, IniSetting::Mode::Constant,
                   "hhvm.build_type",
                   IniSetting::SetAndGet<std::string>(
    [](const std::string&) {
      return false;
    },
    []() {
      return s_hhvm_build_type.c_str();
    }
  ));

  ExtensionRegistry::moduleLoad(ini, config);
  initialize_apc();

  if (TraceFunctions.size()) Trace::ensureInit(getTraceOutputFile());

  if (Cfg::Jit::EnableRenameFunction && RO::RepoAuthoritative) {
      throw std::runtime_error("Can't use Eval.JitEnableRenameFunction if "
                               " RepoAuthoritative is turned on");
  }

  // Bespoke array-likes

  auto const enable_logging = RO::EvalBespokeArrayLikeMode != 0;
  bespoke::setLoggingEnabled(enable_logging);
  specializeVanillaDestructors();

  // Coeffects
  CoeffectsConfig::init(RO::EvalCoeffectEnforcementLevels);

  // Initialize defaults for repo-specific parser configuration options.
  RepoOptions::setDefaults(config, ini);

  if (UNLIKELY(RO::EvalRecordReplay && RO::EvalRecordSampleRate)) {
    Recorder::onRuntimeOptionLoad(ini, config, cmd);
  }
}

///////////////////////////////////////////////////////////////////////////////

namespace {

ServiceData::CounterCallback s_build_info([](ServiceData::CounterMap& counters) {
  if (RuntimeOption::BuildId.empty()) return;

  std::vector<std::string> pieces;
  pieces.reserve(4);
  // ignore-job-rev-timestamp
  folly::split('-', RuntimeOption::BuildId, pieces, true);

  counters.emplace("admin.build.job", folly::to<int64_t>(pieces.at(1)));
  counters.emplace("admin.build.rev", folly::to<int64_t>(pieces.at(2)));

  std::tm t{};
  t.tm_isdst = -1;
  std::istringstream ss{std::move(pieces.at(3))};
  ss >> std::get_time(&t, "%Y%m%d%H%M%S");
  if (!ss.fail()) {
    counters.emplace("admin.build.age", std::time(nullptr) - mktime(&t));
  }
});

}

///////////////////////////////////////////////////////////////////////////////

uintptr_t lowArenaMinAddr() {
  const char* str = getenv("HHVM_LOW_ARENA_START");
  if (str == nullptr) {
#if !defined(INSTRUMENTED_BUILD) && defined(USE_LOWPTR)
   return 1u << 30;
#else
   return 1u << 31;
#endif
  }
  uintptr_t start = 0;
  if (sscanf(str, "0x%lx", &start) == 1) return start;
  if (sscanf(str, "%lu", &start) == 1) return start;
  fprintf(stderr, "Bad environment variable HHVM_LOW_ARENA_START: %s\n", str);
  abort();
}

///////////////////////////////////////////////////////////////////////////////

}
