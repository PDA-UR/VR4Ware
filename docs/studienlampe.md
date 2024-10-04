
# Studienlampe

*Bilder/Diagramme, Schaltpläne, etc. (wo sinnvoll) einfügen*
TODO: BILD

<!-- → Verwendung: Was macht das? Wie kann man das benutzen?, … -->
- Lampe, die anzeigt, ob eine Studie ist
- Lampe kann vor Ort über einen Schalter oder in ioBroker über eine Knopf an-/aus geschaltet werden
- Status wird in ioBroker angezeigt
- Nutzung eines Wemos [D1 mini](https://www.wemos.cc/en/latest/d1/d1_mini.html) für Logik und Kommunikation

### Aufbau
<!-- → z.B.: Verkabelung, Infrastruktur, Ort,  -->

- [Wipp-Schalter](https://www.pollin.de/p/marquardt-kontroll-wippenschalter-1656-1601-420513)
  - 1 = COM
  - 1a = NO/NC
  - LED (Schalter)
- D1-mini (Dasduino esp8266 board)
- LED-Strip (WS2812B)
  - GND, 5V, DIN
  - mit `<FastLED.h>`
- funktionierendes Skript: [`study_lamp_1.ino`](https://github.com/thiilo/VR4Ware/blob/main/Scripts/studylamp/study_lamp.ino) (LINK!!)

![A circuit diagram for the study lamp](images/study_lamp_circuit.png)

#### Kommunikation:
- mit MQTT via [pubsubclient](https://pubsubclient.knolleary.net/) und `<esp8266Wifi.h>` (kommt mit Board)
- ioBroker:
  - über einen Button in vis wird das Objekt *mqtt.0.remote_study_lamp* auf true/false gesetzt und damit ein MQTT in die Lampe geschickt um ihren Status upzudaten
  - die Lampe schreibt ihren Status in das Objekt *mqtt.0.study_lamp* und dieser wird dann in ioBroker widergespiegelt
  - Theoretisch würde diese Funktionalität mit nur einem einzigen Objekt gehen, aber für bessere Wartbarkeit und Robustheit wurden aus- und eingehende MQTTs getrennt
  - Je nach Status werden die LEDs (im Schalter + LED-Strip) an- oder ausgeschaltet. Der Wipp-Schalter ist nur zum Umschalten des Statuses

### Wartung und Troubleshooting
<!-- → Wie kommt man ran?, Was kann man einfach ändern?, Bugs, die uns begegnet sind und wie sie gelöst wurden, … -->
#### Setup Arduino IDE
- gutes Tutorial: [Setup-Guide](https://chewett.co.uk/blog/937/configuring-wemos-d1-mini-pro-esp8266-arduino-ide/) für Wemos D1 mini Pro
- [Pinout des D1 mini](https://chewett.co.uk/blog/1066/pin-numbering-for-wemos-d1-mini-esp8266/)
- zu installierende Libraries: pubsubclient (knolleary), FastLED (FastLED)

---

#### Anmerkungen
<!-- → Zusätzlicher Punkt für Notizen/Anmerkungen, etc. (wenn nichts wichtiges, dann weglassen) -->
- eigentlich alle verwendeten Libraries haben gut Beispiele, die über Arduino IDE einfach zugänglich sind (`File > Examples > ...`)
- Anfangs wurde die Lampe mit einem blinkenden LED-Strip konzipiert, da die Idee war auf die Studie möglichst deutlich aufmerksam zu machen. Nach Feedback von Studierenden wurde das Blinken auf ein statisches Leuchten geändert, da es fast gefährlich wirkte
- Der Code des d1 minis kann über ein USB-Kabel upgedatet werden. Hierzu einfach den Deckel des Kabelkanals nach oben schieben (und den unteren Teil ablösen) bis man zu einer USB-auf-USB-Schnittstelle kommt.
- Arduino IDE: `#define ...` funktioniert nur, wenn eine Library davor geladen wird
- Light-Sleep und Deep-Sleep wurden nicht implementiert, da der Wemos am Netz hängt 
  - Test mit light-Sleep: das Aufwecken ist nicht sehr schnell ist (mit Internet verbinden und dann mit MQTT-Broker verbinden), dass heißt es würde evtl. Delay geben
  - Deep-Sleep nur etwa 71min am Stück schlafen kann


#### Ressourcen 
<!-- → Verwendete Tutorials, Materialien, Quellenangaben, etc. (wenn nichts wichtiges, dann weglassen) -->
- Wemos D1 mini: https://www.wemos.cc/en/latest/d1/d1_mini.html
- FastLED.h : https://github.com/FastLED/FastLED
- Produktbeschreibung Wipp-Schalter: https://www.pollin.de/p/marquardt-kontroll-wippenschalter-1656-1601-420513
- pubsubclient: https://pubsubclient.knolleary.net/
- D1 mini Arduino IDE Setup-Guide: https://chewett.co.uk/blog/937/configuring-wemos-d1-mini-pro-esp8266-arduino-ide/
- D1 mini pinout: https://chewett.co.uk/blog/1066/pin-numbering-for-wemos-d1-mini-esp8266/
- light/deep-sleep: https://makesmart.net/blog/read/esp8266-deep-sleep-alles-was-du-ueber-den-stromsparmodus-wissen-musst, https://www.losant.com/blog/making-the-esp8266-low-powered-with-deep-sleep
