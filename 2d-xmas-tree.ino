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

constexpr int sqr1(int arg)
{ return arg * arg; }

static const bool state_table[14][21] PROGMEM = {
                 {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                 {0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                 {0,0,0,0,0,0,1,0,1,0,1,1,0,0,0,0,0,0,0,0},
                 {0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,1,0,0,0},
                 {0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,1,1},
                 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
                 {0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0},
                 {1,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
                 {0,0,0,0,0,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0},
                 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0},
                 {0,1,0,0,1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0},
                 {0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0},
                 {0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1},
                 {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0}
};
#define NumRows 14  //default is 14 (and about the space limit)

//the delay in ms between the patterns:
#define PATTERN_DELAY 500

//intro time (how long the random change happens) in ms:
#define INTROTIME 10000

//time a led is on in us:
//increase this to have the leds "flicker"
#define ondelay 1000


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
bool ledstate[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 

void setup() {
  //Need to define the Pins as input (defaults)
  tristate();

  // disable ADC (power saving):
  ADCSRA = 0;  
}

void loop(){
  // put your main code here, to run repeatedly:

  // every 100ms change a random led to a random state
  unsigned long instroStartTime = millis();
  while (millis() - instroStartTime < INTROTIME) {
    int ledtoset = random(20);
    bool ledbool = random(2);
    ledstate[ledtoset] = ledbool;
    showleds(10); 
  }
  
  //loop trough the states in state_table  

  for (int iState = 0; iState < NumRows; ++iState) {
    for (int i=0; i<20; i++) {
      int state = pgm_read_word(&state_table[iState][i]);
      //PROGMEM with bool gives sometimes strange results, this fixes it:
      if (state==256){
        state = 0;
      }
     
      ledstate[i] = state;
    }
    showleds(PATTERN_DELAY); 
  }
}

//-------------------------------------------------------------------------------------

void showleds(int showTimeMs){
  // this cycles trough all the leds in the array and show them.
  unsigned long startTime = millis();
  while(millis() - startTime <= showTimeMs) {
    for (int i=0; i<20; i++) {
     bool state = ledstate[i];
     showled(i,state);
    }
  }  
}

void showled(uint8_t led, bool state){
  //this outputs the led state

  if(state==true){
    //Lookup the pin for high and low:
    const LedControlPin &pinouts = ledControlPins[led];

    //set the pins to output:
    DDRB |= (1 << pinouts.pvcc) | (1 << pinouts.pgnd);

    // write the status:
    PORTB |= (1 << pinouts.pvcc); // set HIGH

    //wait a short time to let the LED shine
    delayMicroseconds(ondelay);
    
    //ensure its all turned off properly for the next state
    tristate();
  }
 
}

void tristate(){
  //This tristates all pins
  DDRB &= ~(0b00011111); // Input state for pins 0-4 pins
  PORTB &= ~(0b00011111); //ensure pullups are off.
}
