<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 WiFi Connection</title>
    <?php // Increase the PHP execution time limit
// Increase the PHP execution time limit
// Increase the PHP execution time limit
    // Increase the PHP execution time limit
    // Increase the PHP execution time limit
    // Increase the PHP execution time limit
    set_time_limit(120);
// Set to 120 seconds
?>
    <style>
        /* Base styles */
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background-color: #f0f8f0;
            margin: 0;
            padding: 0;
            color: #333;
            line-height: 1.6;
        }

        /* Main container */
        .container {
            background-color: #e0f7e0;
            padding: 30px;
            margin: 40px auto;
            max-width: 800px;
            border-radius: 15px;
            box-shadow: 0 5px 15px rgba(0, 0, 0, 0.1);
        }

        /* Header styles */
        h1 {
            color: #2e7d32;
            text-align: center;
            margin-top: 0;
            padding-bottom: 15px;
            border-bottom: 2px solid #c8e6c9;
            font-size: 2.2em;
        }

        h2 {
            color: #388e3c;
            text-align: center;
            margin-top: 25px;
            font-size: 1.7em;
        }

        /* Paragraph styles */
        p {
            color: #555;
            text-align: center;
            margin: 15px 0;
            font-size: 1.1em;
        }

        /* Link styles */
        a {
            display: inline-block;
            color: #ffffff;
            background-color: #4caf50;
            text-align: center;
            margin: 10px 5px;
            text-decoration: none;
            padding: 10px 20px;
            border-radius: 5px;
            transition: all 0.3s ease;
            font-weight: 500;
        }

        a:hover {
            background-color: #388e3c;
            box-shadow: 0 3px 8px rgba(0, 0, 0, 0.2);
            transform: translateY(-2px);
        }

        /* Result container */
        .result-container {
            background-color: #f5fff5;
            border-radius: 8px;
            padding: 20px;
            margin: 20px auto;
            text-align: center;
            box-shadow: 0 3px 10px rgba(0, 0, 0, 0.1);
        }

        /* IP display */
        .ip-display {
            background-color: #e8f5e9;
            padding: 10px;
            border-radius: 4px;
            border: 1px solid #c8e6c9;
            margin: 15px 0;
            font-family: monospace;
            font-size: 1.1em;
        }

        /* Form styling */
        form {
            margin: 20px 0;
            text-align: center;
        }

        input[type="checkbox"] {
            margin-right: 8px;
            transform: scale(1.2);
        }

        input[type="submit"] {
            background-color: #4caf50;
            color: white;
            border: none;
            padding: 12px 25px;
            border-radius: 5px;
            cursor: pointer;
            font-size: 1.1em;
            font-weight: 500;
            margin-top: 15px;
            transition: all 0.3s ease;
        }

        input[type="submit"]:hover {
            background-color: #388e3c;
            box-shadow: 0 3px 8px rgba(0, 0, 0, 0.2);
        }

        /* Navigation container */
        .navigation {
            display: flex;
            justify-content: center;
            gap: 20px;
            margin: 20px 0;
        }

        /* Success and error messages */
        .success-message {
            background-color: #c8e6c9;
            color: #2e7d32;
            border: 1px solid #81c784;
            padding: 15px;
            margin: 20px 0;
            border-radius: 5px;
        }

        .error-message {
            background-color: #ffcdd2;
            color: #c62828;
            border: 1px solid #ef9a9a;
            padding: 15px;
            margin: 20px 0;
            border-radius: 5px;
        }

        /* ESP32 direct connection notice */
        .direct-connect {
            margin-top: 25px;
            padding: 15px;
            background-color: #e8f5e9;
            border-radius: 8px;
            border: 1px dashed #81c784;
        }

        /* WiFi Manager button - added style */
        .wifi-manager-btn {
            background-color: #ff9800;
            margin-top: 10px;
        }

        .wifi-manager-btn:hover {
            background-color: #f57c00;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>ESP32 WiFi Connection</h1>
        <div class="result-container">
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
                curl_setopt($ch_verify, CURLOPT_CONNECTTIMEOUT, 5); // 5 second connection timeout
                curl_setopt($ch_verify, CURLOPT_TIMEOUT, 10); // 10 second execution timeout
                if (curl_exec($ch_verify) == false) {
                    $f = fopen("../file_esp32_ip.txt", "w");
                    fwrite($f, "192.168.4.1");
                    fclose($f);
                    curl_close($ch_verify);
                    $url = "http://192.168.4.1";
                } elseif ($url != "http://192.168.4.1") {
                    echo '<div class="error-message">';
                    echo "It looks like you are trying to change WiFi connection after being connected once, it won't change";
                    echo "<br>You have already enabled the ESP32, which is internally connected to WiFi.</div>";
                    echo '<div class="navigation">';
                    echo '<a href="../esp32_user_control.html">Go to User Control Page</a>';
                    echo '<a href="http://localhost:8000/../esp32_user_control.html">Go Back to Local Host</a>';
                    echo "</div>";
                    echo "<p>If you want to enable your ESP32, go back to localhost:8000, as once you enable, the present connection will be lost.</p>";
                    echo "</div></div></body></html>";
                    exit();
                }

                echo "<h2>Connection Status</h2>";
                echo '<p>Currently accessing the URL: <span class="ip-display">' .
                    $url .
                    "</span></p>";

                $ssid = urlencode($_POST["ssid"]);
                $pass = urldecode($_POST["password"]);
                $url =
                    $url .
                    "/wifi_connect/ssid=" .
                    "$ssid" .
                    "/pass=" .
                    "$pass" .
                    "/end";
                $ch = curl_init($url);
                curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
                curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 10); // 10 second connection timeout
                curl_setopt($ch, CURLOPT_TIMEOUT, 20); // 20 second execution timeout
                $response = curl_exec($ch);

                if ($response === false) {
                    $error = curl_error($ch);
                    echo '<div class="error-message">Connection to ESP32 failed: ' .
                        $error .
                        "</div>";
                    echo '<div class="navigation">';
                    echo '<a href="esp32_wifi_login.php">Try Again</a>';
                    // Add WiFiManager access here too
                    echo "</div>";
                } elseif (
                    strpos($response, "WiFi Ip:") !== false &&
                    isset($response) == 1
                ) {
                    $pos_start = strpos($response, "WiFi Ip:");
                    $ip = trim(substr($response, $pos_start + 8, -1));

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
                        $ip_updated .= $c;
                        if ($a == 3) {
                            break;
                        }
                    }

                    echo '<div class="success-message">Successfully connected to WiFi!</div>';
                    echo '<p>ESP32 IP Address: <span class="ip-display">' .
                        $ip .
                        "</span></p>";

                    echo '<form action="esp32_php_server_start.php" method="post">';
                    echo '<input type="checkbox" name="wifi-set" value="1"> WiFi Change is Complete<br>';
                    echo '<input type="hidden" name="ip" value="' .
                        $ip_updated .
                        '">';
                    echo '<input type="submit" name="submit" value="Continue">';
                    echo "</form>";

                    // Add a link to access WiFiManager after successful connection
                    echo '<div class="navigation">';
                    echo '<a href="http://' .
                        $ip .
                        '/wifi" target="_blank" class="wifi-manager-btn">Configure Network Settings with WiFiManager</a>';
                    echo "</div>";
                } else {
                    echo '<div class="error-message">Failed to connect to WiFi. Please check your SSID and password.</div>';
                    echo '<div class="navigation">';
                    echo '<a href="esp32_wifi_login.php">Try Again</a>';
                    echo '<a href="../esp32_user_control.html">Go to ESP32 Control Page</a>';
                    echo "</div>";
                }
                curl_close($ch);
            } ?>
        </div>
    </div>
</body>
</html>
