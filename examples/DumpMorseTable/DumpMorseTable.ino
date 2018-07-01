/***************************************************************************
  This is a library for the ArduinoMorse
      Dump the Morse table on the Serial connection. 

  Designed specifically to work with the LED and Piezo Buzzer
  ----> https://shop.mchobby.be/senseur-divers/57-piezo-3232100000575.html
  ----> https://shop.mchobby.be/senseur-divers/1334-piezo-buzzer-large-3232100013346.html
  ----> https://shop.mchobby.be/minikits/66-assortiment-leds-5mm-mini-kit-3232100000667.html

  MC Hobby SPRL invests time and resources providing this open source code,
  please support MC Hobby and open-source by purchasing products
  from MC Hobby! shop.mchobby.be

  Written by Meurisse D. for MC Hobby SPRL.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/
#include <MCHobby_ArduinoMorse.h>

void setup() {
   Serial.begin(9600);
}

// char stringToMorse[] = "Hello from MCHobby Belgium!";
//int end_of_msg[] = { END_OF_TRANSMISSION, INVITE_ANY_STATION };
struct NamedExtRecord {
   uint8_t asciiCode;
   const String NameString;    
};

// This translation table must be completed as new entry
// is added to the MorseTable
const int NamedExtCount = 15;
NamedExtRecord ExtRecord[NamedExtCount] = {
  { BEGIN_OF_TRANSMISSION, "BEGIN_OF_TRANSMISSION"   },
  { END_OF_TRANSMISSION, "END_OF_TRANSMISSION" },
  { ERROR, "ERROR" },
  { NEW_LINE, "NEW_LINE"  },
  { NEW_PAGE, "NEW_PAGE"  },
  { NEW_PARAGRAPH, "NEW_PARAGRAPH"  },
  { WAIT, "WAIT"  },
  { BREAK, "BREAK"  },
  { CLOSING, "CLOSING"  },
  { ATTENTION, "ATTENTION"  },
  { END_OF_CONTACT, "END_OF_CONTACT"  },
  { INVITE_NAMED_STATION, "INVITE_NAMED_STATION"  },
  { INVITE_ANY_STATION, "INVITE_ANY_STATION"  },
  { SHIFT_WABUN_CODE, "SHIFT_WABUN_CODE"  },
  { SOS, "SOS"  }
};

String getNamedExt( uint8_t aChar ){
  
  for( int j = 0; j < NamedExtCount; j++ ){
     
     if( ExtRecord[j].asciiCode == aChar ){
         return (ExtRecord[j].NameString);
     }
  }
  return "(not named!)";
} 

void loop() {
   // put your main code here, to run repeatedly:
   Serial.println( "============================================" );
   Serial.println( "Dumping the morse table to Serial connection" );
   Serial.println( "============================================" );
   Serial.println( " Item#  | ASCII | characters | Morse | Remark"  );
   MorseRecord thisItem;
   for( int i = 0; i < ArraySize( MorseTable ); i++ ){
      PROGMEM_readAnything( &MorseTable[i], thisItem);
      Serial.print( "#" );
      Serial.print( i );
      Serial.print( " | " );
      Serial.print( thisItem.asciiCode, DEC );
      Serial.print( " | " );
      Serial.write( thisItem.asciiCode ); // display as char
      Serial.print( " | " );
      Serial.print( thisItem.morseString );
      if( thisItem.asciiCode >= 200 ){
         Serial.print( " | ");
         Serial.print( getNamedExt( thisItem.asciiCode ) );
         Serial.println( " - Extended character (>200)" );
      }
      else 
        if( thisItem.asciiCode < 32 ){
           Serial.print( " | ");
           Serial.print( getNamedExt( thisItem.asciiCode ) );
           Serial.println( " - ASCII Control Character (<32)" );
        }
        else{
           Serial.println( " | " );
        }
   }   

   // At the end of the string long pause before looping and starting again
   delay( 8000 );   
}
