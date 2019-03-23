# Information

The DasKit C7BB-Display sold with several NCM Bikes (Moscow, Moscow+, Moscow Plus, Venice, Venice+, ...) does not offer any tuning/unlocking options. Also, there is no thumb throttle function.
NCM sells another Display unlocking this feature (C6BBT), however this is very rarely available.

With the help of this code, a cheap ESP32 and a cable you will be able to unlock these and add additional features with the original C7BB display.

<i>This is based on the works of "Arthus Dent" and "vspadrva" of the German pedelecforum.
For more information see https://www.pedelecforum.de/forum/index.php?threads/kommunikation-zwischen-c7-display-und-motorkontroller-ncm-venice-das-kit.57050/</i>

Note: The code was originally created for a Teensy / ESP2866 and was incompatible for an ESP32.

In this repository you will find several versions:

- <b>Original</b> - Original code by vspadrva<br>
- <b>Original-ESP32</b> - ESP32-ported version of the original Arduino code by vspadrva<br>
<i>This enables a "power"-mode when in PAS 2. The bike will assist up to approx. 36 km/h</i><br>
- <b>WiFi-AP</b> - Modified version of "Original-ESP32" wich creates a WiFi Access Point with a web server to enable/disable the options of this firmware.

# Step by Step guide
