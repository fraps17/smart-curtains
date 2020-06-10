#include<SPI.h>
#include<RF24.h>
#include <printf.h>

#define SPEED 255
#define PINBUTTON 5
#define PINSPEED 2
#define PINMA 3
#define PINMB 4


// ce, csn pins
RF24 radio(9, 10);

boolean toggle;
boolean moving = 0;
boolean closed = 0;
boolean buttonState = 0;


void setup(void) {
  Serial.begin(9600);
  printf_begin();

  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(0x76);
  radio.openWritingPipe(0xF0F0F0F0E1LL);
  radio.openReadingPipe(1, 0xE8E8F0F0E1LL);
  radio.setPayloadSize(32);
  radio.setAutoAck(true);   

  radio.enableDynamicPayloads();
  radio.powerUp();
  radio.printDetails();
  closed = true;
  moving = false;

  pinMode(PINMA, OUTPUT);
  pinMode(PINMB, OUTPUT);
  pinMode(PINSPEED, OUTPUT);
  pinMode(PINBUTTON, INPUT);

  digitalWrite(PINMA, LOW);
  digitalWrite(PINMB, HIGH);
}

void loop(void) {
  toggle = 0;
  radio.startListening(); //This sets the module as receiver
  if(radio.available()){
    //READ COMMAND
    // c0, c1
    // 0, 0 --> GET STATUS
    // 1, 1 --> TOGGLE
    boolean command[2] = { 0, 0};
    radio.read(command, sizeof(command));
    radio.stopListening();
    // SEND CURRENT STATUS;
    const boolean status[2] = { moving, closed };
    radio.write(&status, sizeof(status));
    toggle = (command[0] && command[1]);
  }
  delay(100);
  
  if (moving) {
    if (toggle) {
      moving = false;
      closed = !closed;
      analogWrite(PINSPEED, 0);
    }
    else {
      analogWrite(PINSPEED, SPEED);
      digitalWrite(PINMA, closed ? LOW : HIGH);
      digitalWrite(PINMB, closed ? HIGH : LOW);
    }
  }
  else {
    if (toggle) {
      moving = true;
    }
  }
  delay(500);
}