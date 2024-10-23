# Arduino IDE

Dies ist eine Quellensammlung zum Aufsetzen der Arduino IDE für verschiedene Boards.
Zusätzlich wurden einige Notizen gesammelt, um den Einstieg zu erleichtern.

## Aufsetzten
- Wemos d1 mini (esp8266):
  - Offizielles [Repo](https://github.com/esp8266/Arduino?tab=readme-ov-file) mit Anleitung
  - [Setup-Guide](https://chewett.co.uk/blog/937/configuring-wemos-d1-mini-pro-esp8266-arduino-ide/)
  - [Pinouts](https://chewett.co.uk/blog/1066/pin-numbering-for-wemos-d1-mini-esp8266/) 
<!-- - CO2-Ampel alternative Firmware:
  - TODO
- Inkplate
  - TODO -->

## Notizen
- `const int PIN_NUM = x;` um Pinnummern zu definieren
  - siehe z.B.: [Pinout des D1 mini](https://chewett.co.uk/blog/1066/pin-numbering-for-wemos-d1-mini-esp8266/)
  - z.B.: `const int BOARD_LED = 2;`
- `#define PIN 2` als alternative
  - funktioniert nur, wenn darüber eine Library importiert 
  - siehe z.B.: [1](https://forum.arduino.cc/t/when-to-use-const-int-int-or-define/668071/5) oder [2](https://forum.arduino.cc/t/define-datapin-4-throws-error/332343/5)
- Für die meisten Bibliotheken existieren vorgefertigte Beispiel-Skripte, die man als Anhaltspunkt benutzen kann:
  - offizielle [Examples](https://docs.arduino.cc/built-in-examples/)
  - in Arduino IDE: `File > Examples > ...`
