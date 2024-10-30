# Klingelsystem und Sensorkit

`doorbell_with_led.ino` ist das Skript, das auf dem Klingelteil läuft. Es schickt beim Knopfdruck ein Klingelsignal an ioBroker und aktiviert eine Feedback-LED.

`doortbell-pir.ino` ist das Skript, das auf dem Sensorkit läuft und pro Raum die Klingelwiedergabe (auditiv und visuell) sowie Bewegungssensoren handelt. Hier ist gerade das `motionSensorLabor`-Topic addressiert, für die anderen beiden Räume müsste das jeweils geändert werden.
