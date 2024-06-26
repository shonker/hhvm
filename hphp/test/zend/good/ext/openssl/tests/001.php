<?hh <<__EntryPoint>> function main(): void {
echo "Creating private key\n";

/* stack up some entropy; performance is not critical,
 * and being slow will most likely even help the test.
 */
for ($z = "", $i = 0; $i < 1024; $i++) {
    $z .= $i * $i;
    if (function_exists("usleep"))
        usleep($i);
}

$privkey = openssl_pkey_new();

if ($privkey === false)
    exit("failed to create private key");

$passphrase = "banana";
$key_file_name = tempnam(sys_get_temp_dir(), "ssl");
if ($key_file_name === false)
    exit("failed to get a temporary filename!");

echo "Export key to file\n";

openssl_pkey_export_to_file($privkey, $key_file_name, $passphrase) || exit("failed to export to file $key_file_name");

echo "Load key from file - array syntax\n";

$loaded_key = openssl_pkey_get_private(vec["file://$key_file_name", $passphrase]);

if ($loaded_key === false)
    exit("failed to load key using array syntax");

openssl_pkey_free($loaded_key);

echo "Load key using direct syntax\n";

$loaded_key = openssl_pkey_get_private("file://$key_file_name", $passphrase);

if ($loaded_key === false)
    exit("failed to load key using direct syntax");

openssl_pkey_free($loaded_key);

echo "Load key manually and use string syntax\n";

$key_content = file_get_contents($key_file_name);
$loaded_key = openssl_pkey_get_private($key_content, $passphrase);

if ($loaded_key === false)
    exit("failed to load key using string syntax");

openssl_pkey_free($loaded_key);

echo "OK!\n";

unlink($key_file_name);
}
