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

#include <Button2.h>
#include <RotaryEncoder.h>


// Note: You also have to connect GND, 5V/VIO and VM.
//       A connection diagram can be found in the schematics.
#define EN_PIN        26  //enable (CFG6)
#define DIR_PIN       33  //direction
#define STEP_PIN      25  //step
#define ACTIVE        0
#define EN_PIN2       16  //enable (CFG6)
#define DIR_PIN2      5  //direction
#define STEP_PIN2     17  //step
#define ACTIVE2       4
#define BUTTONPIN1    27
#define BUTTONPIN2    2
#define ROTARY1_PIN1  14
#define ROTARY1_PIN2  12
#define ROTARY2_PIN1  15
#define ROTARY2_PIN2  13
#define MAX_SPEED     5000
#define MIN_SPEED     250
#define DEF_SPEED     1000

int debounce = 30;
int button_state = 0;
int button2_state = 0;

Button2 button;
Button2 button2;
RotaryEncoder encoder(ROTARY1_PIN1, ROTARY1_PIN2, RotaryEncoder::LatchMode::FOUR3);
RotaryEncoder encoder1(ROTARY2_PIN1, ROTARY2_PIN2, RotaryEncoder::LatchMode::FOUR3);

typedef struct {
  bool state;
  unsigned long last_time;
  int dur;
  int level;
  int step_pin;
} Motor;

Motor motor1 = { false, 0, DEF_SPEED, LOW, STEP_PIN };
Motor motor2 = { false, 0, DEF_SPEED, LOW, STEP_PIN2 };

void setup()
{

  Serial.begin(115200);

  button.begin(BUTTONPIN1);
  button2.begin(BUTTONPIN2);
  button.setReleasedHandler(released);
  button2.setReleasedHandler(released);
  encoder.setPosition(DEF_SPEED);
  encoder1.setPosition(DEF_SPEED);
  
  
  pinMode(ACTIVE, OUTPUT);
  digitalWrite(ACTIVE, LOW);

  pinMode(ACTIVE2, OUTPUT);
  digitalWrite(ACTIVE2, LOW);
  
  delay(2000);
  
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, HIGH); //deactivate driver (LOW active)
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(DIR_PIN, LOW); //LOW to CCW
  //digitalWrite(DIR_PIN, HIGH); //HIGH to CW
  pinMode(STEP_PIN, OUTPUT);
  digitalWrite(STEP_PIN, LOW);
  digitalWrite(EN_PIN, LOW); //activate driver

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
  button2.loop();
  encoder.tick();
  encoder1.tick();
  check_speed(&motor1, encoder);
  //check_speed(&motor2, encoder1);
  check_motor(&motor1);
  check_motor(&motor2);
  //make steps

  
  //digitalWrite(ACTIVE2, button_state);

  /*
  if (button_state) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(1000);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(1000);
  }

  if (button2_state) {
    digitalWrite(STEP_PIN2, HIGH);
    delayMicroseconds(1000);
    digitalWrite(STEP_PIN2, LOW);
    delayMicroseconds(1000);
  }
  */
  
}

void released(Button2& btn) {
  /*
  Serial.print("RELEASED!\tBUTTON STATE:\t");
  Serial.println(button_state);
  Serial.print("ACTIVE2:\t");
  Serial.println(digitalRead(ACTIVE2));
  button_state = 1 - button_state; 
  */
  if (btn == button) {
    motor1.state = !motor1.state;
  } else if (btn == button2) {
    motor2.state = !motor2.state;
  }
}

void check_motor(Motor *m) {
  unsigned long _now = micros();

  if (m->state) {
    if ((_now - m->last_time) >= m->dur) {
      m->level = !(m->level);
      digitalWrite(m->step_pin,m->level);
      m->last_time = _now;      
    } 
  }
}

void check_speed(Motor *m, RotaryEncoder enc) {
  /*
  int _dir = (int)(enc.getDirection());
  if (_dir != 0)
    Serial.println(m->dur);
  m->dur = constrain(m->dur + _dir, MIN_SPEED, MAX_SPEED);
  */
  int di = (int)(enc.getDirection());
  Serial.println(di);
}
