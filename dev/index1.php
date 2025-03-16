    <!doctype html>
    <html>
        <head>
            <title>Damn - Esp32</title>
            <style>
                * {
                    box-sizing: border-box;
                    line-height: 2.5em;
                }
            </style>
        </head>
        <body>
            <a href="ssid.html">TO connect to wifi</a>
            <form action="ex2.php" method="post">
                <fieldset>
                    <legend>Esp-32-Wroom</legend>
                    <label for="gpio-digital-out">GPIO - Digital: </label>
                    <select name="gpio-digital-out">
                    <option value="23">23</option>
                    <option value="22">22</option>
                    <option value="21">21</option>
                    <option value="19">19</option>
                    <option value="18">18</option>
                    <option value="5">5</option>
                    </select>
                    <input type="radio" name="gpio-digital-status" value="1" /> HIGH
                    <input type="radio" name="gpio-digital-status" value="0" /> LOW
                    <br />
                    <input
                        type="checkbox"
                        name="CONNECTION_STATUS"
                        value="LOW"
                    />Disconnect <br />
                    <input type="submit" name="submit" value="Submit" />
                </fieldset>
            </form>
        </body>
        </html>
