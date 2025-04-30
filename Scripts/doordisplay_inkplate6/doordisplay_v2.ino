/*
  Script to display an image of a timetable.
  Uses e-radionica.com Inkplate6
  Connects to the internet and gets an image from a server (here: ioBroker), displays it, 
  and then goes into deep sleep for a while.
  Displays the current battery (not really representational, yet)
  Gets a .txt file containing a number from ioBroker and changes it's update-rate accordingly.
  Displays an error msg when there was an error fetching the image.
*/

#include <Inkplate.h>
#include <HTTPClient.h> 

Inkplate display(INKPLATE_3BIT);
double volt;
double high; 
double low; 
double sum;

String last_hash;
String mac_addr;
String htmlText;

unsigned long long defaultUpdateHours = 2ULL;
unsigned long long updateHours = defaultUpdateHours;

void setup_wifi()
{
    // Connect to Wi-Fi network
    WiFi.begin("PLACEHOLER", "PLACEHOLDER");
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
  Serial.print("Entering deep sleep for h = ");
  Serial.println(updateHours);
  display.einkOff();
  unsigned long long timer = updateHours * 60 * 60 * 1000 * 1000;
  esp_sleep_enable_timer_wakeup(timer); // works
  esp_deep_sleep_start();
}

// Get interval from a file in ioBroker
void get_update_interval()
{
  HTTPClient http; 
  // Change the update file here ↓ 
  http.begin("http://PLACEHOLDER/path/to/calendarUpdate.txt");
  if (http.GET() == 200)
  { // If connection was successful, try to read content of the Web page and display it on screen
    htmlText = http.getString();
    updateHours = strtoull(htmlText.c_str(), NULL, 0);
    Serial.print("HTML_TEXT: ");
    Serial.println(htmlText);
  } else {
    Serial.println("Fallback to default update hours = 2");
    updateHours = defaultUpdateHours;
  }

}

void draw_error() 
{
  Serial.println("ERROR fetching image...");

  int x = 30;
  display.setCursor(x, 50);
  display.setTextSize(10); 
  display.print("Error :(");

  display.setTextSize(2);
  display.setCursor(x, 150);
  display.print("Failed to get data from the server...");

  display.setTextSize(2);
  display.setCursor(x, 200);
  char hourStr[100];
  sprintf(hourStr, "Automatic retry every %llu h.", updateHours);
  display.print(hourStr);

  display.setCursor(x, 220);
  display.print("Or try a manual reboot.");
}

void update_display()
{
    Serial.println("Fetching image...");
    display.clearDisplay(); // Clear frame buffer of display

    HTTPClient http;
    // Change the update image-file here ↓
    String server_addr = "http://PLACEHOLDER/path/to/calendarTB.png";
    
    int status = display.drawImage(server_addr, display.PNG, 0, 0);

    // check if image exists otherwise draw error msg
    if (status == 0) {
      draw_error();
    } 
    
    display.setCursor(10, 750);
    char str1[100];
    sprintf(str1, "update in: %s h", htmlText);
    display.print(str1);

    display.setCursor(10, 770);
    char str[100];
    sprintf(str, "Battery: %.2f%%", get_percentage());
    display.print(str);
    display.display();

    Serial.print("Updated! ");
    Serial.println(get_percentage());
    
}

void setup()
{
    Serial.begin(115200);
    Serial.println(esp_sleep_get_wakeup_cause());
    Serial.println("SETUP");
    
    display.begin();    
    display.einkOn();
  
    display.setRotation(3);
    display.setTextSize(2);
    display.setTextColor(BLACK);

    setup_wifi();

    get_update_interval();
    update_display();
  
    enter_deep_sleep();
}

void loop()
{
  // nixe
}
