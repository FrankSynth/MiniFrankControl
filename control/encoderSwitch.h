
#ifndef MF_ENCODERSWITCH_H
#define MF_ENCODERSWITCH_H

#include "Arduino.h"

#define NOPE 0x0
#define PUSH 0x1
#define RELEASE 0x2

class encoderSwitch
{
public:
  //Init switch object per encoder
  encoderSwitch() {
    state = 1;
  }
  //test switch release or push
  byte process(byte newState) {

    if(state != newState){ //Pin state changed
      if(newState){  //new state is Release =>
        return RELEASE;
      }
      else{           //new state is Push =>
        return PUSH;
      }
    }
    else{           //nothing changed..
      return NOPE;
    }
  }

private:
  unsigned char state;
};

#endif
