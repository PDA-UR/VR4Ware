# 3D-Drucker

<!-- *Bilder/Diagramme, Schaltpläne, etc. (wo sinnvoll) einfügen* -->

<!-- → Verwendung: Was macht das? Wie kann man das benutzen?, … -->

Die existierende Infrastruktur des 3D-Druckers, der OctoPrint Server, wurde über einen Adapter in ioBroker integriert. Die Einbindung ist read-only. Es wird die existierende Webcam, Meta-Daten zum Drucker und Druck, sowie der Druckfortschritt angezeigt.

<!-- Bild von Dashboard -->

### Aufbau
<!-- → z.B.: Verkabelung, Infrastruktur, Ort,  -->

Der [OctoPrint](http://192.168.2.220/) Server läuft auf einem Raspberry Pi. Die Zugangsdaten stehen auf dem Raspi.
Existierende Daten werden in ioBroker über den OctoPrint-[Adapter](https://github.com/klein0r/ioBroker.octoprint) ausgelesen und im Dashboard über Objekte dargestellt.

### Wartung und Troubleshooting
<!-- → Wie kommt man ran?, Was kann man einfach ändern?, Bugs, die uns begegnet sind und wie sie gelöst wurden, … -->
Manchmal hängt sich der Server auf. Dann sollte einlogen und neustarten ausreichen..
Die Vollbildansicht der Webcam funktioniert nicht immer in ioBroker. Das ist möglicherweise Browser abhängig.
<!-- - Manchmal hängt sich der Server auf:
  - auf Server einloggen und neustarten sollte helfen -->

<!-- --- -->

<!-- #### Anmerkungen -->
<!-- → Zusätzlicher Punkt für Notizen/Anmerkungen, etc. (wenn nichts wichtiges, dann weglassen) -->

<!-- #### Ressourcen  -->
<!-- → Verwendete Tutorials, Materialien, Quellenangaben, etc. (wenn nichts wichtiges, dann weglassen) -->

