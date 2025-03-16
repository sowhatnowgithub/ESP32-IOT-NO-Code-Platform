<?php
$url = "http://192.168.4.1";

$ch = curl_init($url);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true); // Return the response as a string
curl_setopt($ch, CURLOPT_FOLLOWLOCATION, true); // Follow redirects

$response = curl_exec($ch);
if (curl_errno($ch)) {
    echo "Curl Error: " . curl_error($ch);
}

echo $response;

?>
