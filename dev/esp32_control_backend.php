<?php

$url = "http://192.168.4.1";
if (file_exists("file_esp32_ip.txt")) {
    $u = file("file_esp32_ip.txt");
    $url = str_replace("\r\n", "\n", $u[0]);
    $url = trim($url);
    $url = "http://" . $url;
}

$ch_verify = curl_init($url);
curl_setopt($ch_verify, CURLOPT_RETURNTRANSFER, true);
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

include "esp32_user_control.html";
