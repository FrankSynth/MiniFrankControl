#ifndef MF_ENCODER_H_
#define MF_ENCODER_H_

/* Describes new objects based on the Rotary*/
#include <Rotary.h>
#include <switch.h>


/* function pointer definition */
typedef void (*encoderActionFunc)(bool switch , bool action, int id);


/* We describe an object in which we instantiate a rotary encoder*/
class encoder {
public:
  encoder(byte pinA, byte pinB, byte switch ,encoderActionFunc actionFunc = nullptr, int id = 0)
  : rot(pinA, pinB), //create rotaryObject
  switch(switch), //and the switchObject
  pinA(pinA), pinB(pinB),
  actionFunc(actionFunc), id(id) {
  }
}

private:
  Rotary rot;                             /* the rotary object which will be created*/
  uint8_t pinA = 0;
  uint8_t pinB = 0;                       /* the pin numbers for output A and output B */
  encoderActionFunc actionFunc = nullptr;  /* function pointer, will be called when there is an action happening */
  int id = 0;                             /* optional ID for identification */
};

void readEncoders(encoder *rotaryEncoders, byte nbEncoder) {
  uint32_t inputs;  //enough space for 32 inputs
  //Combine all Inputs
  inputs = PINA;
  inputs = PINB <<8 | inputs;
  inputs = PINC <<16 | inputs;
  inputs = PIND <<24 | inputs;

  for(int x = 0; x< nbEncoder; x++){
    uint8_t pinValA = bitRead(inputs, rotaryEncoders[x]->pinA);
    uint8_t pinValB = bitRead(inputs, rotaryEncoders[x]->pinB);
    uint8_t event = rot.process(pinValA, pinValB);
    if(event == DIR_CW || event == DIR_CCW) {
      //clock wise or counter-clock wise
      bool clockwise = event == DIR_CW;
      //Call into action function if registered
      actionFunc(0, clockwise, rotaryEncoders[x]->id);
    }
  }

  for(int x = 0; x< nbEncoder; x++){
    uint8_t state = bitRead(inputs, rotaryEncoders[x]->switch);
    uint8_t event = switch.process(state);
    if(event == PUSH || event == RELEASE) {
      //pushed or RELEASE
      bool switch = event == PUSH;
      //Call into action function if registered
      actionFunc(1, switch, rotaryEncoders[x]->id);
    }
  }
}

#endif
