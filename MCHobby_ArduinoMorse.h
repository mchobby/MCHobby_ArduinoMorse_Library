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

/*=========================================================================
    PROGMEM_ReadAnything                
  ========================================================================= 
    https://github.com/bcourter/Arduino-Projects/blob/master/tttt/PROGMEM_readAnything.h  
*/
#ifndef __PROGMEM_ReadAnything_H__
#define __PROGMEM_ReadAnything_H__

template <typename T> void PROGMEM_readAnything (const T * sce, T& dest)
  {
  memcpy_P (&dest, sce, sizeof (T));
  }

template <typename T> T PROGMEM_getAnything (const T * sce)
  {
  static T temp;
  memcpy_P (&temp, sce, sizeof (T));
  return temp;
}

#endif


/*=========================================================================
    Arduino Morse                
  ========================================================================= */

#ifndef __ARDUINOMORSE_H__
#define __ARDUINOMORSE_H__

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#define piezo_pin_default 8
#define morse_led_pin_default 11
/*=========================================================================
    ASCII tables control characters                
  ========================================================================= 
  International morse only use charaters under 128  
*/
#define SOH 0x01
#define EOT 0x04
#define ACK 0x06
#define NAK 0x15
#define FF  0x0C /* Form Feed, New Page */
#define LF  0x0A /* Line Feed, New Line */
#define CR  0x0B /* Carriage return (used in word processor to add a new paragraphe) */
#define SYN 0x16 /* Synchronous Idle (wait) */

/* Using range over 200 for special MORSE combination */
#define _BREAK   0xC8 /* Break = BR */
#define _CLOSING 0xC9 /* Closing = CL */
#define _EOC     0xCA /* EndOfContact */
#define _INS     0xCB /* Invite Named Station to transmit */
#define _IAS     0xCC /* Invice Any Station to transmit */
#define _ATTN    0xCD /* Attention */
#define _SWC     0xCE /* Shift to WABUN MORSE CODE (KANA CODE for Japanese) */
#define _SOS     0xCF /* SOS */

/*=========================================================================
    Morse Control transmission                
  ========================================================================= 
    every Morse Control Transmission entry should have a correspondance 
    in the Ascii table.
*/

#define BEGIN_OF_TRANSMISSION SOH
#define END_OF_TRANSMISSION   EOT
#define ERROR                 NAK
#define OK                    ACK
#define NEW_LINE              LF
#define NEW_PAGE              FF
#define NEW_PARAGRAPH         CR
#define WAIT                  SYN
#define BREAK                 _BREAK
#define CLOSING               _CLOSING
#define ATTENTION             _ATTN
#define END_OF_CONTACT        _EOC
#define INVITE_NAMED_STATION  _INS /* ... to transmit */
#define INVITE_ANY_STATION    _IAS /* ... to transmit */
#define SHIFT_WABUN_CODE      _SWC 
#define SOS                   _SOS /* send SOS message /*

/*=========================================================================
    MORSE CODE definition                
  ========================================================================= 
    https://arduino.stackexchange.com/questions/13545/using-progmem-to-store-array-of-structs
    https://www.arduino.cc/reference/en/language/variables/utilities/progmem/
    https://www.arduino.cc/reference/en/language/variables/data-types/array/
*/
struct MorseRecord {
   uint8_t asciiCode;
   const char* morseString;    
};

// number of items in an array
template< typename T, size_t N > size_t ArraySize (T (&) [N]){ return N; }

// . is a Morse Dot
// - is a Morse Dash
// space is mose space between words
const MorseRecord MorseTable[70] PROGMEM = {
  { 'A', ".-"   },
  { 'B', "-..." },
  { 'C', "-.-." },
  { 'D', "-.."  },
  { 'E', "."    },
  { 'F', "..-." }, 
  { 'G', "--."  },
  { 'H', "...." },
  { 'I', ".."   },
  { 'J', ".---" },
  { 'K', "-.-"  },
  { 'L', ".-.." },
  { 'M', "--"   },
  { 'N', "-."   },
  { 'O', "---"  },
  { 'P', ".--." },
  { 'Q', "--.-" },
  { 'R', ".-."  },
  { 'S', "..."  },
  { 'T', "-"    },
  { 'U', "..-"  },
  { 'V', "...-" },
  { 'W', ".--"  },
  { 'X', "-..-" },
  { 'Y', "-.--" },
  { 'Z', "--.." },
  { '0', "-----" },
  { '1', ".----" },
  { '2', "..---" },
  { '3', "...--" },
  { '4', "....-" },
  { '5', "....." },
  { '6', "-...." },
  { '7', "--..." },
  { '8', "---.." },
  { '9', "----." },
  { ',', "--..--" },
  { '.', ".-.-.-" },
  { ':', "---..." },
  { '?', "..--.." },
  { '!', "-.-.--" },
  { ';', "..--.." },
  { 0x27, ".----." }, /* Apostrophe */
  { '"', ".-..-."  },
  { '+', ".-.-."   },
  { '-', "-....-"  },
  { '/', "-..-."   },
  { '(', "-.--."   }, /* just before a word  */
  { ')', "-.--.-"  }, /* just after a word */
  { '_', "..--.-"  }, /* just before or after a word (or phrase part) */
  { '=', "-...-"   },
  { '$', "...-..-" },
  { '@', ".--.-."  }, 
  { ' ', " " },
  { SOH, "-.-.-" },
  { EOT, ".-.-." },
  { ACK, "...-." }, /* OK */
  { NAK, "........" },
  { LF , ".-.-"     }, /*NL Line feed, New Line */
  { FF , ".-.-."    }, /*NP Form feed, New page */
  { CR , "-...-"    }, /* Carriage Return (like a new paragraph) ATTN: this look identical to = */
  { SYN, ".-..."    }, /* Synchronous Idle (wait) */
  { _BREAK  , "-... -.-"  }, /* Break coded with BR */
  { _CLOSING, "-.-. .-.." }, /* Closing coded with CL */
  { _ATTN   , "-.-.-"     }, /* Attention */ 
  { _EOC    , "..."       }, /* End Of Contact */
  { _INS    , "-.--."     }, /* Invitation for named station to transmit */
  { _IAS    , "-.-"       }, /* Invitation for named station to transmit */
  { _SWC    , "-..---"    }, /* Shift to Wabun/Kana code */
  { _SOS    , "...---..." }  /* SOS message */
};


class MCHobby_ArduinoMorse
{
  public:
	MCHobby_ArduinoMorse(void);
	MCHobby_ArduinoMorse(uint8_t piezo_pin, uint8_t led_pin);

    bool begin();

    void morse_speed( uint16_t dot_len_ms );
    void morse_tone( uint16_t new_tone );

    void morse_char( uint8_t aChar );
	void morse_string( char aString[] );
	void morse_chars( int arr[] );    
  private:
  	uint8_t _piezo_pin, _led_pin; 
  	uint16_t _tone; // Music Note/pitch

    // Configure the morse speed configuration in ms  
	uint16_t _dotLen        ; // length of the morse code 'dot'
	uint16_t _dashLen       ; // length of the morse code 'dash'
	uint16_t _interDotLen   ; // space between 2 dots (2 dash, ...) of a single letter 
	uint16_t _interLetterLen; // space between 2 letters of a word.
	uint16_t _interWordLen  ;  

	void morse_beep( uint16_t beepLen );
	void morse_silent( uint16_t silentLen );
};

#endif