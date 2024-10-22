# Türdisplay

<!-- *Bilder/Diagramme, Schaltpläne, etc. (wo sinnvoll) einfügen* -->
TODO: BILD

<!-- → Verwendung: Was macht das? Wie kann man das benutzen?, … -->

Um einen schnellen Überblick zu bekommen wurden ein Belegungsplan konzipiert der an die Tür gehängt wird. Hier werden Reservierungen für den aktuellen und folgenden Tag angezeigt.
Die Daten kommen vom offiziellen [Belegungsplan](https://wiki.mi.ur.de/lab/belegung) der Uni. Daraus wird ein Bild generiert, dass auf einem e-Ink Display angezeigt wird.

<!-- - ein Belegungsplan, der an der Tür hängt
- zeigt Reservierungen für den aktuellen Tag und die nächste folgende Belegung
- nutzt den offiziellen [Belegungsplan](https://wiki.mi.ur.de/lab/belegung) der Uni und generiert daraus ein Bild, dass auf einem e-Ink Display angezeigt wird
-  -->
  
### Aufbau
<!-- → z.B.: Verkabelung, Infrastruktur, Ort,  -->
Verwendet wird ein *e-radionica.com* [Inkplate6](https://github.com/SolderedElectronics/Inkplate-Arduino-library) das in bestimmten Abständen das generierte Bild von ioBroker holt und dann in einen Deep Sleep geht.
Dieses Bild wird in ioBroker über ein Skript (LINK) erstellt und gespeichert.

<!-- - e-radionica.com [Inkplate6](https://github.com/SolderedElectronics/Inkplate-Arduino-library)
- Skript in ioBroker, dass die Daten vom Belegungsplan zieht und daraus ein Bild generiert, dass in ioBroker gespeichert wird
- Inkplate holt in bestimmten Abständen das Bild von ioBroker und geht dann wieder in Deep Sleep -->

### Wartung und Troubleshooting
<!-- → Wie kommt man ran?, Was kann man einfach ändern?, Bugs, die uns begegnet sind und wie sie gelöst wurden, … -->
Das Inkplate kann über die Arduino IDE ([Einrichtung](https://inkplate.readthedocs.io/en/latest/get-started.html)) programmiert werden. Dabei ist es wichtig sicherzustellen, dass das richtige Board ausgewählt ist. Aktuell wird ein 
**e-radionica.com Inkplate6**.
Das Skript ist [hier](https://github.com/thiilo/VR4Ware/blob/main/doordisplay_inkplate6/doordisplay_inkplate6.ino) zu finden.
<!-- - Änderung des Skriptes erfolgt über Arduino IDE ([Einrichtung](https://inkplate.readthedocs.io/en/latest/get-started.html))
- Sicherstellen, dass das ausgewählte Board: **e-radionica.com Inkplate6** ist
-  -->

---

#### Anmerkungen
<!-- → Zusätzlicher Punkt für Notizen/Anmerkungen, etc. (wenn nichts wichtiges, dann weglassen) -->
Die ioBroker [Bilddatei](http://PLACEHOLDER:8081/files/0_userdata.0/calendar1.png) ist nicht ohne Anmeldung zugänglich. Aber über die REST API kann man öffentlich auf das [Bild](http://PLACEHOLDER:8093/v1/file/web/calendar1.png) zugreifen.

Anfangs wurde das Inkplate alle halbe Stunde aktualisiert um möglichst zeitnah Veränderungen abhorchen zu können, das hat aber viel Batterie verbraucht, sodas das e-Ink Display nur etwa 160h gehalten hat. Nach Verlängerung der Aktualisierungsfrequenz auf 2.5 Stunden hält das Inkplate deutlich länger (locker über einen Monat)

Das Skript aus ioBroker generiert alle 15 Minuten ein neues Bild und speichert es unter: *./files/0_userdata.0/calendar1.png*

#### Ressourcen 
<!-- → Verwendete Tutorials, Materialien, Quellenangaben, etc. (wenn nichts wichtiges, dann weglassen) -->
- Offizieller Belegungsplan der Uni: https://wiki.mi.ur.de/lab/belegung
- Inkplate Doku: https://github.com/SolderedElectronics/Inkplate-Arduino-library
- Einrichtung Arduino IDE: https://inkplate.readthedocs.io/en/latest/get-started.html 
- Belegungsplan-Bild: http://PLACEHOLDER:8081/files/0_userdata.0/calendar1.png
- Belegungsplan-Bild (REST): http://PLACEHOLDER:8093/v1/file/web/calendar1.png
