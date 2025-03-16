<?php

$url = "http://192.168.4.1";

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

include "index.php";
