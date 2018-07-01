/***************************************************************************
  This is a library for the ArduinoMorse
      Demonstrate the basic usage for sending a character, a string, 
        an array with special chars. 

  Designed specifically to work with the LED and Piezo Buzzer
  ----> https://shop.mchobby.be/senseur-divers/57-piezo-3232100000575.html
  ----> https://shop.mchobby.be/senseur-divers/1334-piezo-buzzer-large-3232100013346.html
  ----> https://shop.mchobby.be/minikits/66-assortiment-leds-5mm-mini-kit-3232100000667.html

  The driver use 2 required pins (one PWM for the buzze and one for digital I/O).
  Schematic on github 
  ----> https://github.com/mchobby/MCHobby_ArduinoMorse_library

  MC Hobby SPRL invests time and resources providing this open source code,
  please support MC Hobby and open-source by purchasing products
  from MC Hobby! shop.mchobby.be

  Written by Meurisse D. for MC Hobby SPRL.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/
#include <MCHobby_ArduinoMorse.h>

#define morseOutPin  11  // Digital output placed HIGH when emitting dot or Dash.
                         //    wire a LED + R 1K on that output 
#define piezoPin      8  // output audio on pin 8
                         //    wire a Piezo + R 100 on that output

MCHobby_ArduinoMorse morse = MCHobby_ArduinoMorse(piezoPin, morseOutPin);
// -OR- 
// the default contructor using piezo_pin=8 & led_pin=11
// MCHobby_ArduinoMorse morse = MCHobby_ArduinoMorse();

void setup() {
  // put your setup code here, to run once:
  morse.begin();
  //morse.morse_speed( 200 ); // decrease default speed which is 100 
  //morse.morse_tone( 1500 ); // Higher tone than default 1200
}

// String accept escape sequence like \r \n \\ 
char stringToMorse[] = "Hello from MCHobby Belgium!";
int end_of_msg[] = { END_OF_TRANSMISSION, INVITE_ANY_STATION };

void loop() {
  // put your main code here, to run repeatedly:
  morse.morse_char( BEGIN_OF_TRANSMISSION );
  morse.morse_string( stringToMorse );
  morse.morse_chars( end_of_msg ); 

  // At the end of the string long pause before looping and starting again
  delay( 8000 );   

}
