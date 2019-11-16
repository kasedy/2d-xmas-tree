/* 
  2D XMAS Tree Code
  charlieplexing christmas decoration
  5 Pins with 20 LED´s

  Designed by:

    https://www.designer2k2.at

  Enhanced by:

    https://github.com/kasedy

      1. Significantly reduced memory footprint
      2. Improved power consumption optimizations
      3. Converted to platform IO project

  MIT License

  Copyright (c) [2018] [Stephan Martin]

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

#include <avr/pgmspace.h>
#include <util/delay.h>

#include "helpers.h"

//Enter the light pattern here:
//use the online generator: https://grweirioreh.reihrg.eu
//dont forget to update the number of rows if changed

static const uint8_t animation[] PROGMEM = {
  0x03, 0x00, 0xC0, 0x03, 0x00, 0x40, 0x0D, 0x00, 0x28, 0x11, 0x00, 0xE0, 0x0E, 0x00, 0x40, 0x04,
  0x01, 0x93, 0x80, 0x00, 0xE0, 0x20, 0x00, 0x00, 0x0C, 0x12, 0x14, 0x40, 0x10, 0x30, 0x00, 0x02,
  0x08, 0x00, 0x40
};

//the delay in ms between the patterns:
#define PATTERN_DELAY 500

//intro time (how long the random change happens) in ms:
#define INTROTIME 10000

//time a led is on in us:
//increase this to have the leds "flicker"
#define LED_ON_DELAY 1000
#define INTRO_BLYNK_DELAY_US 1000

#define NUM_LEDS 20


//-------------------------------------------------------------------------------------
//below is the code to make this all happen, modify to make even fancier effects as you wish!

//This holds the pin configuration to make the 20 led charlieplexing,
//only 1 in PROGMEM, as the FLASH is full, but both are too large for RAM.

struct LedControlPin {
  uint8_t pvcc : 4;
  uint8_t pgnd : 4;
};

const LedControlPin ledControlPins[] = {
  {0, 1},
  {1, 0},
  {1, 2},
  {2, 1},
  {2, 3},
  {3, 2},
  {3, 4},
  {4, 3},
  {0, 2},
  {2, 0},
  {1, 3},
  {3, 1},
  {2, 4},
  {4, 2},
  {0, 3},
  {3, 0},
  {1, 4},
  {4, 1},
  {0, 4},
  {4, 0},
};

// Placeholder for all current leds.
uint8_t ledstate[NUM_LEDS] = {};

void setup() {
  // Disable ADC (power saving):
  ADCSRA = 0;

  // Shut down Timer/Counter1, Timer/Counter0, ISI, ADC
  PRR = 1;
}

void loop(){
  // Every 10ms change a random led to a random state.
  // Delay makes LEDs flikering.
  unsigned long instroStartTime = millis();
  while (millis() - instroStartTime < INTROTIME) {
    long randomValue = random();
    int ledtoset = randomValue % NUM_LEDS;
    bool ledbool = randomValue & 0b10000000;
    ledstate[ledtoset] = ledbool;
    showleds(10);
    extinguish();
    _delay_us(INTRO_BLYNK_DELAY_US);
  }
  
  // Show preprogrammed animation.
  for (unsigned int iState = 0; iState < sizeof(animation) * 8 / NUM_LEDS; ++iState) {
    uint16_t startPosition = iState * NUM_LEDS;
    uint8_t byteIndex = startPosition / 8;
    uint8_t bitIndex = startPosition % 8;
    for (int i = 0; i < NUM_LEDS; ++i) {
      if (bitIndex == 8) {
        bitIndex = 0;
        byteIndex += 1;
      }
      uint8_t state = pgm_read_byte(animation + byteIndex);
      ledstate[i] = (state & (1 << bitIndex));
      bitIndex += 1;
    }
    showleds(PATTERN_DELAY); 
  }
}

//-------------------------------------------------------------------------------------

void showleds(unsigned int showTimeMs) {
  // This cycles trough all the LEDs in the array and show them one-by-one. That 
  // happens so quickly that human eye sees like LEDs shine all together.
  unsigned long startTime = millis();
  while(millis() - startTime <= showTimeMs) {
    for (int i = 0; i < NUM_LEDS; i++) {
      bool ledOn = ledstate[i];
      if (ledOn) {
        showled(i);
      }
    }
  }  
}

void showled(uint8_t led) {
  // Lookup the pin for high and low:
  const LedControlPin &pinouts = ledControlPins[led];

  // Set pins that contold LED to output. The other pins will be set to input.
  DDRB = (1 << pinouts.pvcc) | (1 << pinouts.pgnd);

  // Write the status.
  PORTB = (1 << pinouts.pvcc); // set HIGH

  // Wait a short time to let the LED shine.
  _delay_us(LED_ON_DELAY);

  // Dont extinguish. Let LED shine while microcontroller decides which LED to 
  // light next.
}

void extinguish() {
  // This tristates all pins
  DDRB &= ~(0b00011111); // Input state for pins 0-4 pins
  PORTB &= ~(0b00011111); //ensure pullups are off.
}
