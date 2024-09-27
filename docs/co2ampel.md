# CO2-Ampeln

*Bilder/Diagramme, Schaltpläne, etc. (wo sinnvoll) einfügen*

<!-- → Verwendung: Was macht das? Wie kann man das benutzen?, … -->

### Aufbau
<!-- → z.B.: Verkabelung, Infrastruktur, Ort,  -->
- 3 [CO2-Ampel](https://learn.watterott.com/de/breakouts/co2-ampel/): 1ne pro Raum
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
  - TODO: aktuelle Einstellungen

### Wartung und Troubleshooting
<!-- → Wie kommt man ran?, Was kann man einfach ändern?, Bugs, die uns begegnet sind und wie sie gelöst wurden, … -->

#### Firmware kompilieren
- TODO: Link zu .bin
- Änderungen in `Config.h`:
  - `#define INTERVAL_DARK 3`
  - `#define LIGHT_INTERVAL 150`

Anleitung:
- [Anleitung](https://github.com/mariolukas/Watterott-CO2-Ampel-Plus-Firmware#eigene-ampel-version-aus-den-quellen-kompilieren) folgen um Arduino IDE aufzusetzten
- *WifiWebServer*-Library auf `v.1.4.2` downgraden um kompilieren zu können (ansonsten gibt es einen Error mit einer private/public Variable)
- Änderungen in `Config.h` vornehmen
  - Alternativ zur Konfiguration mit dem Web-Frontend können hier auch MQTT und WIFI gesetzt werden
- Sicherstellen, dass in `Config.h` `#define DEBUG_LOG 0` steht. Ansonsten kann die Ampel nur über Arduino IDE mit verbundenen Port laufen gelassen werden
- `CO2-Ampel_Plus.ino` kompilieren:
  - `.bin`-File: Menüpunkt: Sketch -> Export Compiled Binary (ALT+STRG+S) und der   [offiziellen](https://learn.watterott.com/de/breakouts/co2-ampel/firmware/#aktualisierungsanleitung) oder [alternativen](https://github.com/mariolukas/Watterott-CO2-Ampel-Plus-Firmware?tab=readme-ov-file#neue-ampel-firmware-version-installieren)Aktualisierungsanleitung folgen 
  - die Ampel mit USB verbinden und den Code direkt hochladen

---

#### Anmerkungen
<!-- → Zusätzlicher Punkt für Notizen/Anmerkungen, etc. (wenn nichts wichtiges, dann weglassen) -->

##### Komische Sensordaten
- Sensordaten hatten komische Schwankungen immer um sehr ähnliche Zeit
- BILD
- Schwankungen korrelieren damit, dass das Licht an-/ausgeschaltet wird (also Lichtsensor hoch/tief)
- Firmware setzt das Measurement Intervall größer, wenn das Licht kleiner als ein bestimmten Wert hat -> also wird weniger oft gemessen
- Temperaturwerte stimmte nicht, deswegen Nachmessung mit [BMP280](https://www.az-delivery.de/en/products/azdelivery-bmp280-barometrischer-sensor-luftdruck-modul-fur-arduino-und-raspberry-pi)

##### Temperatur Offset
- eig über MQTT setzbar: `{"temperature_calibrate_set_current_roomtemp": 23.5}` siehe [hier](https://github.com/mariolukas/Watterott-CO2-Ampel-Plus-Firmware?tab=readme-ov-file#neue-ampel-firmware-version-installieren), aber ging irgendwie nicht
- Alternativ: Offset wurde in ioBroker gesetzt:
  - im gleichen Script, dass die Werte aufsplittet (TODO: LINK)
  - ist dort leicht für Änderungen oder zum Auslesen zugänglich
- MQTT: Temp offset
- in 

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

