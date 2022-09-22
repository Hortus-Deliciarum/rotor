#include <Button2.h>
#include <RotaryEncoder.h>
#include <HortusWifi.h>

// Note: You also have to connect GND, 5V/VIO and VM.
//       A connection diagram can be found in the schematics.
#define EN_PIN1       26  //enable (CFG6)
#define DIR_PIN1      33  //direction
#define STEP_PIN1     25  //step
#define ACTIVE1       0

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
#define MIN_SPEED     200
#define DEF_SPEED     1000
#define MULTER        100

int debounce = 50;
int button_state = 0;
int button2_state = 0;

const char * ssid = "FASTWEB-2yurFq";
const char * password = "yBqCDXC6w8";
const char * ip = "192.168.0.100";
const int recv_port = 54321;

Button2 button1;
Button2 button2;
RotaryEncoder encoder1(ROTARY1_PIN1, ROTARY1_PIN2, RotaryEncoder::LatchMode::FOUR3);
RotaryEncoder encoder2(ROTARY2_PIN1, ROTARY2_PIN2, RotaryEncoder::LatchMode::FOUR3);

typedef struct {
  bool state;
  unsigned long last_time;
  int dur;
  int level;
  int step_pin;
} Motor;

Motor motor1 = { false, 0, DEF_SPEED, LOW, STEP_PIN1 };
Motor motor2 = { false, 0, DEF_SPEED, LOW, STEP_PIN2 };

void setup()
{

  Serial.begin(115200);

  button1.begin(BUTTONPIN1);
  button2.begin(BUTTONPIN2);
  button1.setReleasedHandler(released);
  button2.setReleasedHandler(released);
  encoder1.setPosition(DEF_SPEED);
  encoder2.setPosition(DEF_SPEED);

  HortusWifi(ssid, password, ip);

  OscWiFi.subscribe(recv_port, "/motor1/state",
    [](const OscMessage &m) {
      float _state = m.arg<float>(0);
      set_motor_state(&motor1, _state);
    });

  OscWiFi.subscribe(recv_port, "/motor1/speed",
    [](const OscMessage &m) {
      float _speed = m.arg<float>(0);
      set_motor_speed(encoder1, _speed);
    });

  OscWiFi.subscribe(recv_port, "/motor2/state",
    [](const OscMessage &m) {
      float _state = m.arg<float>(0);
      set_motor_state(&motor2, _state);
    });

  OscWiFi.subscribe(recv_port, "/motor2/speed",
    [](const OscMessage &m) {
      float _speed = m.arg<float>(0);
      set_motor_speed(encoder2, _speed);
    });
  
  
  pinMode(ACTIVE1, OUTPUT);
  digitalWrite(ACTIVE1, LOW);
  pinMode(ACTIVE2, OUTPUT);
  digitalWrite(ACTIVE2, LOW);
  
  delay(2000);
  
  // motor 1
  pinMode(EN_PIN1, OUTPUT);
  digitalWrite(EN_PIN1, HIGH); //deactivate driver (LOW active)
  pinMode(DIR_PIN1, OUTPUT);
  digitalWrite(DIR_PIN1, LOW); //LOW to CCW
  //digitalWrite(DIR_PIN1, HIGH); //HIGH to CW
  pinMode(STEP_PIN1, OUTPUT);
  digitalWrite(STEP_PIN1, LOW);
  digitalWrite(EN_PIN1, LOW); //activate driver

  // motor 2
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
  OscWiFi.update();
  button1.loop();
  button2.loop();
  check_speed(&motor1, encoder1);
  check_speed(&motor2, encoder2);
  check_motor(&motor1);
  check_motor(&motor2);
}

void released(Button2 &btn) {
  if (btn == button1) {
    motor1.state = !motor1.state;
  } else if (btn == button2) {
    motor2.state = !motor2.state;
  }
}

void set_motor_state(Motor *m, float value) {
  if (value)
    m->state = true;
  else
    m->state = false;
}

void set_motor_speed(RotaryEncoder &_enc, float _spd) {
  int _speed = rounder100(_spd);
  _speed = constrain(_speed, MIN_SPEED, MAX_SPEED);
  _enc.setPosition(_speed);
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

void check_speed(Motor *m, RotaryEncoder &_enc) { 
  _enc.tick();
  int newPos = _enc.getPosition();
  
  if (m->dur != newPos) {
    //int factor = newPos - m->dur;
    int _newPos = constrain(newPos, MIN_SPEED, MAX_SPEED);
    _enc.setPosition(_newPos);
    m->dur = newPos;
    Serial.println(m->dur);
  }
}

int rounder100(float n) {
  return (int)n / 100 * 100;
}
