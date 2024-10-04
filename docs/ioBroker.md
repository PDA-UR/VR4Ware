# ioBroker
<!-- Umsetzungen in ioBroker + quick-guide -->

## Umsetzung
<!-- Infos über Visualisierungen und Skripte -->
- Visualisierung mit [vis-2](https://github.com/ioBroker/ioBroker.vis-2)
- Orientierung am Konzept des [Digitalen Zwillings](https://de.wikipedia.org/wiki/Digitaler_Zwilling) (?? bessere Quelle als Wikipedia?)
  - Möglichst realistischen digitale Repräsentation des echten VR4 mithilfe eines abstrahierten Raum-Plans
  - Leichteres Verständnis wie etwas funktioniert und wo im Raum es sich befindet
  - z.B.: Visualisierung der Beziehung zwischen dem Matrix-Switch (LINK) und den Projektoren (LINK) über ein Data-Flow-Diagramm. Damit sieht man direkt die aktuell genutzten HDMI-Verbindungen und welche Beamer an-/aus geschaltet sind  

- Nutzung des [MQTT](https://de.wikipedia.org/wiki/MQTT)-Protokolls zum senden und empfangen von der meisten Daten von verschiedenen Mikrocontrollern (Wemos d1 mini, C02-Ampel, RaspberryPi)
  - via: [MQTT-Adapter](https://github.com/ioBroker/ioBroker.mqtt)
  - Nachrichten werden von ioBroker gelesen und können direkt in vis eingebunden werden
  - Nachrichten können über vis in das MQTT-Objekt geschrieben und dann direkt von ioBroker geschickt werden
  - Die Microcontroller horchen auf ihr spezifisches Topic und verarbeiten die empfangen Nachrichten
- Nutzung von [SNMP](https://de.wikipedia.org/wiki/Simple_Network_Management_Protocol) für Kommunikation zwischen existierenden Geräten (Drucker(LINK), PCs(LINK))
  - via: [SNMP-Adapter](https://github.com/iobroker-community-adapters/ioBroker.snmp)


- Nutzung von Skripten für Automatisierungen oder notwendige Anpassungen von Daten oder Visualisierungen
- Nutzung von existierenden Adaptern wenn möglich:
  - z.B.: Auslesen des Busfahrplans via [Fahrplan Adapter](https://github.com/gaudes/iobroker.fahrplan/)

## Cheat sheet
<!-- Wichtige Funktionalitäten, Tips und Tricks -->
### vis(2)
- Einbindung von Werten aus dem Object-Tab:
  - `{object.value.id}`
  - [Quelle](https://forum.iobroker.net/topic/37043/test-adapter-fahrplan-v0-2-x/147?lang=de)
  - Funktioniert im den Feldern der CSS-Feldern der Widget-Einstellungen und im HTML-Code des HTML-Widget
- Konditionen eines Datenpunkts über Ternäre Operatoren:
  - z.B.: active color: `{val:0_userdata.0.CO2-Ampel.Ampel_Labor.co2; val >= 800 ? "#E0D500" :: val >= 1000 ? "#F50000" :: "#33CC00"}`
  - [1](https://forum.iobroker.net/topic/14224/textfarbe-f%C3%BCr-bestimmtes-level-definieren/9?_=1637858753354) und [2](https://forum.iobroker.net/topic/26740/vis-background-color-mit-datenpunkt-steuern)

**HTML-Widget:**
- Einbindung einer Funktion aus dem Skript-Tab:
  - `<tag>{val:object.value.id;functionName(val)}</tag>`
  - [Mehrere Variablen](https://forum.iobroker.net/topic/49473/gel%C3%B6st-vis-binding-und-verkn%C3%BCpfung-mehrerer-variablen)
- Date Object:
  - `{dateObject.value.id;date(DD.MM.YY - hh:mm)}`
  - [Quelle](https://github.com/gaudes/ioBroker.fahrplan/issues/167#issuecomment-1024989008)

### Scripts (JS-Tab)
> Offizielle [Doku](https://github.com/ioBroker/ioBroker.javascript/blob/master/docs/en/javascript.md#content)
- `write/readFile()` braucht die *root directory*, die unter dem "Files"-Tab eingesehen werden kann
  - z.B.: `writeFile('0_userdata.0', '/calendar.png', buffer, function (error) { });`
  - [Quelle](https://forum.iobroker.net/topic/25541/gel%C3%B6st-wie-writefile-in-js-script-nutzen)
  - [offizielle Docs](https://github.com/ioBroker/ioBroker.javascript/blob/master/docs/en/javascript.md#writefile)

#### Events
##### ioBroker Events
- Horchen auf Werteänderung eines Objektes mit `on(id, function(){...} )`
  - [Quelle](https://forum.iobroker.net/topic/62756/json-parse-array-aus-datenpunkt/7)

##### Zugriff von vis auf JS Funktionen
- von vis aus kann nicht direkt auf die JS-Script Funktionen zugegriffen werden, da diese im Browser und nicht auf dem Server laufen
  - Umweg über Keyword: `vis.[function]`
  - z.B.: Wert für Objekt setzten: `vis.setValue("object.value.id, value);`
  - Siehe auch: [1](https://forum.iobroker.net/topic/35582/html-checkbox-zugriff-auf-state/39) und [2](https://forum.iobroker.net/topic/63323/%C3%BCber-html-widget-mit-toggle-button-datenpunkt-wert-%C3%A4ndern) und [3](https://forum.iobroker.net/topic/7395/zugriff-auf-objekt-in-script/7)


### Misc
- **npm install**
  - `ssh root@PLACEHOLDER` (passwort steht auf NUC)
  - in `../opt/iobroker` navigieren und dort packages **lokal** installieren 
  
- **Objekte**
  - Objekte können manuell nur im `0_userdata`-Ordner erstellt werden
---

### Ressourcen
- Digitaler Zwilling: https://de.wikipedia.org/wiki/Digitaler_Zwilling
- MQTT: https://de.wikipedia.org/wiki/MQTT
- ioBroker MQTT-Adapter: https://github.com/ioBroker/ioBroker.mqtt
- SNMP: https://de.wikipedia.org/wiki/Simple_Network_Management_Protocol
- ioBroker SNMP-Adapter: https://github.com/iobroker-community-adapters/ioBroker.snmp
- vis 2: https://github.com/ioBroker/ioBroker.vis-2
- ioBroker Fahrplan Adapter: https://github.com/gaudes/iobroker.fahrplan/
- Einbindung Object_ID in HTML: https://forum.iobroker.net/topic/37043/test-adapter-fahrplan-v0-2-x/147?lang=de
- Parse Date Object: https://github.com/gaudes/ioBroker.fahrplan/issues/167#issuecomment-1024989008
- Mehrere Variablen für Funktionen: https://forum.iobroker.net/topic/49473/gel%C3%B6st-vis-binding-und-verkn%C3%BCpfung-mehrerer-variablen/
- Ternäre Operatoren: 
  - https://forum.iobroker.net/topic/14224/textfarbe-f%C3%BCr-bestimmtes-level-definieren/9?_=1637858753354
  - https://forum.iobroker.net/topic/26740/vis-background-color-mit-datenpunkt-steuern
- offiziell Javascript Doku: https://github.com/ioBroker/ioBroker.javascript/blob/master/docs/en/javascript.md#content
- write/readFile(): https://forum.iobroker.net/topic/25541/gel%C3%B6st-wie-writefile-in-js-script-nutzen
- write/readFile() (off.): https://github.com/ioBroker/ioBroker.javascript/blob/master/docs/en/javascript.md#writefile
- on(): https://forum.iobroker.net/topic/62756/json-parse-array-aus-datenpunkt/7
- vis Zugriff auf js:
  - https://forum.iobroker.net/topic/35582/html-checkbox-zugriff-auf-state/39
  - https://forum.iobroker.net/topic/63323/%C3%BCber-html-widget-mit-toggle-button-datenpunkt-wert-%C3%A4ndern
  - https://forum.iobroker.net/topic/7395/zugriff-auf-objekt-in-script/7
