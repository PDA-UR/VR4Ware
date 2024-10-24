# Bewegungssensor

Drei Infrarot-Bewegungssensoren würden in den drei Räumen installiert, um datenschutzkonform Informationen zur aktuellen Raumbelegung in ioBroker zu sammeln und darzustellen.

Bewegungen werden im Dashboard über eine grüne Einfärbung des betroffenen Raumes dargestellt.

### Aufbau
Verwendet wurde hierfür ein HC-SR501 Infrarot Sensor, der über MQTT Statusnachrichten an ioBroker sendet.
Die jeweiligen Topics sind: `mqtt.0.motionSensor{Raum}`.

Betrieben wird der Sensor über einen D1 Mini, der mit ihm im [Sensorkit](labor/vr4/roomware/sensorkit) verbaut.
Der Bewegungssensor steckt in der Frontplatte und ist fest mit dem D1 Mini verlötet.


### Wartung und Troubleshooting
Da die übermittelten Statusnachrichten nur einen *Tick* darstellen, werden sie in MQTT mithilfe eines ioBroker-Skripts für 90 Sekunden auf `true/false` gestellt. Ansonsten wären die Daten nicht richtig auslesbar.

Für ein besseres logging und spätere Auswertung bzw. Darstellung mit eCharts wurde die ioBroker-History für alle drei Topics/Objekte angeschaltet.
<!-- - Skript in ioBroker lässt die übermittelten MQTTs für 90 Sekunden gelten -->
<!-- - History wurde in ioBroker für die drei Topics angeschaltet, um später eCharts damit erstellen zu können -->

#### Anmerkungen
<!-- → Zusätzlicher Punkt für Notizen/Anmerkungen, etc. (wenn nichts wichtiges, dann weglassen) -->
Bei der Implementierung wurde dieses [Tutorials](https://www.makerguides.com/hc-sr501-arduino-tutorial/) als Startpunkt genutzt.

Bei dem Bewegungssensor im Studio werden oft False Positives erkannt. Dies hängt vermutlich mit dem OptiTrack-System oder den SteamVR-Trackern zusammen, die auch über Infrarot laufen.
Es kommt also zu "Bewegungen" im Studio in der Nacht.

#### Ressourcen 
<!-- → Verwendete Tutorials, Materialien, Quellenangaben, etc. (wenn nichts wichtiges, dann weglassen) -->
- Tutorial: https://www.makerguides.com/hc-sr501-arduino-tutorial/
- Altes Gehäuse: https://www.thingiverse.com/thing:2489968
