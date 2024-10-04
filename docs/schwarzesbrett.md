# Digitales Schwarzes Brett

<!-- *Bilder/Diagramme, Schaltpläne, etc. (wo sinnvoll) einfügen* -->
TODO: BILD

<!-- → Verwendung: Was macht das? Wie kann man das benutzen?, … -->
- Ein simples schwarzes Brett mit dem Informationen anonym miteinander geteilt werden können
- Admin kann Nachrichten abhaken und löschen

### Aufbau
<!-- → z.B.: Verkabelung, Infrastruktur, Ort,  -->
- Skript in ioBroker erstelle Tabelle mit Checkboxen
  - Abhaken: Zeile wird abgehackt und markiert
  - Löschen: Zeile wird aus der Tabelle gelöscht und archiviert
- Skript läuft, wenn eine neue Nachricht in ein Objekt geschrieben wird
  - Aus Nachricht wird ein JSON-Objekt upgedated
  - Aus dem JSON-Objekt werden HTML-Tabellen erstellt
    - Eine öffentliche und eine Admin-Tabelle (mit Checkboxen)

### Wartung und Troubleshooting
<!-- → Wie kommt man ran?, Was kann man einfach ändern?, Bugs, die uns begegnet sind und wie sie gelöst wurden, … -->
- Skript in ioBroker. Funktionalität ist etwas umständlich zu ändern.
- Gelöschte Nachrichten werden in ioBroker in einer JSON-Datei archiviert (*./files/0_userdata.0/schwarzes-brett/archive.json*)

#### Objekte in ioBroker
| OID | Nutzung  |
| --------------| ---------------------------------------------- |
| *0_userdata.0.msg.HTMLTable*       | generierte Admin Tabelle für vis               |
| *0_userdata.0.msg.HTMLTablePUBLIC* | generierte Public Tabelle für vis (öffentlich) |
| *0_userdata.0.msg.JSON *           | JSON-Objekt                                    |
| *0_userdata.0.msg.JSON-log*        | Zwischenspeichern der gelöschten Nachricht     |
| *0_userdata.0.msg.msgInput*        | Geschriebene Nachricht                         |
| *0_userdata.0.msg.msgTopics *      | Zwischenspeichern des Topics der Nachricht      |

---

#### Anmerkungen
<!-- → Zusätzlicher Punkt für Notizen/Anmerkungen, etc. (wenn nichts wichtiges, dann weglassen) -->
- Styling der Tabelle ging nur inline, weil es in einem Objekt gespeichert wird, das nur Strings speichern kann. Das Objekt wird danach in einem HTML-Widget in vis dargestellt
- Click-Events der Checkboxen gingen nur inline, weil ansonsten keine Events gefeuert werden (da Skript nicht direkt in vis läuft) (siehe: TODO->link zu ioBroker.md#Zugriff von vis auf JS Funktionen)
- In vis ist der Zugang zu den üblichen Skript-Funktionen etwas umständlich
- Man kann die gleiche Nachricht nicht zweimal schicken, da ioBroker auf die Änderung des Objektes lauscht

#### Ressourcen 
<!-- → Verwendete Tutorials, Materialien, Quellenangaben, etc. (wenn nichts wichtiges, dann weglassen) -->

