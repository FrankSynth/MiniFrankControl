#include "encoder.h"

//Ringbuffer
static byte ringBuffer[64];  //64byte buffer
static byte head = 0; //buffer head
static byte tail = 0; //buffer tail
static byte dataAvailable = 0;

void encoderChanged(bool switch ,bool action, int id);

/* Array of all rotary encoders and their pins  PORTA: 0-7 PORTB: 8-15 PORTC: 16-23 PORTD: 24-31 */
encoder rotaryEncoders[] = {
  encoder(0, 1, &EncoderChanged, 0),  //Encoder 0
  encoder(2, 3, &EncoderChanged, 1),  //Encoder 1
  encoder(4, 5, &EncoderChanged, 2),  //Encoder 2
  encoder(6, 7, &EncoderChanged, 3),  //Encoder 3
  encoder(8, 9, &EncoderChanged, 4),  //Encoder 4
  encoder(10, 11, &EncoderChanged, 5),  //Encoder 5
  encoder(16, 17, &EncoderChanged, 6),  //Encoder 6
  encoder(18, 19, &EncoderChanged, 7),  //Encoder 7
  encoder(20, 21, &EncoderChanged, 8),  //Encoder 8
  encoder(22, 23, &EncoderChanged, 9),  //Encoder 9
};

constexpr int nbEncoder = (int)(sizeof(encoder) / sizeof(*rotaryEncoders)); //number of encoder

void setup(){
  //set to inputs     1 is output, 0 is input
  DDRD = DDRD & 11000000; //ignore UART pins
  DDRC = 00000000;
  DDRB = 00000000;
  DDRA = 00000000;

  //enable Pullups
  PORTD = PORTD & 00111111; //ignore UART pins
  PORTC = 11111111;
  PORTB = 11111111;
  PORTA = 11111111;

  Serial.begin("115200") //Start Serial connection

};

void loop(){
  //check encoder changes and execute the encoderChanged function
  encoder.readEncoders(rotaryEncoders[], nbEncoder);

  //empty ringBuffer
  sendBuffer();

//seems we are getting data
  while (Serial.available() > 0) {
    byte rcv = Serial.read();
    //do something with the data...
  }
};

void encoderChanged(bool switch, bool action, int id){
  //buildind the send byte
  // |7|6|5|4|3|2|1|0|   .. byte
  // |switch = 1, rotate = 0|-|-|-|-|direction or push/relese|id|id|   .. byte
  if(switch){
    byte send = 0x01 | id | action<<3;
  }
  else{
    byte send = 0x00 | id | action<<3;
  }
  ringBuffer(send) ;
}

void ringBuffer(byte data){
  rindbuffer[head] = data;
  head = (head + 1)%64; //head counter to 64
  dataAvailable = 1;
  //we expect never to fill the buffer
}


void sendBuffer(byte data){
  while(dataAvailable){
    Serial.write(rindBuffer[tail]);
    tail = (tail + 1)%64 //tail counter to 64

    //head reached?
    if(tail == head) dataAvailable = 0;
  }
}
