# Drucker

<!-- *Bilder/Diagramme, Schaltpläne, etc. (wo sinnvoll) einfügen* -->

<!-- → Verwendung: Was macht das? Wie kann man das benutzen?, … -->

<!-- BILD aus Dashboard -->

Mithilfe von SNMP werden verschiedene Informationen (z.B.: Tintenstand) des Druckers ausgelesen und in ioBroker angezeigt.

### Aufbau
<!-- → z.B.: Verkabelung, Infrastruktur, Ort,  -->
Für den Drucker (Lexmark MS510dn) werden mithilfe des SNMP-[Adapters](https://github.com/iobroker-community-adapters/ioBroker.snmp) Informationen ausgelesen. Hierzu werden OIDs genutzt, die unter `Instances > snmp > Settings > OID` hinzugefügt werden können. Eine Liste mit mögliche OIDs ist z.B.: [hier](https://gist.github.com/agent4788/438639df3c5f2801038123de3f916d1d) zu finden. Nicht alle OID geben etwas ,für diesen spezifischen Drucker, zurück.
<!-- Gewählt wurden: Tintenstand (max, aktuell), gedruckte Seiten, Seriennummer -->


<!-- - in ioBroker können über den SNMP-Adapter Informationen ausgelesen werden
  - Instances: snmp -> Settings -> OID
  - Liste an [OID](https://gist.github.com/agent4788/438639df3c5f2801038123de3f916d1d)s (nicht alle geben etwas zurück) -->


<!-- ### Wartung und Troubleshooting -->
<!-- → Wie kommt man ran?, Was kann man einfach ändern?, Bugs, die uns begegnet sind und wie sie gelöst wurden, … -->

<!-- --- -->

<!-- #### Anmerkungen -->
<!-- → Zusätzlicher Punkt für Notizen/Anmerkungen, etc. (wenn nichts wichtiges, dann weglassen) -->

<!-- #### Ressourcen  -->
<!-- → Verwendete Tutorials, Materialien, Quellenangaben, etc. (wenn nichts wichtiges, dann weglassen) -->

