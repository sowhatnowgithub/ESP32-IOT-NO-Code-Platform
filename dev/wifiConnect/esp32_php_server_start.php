<?php
if (isset($_POST["ip"]) == 1 && isset($_POST["wifi-set"]) == 1) {
    // $ip_command = 'ipconfig getifaddr en0 '; for mac
    $ip = shell_exec("ipconfig getifaddr en0 ");
    $ip = preg_replace("/[^a-zA-Z0-9-_\.]/", "", $ip);
    $pid = shell_exec("cd ../ \n php -S $ip:8000 > /dev/null 2>&1 & echo $!");
    header("Location: http://$ip:8000/esp32_user_control.html");
} else {
    echo "Failed to start php server";
    // header("http://localhost:8000");
}
