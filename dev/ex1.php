<?php
$url = "http://192.168.4.1";
$ssid = urlencode($_POST["ssid"]);
$pass = urlencode($_POST["password"]);
$url = $url . "/wifi_connect/ssid=" . "$ssid" . "/pass=" . "$pass" . "/end";
$ch = curl_init($url);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);

$response = curl_exec($ch);
echo $response;

if (strpos($response, "WiFi Ip:") !== false && isset($response)) {
    $pos_start = strpos($response, "WiFi Ip:");

    $ip = trim(substr($response, $pos_start + 8, -1));
    echo $ip;
    $f = fopen("file_esp32_ip.txt", "w");
    fwrite($f, $ip);
    fclose($f);
    $a = 0;
    $ip_updated = "";
    for ($i = 0; $i < strlen($ip); $i++) {
        $c = $ip[$i];
        if ($c === ".") {
            $a++;
        }
        $ip_updated[$i] = $c;
        if ($a == 3) {
            break;
        }
    }
    echo '<form action="shell_exec.php" method="post">';
    echo '<input type="checkbox" name="wifi-set" value="1"> WiFi Change is Complete <br>';
    echo '<input type="submit" name="submit" value="Submit">';
    echo '<input type="hidden" name="ip" value="' . "$ip_updated" . '"' . ">";
    echo "</form>";
} else {
    echo "<p>Failed to connect to WiFi. Please check your SSID and password.</p>";
}
