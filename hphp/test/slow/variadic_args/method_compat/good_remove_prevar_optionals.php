<?hh

interface DB {
  public function query($query = '', ...$params):mixed;
}

class MySQL implements DB {
  public function query(...$params) :mixed{ }
}

<<__EntryPoint>>
function main(): void {
  echo "Done.\n";
}
