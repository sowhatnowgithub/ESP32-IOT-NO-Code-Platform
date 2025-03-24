<?php

$url = "http://192.168.4.1";
if (file_exists("../file_esp32_ip.txt")) {
    $u = file("file_esp32_ip.txt");
    $url = str_replace("\r\n", "\n", $u[0]);
    $url = trim($url);
    $url = "http://" . $url;
}

$ch_verify = curl_init($url);
curl_setopt($ch_verify, CURLOPT_RETURNTRANSFER, true);
if (curl_exec($ch_verify) == false) {
    $f = fopen("../file_esp32_ip.txt", "w");
    fwrite($f, "192.168.4.1");
    fclose($f);
    curl_close($ch_verify);
    $url = "http://192.168.4.1";
}

if (
    isset($_POST["gpio-digital-in"]) &&
    strcmp($_POST["gpio-digital-in"], "Null") != 0 &&
    isset($_POST["gpio-digital-in-status"])
) {
    $c_url =
        $url .
        "/digital/IN/gpio_pin=" .
        $_POST["gpio-digital-in"] .
        "/status=" .
        $_POST["gpio-digital-in-status"] .
        "/end";
    $ch = curl_init($c_url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true); // Return the response as a string

    $response = curl_exec($ch);
    echo $response;
}
