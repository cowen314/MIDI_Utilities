#include <Bounce.h>

enum RunMode {
  standard,
  debug
};

const RunMode CONTEXT = standard;

const int CHANNEL = 5;

const int DI_FOOT_PIN = 4;
const int AI_FOOT_PIN = 39;
const int CHANGE_THRESH = 2;
const int DIGITAL_NOTE = 106; 
const double Ax = 908.0/(10.0-1.0);
const double Bx = 115.0 - Ax;

int AI_Val_raw;
int AI_Val_log;
int AI_Val_linear;
int lastChangeVal = 0;

double mapDouble(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

Bounce footButton = Bounce(DI_FOOT_PIN,5);

void setup() {
  switch(CONTEXT) {
    case standard: {  
      lastChangeVal = 1; //filler code
    }
      break;
    case debug: {
      Serial.begin(9600);
    }
      break;
  }
  pinMode(DI_FOOT_PIN,INPUT_PULLUP);
  
}

void loop() {

  //ANALOG
  AI_Val_raw = analogRead(AI_FOOT_PIN);
  AI_Val_log = (int)(127*log10( ((double)(AI_Val_raw) - Bx)  / Ax));
  AI_Val_linear = map(AI_Val_raw,115,1023,0,127);
  //Serial.print("log: ");
  //Serial.println(AI_Val_log);
  //Serial.println(AI_Val_linear);
  if(abs(AI_Val_log - lastChangeVal) > CHANGE_THRESH) {
    switch(CONTEXT) {
      case standard: {
        usbMIDI.sendControlChange(11,(int)(AI_Val_log),CHANNEL);
      }
        break;
      case debug: {
        Serial.print("Analog update (lin): ");
        Serial.println(AI_Val_linear);
        Serial.print("Analog update (log): ");
        Serial.println(AI_Val_log);
      }
        break;
    }
    lastChangeVal = AI_Val_log;
  }

  //DIGITAL
  footButton.update();
  if(footButton.fallingEdge()) {
    switch(CONTEXT) {
      case standard: {
        usbMIDI.sendNoteOn(DIGITAL_NOTE,127,CHANNEL);
      }
        break;
      case debug: {
        Serial.println("Foot down");
      }
        
    }
  }
  
  if(footButton.risingEdge()) {
    switch(CONTEXT) {
      case standard: {
        usbMIDI.sendNoteOff(DIGITAL_NOTE,0,CHANNEL);
      }
        break;
      case debug: {
        Serial.println("Foot up");
      }
    }
  }
  
  delay(4);
}
