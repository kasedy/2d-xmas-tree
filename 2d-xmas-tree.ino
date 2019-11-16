/* 
 2D XMAS Tree Code
 charlieplexing christmas decoration
 5 Pins with 20 LED´s

 ATtiny25, 1MHZ Internal Clock, no BOD


 Hosted on Github: 
 
 https://github.com/designer2k2/2d-xmas-tree

 Made by:
 
 https://www.designer2k2.at

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

#define COMPILE_TIME_SIZEOF(t)      template<int s> struct SIZEOF_ ## t ## _IS; \
                                    struct foo { \
                                        int a,b; \
                                    }; \
                                    SIZEOF_ ## t ## _IS<sizeof(t)> SIZEOF_ ## t ## _IS;

#include <avr/pgmspace.h>
#include <util/delay.h>

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
  {0,1},
  {1,0},
  {1,2},
  {2,1},
  {2,3},
  {3,2},
  {3,4},
  {4,3},
  {0,2},
  {2,0},
  {1,3},
  {3,1},
  {2,4},
  {4,2},
  {0,3},
  {3,0},
  {1,4},
  {4,1},
  {0,4},
  {4,0},
};

//placeholder for all current leds:
uint8_t ledstate[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 

void setup() {
  //Need to define the Pins as input (defaults)
  tristate();

  // Disable ADC (power saving):
  ADCSRA = 0;

  // Shut down Timer/Counter1, Timer/Counter0, ISI, ADC
  PRR = 1;
}

void loop(){
  // put your main code here, to run repeatedly:

  // every 100ms change a random led to a random state
  unsigned long instroStartTime = millis();
  while (millis() - instroStartTime < INTROTIME) {
    long randomValue = random();
    int ledtoset = randomValue % NUM_LEDS;
    bool ledbool = randomValue & 0b10000000;
    ledstate[ledtoset] = ledbool;
    showleds(10);
    _delay_us(1000);
  }
  
  //loop trough the states in state_table  

  
  for (int iState = 0; iState < sizeof(animation) * 8 / NUM_LEDS; ++iState) {
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

void showleds(unsigned int showTimeMs){
  // this cycles trough all the leds in the array and show them.
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

void showled(uint8_t led){
  //Lookup the pin for high and low:
  const LedControlPin &pinouts = ledControlPins[led];

  //set the pins to output:
  DDRB |= (1 << pinouts.pvcc) | (1 << pinouts.pgnd);

  // write the status:
  PORTB |= (1 << pinouts.pvcc); // set HIGH

  //wait a short time to let the LED shine
  _delay_us(LED_ON_DELAY);
  
  //ensure its all turned off properly for the next state
  tristate();
}

void tristate(){
  //This tristates all pins
  DDRB &= ~(0b00011111); // Input state for pins 0-4 pins
  PORTB &= ~(0b00011111); //ensure pullups are off.
}
