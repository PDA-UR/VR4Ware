/*
   Image frame example for e-radionica.com Inkplate 10
   For this example you will need only USB cable and Inkplate 10.
   Select "Inkplate 10(ESP32)" from Tools -> Board menu.
   Don't have "Inkplate 10(ESP32)" option? Follow our tutorial and add it:
   https://e-radionica.com/en/blog/add-inkplate-6-to-arduino-ide/

   Want to learn more about Inkplate? Visit www.inkplate.io
   Looking to get support? Write on our forums: http://forum.e-radionica.com/en/
   28 July 2020 by e-radionica.com
*/

#include "Inkplate.h"

Inkplate display(INKPLATE_3BIT);
double volt;
double high; 
double low; 
double sum;

String last_hash;
String mac_addr;

void setup_wifi()
{
    // Connect to Wi-Fi network with PLACEHOLDER and password
    //WiFi.config(local_ip, gateway, subnet, dns1, dns2);

    WiFi.begin("PLACEHOLDER", "PLACEHOLDER");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
}

double get_percentage()
{
  volt = display.readBattery();
  high = 4.2;
  low = 3.0;    
  sum = ((volt - low) / (high - low)) * 100;
  if (round(sum) >= 100)
  {
    sum = 100;
  }

  return sum;
}

void enter_deep_sleep()
{
    Serial.println("Going to sleep");
    delay(100);
    display.einkOff();
    esp_sleep_enable_timer_wakeup(20 * 1000 * 1000);
    esp_deep_sleep_start();
}

void update_display()
{
    HTTPClient http;
    String server_addr = "http://PLACEHOLDER:8093/v1/file/web/calendar1.png";
    display.drawImage(server_addr, display.PNG, 0, 0);

    display.setCursor(10, 770);
    char str[100];
    sprintf(str, "Battery: %.2f%%", get_percentage());
    display.print(str);
    display.display();

    Serial.println("Updated!");
    Serial.println(get_percentage());
    Serial.println(display.rtcGetMinute());
}

void setup()
{
    Serial.begin(115200);
    display.begin();
    display.einkOn();
  
    display.setRotation(3);
    display.setTextSize(2);
    display.setTextColor(BLACK);

    setup_wifi();
    mac_addr = WiFi.macAddress();

    Serial.println(display.rtcGetMinute());
    Serial.println("SETUP");

    update_display();
}

void loop()
{
  Serial.println("Starting Loop");
  update_display();
  delay(600000);
}