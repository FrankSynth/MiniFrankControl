//code for the MiniFrank encoder microcontroller (Atmega128)

//#include "encoder.h"

//Ringbuffer
static byte buffer[64];  //64byte buffer
static byte head = 0; //buffer head
static byte tail = 0; //buffer tail
static byte dataAvailable = 0;

// void encoderChanged(bool isSwitch ,bool action, int id);

/* Array of all rotary encoders and their pins  PORTA: 0-7 PORTB: 8-15 PORTC: 16-23 PORTD: 24-31 */
// encoder rotaryEncoders[] = {
//   //encoder(pinA,pinB,pinSwitch,ID)
//   encoder(0, 1, 31, 0),  //Encoder 0
//   encoder(2, 3, 30, 1),  //Encoder 1
//   encoder(4, 5, 29, 2),  //Encoder 2
//   encoder(6, 7, 28, 3),  //Encoder 3
//   encoder(8, 9, 27, 4),  //Encoder 4
//   encoder(10, 11, 26, 5),  //Encoder 5
//   encoder(16, 17, 15, 6),  //Encoder 6
//   encoder(18, 19, 14, 7),  //Encoder 7
//   encoder(20, 21, 13, 8),  //Encoder 8
//   encoder(22, 23, 12, 9),  //Encoder 9
// };

//constexpr int nbEncoder = (int)(sizeof(encoder) / sizeof(*rotaryEncoders)); //number of encoder

void setup(){
  // //set to inputs     1 is output, 0 is input
  // DDRD = DDRD & 11000000; //ignore UART pins
  // DDRC = 00000000;
  // DDRB = 00000000;
  // DDRA = 00000000;
  //
  // //enable Pullups
  // PORTD = PORTD & 00111111; //ignore UART pins
  // PORTC = 11111111;
  // PORTB = 11111111;
  // PORTA = 11111111;
  //
   Serial.begin(115200); //Start Serial connection
   Serial1.begin(1000000); //Start Serial connection

   Serial.println("Hello");

  while (!Serial1.available()) { //wait for the hello message
  }

  byte rcv = Serial1.read();
  Serial.println(rcv,BIN);

  Serial1.write(rcv); //answer him and start
  delay(50);
}

void loop(){
  //check encoder changes and execute the encoderChanged function
  //readEncoders(rotaryEncoders, nbEncoder, &encoderChanged);
  //send changes to the teensy
byte send;


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


for (size_t i = 0; i < 8; i++) {
  send = 0x80 |  i | 1<<4;
  ringBuffer(send) ;
  sendBuffer();
  delay(5);
}

for (size_t i = 0; i < 8; i++) {
  send = 0x80 |  i | 0<<4;
  ringBuffer(send) ;
  sendBuffer();
  delay(5);
}


for (size_t i = 0; i < 8; i++) {
  send = 0x00 |  i | 1<<4;
  ringBuffer(send) ;
  sendBuffer();
  delay(5);
}

//Rotary
for (size_t i = 0; i < 8; i++) {
  send = 0x00 |  i | 0<<4;
  ringBuffer(send) ;
  sendBuffer();
  delay(5);
}

//////////////////////////

//sendBuffer();


}

// void encoderChanged(bool isSwitch, bool action, int id){
//   byte send;
//
//   //The byte
//   // |7|6|5|4|3|2|1|0|   .. byte
//   // |switch = 1, rotate = 0|-|-|direction or push/relese|id|id|id|id|   .. byte
//   if(isSwitch){
//     send = 0x80 | id | action<<4;
//   }
//   else{
//     send = 0x00 | id | action<<4;
//   }
//   ringBuffer(send) ;
// }

void ringBuffer(byte data){
  buffer[head] = data;
  head = (head + 1)%64; //head counter
  dataAvailable = 1;
  //we expect never to fill the buffer
}

void sendBuffer(){
  while(dataAvailable){
    Serial1.write(buffer[tail]);
    Serial.println(buffer[tail],BIN);

    tail = (tail + 1)%64; //tail counter

    //head reached?
    if(tail == head) dataAvailable = 0;  //seems we send everything lets go back to the main loop
  }
}
