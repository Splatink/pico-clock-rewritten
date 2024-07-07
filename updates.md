**CHANGELOG:**

2.0:
* Removed DHT reading capabilities, second core is now unused
* Added BME280 sensor reading, displayClock() now requires the temperature and humidity to be provided instead of being global variables

1.9:
* Removed the WiFi low power mode timer. The WiFi now disconnects permenantly when on battery after getting NTP time once
* Removed WiFi agressive low power mode when connecting
* Tweaked checks for displaying wifi symbols
* Changed NTP update time to 15 days 

1.8:
* Added a flag that gets set when the WiFi gets disconnected manually by low power mode so that we can differinciate from when a disconnection occurs for other reasons
* Removed delay time when trying to connect to WiFi on low power mode

1.7:
* Added the use of dynamic frequency scaling. The processor will now go down to 20Mhz when the WiFi is turned off and 96Mhz when turned on. This results in a power draw of ~14mA when in low-power mode. Expected battery life on a standard 2000mAh 18650 Li-Ion should now be ~142 Hours (5 - 6 days)

1.6:
* Tweaked some indentations
* Moved clock counter for WiFi turning on/off to the second core
* Moved WiFi status checking to it's own function. displayClock() now only checks a status integer that function returns
* Changed if statement to switch when checking blink

1.5:
* Added a global flag that is set if the board is plugged into power (battery precentage over 99) that is false is plugged in and true if not. If true, WiFi disconnects as expected and while the WiFi is disconnected a moon symbol is shown, and the brightness is set to low by default. If false, the WiFi is constantly connected and if disconnected the x symbol is shown and reconnection is attempted, and brightness is high by default.
* Battery voltage/precentage and WiFi RSSI is now checked in setup

1.4:
* Updated WiFi so that it now connects only every hour to update NTP and corrent any errors. It will remain disconnected for the majority of time. This reduces power consumption from ~50mA (on WiFi agressive low power mode) to ~27mA (while completely disconnected). Expected battery life on a standard 2000mAh 18650 Li-Ion should now be ~74 Hours (3 days)
* Updated NTP update time from 10 hours to 1 hour
* Low brightness mode is now the default, lowering power consumption by ~3mA. This can add ~8 hours of battery life.
* Cores now use sleep_ms instead of delay, which will attempt to put cores in a low-power state when waiting
* WiFi no longer automatically reconnects if disconnected (when x symbol is displayed), since it will do that automatically in the loop

1.31:
* Tweaked readability of the code by moving curly braces (not my choise)
* Added a check setting battery precentage to 1 if it is less or equal to 0, since 0 isn't possible while the board is on

1.3:
* Tweaked some checks for showing the appropriate battery symbols
* Removed functions for variables being called in displayClock() function, instead being passed as arguements from loop(). These include: getDay(), getBatteryPrecentage(), getWifiRSSI(), timeClient.getHours(), timeClient.getMinutes(), checkBrightness()

1.2:
* Removed a check setting battery precentage to 99 if it was 100 or more since values over 99 will never be displayed anyways, instead being replaced with a plug symbol
* Added temperature and humidity value averaging and updated DHT sensor read time to 6 seconds. This means the display will be updated with new values for temperature and humidity every minute

1.11:
* Fixed the RSSI averaging function being called 3 times per second instead of 1

1.1:
* Added WiFi RSSI averaging when display the WiFi singal

1.0:
* Updated battery precentage calculator math to original (measurement - min) / (max - min) * 100
* Created this updates.md file 