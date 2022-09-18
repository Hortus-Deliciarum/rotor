/*
  SilentStepStick TMC2208/TMC2209 Example
  Rsense: 0.11 Ohm
  Other examples/libraries can be found here:
  https://github.com/teemuatlut/TMCStepper
  https://github.com/trinamic/TMC-API
  https://github.com/manoukianv/TMC2208Pilot
  Example source code free to use.
  Further information: https://learn.watterott.com/license/
*/

#include "Button2.h"


// Note: You also have to connect GND, 5V/VIO and VM.
//       A connection diagram can be found in the schematics.
#define EN_PIN    26  //enable (CFG6)
#define DIR_PIN   33  //direction
#define STEP_PIN  25  //step
#define ACTIVE    0

#define EN_PIN2    16  //enable (CFG6)
#define DIR_PIN2   5  //direction
#define STEP_PIN2  17  //step
#define ACTIVE2    4

int BUTTONPIN = 27;
int debounce = 30;
int button_state = 0;

Button2 button;

//long lastime = 0;
//int state = 1;

void setup()
{

  Serial.begin(115200);

  button.begin(BUTTONPIN);
  button.setReleasedHandler(released);
  
  
  pinMode(ACTIVE, OUTPUT);
  digitalWrite(ACTIVE, LOW);

  pinMode(ACTIVE2, OUTPUT);
  digitalWrite(ACTIVE2, LOW);
  
  delay(2000);
  
  pinMode(EN_PIN2, OUTPUT);
  digitalWrite(EN_PIN2, HIGH); //deactivate driver (LOW active)
  pinMode(DIR_PIN2, OUTPUT);
  digitalWrite(DIR_PIN2, LOW); //LOW to CCW
  //digitalWrite(DIR_PIN, HIGH); //HIGH to CW
  pinMode(STEP_PIN2, OUTPUT);
  digitalWrite(STEP_PIN2, LOW);

  digitalWrite(EN_PIN2, LOW); //activate driver
  
}

void loop()
{
  button.loop();
  //make steps

  
  //digitalWrite(ACTIVE2, button_state);

  if (button_state) {
    digitalWrite(STEP_PIN2, HIGH);
    delayMicroseconds(1000);
    digitalWrite(STEP_PIN2, LOW);
    delayMicroseconds(1000);
  }
  
}

void released(Button2& btn) {
  Serial.print("RELEASED!\tBUTTON STATE:\t");
  Serial.println(button_state);
  Serial.print("ACTIVE2:\t");
  Serial.println(digitalRead(ACTIVE2));
  button_state = 1 - button_state; 
}
