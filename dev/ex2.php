<?php
$url = "http://192.168.4.1";
if (file_exists("file_esp32_ip.txt")) {
    $u = file("file_esp32_ip.txt");
    $url = str_replace("\r\n", "\n", $u[0]);
    echo "Currently accessing the url:- " . $url . "<br>";
    $url = trim($url);
}
$url = "http://" . $url;
// $url = "http://192.168.176.99";
//echo $url . "<br>";
if (isset($_POST["CONNECTION_STATUS"])) {
    $connection_status = $_POST["CONNECTION_STATUS"];
    // this guy will make all the connection variables to null, meaning the connectin died and all set variables are gone
    // this will be build when sql is done
}
if (isset($_POST["gpio-digital-out"])) {
    $c_url =
        $url .
        "/digital/OUT/gpio_pin=" .
        $_POST["gpio-digital-out"] .
        "/status=" .
        $_POST["gpio-digital-status"] .
        "/end";
    $ch = curl_init($c_url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true); // Return the response as a string

    $response = curl_exec($ch);
    echo $response;
} else {
}

include "index1.php";
