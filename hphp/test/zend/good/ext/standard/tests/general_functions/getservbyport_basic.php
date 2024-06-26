<?hh <<__EntryPoint>> function main(): void {
if (file_exists("/etc/services")) {
    $file = "/etc/services";
}
else if(substr(PHP_OS,0,3) == "WIN") $file = "C:/WINDOWS/system32/drivers/etc/services";
else exit(PHP_OS. " unsupported");

if(file_exists($file)){
    $services = file_get_contents($file);
            $service = getservbyport( 80, "tcp" );
            if(preg_match("/$service\s+80\/tcp/", $services)) {
        echo "PASS\n";
    }
}else{
    echo "Services file not found in expected location\n";
}
}
