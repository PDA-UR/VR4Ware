# Türdisplay

Um einen schnellen Überblick zur täglichen Raumnutzung zu bekommen, wurde ein Belegungsplan konzipiert, der an der Tür lesbar ist. Hier werden Reservierungen für den aktuellen Tag und in der nächsten Zeit angezeigt.
Die Daten kommen vom offiziellen [Belegungsplan](https://wiki.mi.ur.de/lab/belegung) der Uni. Daraus wird ein Bild generiert, dass auf einem e-Ink Display angezeigt wird.
Die Wiederholrate liegt bei 2 Stunden, um das Display so lange wie möglich per Akku zu betreiben.
  
## Aufbau
Verwendet wird ein *e-radionica.com* [Inkplate6](https://github.com/SolderedElectronics/Inkplate-Arduino-library), welches in einem 3D-gedruckten Gehäuse an der Tür hängt. Das E-Ink-Display kann einfach nach oben aus der Halterung gezogen werden, um es aufzuladen oder zu warten. Der Ein- und Aus-Schalter ist auch im Gehäuse erreichbar.

Auf dem Display wird ein Übersichtsplan alle zwei Stunden vom Server geholt; sobald es gezeichnet wurde, geht das Display in einen stromsparenden _Deep Sleep_.
Dieser Prozess wird durch das Skript `doordisplay_inkplate6.ino` gesteuert.

Der Übersichtsplan wird als Bild in ioBroker über das Skript *ADRESSE/NAME.js* erstellt und gespeichert.
Das Skript liest die für jeden Tag und Raum anstehenden Events aus dem öffentlich verfügbaren .ical-Kalendern des Labors aus.
Diese werden sortiert und mit Beschreibung sowie Namen angezeigt; da diese Daten im Web öffentlich sind, werden sie auch hier in relativ geschützen Räumlichkeiten angezeigt.
Innerhalb von 14 Tagen anstehende Termine werden in einer zusätzlichen Zeile angezeigt.

## Wartung und Troubleshooting

Das Inkplate kann über die Arduino IDE ([Einrichtung](https://inkplate.readthedocs.io/en/latest/get-started.html)) programmiert werden. Dabei ist es wichtig sicherzustellen, dass das richtige Board ausgewählt ist. Aktuell wird ein 
**e-radionica.com Inkplate6**.
Das Skript ist [hier](https://github.com/thiilo/VR4Ware/blob/main/doordisplay_inkplate6/doordisplay_inkplate6.ino) zu finden.

Falls das Display schon länger nicht mehr aktualisiert wurde, ist vermutlich der Akku leer. 
*Die Prozentanzeige der Batterie ist hier leider nicht verlässlich! Bitte eher nach angezeigter Uhrzeit urteilen!*
Um das zu überprüfen, den Schalter unten am Display kurz auf `Aus` und dann wieder `An` stellen. Sollte sich das Display nicht kurz (< 10s) nach dem Einschalten neu zeichnen, bitte laden!
Dazu einfach über den micro-USB-Port mit einem beliebigen 5V-Netzteil verbinden.

Auch weitere Anzeigefehler lassen sich normalerweise durch einen Power Cycle des Displays beheben, weil das Bild neu geholt und dann gezeichnet wird.

Ansonsten:
  * die Online-Version des Bildes mit dem Display abgleichen
  * die ical-Kalender der Labore überprüfen

### Anmerkungen
<!-- → Zusätzlicher Punkt für Notizen/Anmerkungen, etc. (wenn nichts wichtiges, dann weglassen) -->
Die ioBroker [Bilddatei](http://PLACEHOLDER:8081/files/0_userdata.0/calendar1.png) ist nicht ohne Anmeldung zugänglich, aber über die REST API kann man öffentlich auf das [Bild](http://PLACEHOLDER:8093/v1/file/web/calendar1.png) zugreifen.

Anfangs wurde das Inkplate alle halbe Stunde aktualisiert, um möglichst zeitnah Veränderungen zu bekommen.
Die dabei erreichte Batterielaufzeit von 160 Stunden war allerdings nicht angemessen.
Nach Erhöhen der Aktualisierungsfrequenz auf 2 Stunden hält das Inkplate deutlich länger (locker über einen Monat).
Diese 2-Stunden-Intervalle werden *ab dem ersten Anschalten* des Displays gemessen, also nicht relativ zur tatsächlichen Uhrzeit. Die interne Real-Time-Clock funktioniert leider nicht anständig mit _Deep Sleep_.

Das Skript aus ioBroker generiert alle 60 Minuten ein neues Bild und speichert es unter: `./files/0_userdata.0/calendar1.png`.

#### Ressourcen 
<!-- → Verwendete Tutorials, Materialien, Quellenangaben, etc. (wenn nichts wichtiges, dann weglassen) -->
  * [Offizieller Belegungsplan der Uni](https://wiki.mi.ur.de/lab/belegung)
  * [Inkplate-Doku](https://github.com/SolderedElectronics/Inkplate-Arduino-library)
  * [Einrichtung mit Arduino IDE](https://inkplate.readthedocs.io/en/latest/get-started.html)
  * [Belegungsplan-Bild](http://PLACEHOLDER:8081/files/0_userdata.0/calendar1.png)
  * [Belegungsplan-Bild(REST)](http://PLACEHOLDER:8093/v1/file/web/calendar1.png)
