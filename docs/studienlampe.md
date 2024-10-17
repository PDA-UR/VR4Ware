
# Studienlampe

*Bilder/Diagramme, Schaltpläne, etc. (wo sinnvoll) einfügen*
TODO: BILD

<!-- → Verwendung: Was macht das? Wie kann man das benutzen?, … -->
<!-- 
- Lampe, die anzeigt, ob eine Studie ist
- Lampe kann vor Ort über einen Schalter oder in ioBroker über eine Knopf an-/aus geschaltet werden
- Status wird in ioBroker angezeigt
- Nutzung eines Wemos [D1 mini](https://www.wemos.cc/en/latest/d1/d1_mini.html) für Logik und Kommunikation 
- -->

Die Studienlampe ist eine bedienbare Lampe, die während einer Studie angeschaltet werden kann. 
Über ein rotes Licht wird anderen Personen innerhalb und außerhalb des Raumes angezeigt, dass gerade eine Studie läuft. 
Die Lampe kann vor Ort (VR4) und innerhalb des TechBase-Netzwerks über das [Admin](http://PLACEHOLDER:8082/vis-2/index.html#admin)-Dashboard an- oder ausgeschaltet werden.
Sie läuft mithilfe eines Wemos [D1 mini](https://www.wemos.cc/en/latest/d1/d1_mini.html), der die Kommunikation mit dem Dashboard bündelt und die LEDs steuert.

### Aufbau
<!-- → z.B.: Verkabelung, Infrastruktur, Ort,  -->
<!-- 
- [Wipp-Schalter](https://www.pollin.de/p/marquardt-kontroll-wippenschalter-1656-1601-420513)
  - 1 = COM
  - 1a = NO/NC
  - LED (Schalter)
- D1-mini (Dasduino esp8266 board)
- LED-Strip (WS2812B)
  - GND, 5V, DIN
  - mit `<FastLED.h>`
- funktionierendes Skript: [`study_lamp_1.ino`](https://github.com/thiilo/VR4Ware/blob/main/Scripts/studylamp/study_lamp.ino) (LINK!!) -->

Die Lampe ist über einen [Wipp-Schalter](https://www.pollin.de/p/marquardt-kontroll-wippenschalter-1656-1601-420513) bedienbar. Innerhalb des Schalters befindet sich eine LED, die den Status der Lampe wiedergibt. 
Die Kontakte des Schalters (1 und 1a), sowie die der LED sind mit einem Wemos [D1 mini](https://www.wemos.cc/en/latest/d1/d1_mini.html) verbunden.
Zusätzlich dazu wird eine große Lightbox mithilfe eines LED-Strips betrieben, die genau wie die LED im Schalter, den Status der Lampe wiedergibt. 
Läuft eine Studie leuchten die LEDs rot, ist keine Studie sind die LEDs aus.
Ein anschaulicher Schaltplan ist dem [Diagramm](#schaltplan) zu entnehmen.

Anders als die LEDs, hängt die Positions des Wipp-Schalters nicht mit dem Status zusammen. 
Dadurch kann die Lampe auch remote über das Dashboard an- und ausgeschaltet werden und bleibt immer synchron.

<!-- TODO: Update link für public repo -->
Ein funktionierendes [Skript](https://github.com/thiilo/VR4Ware/blob/main/Scripts/studylamp/study_lamp_plus.ino) kann im Repository gefunden werden.
Es beinhaltet noch zusätzlichen Code für das betreiben einer 8x8-Matrix, die für die finale Umsetzung nicht verwendet wurde. 
Ein (ungetestetes) Skript ohne diese Funktionen, ist [hier](https://github.com/thiilo/VR4Ware/blob/main/Scripts/studylamp/study_lamp.ino) zu finden. 

#### Schaltplan
![A circuit diagram for the study lamp](images/study_lamp_circuit.png)

#### Interne Logik und Kommunikation:
<!-- - mit MQTT via [pubsubclient](https://pubsubclient.knolleary.net/) und `<esp8266Wifi.h>` (kommt mit Board)
- ioBroker:
  - über einen Button in vis wird das Objekt *mqtt.0.remote_study_lamp* auf true/false gesetzt und damit ein MQTT in die Lampe geschickt um ihren Status upzudaten
  - die Lampe schreibt ihren Status in das Objekt *mqtt.0.study_lamp* und dieser wird dann in ioBroker widergespiegelt
  - Theoretisch würde diese Funktionalität mit nur einem einzigen Objekt gehen, aber für bessere Wartbarkeit und Robustheit wurden aus- und eingehende MQTTs getrennt
  - Je nach Status werden die LEDs (im Schalter + LED-Strip) an- oder ausgeschaltet. Der Wipp-Schalter ist nur zum Umschalten des Statuses -->
Die Lampe wird über einen Wemos d1 mini gesteuert. Dieser kann mithilfe der Arduino IDE programmiert werden ([Setup](#setup-arduino-ide))
Über [MQTT](https://de.wikipedia.org/wiki/MQTT)-Nachrichten kommuniziert die Lampe mit ioBroker. Dafür werden im Code die Bibliotheken [pubsubclient](https://pubsubclient.knolleary.net/) und `<esp8266Wifi.h>` (kommt mit dem Board) verwendet. 
In ioBroker (LINK iobroker.md) wird ein MQTT-Adapter genutzt um die Nachrichten in Topics zu lesen und zu senden.
Die im Dashboard existierenden Buttons schreiben in die ioBroker-Objekte, die die Topics darstellen. Wenn also das Objekt *mqtt.0.remote_study_lamp* auf `true` oder `false` gesetzt wird, schickt ioBroker eine Nachricht mit dem Topic *remote_study_lamp* an die Lampe, die daraus folgend ihren Status ändert.
Die Lampe selber schreibt ihren Status, wird dieser geändert, über das Topic *study_lamp* in das ioBroker-Objekt *mqtt.0.study_lamp*. Dieses Objekt wird dann im Dashboard genutzt um den aktuellen Status in einem interaktiven Knopf widerzuspiegeln.
Theoretisch würde diese Funktionalität auch in einem einzigen Objekt/Topic funktionieren, für eine bessere Wartbarkeit und Robustheit wurden ein- und ausgehende MQTT jedoch getrennt.
Innerhalb der Lampe wird der Status, der entweder remote über MQTT von ioBroker, oder vorort von dem Wipp-Schalter kommt, verarbeitet. Ist dieser `true` wird die Lampe (also alle LEDs) angeschaltet.
Der Wipp-Schalter selber ist nur zum Umschalten des Status gedacht und seine Position repräsentiert diesen nicht.

### Wartung und Troubleshooting
<!-- → Wie kommt man ran?, Was kann man einfach ändern?, Bugs, die uns begegnet sind und wie sie gelöst wurden, … -->
#### Setup Arduino IDE
<!-- - gutes Tutorial: [Setup-Guide](https://chewett.co.uk/blog/937/configuring-wemos-d1-mini-pro-esp8266-arduino-ide/) für Wemos D1 mini Pro
- [Pinout des D1 mini](https://chewett.co.uk/blog/1066/pin-numbering-for-wemos-d1-mini-esp8266/)
- zu installierende Libraries: pubsubclient (knolleary), FastLED (FastLED) -->

Der Code des d1 minis kann über ein USB-Kabel upgedatet werden. Hierzu einfach den Deckel des Kabelkanals nach oben schieben (und den unteren Teil ablösen) bis man zu einer USB-auf-USB-Schnittstelle kommt.

Um den Wemos d1 mini zu programmieren kann man die [Arduino IDE](https://www.arduino.cc/en/software) verwenden. 
Benötigt wird das `Dasduino esp8266` Board. 
Die Libraries *[pubsubclient](https://pubsubclient.knolleary.net/)* und *FastLED* können mithilfe des Library Managers direkt in der IDE heruntergeladen werden. Außerdem wird die `<esp8266Wifi.h>` Library benötigt.
Für weitere Informationen siehe (LINK zu arduinoIDE.md)
<!-- Zusätzlich müssen die [Pinouts](https://chewett.co.uk/blog/1066/pin-numbering-for-wemos-d1-mini-esp8266/) des d1 minis bekannt sein -->

---

#### Anmerkungen
<!-- → Zusätzlicher Punkt für Notizen/Anmerkungen, etc. (wenn nichts wichtiges, dann weglassen) -->
<!-- - eigentlich alle verwendeten Libraries haben gut Beispiele, die über Arduino IDE einfach zugänglich sind (`File > Examples > ...`) -->
<!-- - Anfangs wurde die Lampe mit einem blinkenden LED-Strip konzipiert, da die Idee war auf die Studie möglichst deutlich aufmerksam zu machen. Nach Feedback von Studierenden wurde das Blinken auf ein statisches Leuchten geändert, da es fast gefährlich wirkte
- Der Code des d1 minis kann über ein USB-Kabel upgedatet werden. Hierzu einfach den Deckel des Kabelkanals nach oben schieben (und den unteren Teil ablösen) bis man zu einer USB-auf-USB-Schnittstelle kommt.
- Arduino IDE: `#define ...` funktioniert nur, wenn eine Library davor geladen wird
- Light-Sleep und Deep-Sleep wurden nicht implementiert, da der Wemos am Netz hängt 
  - Test mit light-Sleep: das Aufwecken ist nicht sehr schnell ist (mit Internet verbinden und dann mit MQTT-Broker verbinden), dass heißt es würde evtl. Delay geben
  - Deep-Sleep nur etwa 71min am Stück schlafen kann -->

Während der Implementierungsphase wurde die Lampe mit einem blinkenden LED-Strip konzipiert. Die Idee war damit möglichst viel Aufmerksamkeit darauf zu lenken, das aktuell eine Studie läuft. Nach Feedback von Studierenden wurde das Blinken auf ein statisches Leuchten geändert. Empfunden wurde das Blinken, weniger als *eye catcher* und mehr als Gefahrenquelle ("so als würde etwas gleich in die Luft fliegen"). Der Code für das Blinken ist noch im Skript zu finden.

Eine Implementierung von Light- oder Deep-Sleep wurde nicht vorgenommen. Nach einigen Tests mit dem Light-Sleep modus wurde festgestellt, dass das Aufwecken eine merkbare Dauer in Anspruch nimmt, sodass die Funktion nur über die Nacht sinnvoll wäre. Das hat damit zu tun, dass die Internetverbindung für beide Sleep-Modi unterbrochen wird. Zusätzlich kamen Probleme mit der neuen Verbindung zum MQTT-Broker auf, die nach der Internetverbindung statt findet.
Deep Sleep hingegen funktioniert nur 71 Minuten am Stück. Das heißt für eine Implementierung über Nacht müsste die Internetverbindung alle 71 Minuten wieder aufgebaut, die Zeit gecheckt und entschieden werden, ob noch einen weiteren Zyklus geschlafen wird.
Zusätzlich sind diese Implementierung abhängig von der Annahme, dass keine Studien in einem bestimmten Zeitraum stattfinden, was zwar wahrscheinlich ist, aber nicht gewährleistet werden kann.
Zwar gibt es die Möglichkeit das Board über einen Knopfdruck (also etwa das Umlegen des Schalters) nach unbegrenzter Zeit wieder aufzuwecken, dies jedoch bringt wieder das Problem mit der Startup Zeit, was dazu führt, dass Studierende den Schalter umlegen, um eine Studie zu starten, die Lampe nicht angeht, weil sich das Board noch mit dem Internet und MQTT-Broker verbinden muss und noch gar keine Inputs verarbeiten kann. Die Lampe bleibt also aus und müsste nach einiger Zeit erneut umgelegt werden.
Aus diesen Gründen, und da die Lampe im Netzbetrieb läuft, wurden Light- oder Deep-Sleep nicht implementiert.

#### Ressourcen 
<!-- → Verwendete Tutorials, Materialien, Quellenangaben, etc. (wenn nichts wichtiges, dann weglassen) -->
- Wemos D1 mini: https://www.wemos.cc/en/latest/d1/d1_mini.html
- FastLED.h : https://github.com/FastLED/FastLED
- Produktbeschreibung Wipp-Schalter: https://www.pollin.de/p/marquardt-kontroll-wippenschalter-1656-1601-420513
- pubsubclient: https://pubsubclient.knolleary.net/
- D1 mini Arduino IDE Setup-Guide: https://chewett.co.uk/blog/937/configuring-wemos-d1-mini-pro-esp8266-arduino-ide/
- D1 mini pinout: https://chewett.co.uk/blog/1066/pin-numbering-for-wemos-d1-mini-esp8266/
- light/deep-sleep: https://makesmart.net/blog/read/esp8266-deep-sleep-alles-was-du-ueber-den-stromsparmodus-wissen-musst, https://www.losant.com/blog/making-the-esp8266-low-powered-with-deep-sleep
- Arduino IDE: https://www.arduino.cc/en/software