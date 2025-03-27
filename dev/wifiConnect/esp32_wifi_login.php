<!doctype html>
<html>
    <head>
        <title>ESP32 WiFi Setup</title>
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <link rel="stylesheet" type="text/css" href="esp32_wifi_login.css" />
        <style>
            .scan-button {
                display: block;
                width: 100%;
                padding: 10px;
                margin-bottom: 20px;
                background-color: #4caf50;
                color: white;
                font-weight: bold;
                border: none;
                border-radius: 4px;
                cursor: pointer;
                text-align: center;
                text-decoration: none;
                font-size: 16px;
            }

            .scan-button:hover {
                background-color: #0b7dda;
            }
        </style>
    </head>
    <body>
        <div class="container">
            <h1>ESP32 WiFi Setup</h1>
<?php
$url = "http://192.168.4.1";
if (file_exists("../file_esp32_ip.txt")) {
    $u = file("../file_esp32_ip.txt");
    $url = str_replace("\r\n", "\n", $u[0]);
    $url = trim($url);
    $url = "http://" . $url;
}

echo '<iframe src="' .
    "$url" .
    '/wifiscan" title="Scanning for wifi"></iframe>';
?>
            <form action="esp32_wifi_credentials_validation.php" method="post">
                <fieldset>
                    <legend>Configure WiFi Connection</legend>

                    <label for="ssid">WiFi Name (SSID):</label>
                    <input
                        type="text"
                        name="ssid"
                        placeholder="Enter your WiFi name"
                        required
                    />
                    <br />

                    <label for="password">WiFi Password:</label>
                    <input
                        type="password"
                        name="password"
                        placeholder="Enter your WiFi password"
                    />
                    <br />

                    <input type="submit" name="submit" value="Connect" />
                </fieldset>
            </form>

            <div class="navigation">
                <a href="../index.html">Back to Main Page</a>
                <a href="../esp32_user_control.html">Control Panel</a>
            </div>

            <div class="footer">&copy; 2025 ESP32 No-Code Platform</div>
        </div>
    </body>
</html>
';
