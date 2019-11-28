
#ifndef MF_SWITCH_H
#define MF_SWITCH_H

#include "Arduino.h"

class switch
{
  public:
    Switch();
    process(bool);

  private:
    unsigned char state;
    unsigned char pinValue;

};

#endif
