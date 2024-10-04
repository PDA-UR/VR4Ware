TODO
# VR4 Klingel

<!-- *Bilder/Diagramme, Schaltpläne, etc. (wo sinnvoll) einfügen* -->
TODO: BILD

<!-- → Verwendung: Was macht das? Wie kann man das benutzen?, … -->
- Klingel, um bei Anwesenheit an der Tür Personen in VR4 zu informieren, falls man keinen Schlüssel hat
- Personen im Raum werden über Klingelton bzw. vis-ioBroker informiert, dass jemand da ist
- Klingelton fällt aus, wenn Studie in Betrieb ist
- Nutzung eines Wemos [D1 mini](https://www.wemos.cc/en/latest/d1/d1_mini.html) für Logik und Kommunikation

### Aufbau
<!-- → z.B.: Verkabelung, Infrastruktur, Ort,  -->
- 2x WEMOS D1 Mini, 1x WEMOS Buzzer Shield, 1x WEMOS Button Shield, 1x LiPo Akku
- Topics: mqtt.0.doorbell
TOPICS

### Wartung und Troubleshooting
<!-- → Wie kommt man ran?, Was kann man einfach ändern?, Bugs, die uns begegnet sind und wie sie gelöst wurden, … -->
- [Setup-Guide](https://chewett.co.uk/blog/937/configuring-wemos-d1-mini-pro-esp8266-arduino-ide/) für Wemos D1 mini Pro
- 2 Skripts für Button und Buzzer. die über MQTT kommunizieren
- Unter ['scream.ino'](https://github.com/thiilo/VR4Ware/blob/main/scream.ino) kann man den Klingelton anpassen
- Unter ['buttonscript.ino'](https://github.com/thiilo/VR4Ware/blob/main/buttonscript.ino) wird der Button konfiguriert und ausgelesen
---

#### Anmerkungen
<!-- → Zusätzlicher Punkt für Notizen/Anmerkungen, etc. (wenn nichts wichtiges, dann weglassen) -->

#### Ressourcen 
<!-- → Verwendete Tutorials, Materialien, Quellenangaben, etc. (wenn nichts wichtiges, dann weglassen) -->

