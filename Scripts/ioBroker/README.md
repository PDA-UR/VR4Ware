# ioBroker-Skripte

Hier findet sich eine Sammlung aller Skripte, die im ioBroker laufen.

Die Skripte wurde im ioBroker Skript-Tab komplett exportiert. Auf dem Server laufen sie als `.js`- oder *blockly*-Files, wurden aber als `json` exportiert, um in die richtige Sprache übersetzt zu werden

Stand: 28.10.2024

## Skripte
- `Anwesenheit`
  - setzt ein Objekt für eine kurze Zeit auf `true`, nachdem eine MQTT eingetroffen ist. Ansonsten nur momentaner Status.
- `calender-img`
  - erstellt ein Bild aus den Belegungsdaten der MI-Labore
- `Klingel`
  - siehe `Anweseneheit`, aber für die Türklingel
- `matrix-status`
  - Fragt die HDMI-Matrix über TCP-Requests alle 5min nach ihrem Status
- `matrix-switch`
  - Horcht auf Objekte, in denen die gewünschten Out-/Inputs gesetzt werden und schickt einen TCP-Befehl an die Matrix
- `mute_doorbell`
  - Horcht auf ein Objekt der Studienlampe um die Türklingel zu informieren/stumm zu schalten
- `parseAmpeln`
  - Liest die JSON-Objekte, die 4 Sensordaten bündeln, der CO2-Ampeln aus und splittet sie in separate Objekte (eins pro Sensordatenpunkt)
- `presentationMode`
  - Setzt die jeweiligen Objekte für den Vorlesungsmodus
- `projektor-status`
  - Schickt MQTT-Statusabfragen an die Projektoren
- `schwarzes-Brett`
  - Das Skript, dass das schwarze Brett erstellt und updatet
- `show-PC-specs`
  - Kleines Skript, das einen HTML-String für die PCs erstellt, damit dieses an-/abgewählt werden kann (geht evtl. auch ohne Skript, nur in vis2)