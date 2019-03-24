For the english version of this readme, go to https://github.com/NorthyIE/DasKit-C7BB-Tuning/blob/master/README.md

# Übersicht

Das C7BB-Display von DasKit, das bei mehreren Pedelecs voh NCM (Moscow+, Moscow Plus, Venice+, ...) verwendet wird, bietet keine Einstell- / Entriegelungsoptionen. Auch gibt es keine Daumengasfunktion.
NCM verkauft ein weiteres Display, das diese Funktionen freischaltet (C6BBT), dies ist jedoch sehr selten verfügbar.

Mit Hilfe von diesem Repository, eines günstigen ESP32 und eines Kabels kann man mit dem originalen C7BB-Display die Geschwindigkeit entsperren und zusätzliche Funktionen hinzufügen.

<i>Basis diesen Codes sind die tolle Arbeit von "Arthus Dent" und "vspadrva" des deutschen Pedelecforums.
Weitere Informationen unter https://www.pedelecforum.de/forum/index.php?threads/kommunikation-zwischen-c7-display-und-motorkontroller-ncm-venice-das-kit.57050/</i>

Hinweis: Der Code wurde ursprünglich für ein Teensy / ESP2866 erstellt ist mit dem ESP32 nicht kompatibel.

In diesem Repository finden Sie verschiedene Versionen:

- <b>Original</b> - Originalcode von vspadrva<br>
- <b>Original-ESP32</b> - ESP32-portierte Version des ursprünglichen Arduino-Codes von vspadrva<br>
<i>Dies ermöglicht einen "Power" -Modus in PAS 2. Das Fahrrad unterstützt bis zu ca. 36 Km/h</i><br>
- <b>WiFi-AP</b> - Modifizierte Version von dem "Original-ESP32"-Code, die einen WLAN Access Point mit einem Webserver erstellt, um die Optionen dieser Firmware zu aktivieren/deaktivieren.<br>
<b>Achtung: Diese Version ist noch in Arbeit und funktioniert eventuell nicht einwandfrei!</b><br>

# Schritt für Schritt Anleitung

Folgt..







