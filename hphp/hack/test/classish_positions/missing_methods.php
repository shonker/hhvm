<?hh

trait FooTrait {
  abstract public function bar(): void;
  abstract public function baz(): void;
}

abstract class AbstractBar {
  abstract public function baf(): void;
}

interface I {
  public static function i(): int;
}

class Foo extends AbstractBar implements I {
  use FooTrait;
/*range-start*/}/*range-end*/
