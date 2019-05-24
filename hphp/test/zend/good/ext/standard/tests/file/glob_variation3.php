<?php <<__EntryPoint>> function main() {
$path = dirname(__FILE__);

ini_set('open_basedir', NULL);

var_dump(glob("$path/*.none"));
var_dump(glob("$path/?.none"));
var_dump(glob("$path/*{hello,world}.none"));
var_dump(glob("$path/*/nothere"));
var_dump(glob("$path/[aoeu]*.none"));
var_dump(glob("$path/directly_not_exists"));

var_dump(!(ini_get('open_basedir') ?? false));
echo "==DONE==";
}
