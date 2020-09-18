#include <PS2X_lib.h>
#include <Servo.h>

PS2X ps2x;
Servo servo[8];

bool BtnLR[8] = {false, false, false, false,
                false, false, false, false};
int motor[] = {2, 3, 4, 5};
int spd;

void setup() {
  Serial.begin(57600);
  for(int i = 0; i < 4; i++) {
    pinMode(motor[i], OUTPUT);
  }
  
  servo[0].attach(6); // 90 - 130
  servo[1].attach(7); // 100 - 0
  servo[2].attach(8); // 90 - 130
  servo[3].attach(9); // 0 - 100
  servo[4].attach(10); // 90 - 130
  servo[5].attach(11); // 0 - 100
  servo[6].attach(12); // 90 - 130
  servo[7].attach(13); // 0 - 100
  
  servo[0].write(90);
  servo[1].write(0);
  servo[2].write(90);
  servo[3].write(0);
  servo[4].write(90);
  servo[5].write(0);
  servo[6].write(90);
  servo[7].write(0);
  ps2x.config_gamepad(50, 42, 48, 40, true, true);
}

void loop() {
  ps2x.read_gamepad(false, 0);

  if(ps2x.Button(PSB_BLUE)) {
    spd = 255;
  } else {
    spd = 125;
  }
  
  if(ps2x.Button(PSB_PAD_UP)) {
    analogWrite(motor[0], spd);
    analogWrite(motor[1], 0);
    analogWrite(motor[2], spd); 
    analogWrite(motor[3], 0);
  } else if(ps2x.Button(PSB_PAD_DOWN)) {
    analogWrite(motor[0], 0);
    analogWrite(motor[1], spd);
    analogWrite(motor[2], 0); 
    analogWrite(motor[3], spd);
  } else if(ps2x.Button(PSB_PAD_LEFT)) {
    analogWrite(motor[0], 0);
    analogWrite(motor[1], spd);
    analogWrite(motor[2], spd);
    analogWrite(motor[3], 0); 
  } else if (ps2x.Button(PSB_PAD_RIGHT)){
    analogWrite(motor[0], spd);
    analogWrite(motor[1], 0);
    analogWrite(motor[2], 0);
    analogWrite(motor[3], spd); 
  } else {
    analogWrite(motor[0], 0);
    analogWrite(motor[1], 0);
    analogWrite(motor[2], 0);
    analogWrite(motor[3], 0); 
  }
  
  if(ps2x.Button(PSB_PINK)) {
    if(ps2x.ButtonReleased(PSB_L1)) Btn_L1(false);
    if(ps2x.ButtonReleased(PSB_L2)) Btn_L2(false);
    if(ps2x.ButtonReleased(PSB_R1)) Btn_R1(false);
    if(ps2x.ButtonReleased(PSB_R2)) Btn_R2(false);  
  } else {
    if(ps2x.ButtonReleased(PSB_L1)) Btn_L1(true);
    if(ps2x.ButtonReleased(PSB_L2)) Btn_L2(true);
    if(ps2x.ButtonReleased(PSB_R1)) Btn_R1(true);
    if(ps2x.ButtonReleased(PSB_R2)) Btn_R2(true);
  }
  
  if(ps2x.ButtonPressed(PSB_RED)) {
    Serial.println("Bulat");
  }
  if(ps2x.ButtonPressed(PSB_PINK)) {
    Serial.println("Kotak");
  }
  if(ps2x.ButtonPressed(PSB_BLUE)) {
    Serial.println("Biru");
  }
  if(ps2x.ButtonPressed(PSB_GREEN)) {
    Serial.println("Segitita");
  }
  delay(50);
}

void Btn_L1(bool g) {
  if(g) {
    Serial.println("a");
    if(BtnLR[0]) {
      servo[0].write(90);
      BtnLR[0] = false;
    } else {
      servo[0].write(130);
      BtnLR[0] = true;
    } 
  } else {
    Serial.println("b");
    if(BtnLR[4]) {
      servo[1].write(0);
      BtnLR[4] = false;
    } else {
      servo[1].write(100);
      BtnLR[4] = true;
    } 
  }
}
void Btn_R1(bool g) {
  if(g) {
    if(BtnLR[1]) {
      servo[2].write(90);
      BtnLR[1] = false;
    } else {
      servo[2].write(130);
      BtnLR[1] = true;
    } 
  } else {
    if(BtnLR[5]) {
      servo[3].write(0);
      BtnLR[5] = false;
    } else {
      servo[3].write(100);
      BtnLR[5] = true;
    } 
  }
}
void Btn_L2(bool g) {
  if(g) {
    if(BtnLR[2]) {
      servo[4].write(90);
      BtnLR[2] = false;
    } else {
      servo[4].write(130);
      BtnLR[2] = true;
    } 
  } else {
    if(BtnLR[6]) {
      servo[5].write(0);
      BtnLR[6] = false;
    } else {
      servo[5].write(100);
      BtnLR[6] = true;
    } 
  }
}
void Btn_R2(bool g) {
  if(g) {
    if(BtnLR[3]) {
      servo[6].write(90);
      BtnLR[3] = false;
    } else {
      servo[6].write(130);
      BtnLR[3] = true;
    } 
  } else {
    if(BtnLR[7]) {
      servo[7].write(0);
      BtnLR[7] = false;
    } else {
      servo[7].write(100);
      BtnLR[7] = true;
    } 
  }
}
