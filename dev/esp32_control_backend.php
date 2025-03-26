<?php

$url = "http://192.168.4.1";
if (file_exists("file_esp32_ip.txt")) {
    $u = file("file_esp32_ip.txt");
    $url = str_replace("\r\n", "\n", $u[0]);
    $url = trim($url);
    $url = "http://" . $url;
}

// Increase the PHP execution time limit
set_time_limit(120); // Set to 120 seconds

$ch_verify = curl_init($url);
curl_setopt($ch_verify, CURLOPT_RETURNTRANSFER, true);
curl_setopt($ch_verify, CURLOPT_CONNECTTIMEOUT, 5); // 5 second connection timeout
curl_setopt($ch_verify, CURLOPT_TIMEOUT, 10); // 10 second execution timeout

if (curl_exec($ch_verify) == false) {
    $f = fopen("file_esp32_ip.txt", "w");
    fwrite($f, "192.168.4.1");
    fclose($f);
    curl_close($ch_verify);
    $url = "http://192.168.4.1";
} else {
    echo "If you want to go back to accesspoint, you can enable the esp32 and the present wifi connection is lost and you have to connect back to esp32 access point to control it.";
    echo '<br> <a href="http://localhost:8000/esp32_user_control.html" >Go back to access point </a><br>';
}
echo "Currently accessing the url:- " . $url . "<br>";
if (isset($_POST["CONNECTION_STATUS"])) {
    $connection_status = $_POST["CONNECTION_STATUS"];
    // this guy will make all the connection variables to null, meaning the connectin died and all set variables are gone
    // this will be build when sql is done
}
if (isset($_POST["gpio-digital-out"]) && isset($_POST["gpio-digital-status"])) {
    $c_url =
        $url .
        "/digital/OUT/gpio_pin=" .
        $_POST["gpio-digital-out"] .
        "/status=" .
        $_POST["gpio-digital-status"] .
        "/end";
    $ch = curl_init($c_url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true); // Return the response as a string
    curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 5); // 5 second connection timeout
    curl_setopt($ch, CURLOPT_TIMEOUT, 10); // 10 second execution timeout

    $response = curl_exec($ch);
    curl_close($ch);
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
    curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 5); // 5 second connection timeout
    curl_setopt($ch, CURLOPT_TIMEOUT, 10); // 10 second execution timeout

    $response = curl_exec($ch);
    curl_close($ch);

    echo $response;
}
if (isset($_POST["dac-write-pin"]) && isset($_POST["dac-write-value"])) {
    $c_url =
        $url .
        "/digital/dac/gpio_pin=" .
        $_POST["dac-write-pin"] .
        "/status=" .
        $_POST["dac-write-value"] .
        "/end";
    $ch = curl_init($c_url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true); // Return the response as a string
    curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 5); // 5 second connection timeout
    curl_setopt($ch, CURLOPT_TIMEOUT, 10); // 10 second execution timeout

    $response = curl_exec($ch);
    curl_close($ch);
}

if (isset($_POST["dac-disable-pin"]) && isset($_POST["dac-disable-valid"])) {
    $c_url =
        $url .
        "/digital/disable-dac/gpio_pin=" .
        $_POST["dac-disable-pin"] .
        "/end";
    $ch = curl_init($c_url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true); // Return the response as a string
    curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 5); // 5 second connection timeout
    curl_setopt($ch, CURLOPT_TIMEOUT, 10); // 10 second execution timeout

    $response = curl_exec($ch);
    curl_close($ch);
}
if (isset($_POST["delay-seconds"])) {
    $c_url = $url . "/delay/delay=" . $_POST["delay-seconds"] . "/end";
    $ch = curl_init($c_url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true); // Return the response as a string
    curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 5); // 5 second connection timeout
    curl_setopt($ch, CURLOPT_TIMEOUT, 10); // 10 second execution timeout

    $response = curl_exec($ch);
    curl_close($ch);
}
if (
    isset($_POST["analog-write-pin"]) &&
    isset($_POST["analog-write-freq"]) &&
    isset($_POST["analog-write-res"]) &&
    isset($_POST["analog-write-duty"]) &&
    strcmp($_POST["analog-write-pin"], "null") != 0
) {
    $c_url =
        $url .
        "/analog/write/gpio_pin=" .
        $_POST["analog-write-pin"] .
        "/freq=" .
        $_POST["analog-write-freq"] .
        "/res=" .
        $_POST["analog-write-res"] .
        "/duty=" .
        $_POST["analog-write-duty"] .
        "/end";
    $ch = curl_init($c_url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true); // Return the response as a string
    curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 5); // 5 second connection timeout
    curl_setopt($ch, CURLOPT_TIMEOUT, 10); // 10 second execution timeout

    $response = curl_exec($ch);
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
    curl_close($ch);
}
include "esp32_user_control.html";
