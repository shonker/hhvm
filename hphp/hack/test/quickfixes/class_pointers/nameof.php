<?hh

function expect_string(string $s): void {}
class C {}

function main(): void {
  expect_string(C::class);
}
