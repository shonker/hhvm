<?hh

class X { static function y() :mixed{} }
function t() :mixed{ var_dump(__FUNCTION__); }

<<__EntryPoint>>
function main() :mixed{
  set_error_handler(($_n, $str) ==> { echo "Warning: $str\n"; return true; });

  (new ReflectionFunction('t'))->invoke();
  var_dump(HH\classname_to_class(HH\classname_to_class('X')));
}
