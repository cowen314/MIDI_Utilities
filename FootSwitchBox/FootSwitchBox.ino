#include <Bounce.h>

const int CHANNEL = 5;
//const int[1] EXPRESSION_PEDAL_PINS = {39};
//const int EXPRESSION_PEDAL_PINS_L = 1;
//const int[1] DIGITAL_PEDAL_PINS = {4};
//const int DIGITAL_PEDAL_PINS_L = 1;
//these hardcoded lengths are janky .... but it'll allow iteration

const int DI_FOOT_PIN = 4;
const int AI_FOOT_PIN = 39;
const int CHANGE_THRESH = 3;
const int DIGITAL_NOTE = 106; 

int AI_Val;
int lastChangeVal = 0;

Bounce footButton = Bounce(DI_FOOT_PIN,5);

void setup() {
  pinMode(DI_FOOT_PIN,INPUT_PULLUP);
  
}

void loop() {

  //ANALOG
  AI_Val = analogRead(AI_FOOT_PIN);
  if(abs(AI_Val - lastChangeVal) > CHANGE_THRESH) {
    usbMIDI.sendControlChange(11,AI_Val/8,CHANNEL); //11 should be expression pedal4
    lastChangeVal = AI_Val;
  }

  //DIGITAL
  footButton.update();
  if(footButton.fallingEdge()) {
    usbMIDI.sendNoteOn(DIGITAL_NOTE,127,CHANNEL);
  }
  if(footButton.risingEdge()) {
    usbMIDI.sendNoteOff(DIGITAL_NOTE,0,CHANNEL);
  }
  
  delay(4);
}
