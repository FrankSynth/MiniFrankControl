//code for the MiniFrank encoder microcontroller (Atmega128)

#include "encoder.h"

//Ringbuffer
static byte buffer[64];  //64byte buffer
static byte head = 0; //buffer head
static byte tail = 0; //buffer tail
static byte dataAvailable = 0;

 void encoderChanged(bool isSwitch ,bool action, int id);

/* Array of all rotary encoders and their pins  PORTA: 0-7 PORTB: 8-15 PORTC: 16-23 PORTD: 24-31 */
encoder rotaryEncoders[] = {
//   //encoder(pinA,pinB,pinSwitch,ID)
   encoder(15, 14, 13, 0),  //Encoder 1
   encoder(19, 18, 26, 1),  //Encoder 2
   encoder(21, 20, 27, 2),  //Encoder 3
   encoder(16, 17, 28, 3),  //Encoder 4
   encoder(0, 1, 8, 4),  //Encoder 5
   encoder(22, 23, 9, 5),  //Encoder 6
   encoder(7, 6, 10, 6),  //Encoder 7
   encoder(5, 4, 11, 7),  //Encoder 8
   encoder(30,31, 29, 8),  //Encoder 9
   encoder(3, 2, 12, 9),  //Encoder 10


 };

constexpr int nbEncoder = 10; //number of encoder

void setup(){
  // //set to inputs     1 is output, 0 is input
pinMode(0,INPUT);
pinMode(1,INPUT);
pinMode(2,INPUT);
pinMode(3,INPUT);
pinMode(4,INPUT);
pinMode(5,INPUT);
pinMode(6,INPUT);
pinMode(7,INPUT);
pinMode(8,INPUT);
pinMode(9,INPUT);
pinMode(10,INPUT);
pinMode(11,INPUT);
pinMode(12,INPUT);
pinMode(13,INPUT);
pinMode(14,INPUT);
pinMode(15,INPUT);
pinMode(16,INPUT);
pinMode(17,INPUT);
pinMode(18,INPUT);
pinMode(19,INPUT);
pinMode(20,INPUT);
pinMode(21,INPUT);
pinMode(22,INPUT);
pinMode(23,INPUT);
pinMode(24,INPUT);
pinMode(25,INPUT);
pinMode(26,INPUT);
pinMode(27,INPUT);
pinMode(28,INPUT);
pinMode(29,INPUT);
pinMode(30,INPUT);
pinMode(31,INPUT);

digitalWrite(0,HIGH);
digitalWrite(1,HIGH);
digitalWrite(2,HIGH);
digitalWrite(3,HIGH);
digitalWrite(4,HIGH);
digitalWrite(5,HIGH);
digitalWrite(6,HIGH);
digitalWrite(7,HIGH);
digitalWrite(8,HIGH);
digitalWrite(9,HIGH);
digitalWrite(10,HIGH);
digitalWrite(11,HIGH);
digitalWrite(12,HIGH);
digitalWrite(13,HIGH);
digitalWrite(14,HIGH);
digitalWrite(15,HIGH);
digitalWrite(16,HIGH);
digitalWrite(17,HIGH);
digitalWrite(18,HIGH);
digitalWrite(19,HIGH);
digitalWrite(20,HIGH);
digitalWrite(21,HIGH);
digitalWrite(22,HIGH);
digitalWrite(23,HIGH);
digitalWrite(24,HIGH);
digitalWrite(25,HIGH);
digitalWrite(26,HIGH);
digitalWrite(27,HIGH);
digitalWrite(28,HIGH);
digitalWrite(29,HIGH);
digitalWrite(30,HIGH);
digitalWrite(31,HIGH);



  //
   Serial.begin(115200); //Start Serial connection

  while (!Serial.available()) { //wait for the hello message
  }

  byte rcv = Serial.read();
  //Serial.println(rcv,BIN);

  Serial.write(rcv); //answer him and start
  delay(50);
}

void loop(){
  //check encoder changes and execute the encoderChanged function
  readEncoders(rotaryEncoders, nbEncoder, &encoderChanged);
  sendBuffer();

  //send changes to the teensy
//byte send;


//Test Szenario///////
//Switches
// for (size_t i = 0; i < 8; i++) {
//   send = 0x80 |  i | 1<<4;
//   ringBuffer(send) ;
//   sendBuffer();
//
//
//   delay(1000);
// }

//Rotary

//
// for (size_t i = 0; i < 8; i++) {
//   send = 0x80 |  i | 1<<4;
//   ringBuffer(send) ;
//   sendBuffer();
//   delay(5);
// }
//
// for (size_t i = 0; i < 8; i++) {
//   send = 0x80 |  i | 0<<4;
//   ringBuffer(send) ;
//   sendBuffer();
//   delay(5);
// }
//
//
// for (size_t i = 0; i < 8; i++) {
//   send = 0x00 |  i | 1<<4;
//   ringBuffer(send) ;
//   sendBuffer();
//   delay(5);
// }
//
// //Rotary
// for (size_t i = 0; i < 8; i++) {
//   send = 0x00 |  i | 0<<4;
//   ringBuffer(send) ;
//   sendBuffer();
//   delay(5);
// }

//////////////////////////



}

void encoderChanged(bool isSwitch, bool action, int id){
  byte send;

  //The byte
  // |7|6|5|4|3|2|1|0|   .. byte
  // |switch = 1, rotate = 0|-|-|direction or push/relese|id|id|id|id|   .. byte
  if(isSwitch){
    send = 0x80 | id | action<<4;
  }
  else{
    send = 0x00 | id | action<<4;
  }
  ringBuffer(send) ;
}

void ringBuffer(byte data){
  buffer[head] = data;
  head = (head + 1)%64; //head counter
  dataAvailable = 1;

  //we expect never to fill the buffer
}

void sendBuffer(){
  while(dataAvailable){
    Serial.write(buffer[tail]);

    tail = (tail + 1)%64; //tail counter

    //head reached?
    if(tail == head) dataAvailable = 0;  //seems we send everything lets go back to the main loop
  }
}
