# CO2-Ampeln

*Bilder/Diagramme, Schaltpläne, etc. (wo sinnvoll) einfügen*

<!-- → Verwendung: Was macht das? Wie kann man das benutzen?, … -->

Die vorhandenen CO2-Ampeln wurden upgedated und in das Dashboard integriert. Die von ihnen ermittelten Daten werden über MQTT-Nachrichten an ioBroker gesendet und dort verarbeitet. Die Anzeige ist eine einfache Wertedarstellung.


### Aufbau
<!-- → z.B.: Verkabelung, Infrastruktur, Ort,  -->
<!-- - 3 [CO2-Ampel](https://learn.watterott.com/de/breakouts/co2-ampel/): 1ne pro Raum
- Auslesung der Sensorwerte über MQTT in ioBroker
  - CO2-Konzentration, Luftfeuchtigkeit, Temperatur, Helligkeit
- für MQTT Kommunikation wurde eine neue Firmware benötigt:
  - original [Plus-Firmware](https://github.com/mariolukas/Watterott-CO2-Ampel-Plus-Firmware/releases/tag/v.3.0.1)
  - neu Kompilierung wegen Anpassungen:
    - Interval für Lichtmessung verkürzt (war 1/h)
    - Interval für Sensormessung in der Dunkelheit angepasst
    - siehe [hier](#firmware-kompilieren)
    - [Warum?](#firmware-probleme)
- MQTT sendet ein JSON-Objekt das in ioBroker mithilfe von einem Skript alle 15min in unterschiedliche Objekte aufgedröselt wird. So lässt sich einfacher damit arbeiten.
- ioBroker-Objekte werden in History gespeichert um eCharts damit erstellen zu können:
  - TODO: aktuelle Einstellungen -->

Für jeden Raum ist eine [CO2-Ampel](https://learn.watterott.com/de/breakouts/co2-ampel/) installiert. Diese liest die CO2-Konzentration, Luftfeuchtigkeit, Temperatur und Helligkeit aus.
Um diese Daten mit MQTT an ioBroker schicken zu können wurde eine vom Hersteller empfohlene [Plus-Firmware](https://github.com/mariolukas/Watterott-CO2-Ampel-Plus-Firmware/releases/tag/v.3.0.1) verwendet. Für bessere Messwerte und -intervalle wurde diese Firmware neu kompiliert. Veränderungen dabei waren die Verkürzung des Intervals für Lichtmessung von einer Stunde auf zwei Minuten. Das Intervall für die Sensormessung in der Dunkelheit. Für ausführlichere Informationen siehe [hier](#firmware-kompilieren).
Die MQTT-Nachricht sendet ein JSON-Objekt an ioBroker, da sich dieses Objekte schlecht/bzw. nur sehr umständlich in vis (LINK Dashboard) einbinden lässt, wird ein [Skript](https://github.com/thiilo/VR4Ware/blob/main/Scripts/parseAmpeln.js) genutzt, dass alle 15 Minuten das Objekt in unterschiedliche ioBroker-Objekte aufdröselt.
Zusätzlich zur Anzeige im Dashboard werden die Daten der Objekte mit der History gespeichert um später damit eCharts erstellen zu können.

### Wartung und Troubleshooting
<!-- → Wie kommt man ran?, Was kann man einfach ändern?, Bugs, die uns begegnet sind und wie sie gelöst wurden, … -->

<!-- - TODO: Konfigurieren mit Frontend -->
Um WLAN und MQTT-Daten zu setzten kann das Web-Frontend der Firmware verwendet werden. Hierzu kann einfach der offiziellen [Anleitung](https://github.com/mariolukas/Watterott-CO2-Ampel-Plus-Firmware?tab=readme-ov-file#wifi-einrichten) gefolgt werden.

#### Firmware kompilieren
Die aktuell verwendete Firmware ist [hier](https://github.com/thiilo/VR4Ware/blob/main/CO2-Ampel_Plus_v_VR4.bin) zu finden:
Die Änderungen in `Config.h` sind:
  - `#define INTERVAL_DARK 3`
  - `#define LIGHT_INTERVAL 150`
> Daraus ergibt sich 1. der Intervall für die Abfrage der drei Luftsensoren (alle 3 Sekunden) und der Lichtmessung (alle 150s). Beim Messen der Lichtstärke wird momentär die eigene LED ausgeschaltet, um Interferenz zu vermeiden. Das könnte für Betrachter wie ein Flackern wirken, ist aber so beabsichtigt. 

Um die Firmware neu zu kompililieren können folgende Schritte gefolgt werden:
- Der [Anleitung](https://github.com/mariolukas/Watterott-CO2-Ampel-Plus-Firmware#eigene-ampel-version-aus-den-quellen-kompilieren) folgen um Arduino IDE aufzusetzten
- Die *WifiWebServer*-Library auf `v.1.4.2` downgraden um kompilieren zu können (ansonsten gibt es einen Error mit einer private/public Variable) (Stand: Oktober 2024)
- Änderungen in `Config.h` vornehmen
  - Alternativ zur Konfiguration mit dem Web-Frontend können hier auch MQTT und WIFI-Daten gesetzt werden
- Sicherstellen, dass in `Config.h` `#define DEBUG_LOG 0` steht. Ansonsten kann die Ampel nur über Arduino IDE mit verbundenen Port laufen gelassen werden
- `CO2-Ampel_Plus.ino` kompilieren:
  - `.bin`-File: Menüpunkt: Sketch -> Export Compiled Binary (ALT+STRG+S)
- die Firmware [hochladen](#eine-firmware-hochladen)

#### Eine Firmware hochladen 
Der  [offiziellen](https://learn.watterott.com/de/breakouts/co2-ampel/firmware/#aktualisierungsanleitung) oder [alternativen](https://github.com/mariolukas/Watterott-CO2-Ampel-Plus-Firmware?tab=readme-ov-file#neue-ampel-firmware-version-installieren) Aktualisierungsanleitung folgen.
Oder die Ampel mit USB verbinden und den Code direkt mithilfe von Arduino IDE kompiliere und hochladen.

---

#### Anmerkungen
<!-- → Zusätzlicher Punkt für Notizen/Anmerkungen, etc. (wenn nichts wichtiges, dann weglassen) -->

##### Komische Sensordaten
Über einen längeren Zeitraum wurden komische Schwankungen in den Sensordaten beobachtet. Diese Schwankungen korrelierten damit, ob es hell oder dunkel war. In der Firmware wird der der Wert für das Messintervall größer gesetzt, wenn die Helligkeit unterhalb eines bestimmten Wertes liegt. Es wird in der Nacht also weniger oft gemessen. 
Deswegen wurden mithilfe eines [BMP280](https://www.az-delivery.de/en/products/azdelivery-bmp280-barometrischer-sensor-luftdruck-modul-fur-arduino-und-raspberry-pi) die Temperatur und Luftfeuchtigkeit nachgemessen. Außerdem wurden, wie [hier](#firmware-kompilieren) beschrieben, die jeweiligen Variablen in der Firmware angepasst.
<!-- - Sensordaten hatten komische Schwankungen immer um sehr ähnliche Zeit
- BILD
- Schwankungen korrelieren damit, dass das Licht an-/ausgeschaltet wird (also Lichtsensor hoch/tief)
- Firmware setzt das Measurement Intervall größer, wenn das Licht kleiner als ein bestimmten Wert hat -> also wird weniger oft gemessen -->
<!-- - Temperaturwerte stimmte nicht, deswegen Nachmessung mit [BMP280](https://www.az-delivery.de/en/products/azdelivery-bmp280-barometrischer-sensor-luftdruck-modul-fur-arduino-und-raspberry-pi) -->


##### Temperatur Offset
Bei der verwendeten Firmware sollte es eigentlich möglich sein über ein MQTT einen Offset für die Temperatur setzen zu können (siehe [hier](https://github.com/mariolukas/Watterott-CO2-Ampel-Plus-Firmware?tab=readme-ov-file#temperatur-sensor-kalibration)). Das hatte aber nicht funktioniert.
Deswegen und um diese Funktion an einem Ort zu bündelt wird das [Skript]((https://github.com/thiilo/VR4Ware/blob/main/Scripts/parseAmpeln.js)) zum aufdröseln des JSON-Objekts genutzt um die Offsets einfach auf die Werte zu rechnen.
<!-- - Alternativ: Offset wurde in ioBroker gesetzt:
  - im gleichen Script, dass die Werte aufsplittet (TODO: LINK)
  - ist dort leicht für Änderungen oder zum Auslesen zugänglich
- MQTT: Temp offset -->


##### Visualisierung ioBroker
- Farben werden über Ternäre Operatoren und die Objekt-IDs gesetzt:
```
{val:0_userdata.0.CO2-Ampel.Ampel_Labor.co2; val >= 800 ? "#E0D500" :: val >= 1000 ? "#F50000" :: "#33CC00"}
{val:0_userdata.0.CO2-Ampel.Ampel_Studio.co2; val >= 800 ? "#E0D500" :: val >= 1000 ? "#F50000" :: "#33CC00"}
{val:0_userdata.0.CO2-Ampel.Ampel_Werkstatt.co2; val >= 800 ? "#E0D500" :: val >= 1000 ? "#F50000" :: "#33CC00"}

{val:0_userdata.0.CO2-Ampel.Ampel_Labor.temp; val < 10 ? "#008DF0" :: val < 20 ? "#33CC00" :: val < 30 ? "#E0D500" :: "#F27900"}
{val:0_userdata.0.CO2-Ampel.Ampel_Studio.temp; val < 10 ? "#008DF0" :: val < 20 ? "#33CC00" :: val < 30 ? "#E0D500" :: "#F27900"}
{val:0_userdata.0.CO2-Ampel.Ampel_Werkstatt.temp; val < 10 ? "#008DF0" :: val < 20 ? "#33CC00" :: val < 30 ? "#E0D500" :: "#F27900"}

{val:0_userdata.0.CO2-Ampel.Ampel_Labor.hum; val < 50 ? "#008DF0" :: val < 70 ? "#33CC00" :: val < 90 ? "#E0D500" :: "#F27900"}
{val:0_userdata.0.CO2-Ampel.Ampel_Studio.hum; val < 50 ? "#008DF0" :: val < 70 ? "#33CC00" :: val < 90 ? "#E0D500" :: "#F27900"}
{val:0_userdata.0.CO2-Ampel.Ampel_Werkstatt.hum; val < 50 ? "#008DF0" :: val < 70 ? "#33CC00" :: val < 90 ? "#E0D500" :: "#F27900"}
```

#### Ressourcen 
<!-- → Verwendete Tutorials, Materialien, Quellenangaben, etc. (wenn nichts wichtiges, dann weglassen) -->
- Ampeln: https://learn.watterott.com/de/breakouts/co2-ampel/
- MQTT Firmware: https://github.com/mariolukas/Watterott-CO2-Ampel-Plus-Firmware
- offizielle Firmware: https://learn.watterott.com/de/breakouts/co2-ampel/firmware/
- BMP280: https://www.az-delivery.de/en/products/azdelivery-bmp280-barometrischer-sensor-luftdruck-modul-fur-arduino-und-raspberry-pi
- BMP280 Mess-Script: TODO

