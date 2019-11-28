
#include "Arduino.h"
#include "switch.h"

#define NOPE 0x0
#define PUSH 0x1
#define RELEASE 0x2


/*
 * Constructor. Each arg is the pin number for each encoder contact.
 */
switch::switch() {
  state = 1;
}

unsigned char switch::process(bool newState) {

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
