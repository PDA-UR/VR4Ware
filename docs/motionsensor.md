# Bewegungssensor

<!-- *Bilder/Diagramme, Schaltpläne, etc. (wo sinnvoll) einfügen* -->
TODO: BILD

<!-- → Verwendung: Was macht das? Wie kann man das benutzen?, … -->
Drei Bewegungssensoren würden in den drei Räumen installiert um anonymisiert Informationen zur aktuellen Raumbelegung in ioBroker zu sammeln und darzustellen.

Bewegungen werden im Dashboard über eine grüne Einfärbung des betroffenen Raumes dargestellt.
<!-- 
- Sensor um Bewegungen im Labor in ioBroker widerzuspiegeln
- Keine Aufnahme personenbezogener Daten
- Status wird über MQTT an ioBroker weitergeleitet -->

### Aufbau
<!-- → z.B.: Verkabelung, Infrastruktur, Ort,  -->
Verwendet wurde hierfür ein HC-SR501 Infrarot Sensor, der über MQTT Statusnachrichten an ioBroker sendet.
Die jeweiligen Topics sind: *mqtt.0.motionSensorLabor, mqtt.0.motionSensorStudio, mqtt.0.motionSensorWerkstatt*
Betrieben wird der Sensor über einen d1 mini und ist im Sensorkit (LINK) verbaut. 


### Wartung und Troubleshooting
<!-- → Wie kommt man ran?, Was kann man einfach ändern?, Bugs, die uns begegnet sind und wie sie gelöst wurden, … -->
Da die übermittelten Statusnachrichten nur einen *Tick* darstellen werden die MQTT mithilfe eines ioBroker-Skripts für 90 Sekunden auf `true/false` gestellt. Ansonsten werden die Daten nicht richtig auslesbar.

Für ein besseres logging und spätere Auswertung, bzw. Darstellung mit eCharts wurde die ioBroker-History für alle drei Topics/Objekte angeschalten.
<!-- - Skript in ioBroker lässt die übermittelten MQTTs für 90 Sekunden gelten -->
<!-- - History wurde in ioBroker für die drei Topics angeschaltet, um später eCharts damit erstellen zu können -->

---

#### Anmerkungen
<!-- → Zusätzlicher Punkt für Notizen/Anmerkungen, etc. (wenn nichts wichtiges, dann weglassen) -->
Bei der Implementierung wurde dieses [Tutorials](https://www.makerguides.com/hc-sr501-arduino-tutorial/) als Startpunkt genutzt.

Bei dem Bewegungssensor im Studio werden oft False Positives (?) erkannt. Dies hängt vermutlich mit dem OptiTrack-System zusammen, da "Bewegungen" auch Nachts und nur in diesem Raum angezeigt werden.

#### Ressourcen 
<!-- → Verwendete Tutorials, Materialien, Quellenangaben, etc. (wenn nichts wichtiges, dann weglassen) -->
- Tutorial: https://www.makerguides.com/hc-sr501-arduino-tutorial/
- Altes Gehäuse: https://www.thingiverse.com/thing:2489968
