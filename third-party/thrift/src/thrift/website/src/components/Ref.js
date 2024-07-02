/**
 * (c) Meta Platforms, Inc. and affiliates. Confidential and proprietary.
 */

import React from 'react';
import Link from '@docusaurus/Link';
import styles from './Ref.module.css';
import CodeBlock from '@theme/CodeBlock';
import Admonition from '@theme/Admonition';

// MDXHeading use:
//import MDXHeading from '@theme/Heading';
//return MDXHeading("h3")({children: "Lorum ipsum"});


export function ParentLinks({data}) {
  const parents = data.parent_links || [];
  if (parents.length == 0) {
    return null;
  }

  return (
    <span className={styles.refhighlight} >
      <small>
        &nbsp;
        {parents.map(pl => {
          return (
            <span><Link to={pl.href} dangerouslySetInnerHTML={{__html: pl.text_html}} />&nbsp;&gt;&nbsp;</span>
          );
        })}
      </small>
    </span>
  );
}

export function Heading({data}) {
  return (
    <h1>
      <code dangerouslySetInnerHTML={{__html: data.compoundname_html}} />
    </h1>
  );
}

export function Decl({data}) {
  return <CodeBlock language="cpp" children={data.declaration_raw} />
}

export function Desc({data}) {
  if (!data.description) {
    return null;
  }
  if (data.description.detailed_html.length == 0) {
    return null;
  }
  return <div dangerouslySetInnerHTML={{__html: data.description.detailed_html}} />
}

export function Bases({data}) {
  const publicBases = (data.basecompoundref || []).filter(b => b.prot == "public");
  if (!publicBases || publicBases.length == 0) {
    return null;
  }
  return (
    <div>
      <h3>
        {publicBases.length == 1 ? "Base class" : "Base classes"}
      </h3>
      <table class={styles.refTable}>
        {publicBases.map(b => {
          return (
            <tr>
              <td>
                {b.href ?
                  <Link to={b.href}>
                    <code dangerouslySetInnerHTML={{__html: b.compoundname_html || b.text_html}} />
                  </Link>
                 : <code dangerouslySetInnerHTML={{__html: b.compoundname_html || b.text_html}} />
                }
              </td>
              <td>
                {b.virtual == "virtual" &&
                  <small>
                    (virtual)
                  </small>
                }
              </td>
            </tr>
          );
        })}
      </table>
    </div>
  );
}

export function TemplateParameters({data}) {
  const tds = (data.description || {}).templateparam
  if (!tds || tds.length == 0) {
    return null;
  }

  // todo td.direction
  return (
    <div>
      <h3>
        Template parameters
      </h3>
      <table class={styles.refTable}>
        {tds.map(td => {
          return (
            <tr>
              <td>
                <code dangerouslySetInnerHTML={{__html: td.names_html.join(',')}} />
              </td>
              <td dangerouslySetInnerHTML={{__html: td.description_html}} />
            </tr>
          );
        })}
      </table>
    </div>
  );
}

export function DisplayEnum({data}) {
  const values = data.enumvalue || [];
  if (values.length == 0) {
    return <em>empty enum</em>;
  }
  const lead = (data.strong == "yes" ? "enum class" : "enum");
  if (values.length == 1) {
    const s = lead + " { " + values[0].name_html + " }";
    return <code dangerouslySetInnerHTML={{__html: s}} />
  }
  const lines = values.map(value => "  " + value.name_html + ",").join("\n");
  const s = lead + " {\n" + lines + "\n}";
  return <code><pre dangerouslySetInnerHTML={{__html: s}} /></code>
}

export function MemberTypes({data}) {
  const publicTypes = (data.types || []).filter(t => t.prot == "public");
  const publicEnums = (data.enums || []).filter(t => t.prot == "public");
  if (publicTypes.length == 0 && publicEnums.length == 0) {
    return null;
  }

  return (
    <div>
      <h3>
        Member types
      </h3>
      <table class={styles.refTable}>
        <tr>
          <th>
            Type
          </th>
          <th>
            Definition
          </th>
        </tr>
        {publicTypes.map(type => {
          return (
            <tr>
              <td>
                <code dangerouslySetInnerHTML={{__html: type.name_html}} />
              </td>
              <td>
                <code dangerouslySetInnerHTML={{__html: type.type_html}} />
              </td>
            </tr>
          );
        })}
        {publicEnums.map(type => {
          return (
            <tr>
              <td>
                <code dangerouslySetInnerHTML={{__html: type.name_html}} />
              </td>
              <td>
                <DisplayEnum data={type} />
              </td>
            </tr>
          );
        })}
      </table>
    </div>
  );
}

export function Overloadgroup({data}) {
  const publicOverloadsets = data.overloadsets.filter(f => f.minprot == "public");
  if (publicOverloadsets.length == 0) {
    return null;
  }
  return (
    <div>
      {data.groupname_html != "" &&
        <h4 dangerouslySetInnerHTML={{__html: data.groupname_html}} />
      }
      <table class={styles.refTable}>
        <tbody>
          {publicOverloadsets.map(os => {
            return (
              <tr>
                <td>
                  <Link to={os.href}>
                    <code dangerouslySetInnerHTML={{__html: os.displayname_html}} />
                  </Link>
                </td>
                <td>
                  <div dangerouslySetInnerHTML={{__html: os.brief_html}} />
                </td>
              </tr>
            );
          })}
        </tbody>
      </table>
    </div>
  );
}

export function MemberFunctions({data}) {
  const overloadgroups = data.overloadgroups || [];
  if (overloadgroups.length == 0) {
    return null;
  }
  return (
    <div>
      <h3>
        Member functions
      </h3>
      {overloadgroups.map(og => <Overloadgroup data={og} />)}
    </div>
  );
}

export function FreeClasses({data}) {
  const freeClasses = data.innerclass || []
  const unDetail = freeClasses.filter(c => !c.text_html.includes("detail::") && c.href && !c.is_transitive_innerclass);
  if (unDetail.length == 0) {
    return null;
  }
  return (
    <div>
      <h3>
        Classes
      </h3>
      <table class={styles.refTable}>
        <tbody>
          {unDetail.map(cr => {
            return (
              <tr>
                <td>
                  <Link to={cr.href}>
                    <code dangerouslySetInnerHTML={{__html: cr.text_html}} />
                  </Link>
                </td>
              </tr>
            );
          })}
        </tbody>
      </table>
    </div>
  );
}

export function FreeTypedefs({data}) {
  const freeTypedefs = data.free_typedefs || []
  const toShow = freeTypedefs.filter(t => t.prot == "public")
  if (toShow.length == 0) {
    return null;
  }
  return (
    <div>
      <h3>
        Typedefs
      </h3>
      <table class={styles.refTable}>
        <tbody>
          {toShow.map(t => {
            return (
              <tr>
                <td>
                  <code dangerouslySetInnerHTML={{__html: t.definition_html}} />
                </td>
              </tr>
            );
          })}
        </tbody>
      </table>
    </div>
  );
}

export function FreeFunctions({data}) {
  const freeGroups = data.free_overloadgroups || [];
  if (freeGroups.length == 0) {
    return null;
  }
  return (
    <div>
      <h3>
        File functions
      </h3>
      {freeGroups.map(og => <Overloadgroup data={og} />)}
    </div>
  );
}

function MacroSig(m) {
  if ("param" in m) {
    const defnames = m.param.map(p => p.defname_html)
    return `${m.name_html}(${defnames.join(", ")})`;
  }
  return m.name_html;
}

export function Macros({data}) {
  const macros = (data.defines || {}).macros || [];
  const toShow = macros.filter(m => m && !m.name_html.includes("DETAIL"));
  if (toShow.length == 0) {
    return null;
  }
  return (
    <div>
      <h3>
        Macros
      </h3>
      <table class={styles.refTable}>
        <tbody>
          {toShow.map(macro => {
            return (
              <tr>
                <td>
                  <code dangerouslySetInnerHTML={{__html: MacroSig(macro)}} />
                </td>
                <td>
                  {"description" in macro &&
                    <div dangerouslySetInnerHTML={{__html: macro.description.detailed_html}} />
                  }
                </td>
              </tr>
            );
          })}
        </tbody>
      </table>
    </div>
  );
}

export function FreeVariables({data}) {
  const vs = data.free_vars || []
  if (vs.length == 0) {
    return null;
  }
  return (
    <div>
      <h3>
        Free variables
      </h3>
      <table class={styles.refTable}>
        <tbody>
          {vs.map(v => {
            return (
              <tr>
                <td>
                  <code dangerouslySetInnerHTML={{__html: v.name_html}} />
                </td>
                <td>
                  {"description" in v &&
                    <div>
                    <div dangerouslySetInnerHTML={{__html: v.description.brief_html}} />
                    <div dangerouslySetInnerHTML={{__html: v.description.detailed_html}} />
                    </div>
                  }
                </td>
              </tr>
            );
          })}
        </tbody>
      </table>
    </div>
  );
}

export function Example({refcode}) {
  if (!refcode) {
    return null;
  }
  return (
    <div>
      <h3>
        Example
      </h3>
      <CodeBlock language="cpp" children={refcode.src_raw} />
      <CodeBlock language="sh" children={refcode.buck_test_cmd_raw} />
    </div>
  );
}

export function MethodDecls({data}) {
  const funcs = (data.functions || []).filter(f => f.prot == "public")
  var i = 0;
  return (
    <table class={styles.refTable}>
      {funcs.map(f => {
        return (
          <tr className={styles.topAlign} >
            <td>
              <CodeBlock language="cpp" children={f.definition.simple_raw} />
            </td>
            <td>
              <small>
                ({++i})
              </small>
            </td>
          </tr>
        );
      })}
    </table>
  );
}

export function MethodDescription({f}) {
  return (
    <div>
      <div className={styles.lighthighlight} >
        <pre>
          <code dangerouslySetInnerHTML={{__html: f.definition.detailed_html}} />
        </pre>
      </div>
      {f.description &&
        <div>
          <div dangerouslySetInnerHTML={{__html: f.description.brief_html}} />
          <div dangerouslySetInnerHTML={{__html: f.description.detailed_html}} />
          {f.description.param &&
            <table class={styles.refTable}>
              <td>
                <b>Parameters</b>
              </td>
              {f.description.param.map(p => (
                <tr>
                  <td>
                    <code dangerouslySetInnerHTML={{__html: p.names_html.join(",")}} />
                  </td>
                  <td>
                    <div dangerouslySetInnerHTML={{__html: p.description_html}} />
                  </td>
                </tr>
              ))}
            </table>
          }
          {f.description.preconditions_html &&
            <table class={styles.refTable}>
              <td>
                <b>Preconditions</b>
              </td>
              {f.description.preconditions_html.map(e => (
                <tr>
                  <td dangerouslySetInnerHTML={{__html: e}} />
                </tr>
              ))}
            </table>
          }
          {f.description.return_html &&
            <table class={styles.refTable}>
              <td>
                <b>Returns</b>
              </td>
              <tr>
                <td dangerouslySetInnerHTML={{__html: f.description.return_html}} />
              </tr>
            </table>
          }
          {f.description.postconditions_html &&
            <table class={styles.refTable}>
              <td>
                <b>Postconditions</b>
              </td>
              {f.description.postconditions_html.map(e => (
                <tr>
                  <td dangerouslySetInnerHTML={{__html: e}} />
                </tr>
              ))}
            </table>
          }
          {f.description.exception &&
            <table class={styles.refTable}>
              <td>
                <b>Throws</b>
              </td>
              {f.description.exception.map(e => (
                <tr>
                  <td dangerouslySetInnerHTML={{__html: e.names_html}} />
                  <td dangerouslySetInnerHTML={{__html: e.description_html}} />
                </tr>
              ))}
            </table>
          }
        </div>
      }
    </div>
  );
}

export function MethodDescriptions({data}) {
  const funcs = data.functions;
  return (
    <ol>
      {funcs.map(f => {
        return (
          <li>
            <MethodDescription f={f} />
          </li>
        );
      })}
    </ol>
  );
}

export function Include({data}) {
  const path = data.file_location.file.substring("fbcode/".length)
  const include = "#include <" + path + ">"
  return <CodeBlock language="cpp" children={include} />
}

export function Contribute({empty}) {
  return (
    <Admonition type="tip">
      <p>
        Need help? Post in the <Link to={"https://fb.workplace.com/groups/fbcref"}>fbcref workplace group</Link>.
      </p>

      <p>
        Want to contribute to this documentation page, or other fbcref pages?
        Just make <Link to={"https://www.internalfb.com/intern/staticdocs/fbcref/docs/Documenting/"}>Doxygen-style</Link> comments in code,
        and optionally <Link to={"https://www.internalfb.com/intern/staticdocs/fbcref/docs/Local"}>see your changes locally</Link>.
      </p>
    </Admonition>
  );
}

export function Class({data}) {
  return (
    <div>
      <ParentLinks data={data} />
      <Heading data={data} />
      <Decl data={data} />
      <Desc data={data} />
      <Bases data={data} />
      <TemplateParameters data={data} />
      <MemberTypes data={data} />
      <MemberFunctions data={data} />
      <FreeFunctions data={data} />
      <Example refcode={(data.description || {}).refcode} />
      <Contribute />
    </div>
  );
}

export function Struct({data}) {
  return <Class data={data} />;
}

export function Overloadset({data}) {
  return (
    <div>
      <ParentLinks data={data} />
      <Heading data={data} />
      <MethodDecls data={data} />
      <MethodDescriptions data={data} />
      <Example refcode={data.refcode} />
      <Contribute />
    </div>
  );
}

export function File({data}) {
  return (
    <div>
      <ParentLinks data={data} />
      <Heading data={data} />
      <Include data={data} />
      <Desc data={data} />
      <Macros data={data} />
      <FreeClasses data={data} />
      <FreeTypedefs data={data} />
      <FreeFunctions data={data} />
      <FreeVariables data={data} />
      <Example refcode={(data.description || {}).refcode} />
      <Contribute />
    </div>
  );
}
