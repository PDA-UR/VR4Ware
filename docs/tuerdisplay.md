# Türdisplay

*Bilder/Diagramme, Schaltpläne, etc. (wo sinnvoll) einfügen*
TODO: BILD

<!-- → Verwendung: Was macht das? Wie kann man das benutzen?, … -->
- ein Belegungsplan, der an der Tür hängt
- zeigt Reservierungen für den aktuellen Tag und die nächste folgende Belegung
- nutzt die offiziellen [Belegungsplan](https://wiki.mi.ur.de/lab/belegung) der Uni und generiert daraus ein Bild, dass auf einem e-Ink display angezeigt wird

### Aufbau
<!-- → z.B.: Verkabelung, Infrastruktur, Ort,  -->
- e-radionica.com [Inkplate6](https://github.com/SolderedElectronics/Inkplate-Arduino-library) 6
- Skript in ioBroker, dass die Daten vom Belegungsplan zieht und daraus ein Bild generiert, dass in ioBroker gespeichert wird
- Inkplate holt in bestimmten Abständen das Bild von ioBroker und geht dann wieder in Deep Sleep

### Wartung und Troubleshooting
<!-- → Wie kommt man ran?, Was kann man einfach ändern?, Bugs, die uns begegnet sind und wie sie gelöst wurden, … -->
- Änderung des Skriptes erfolgt über Arduino IDE ([Einrichtung](https://inkplate.readthedocs.io/en/latest/get-started.html))
- Sicherstellen, dass das ausgewählte Board: **e-radionica.com Inkplate6** ist
- LINK SCRIPT

---

#### Anmerkungen
<!-- → Zusätzlicher Punkt für Notizen/Anmerkungen, etc. (wenn nichts wichtiges, dann weglassen) -->
- Die ioBroker [Bilddatei](http://PLACEHOLDER:8081/files/0_userdata.0/calendar1.png) ist nicht ohne Anmeldung zugänglich
- Über die REST API kann man öffentlich auf das [Bild](http://PLACEHOLDER:8093/v1/file/web/calendar1.png) zugreifen
- Anfangs wurde das Inkplate alle halbe Stunde aktualisiert um möglichst zeitnah Veränderungen abhorchen zu können, das hat aber viel Batterie verbraucht, sodas das e-Ink Display nur etwa 160h gehalten hat. Nach Verlängerung der Aktualisierungsfrequenz auf 2.5 Stunden hält das Inkplate deutlich länger (locker über 1 Woche)
- das Skript aus ioBroker generiert alle 15 Minuten ein neues Bild
- Bild in ioBroker: *./files/0_userdata.0/calendar1.png*

#### Ressourcen 
<!-- → Verwendete Tutorials, Materialien, Quellenangaben, etc. (wenn nichts wichtiges, dann weglassen) -->
- Offizieller Belegungsplan der Uni: https://wiki.mi.ur.de/lab/belegung
- Inkplate Doku: https://github.com/SolderedElectronics/Inkplate-Arduino-library
- Einrichtung Arduino IDE: https://inkplate.readthedocs.io/en/latest/get-started.html 
- Belegungsplan-Bild: http://PLACEHOLDER:8081/files/0_userdata.0/calendar1.png
- Belegungsplan-Bild (REST): http://PLACEHOLDER:8093/v1/file/web/calendar1.png
