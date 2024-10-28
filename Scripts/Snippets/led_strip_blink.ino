/// @file    Blink.ino
/// @brief   Blink the first LED of an LED strip
/// @example Blink.ino

#include <FastLED.h>

// How many leds in your strip?
#define NUM_LEDS 6

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 0
#define CLOCK_PIN 13

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
}

void loop() { 
  for (int j = 0; j < 6; j++) {
    // Turn the LED on, then pause
    leds[j] = CRGB::Red;
    FastLED.show();
    delay(500);
    // Now turn the LED off, then pause
    leds[j] = CRGB::Blue;
    FastLED.show();
    delay(500);
  }
}
