/***************************************************************************
  This is a library for the ArduinoMorse
      Demonstrate the basic usage to create Morse Beacon.
      See schematics and description on the GitHib Page 

  Designed specifically to work with the LED and Piezo Buzzer
  ----> https://shop.mchobby.be/senseur-divers/57-piezo-3232100000575.html
  ----> https://shop.mchobby.be/senseur-divers/1334-piezo-buzzer-large-3232100013346.html
  ----> https://shop.mchobby.be/minikits/66-assortiment-leds-5mm-mini-kit-3232100000667.html
  ----> https://shop.mchobby.be/breakout/507-module-deux-relais-3232100005075-pololu.html
  ----> https://shop.mchobby.be/bouton/548--switch-pour-breadboard-spdt-3232100005488.html
  ----> https://shop.mchobby.be/autres/487-potentiometre-rond-court-10k-3232100004870.html

  The driver use 2 several pins.
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
#define commPin       12 // Communication Pin (activate when start a cycle)
#define configPin     2  // Configuration (INPUT) pin
#define speedPin      A0 // Potentiometer (INPUT) Analog Pin
#define tonePin       A1 // Potentiometer (INPUT) Analog Pin

MCHobby_ArduinoMorse morse = MCHobby_ArduinoMorse(piezoPin, morseOutPin);


void setup() {
  morse.begin();
  pinMode( commPin, OUTPUT );
  pinMode( configPin, INPUT_PULLUP );

  //morse.morse_speed( 200 ); // decrease default speed which is 100 
  //morse.morse_tone( 1500 ); // Higher tone than default 1200
}

char line1ToMorse[] = "Cq cq cq de on1aa\n"; // add a Line feed at the end
char line2ToMorse[] = "jo20gm plse report to pl.delannoy@ yahoo.com or qsl via bureau 73’s";

void beaconMsg(){
  /* Normal Beacon Message */

  digitalWrite( commPin, HIGH ); 
  delay( 1000 );
  morse.morse_char( BEGIN_OF_TRANSMISSION );
  morse.morse_string( line1ToMorse );
  morse.morse_string( line2ToMorse );
  morse.morse_char( END_OF_TRANSMISSION );
  digitalWrite( commPin, LOW );
  delay( 30000 ); // attendre 30 sec
}

void beaconCfgMode(){
  /* Configure the beacon parameter */

  int valTone  = analogRead( tonePin );
  morse.morse_speed( readSpeed() );
  morse.morse_tone( readTone() );
  morse.morse_char( 'A' );
}

int readSpeed(){
  int value = analogRead( speedPin );
  return map( value, 0, 1024, 10, 300 );
}

int readTone(){
  int value = analogRead( tonePin );
  return map( value, 0, 1024, 800, 1600 );
}

void loop() {
  // put your main code here, to run repeatedly:

  if( digitalRead( configPin )==LOW ){
    morse.morse_speed( readSpeed() );
    morse.morse_tone( readTone() );
    beaconMsg();
  } 
  else {
    beaconCfgMode(); 
    delay( 100 );
  }
}
