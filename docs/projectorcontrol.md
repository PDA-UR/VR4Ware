# projectorcontrol

*Bilder/Diagramme, Schaltpläne, etc. (wo sinnvoll) einfügen*

<!-- → Verwendung: Was macht das? Wie kann man das benutzen?, … -->
<!-- - Möglichkeit die zwei Projektoren (3 = Boden, 4 = Tafel) an-/auszuschalten
- in ioBroker-Dashboard über Buttons steuerbar
- Status wird über ein Script in ioBroker alle paar Minuten ausgelesen und aktualisiert -->

Implementierung einer Möglichkeit die zwei Projektoren im Hauptraum unabhängig voneinander und ortsungebunden an- und ausschalten zu können.
Benutzt man die Fernbedienung um die Beamer anzuschalten werden immer beide Geräte angesprochen. Dieses Problem soll hier umgangen werden.
Die Beamer sind nun über Buttons im [Admin](http://PLACEHOLDER:8082/vis-2/index.html#admin)-Dashboard steuerbar und können unabhängig voneinander an- und ausgeschaltet werden. Zusätzlich existiert ein Knopf, der einen *Vorlesungsmodus* startet. Dabei wird der Tafel-Beamer angeschaltet und der HDMI-Input auf der Kabel 5 (vorne) gesetzt.
Um die Statusanzeige in ioBroker aktuell zu halten wird mithilfe eines einfachen Blocky-Scripts alle paar Minuten der Status der Beamer abgefragt.

Hierzu kann mithilfe der HDMI-Matrix (LINK zu hdmihub.md) zwischen verschiedene In- und Outputs umgeschalten werden.


### Aufbau
<!-- → z.B.: Verkabelung, Infrastruktur, Ort, 
- [Raspberry Pi](https://www.raspberrypi.com/products/raspberry-pi-3-model-b/) über Serial-Verbindung an Beamer gehängt (1 pro Beamer)
- Script mit [paho-mqtt](https://pypi.org/project/paho-mqtt/) und [pySerial](https://pyserial.readthedocs.io/en/latest/index.html)
  - empfängt und sendet MQTT Nachrichten über Power on/off-Befehle und Statusabfragen
  - RS232-Befehle an Beamer über Serial-Schnittstelle mithilfe von HexCodes
  - LINK ZU (BSP-)SCRIPT (TODO)
  - Status wird über ein ioBroker Script im 5min Takt abgefragt, damit die Visualisierung und die Realität übereinstimmen
- Raspi-Script ist autostart -->

Zur remote Steuerung der Beamer wurden [Raspberry Pis](https://www.raspberrypi.com/products/raspberry-pi-3-model-b/) über Serial-Kabel an den Beamer gehängt. Für eine bessere Wartbarkeit wird ein Raspi pro Beamer verwendet.
Das [Skript](https://github.com/thiilo/VR4Ware/blob/main/Scripts/projector-control-3.py) verwendet [paho-mqtt](https://pypi.org/project/paho-mqtt/) um MQTT-Nachrichten aus ioBroker zu empfangen und verarbeiten. Danach werden die richtigen RS232-Befehle mit [pySerial](https://pyserial.readthedocs.io/en/latest/index.html) über die Serial-Schnittstelle  an den Beamer weitergeleitet. Die Befehle sind in [HexCodes](#rs232-hexcodes-für-viewsonic-projektoren) angegeben.
Um die unabhängige Benutzung der Beamer (physisch und digital) zu gewährleisten läuft in ioBroker ein Blocky-Script, dass im 5min Takt den Status der Beamer abfragt. Hierzu wird ein MQTT gesendet und mit einem RS232-Befehl der Status der Beamer abgefragt. Wird ein Beamer direkt über das ioBroker-Dashboard geändert, wird diese Veränderung sofort verarbeitet und angezeigt.

Die Skripte auf den Raspis laufen auf Autostart. Mithilfe eines Watchdogs werden die Raspis, falls sie sich ausschalten automatisch wieder hochgefahren. 


### Wartung und Troubleshooting
<!-- → Wie kommt man ran?, Was kann man einfach ändern?, Bugs, die uns begegnet sind und wie sie gelöst wurden, … -->

Verkabelung: Beamer –SerialKabel-→  Serial auf USB Adapter  -→ Raspi -→ Ladekabel
<!-- TODO: kleines/einfaches Diagram? -->

Die *Projectorcontroller* sollte reibungslos funktionieren. Aus Erfahrung fallen sie jedoch immer mal wieder aus.
Vor allem Beamer 3 (Tafel) scheint anfällig zu sein. Ein einfacher reboot sollte die meisten Probleme beseitigen. Für mehr Infos siehe [hier](#anmerkungen).

Zusätzlich kommt das Problem auf, dass die Beamer während sie sich hoch- oder runterfahren nicht ansprechbar sind. Um ungewollte Anfragen (durch ioBroker oder direkt vom Skript) zu unterbinden, wurde zusätzlich ein 30 Sekunden `sleep` im Skript eingebaut. Dieser wird ausgelöst nachdem ein an/aus-Befehl an das Gerät geschickt wurde. Die Dauer wurde formlos während Testabfragen gemessen.

Wird ein Raspi entfernt oder ausgetauscht, so muss dieser zuerst an den Serial-Port angeschlossen werden, bevor man ihn an den Strom ansteckt. Da das Skript sofort startet, wenn der Raspi hochfährt, muss die Serial-Verbindung stehen, bevor das Skript startet. Ansonsten terminiert das Skript wegen eines nicht gefundenen Ports.

<!-- - Status von Beamer kann nicht ausgelesen werden, wenn diese gerade hoch-/runterfährt
  - Deswegen ist im Skript ein 30sec sleep eingebaut, bevor die Serielle Schnittstelle wieder geschlossen wird
- Erst Serial anschließen, dann Strom — ansonsten ist erst beim Booten kein Gerät per Serial verbunden und Skript schmeißt einen Fehler. -->

#### Zugang und Kontrolle über ssh
Die Raspi's sind über ihre statischen **IP-Adressen** zugänglich:

| Beamer    | Raspberry Pi       | IP-Adresse    |
| --------- | ------------------ | ------------- |
| 3 (Tafel) | projectorcontrol-3 | 192.168.2.113 |
| 4 (Boden) | projectorcontrol-4 | 192.168.2.114 |

Einloggen kann man sich mit den Befehl: `ssh projectorcontrol@192.168.2.113` → passwort.
Das Passwort hierfür steht auf dem Case der Raspi's (gleiches Passwort).
Eine Anfrage der aktuellen Prozesse (z.B.: mit `ps -aux | grep python`) sollte mehrere laufende python Prozesse ausgeben.

Zusätzlich liegt auf dem Raspi-3 eine log-Datei mit dem Dateipfad: `/home/projectorcontrol/Dokumente/proj3.log`. Diese kann etwa mit *nano* ausgelesen werden. Der erweiterte Code findet sich [hier](https://github.com/thiilo/VR4Ware/blob/main/Scripts/projector-control-3-log.py)

<!-- - z.B.: `ssh projectorcontrol@192.168.2.113` → pw
- Aktuelle python Prozesse ausgeben: `ps -aux | grep python`
- projectorcontrol-3 hat eine log-Datei:
  - `/home/projectorcontrol/Dokumente/proj3.log` -->

#### Topics
Folgende Topics schicken und senden MQTT-Nachrichten zwischen den Raspi's und ioBroker:

| Topic                  | Message    | Aktion                   |
| ---------------------- | ---------- | ------------------------ |
| projector/3/out/power  | true/false | Beamer 3 an-/ausschalten |
| projector/3/out/status | true/false | Status 3 abfragen        |
| projector/3/out/power  | true/false | Beamer 3 an-/ausschalten |
| projector/3/in/status  | true/false | Status 3 zurückgeben     |
| projector/4/out/power  | true/false | Beamer 4 an-/ausschalten |
| projector/4/out/status | true/false | Status 4 abfragen        |
| projector/4/in/status  | true/false | Status 4 zurückgeben     |

Die dazugehörigen ioBroker-Objekte sind im Objekt-Tab unter dem MQTT-Ordner zugänglich (Ordner-Struktur) und sind über interaktive Buttons im Dashboard eingebunden.

#### Kommunikation über Serial Kabel
Testweise kann man mit dem PC direkt mit dem Beamer kommunizieren. Der Aufbau ist wie folgt:
- Beamer → Serial Kabel → Serial auf USB Adapter → USB-A F auf M Kabel → PC
Mit [CoolTerm](https://freeware.the-meiers.org/) oder einer ähnlichen Applikation kann auf den Beamer zugegriffen werden.
Anleitung mit CoolTerm:
  - "Send String"/"Neuen String" + Hex-Input
  - Power ON: 0614000400341100005D
  - Power OFF: 0614000400341101005E
  - HEXcodes aus der [Doku](https://viewsonicvsa.freshdesk.com/support/solutions/articles/43000470420-viewsonic-projector-rs232-protocol) (S.17), mit 0x und Leerzeichen entfernt.

---

#### Anmerkungen
<!-- → Zusätzlicher Punkt für Notizen/Anmerkungen, etc. (wenn nichts wichtiges, dann weglassen) -->

<!-- - Anschluss der Pi's an die Projektoren geht nur mit Serial-Kabel, Adapter direkt an Beamer klappt nicht 
- Installation eines Hardware [Watchdogs](https://diode.io/blog/running-forever-with-the-raspberry-pi-hardware-watchdog) um den Raspi neu zu starten, wenn dieser ausgeht
- Bei Raspi3 gab es Probleme mit den Autorun des Skripts, da dieses Probleme hatte sich mit dem MQTT-Broker zu verbinden, was das Skript beendet hat:
  - Ändern von `client.loop_forever()` auf `client.loop_forever(retry_first_connection=True)` scheint das Problem zu lösen -->

Der Anschluss der Raspi's an die Beamer geht nur mit Serial-Kabel. Den Adapter direkt an den Beamer und dann mit einem USB-Kabel an den Raspi hat nicht funktioniert.

Für eine robustere Laufzeit wurde auf beiden Raspis ein Hardware [Watchdogs](https://diode.io/blog/running-forever-with-the-raspberry-pi-hardware-watchdog) installiert. Dieser startet die Raspi's neu, wenn diese unerwartet ausgehen. Da die Skripte auf Autorun laufen, werden auch sie automatisch neu gestartet.

Bei Raspi-3 (Tafel) gab es Probleme mit den Autorun des Skripts, da es er sich nicht immer mit MQTT-Broker verbinden konnte was dann unerwartet das Skript beendet hat. Hier wurde ein Hotfix vorgenommen und `client.loop_forever()` auf `client.loop_forever(retry_first_connection=True)` geändert. Dies schien das Problem zu beheben.
(Auf dem anderen Raspi wurde das Problem nicht beobachtet.)

Da es weitere Probleme mit diesem Raspi/Beamer gab, wurde eine automatischer Restart um 4 Uhr Nachts eingerichtet (siehe z.B.: [hier](https://www.machs-smart.de/raspberry-reboot-neustart-automatisch-taeglich-crontab/)). Meistens schien ein einfacher Restart die gefundenen Probleme, wie etwa Unansprechbarkeit, zu beheben.


##### Einen Raspi einrichten
Beide Raspberries wurden mit Debian 12 (Bookworm-64bit) aufgesetzt. Nach dem Installationsprozess muss das Skript hochgeladen (z.B.: mit USB-Stick) und, wenn nötig, angepasst werden (z.B.: Topics updaten).
*paho-mqtt* wurde global mit  `sudo apt install python3-paho-mqtt` installiert, da es sonst environment Probleme gab.
Autorun wurde mithilfe dieser [Anleitung]((https://www.dexterindustries.com/howto/run-a-program-on-your-raspberry-pi-at-startup/)) eingerichtet. Speziell folgende Änderungen vorgenommen werden:
- in rc.local: `sudo python3 /home/projectorcontrol/Dokumente/projector-control-3.py &` (je nach Pfad)
Zusätzlich wurden SSH einrichten:
  - im Raspi: Einstellungen → Raspberry Pi Konfiguration: Schnittstellen → SSH an
  - im Router die IP-Adresse statisch setzten: Netzwerk → DHCP und DNS → Statische Einträge

<!-- - Debian 12 Installieren (Bookworm) und Raspi einrichten
- Script hochladen (z.b.: mit USB-Stick) und wenn nötig anpassen (z.B.: Topics updaten)
- im Terminal:
  - `sudo apt install python3-paho-mqtt` (via: sudo apt search paho-mqtt)
  - Autorun einrichten ([Anleitung](https://www.dexterindustries.com/howto/run-a-program-on-your-raspberry-pi-at-startup/))
  - in rc.local: `sudo python3 /home/projectorcontrol/Dokumente/projector-control-3.py &`
- SSH einrichten:
  - im Raspi: Einstellungen → Raspberry Pi Konfiguration: Schnittstellen → SSH an
  - im Router die IP-Adresse statisch setzten: Netzwerk → DHCP und DNS → Statische Einträge -->

##### RS232 HEXcodes für Viewsonic Projektoren

Ein Überblick über die genutzten RS232-Befehle und ihrer Verwendungsart:

Befehl | HEXcode | CoolTerm-String | pySerial |
|----- | ------- | --------------- | -------- |
| ON | 0x06 0x14 0x00 0x04 0x00 0x34 0x11 0x00 0x00 0x5D | 0614000400341100005D | `bytearray([0x06,0x14,0x00,0x04,0x00,0x34,0x11,0x00,0x00,0x5D])` |
| OFF | 0x06 0x14 0x00 0x04 0x00 0x34 0x11 0x01 0x00 0x5E | 0614000400341101005E | `bytearray([0x06,0x14,0x00,0x04,0x00,0x34,0x11,0x01,0x00,0x5E])` |
| Status | 0x07 0x14 0x00 0x05 0x00 0x34 0x00 0x00 0x11 0x00 0x5E | 071400050034000011005E | `bytearray([0x07,0x14,0x00,0x05,0x00,0x34,0x00,0x00,0x11,0x00,0x5E])` |

Weitere HEXcodes sind der offiziellen [Doku](https://viewsonicvsa.freshdesk.com/support/solutions/articles/43000470420-viewsonic-projector-rs232-protocol) (S.17) zu entnehmen.


#### Ressourcen 
<!-- → Verwendete Tutorials, Materialien, Quellenangaben, etc. (wenn nichts wichtiges, dann weglassen) -->
- Raspberry Pi 3: https://www.raspberrypi.com/products/raspberry-pi-3-model-b/
- paho-mqtt: https://pypi.org/project/paho-mqtt/
- pySerial: https://pyserial.readthedocs.io/en/latest/index.html
- ViewSonic RS232 protocol: https://viewsonicvsa.freshdesk.com/support/solutions/articles/43000470420-viewsonic-projector-rs232-protocol
- Beamer User Guide: https://www.viewsonicglobal.com/public/products_download/user_guide/Projector/LS700-4K/LS700-4K_UG_ENG.pdf?pass
- Warum kein D1 mini: https://forum.arduino.cc/t/how-to-wire-rs232-serial-to-rx-tx/26212
- CoolTerm: https://coolterm.en.lo4d.com/windows, https://freeware.the-meiers.org/
- Autorun: https://www.dexterindustries.com/howto/run-a-program-on-your-raspberry-pi-at-startup/
- Hardware Watchdog: https://pimylifeup.com/raspberry-pi-watchdog/, https://diode.io/blog/running-forever-with-the-raspberry-pi-hardware-watchdog
- loop_forever(): https://stackoverflow.com/a/78868318