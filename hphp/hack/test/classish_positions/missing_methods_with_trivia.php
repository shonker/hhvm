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


  // Calculating where the closing brace is is hard work, especially
  // with a lot of trivia (whitespace) before it.
  //
  // So let's test it properly, whitespace incoming!





/*range-start*/}/*range-end*/
