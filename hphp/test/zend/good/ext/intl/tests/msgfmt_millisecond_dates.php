<?hh <<__EntryPoint>> function main(): void {
ini_set("intl.error_level", E_WARNING);
date_default_timezone_set('Europe/Lisbon'); //ignored for now, see bug #58756

$d = 1336308097.123;
$mf = new MessageFormatter('en_US',
    "On {0,time,yyyy-MM-dd G 'at' HH:mm:ss.SSS zzz} something odd happened");

var_dump($mf->format(dict[0 => 1336310569.123]));

$p = 'On 2012-05-06 AD at 15:22:49.123 GMT+02:00 something odd happened';
var_dump($mf->parse($p));
echo "==DONE==";
}
