#ifndef MF_ENCODER_H_
#define MF_ENCODER_H_

/* Describes new objects based on the Rotary*/
#include <Rotary.h>
#include "encoderSwitch.h"


/* function pointer definition */
typedef void (*encoderActionFunc)(bool isSwitch , bool action, int id);

byte bitReadInt(unsigned long b, byte bitPos){
  unsigned long x = b & ((unsigned long)1 << bitPos);
  return x == 0 ? 0 : 1;
}

/* We describe an object in which we instantiate a rotary encoder*/
class encoder {
public:
  encoder(byte pinA, byte pinB, byte pinSwitch, int id)
  : rot(pinA, pinB),  //create rotaryObject
  encSwitch(),        //create switchObject
  pinA(pinA), pinB(pinB),
  pinSwitch(pinSwitch),
  id(id) {
  }


  Rotary rot;                   /* the rotary object which will be created*/
  encoderSwitch encSwitch;      /* the switch object which will be created*/

  uint8_t pinA = 0;                     /* the pin numbers for output A and output B */
  uint8_t pinB = 0;
  uint8_t pinSwitch = 0;                 //Switch pin
  int id = 0;                             /* ID for identification */
};

void readEncoders(encoder *rotaryEncoders, byte nbEncoder, encoderActionFunc actionFunc) {
  unsigned long inputs = 0;  //enough space for 32 inputs
  //Combine all Inputs
  inputs = (unsigned long)PINA;
  inputs = (unsigned long)((unsigned long)PINB <<8) | inputs;
  inputs = (unsigned long)((unsigned long)PINC <<16) | inputs;
  inputs = (unsigned long)((unsigned long)PIND <<24) | inputs;

  //Lets check all encoder
  for(int x = 0; x< nbEncoder; x++){
    //Test rotation
    uint8_t pinValA = bitReadInt(inputs, rotaryEncoders[x].pinA); //current pinA Value
    uint8_t pinValB = bitReadInt(inputs, rotaryEncoders[x].pinB); //current pinB Value
    uint8_t event = rotaryEncoders[x].rot.process(pinValA, pinValB);     //process Values and get event state
    if(event == DIR_CW || event == DIR_CCW) {  //clock wise or counter-clock wise

      event = event == DIR_CW;  //Clockwise = 1

      //Call into action function if registered
      actionFunc(0, event, rotaryEncoders[x].id);
    }

    //Test push switch
    uint8_t state = bitReadInt(inputs, rotaryEncoders[x].pinSwitch); //current pinSwitch value
    event =  rotaryEncoders[x].encSwitch.process(state); //process switchValue and get event state
    if(event == PUSH || event == RELEASE) {      //pushed or RELEASE

      event = event == PUSH;  //Push = 1
      //Call into action function if registered
      actionFunc(1, event, rotaryEncoders[x].id);
    }
  }
}

#endif
