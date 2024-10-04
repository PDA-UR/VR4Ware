# Bewegungssensor

<!-- *Bilder/Diagramme, Schaltpläne, etc. (wo sinnvoll) einfügen* -->
TODO: BILD

<!-- → Verwendung: Was macht das? Wie kann man das benutzen?, … -->
- Sensor um Bewegungen im Labor in ioBroker widerzuspiegeln
- Keine Aufnahme personenbezogener Daten
- Status wird über MQTT an ioBroker weitergeleitet

### Aufbau
<!-- → z.B.: Verkabelung, Infrastruktur, Ort,  -->
- HC-SR501 infrarot Sensor
- Topics: *mqtt.0.motionSensorLabor, mqtt.0.motionSensorStudio, mqtt.0.motionSensorWerkstatt*

### Wartung und Troubleshooting
<!-- → Wie kommt man ran?, Was kann man einfach ändern?, Bugs, die uns begegnet sind und wie sie gelöst wurden, … -->
- Skript in ioBroker lässt die übermittelten MQTTs für 90 Sekunden gelten
- History wurde in ioBroker für die drei Topics angeschaltet, um später eCharts damit erstellen zu können

---

#### Anmerkungen
<!-- → Zusätzlicher Punkt für Notizen/Anmerkungen, etc. (wenn nichts wichtiges, dann weglassen) -->
- Nutzung dieses [Tutorials](https://www.makerguides.com/hc-sr501-arduino-tutorial/) als Startpunkt

#### Ressourcen 
<!-- → Verwendete Tutorials, Materialien, Quellenangaben, etc. (wenn nichts wichtiges, dann weglassen) -->
- Tutorial: https://www.makerguides.com/hc-sr501-arduino-tutorial/
- Gehäuse: https://www.thingiverse.com/thing:2489968
