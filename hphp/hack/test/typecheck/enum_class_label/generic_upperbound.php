<?hh

enum class E: mixed {}

abstract class I {
  abstract const type T as E;
}

final class C<TX as I> {
  public function getTypedParamNew<TParams>(
    HH\EnumClass\Label<TParams, mixed> $label,
  ): void where TParams = TX::T {
  }
}

enum class A: mixed extends E {
  int X = 42;
}

function get<TI as I>(C<TI> $param_bag): void where TI::T as A {
  $param_bag->getTypedParamNew(#X);
}
