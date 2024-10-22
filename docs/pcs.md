# PC Specs

<!-- *Bilder/Diagramme, Schaltpläne, etc. (wo sinnvoll) einfügen* -->

<!-- → Verwendung: Was macht das? Wie kann man das benutzen?, … -->
Im Dashboard werden verschiedene statische Daten der existierenden Computer angezeigt, um auch von außerhalb einen einfachen Zugang du wichtigen Specs zu erlauben.
<!-- - Einfacher Zugang zu PC Specs (auch von außerhalb)
- Abgleichen von Systemdaten, etc. -->

### Aufbau
<!-- → z.B.: Verkabelung, Infrastruktur, Ort,  -->
Die unveränderlichen Metadaten wurden händisch gesammelt und sind in statischen JSON-Dateien in ioBroker hinterlegt. Die Daten sind im Dashboard über Buttons zugänglich.
<!-- - Statische JSON-Dateien in ioBroker, mit den statischen Daten
- Visualisierung in ioBroker
- Script auf den PCs, dass per SNMP Infos an ioBroker schickt (TODO) -->

### Wartung und Troubleshooting
<!-- → Wie kommt man ran?, Was kann man einfach ändern?, Bugs, die uns begegnet sind und wie sie gelöst wurden, … -->
Dateipfade in ioBroker:
- *./files/0_userdata.0/pc-specs/PC1.json*
- *./files/0_userdata.0/pc-specs/PC2.json*
- *./files/0_userdata.0/pc-specs/PC3.json*
- *./files/0_userdata.0/pc-specs/PC4.json*

---

#### Anmerkungen
<!-- → Zusätzlicher Punkt für Notizen/Anmerkungen, etc. (wenn nichts wichtiges, dann weglassen) -->
Geplant war zusätzlich mithilfe von SNMP veränderliche Infos, wie etwa existierende Unity-Installationen oder verfügbaren Speicherplatz, an ioBroker zu schicken. Dies wurde aus Zeitgründen, Netzwerkprobleme und niedriger Priorität nicht umgesetzt.

<!-- #### Ressourcen  -->
<!-- → Verwendete Tutorials, Materialien, Quellenangaben, etc. (wenn nichts wichtiges, dann weglassen) -->

