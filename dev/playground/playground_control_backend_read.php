<?php

$url = "http://192.168.4.1";
if (file_exists("../file_esp32_ip.txt")) {
    $u = file("../file_esp32_ip.txt");
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
    $response = substr($response, -10, 4);
    echo $response;
    curl_close($ch);
}
if (isset($_POST["dht11_data_pin"])) {
    $c_url =
        $url . "/sensor/DHT11/gpio_pin=" . $_POST["dht11_data_pin"] . "/end";
    $ch = curl_init($c_url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true); // Return the response as a string
    curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 5); // 5 second connection timeout
    curl_setopt($ch, CURLOPT_TIMEOUT, 10); // 10 second execution timeout

    $response = curl_exec($ch);
    echo $response;

    curl_close($ch);
}
