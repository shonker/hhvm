<?hh
namespace Foo {

class Biz {
  function wat() :mixed{
    \var_dump(\get_class($this));
  }
}

namespace Bar {

class Baz {
  function wat() :mixed{
    \var_dump(\get_class($this));
  }
}

} /* Bar */

class Fiz {
  function wat() :mixed{
    \var_dump(\get_class($this));
  }
}

} /* Foo */

namespace {
<<__EntryPoint>> function main(): void {
$biz = new Foo\Biz();
$baz = new Foo\Bar\Baz();
$fiz = new Foo\Fiz();
$biz->wat();
$baz->wat();
$fiz->wat();
}
}
