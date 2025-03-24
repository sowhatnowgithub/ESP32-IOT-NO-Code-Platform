
<?php if (isset($_POST["ip"]) != 1) {
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
    } elseif ($url != "http://192.168.4.1") {
        echo "It looks like you are trying to change wifi connection after being connected once, it won't change";
        echo "<br>Bruh, you have enable the esp32, cause internally it is connected to wifi already.<br>";
        echo '<a href="esp32_user_control.html">Go to user control page</a>';
        echo "<p>Bruh, if you want to enable your esp32, go back to the localhost:8000, cause once you enable, the present connection is lost so, and you have to connect back esp32 access point</p>";
        echo '<a href="http://localhost:8000/esp32_user_control.html"> Go back to local host</a>';
        exit();
    }
    echo "Currently accessing the url:- " . $url . "<br>";
    $ssid = urlencode($_POST["ssid"]);
    $pass = urlencode($_POST["password"]);
    $url = $url . "/wifi_connect/ssid=" . "$ssid" . "/pass=" . "$pass" . "/end";
    $ch = curl_init($url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    $response = curl_exec($ch);
    echo $response;
    if (strpos($response, "WiFi Ip:") !== false && isset($response) == 1) {
        $pos_start = strpos($response, "WiFi Ip:");
        $ip = trim(substr($response, $pos_start + 8, -1));
        echo $ip;
        $f = fopen("../file_esp32_ip.txt", "w");
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
        echo '<form action="esp32_php_server_start.php" method="post">';
        echo '<input type="checkbox" name="wifi-set" value="1"> WiFi Change is Complete <br>';
        echo '<input type="submit" name="submit" value="Submit">';
        echo '<input type="hidden" name="ip" value="' .
            "$ip_updated" .
            '"' .
            ">";
        echo "</form>";
    } else {
        echo "<p>Failed to connect to WiFi. Please check your SSID and password.</p>";
        echo '<a href="../esp32_wifi_login.html" >Go to wifi login page </a>';
        echo '<a href="../esp32_user_control.html">Go to Esp32-Control Page </a>';
    }
}
