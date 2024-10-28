# Projector Control

Das `projector-control-3.py` Script ist ein funktionierendes Skript für den Raspi-3. Für deployment auf anderen Raspis sollten die jeweiligen Topics geändert werden.

Das Skript `projector-control-3-log.py` ist eine überarbeitete Version (wegen Troubleshooting) und implementiert einen Logger, sowie eine Änderung zu der (letzten) Zeile: `client.loop_forever(retry_first_connection=True)`