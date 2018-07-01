
/***************************************************************************
  This is a library for the ArduinoMorse 

  Designed specifically to work with the LED and Piezo Buzzer
  ----> https://shop.mchobby.be/senseur-divers/57-piezo-3232100000575.html
  ----> https://shop.mchobby.be/senseur-divers/1334-piezo-buzzer-large-3232100013346.html
  ----> https://shop.mchobby.be/minikits/66-assortiment-leds-5mm-mini-kit-3232100000667.html

  The driver use 2 required pins (one PWM for the buzze and one for digital I/O).

  MC Hobby SPRL invests time and resources providing this open source code,
  please support MC Hobby and open-source by purchasing products
  from MC Hobby! shop.mchobby. be

  Written by Meurisse D. for MC Hobby SPRL.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/
#include "MCHobby_ArduinoMorse.h"

MCHobby_ArduinoMorse::MCHobby_ArduinoMorse(void) : MCHobby_ArduinoMorse( piezo_pin_default, morse_led_pin_default )
{ }

MCHobby_ArduinoMorse::MCHobby_ArduinoMorse( uint8_t piezo_pin, uint8_t led_pin ){ 
	_piezo_pin = piezo_pin;
	_led_pin   = led_pin; 
	_tone = 1200; // default tone for the buzzer
	morse_speed( 100 ); // default morse speed (dot length)
}

bool MCHobby_ArduinoMorse::begin(   ) {
	pinMode( _piezo_pin, OUTPUT );
	pinMode( _led_pin, OUTPUT );

	return true;
}

void MCHobby_ArduinoMorse::morse_speed( uint16_t dot_len_ms ){
	/* Configure the morse speed. 
	 *  dotLen = time of the dot in milliseconds.
	 *  dashLen = 3 x dot len
	 *  interDotLen    = dot len (it is the time between dot and dash when sending a charatacter in morse)
	 *  interLetterLen = 3 x dot len (it is the time between 2 characters when morsing a message)
	 *  interWordLen   = 5 x dot len (it is the time between 2 words when morsing a message)
	 * 
	 */
	_dotLen         = dot_len_ms;         // length of the morse code 'dot'
	_dashLen        = _dotLen * 3;  // length of the morse code 'dash'
	_interDotLen    = _dotLen;      // space between 2 dots (2 dash, ...) of a single letter 
	_interLetterLen = _dotLen * 3;  // space between 2 letters of a word.
	_interWordLen   = _dotLen * 5;  // space between 2 words	
}

void MCHobby_ArduinoMorse::morse_tone( uint16_t new_tone ){
	_tone = new_tone;    
}

void MCHobby_ArduinoMorse::morse_beep( uint16_t beepLen ){
  /* Beep for x milliseconds */

  digitalWrite(_led_pin, HIGH);     // switch on the Led
  tone(_piezo_pin, _tone, beepLen);// start playing a tone
  delay(beepLen);                  // hold in this position
  noTone(_piezo_pin);              // Stop tone
  digitalWrite(_led_pin, LOW);     // switch off the Led
}

// Make a silent for x milliseconds between beep
void MCHobby_ArduinoMorse::morse_silent( uint16_t silentLen ){
  noTone(_piezo_pin);             // ENSURE stop tone
  digitalWrite(_led_pin, LOW);    // ENSURE switch off the Led
  delay(silentLen);               // hold silent
}

void MCHobby_ArduinoMorse::morse_char( uint8_t aChar ){
	// compose the morse code for a given char
	MorseRecord thisItem;
	for( int i = 0; i < ArraySize( MorseTable ); i++ ){
	  PROGMEM_readAnything( &MorseTable[i], thisItem);
	  if( thisItem.asciiCode == aChar ){
	    for( int j=0; j < strlen( thisItem.morseString ); j++ ){
	      //Serial.print( thisItem.morseString[j] );
	      
	      // a morseString is composed of '.' or '-' or ' ' (space) 
	      switch (thisItem.morseString[j]) {
	        case ' ' :
	           morse_silent( _interWordLen );
	           //break;
	           return ; /* do not add interLetterLen after interWorlLen */
	        case '.' :
	          morse_beep( _dotLen );
	          if( j < strlen( thisItem.morseString )-1 )
	             morse_silent( _interDotLen );
	          break;
	        case '-' :
	          morse_beep( _dashLen );
	          if( j < strlen( thisItem.morseString )-1 )
	             morse_silent( _interDotLen );
	          break;
	        default:
	          // if not a '.', '-', ' ' then morse an interLetterLen
	          // This case should not happens otherwise it means that 
	          //    invalid character is used into thisItem.moreString
	          break; 
	      } /* switch case */
	      
	    } /* for j .-.-- */
	    morse_silent( _interLetterLen );
	    //Serial.println("");
	    return;

	  } /* if asciiCode == aCharc */
	}

	//Serial.print( "No entry in MorseTable for " );
	//Serial.println( aChar, DEC );  
}

void MCHobby_ArduinoMorse::morse_string( char aString[] ){
  /* Walk the string and sends out every characters on the morse output.
     Accept escape sequence like \\, \r, \n   
   */

  //Serial.println( "Morse String" );
  //Serial.println( aString );

  // Loop through the string and get each character one at a time until the end is reached
  for (int i = 0; i < strlen(aString); i++) {
    char tmpChar;
    tmpChar = aString[i];
    // Handle escape sequence
    if( tmpChar == '\\' ){
    	i += 1; // catch next char
    	// if we are over the string, treat it as a "\"
    	if( i >= strlen(aString) ){
    		tmpChar = '\\';
    	}
    	else {
    		// détect the escape sequence 
    		tmpChar = aString[i];
    		switch( tmpChar ){
    			case 'r':
    			   tmpChar = CR;
    			   break;
    			case 'n':
    			   tmpChar = LF;
    			   break;
    			default:
    			   tmpChar = '?';
    			   break;
    		} // eof: switch	
    	} // eof: else

    } // eof: escape sequence 

    // Get the character and UpperCase it as necessary (a..z)
    if( tmpChar >= 97 && tmpChar <= 122 ) 
      tmpChar = toUpperCase( tmpChar );
 
    // Call the subroutine to morse the character
    //GetChar(tmpChar);
    morse_char(tmpChar);
  }
}

void MCHobby_ArduinoMorse::morse_chars( int arr[] ){
  /* Morse an array of integer (special characters). 
     Each entry is a value between 0 & 255 */ 


  // special characters as BEGIN_OF_TRANSMISSION use #define
  // and #define are converted to interger.
  // Serial.println( "Morse Chars" );
  for( int i=0; i <= (sizeof(arr)/sizeof(int)); i++ ){
    morse_char( uint8_t( arr[i] ) );  
  }
}
