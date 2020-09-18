const int jumlahSensor = 4;
const int pinSensor[jumlahSensor] = {8,9,10,11};
const int pinMotor[] = {4,5,6,7};

float val[4] = {},
      sp = 0,
      ts = 0,
      lastLastError = 0,
      error = 0,
      lastError = 0;
      
const float kp = 0.5,
            ki = 2,
            kd = 0.1;
      

void setup() {
  Serial.begin(9600);
  for(int i = 0; i < jumlahSensor; i++) {
    pinMode(pinSensor[i], INPUT);
  }
  for(int i = 0; i < 4; i++) {
    pinMode(pinMotor[i], OUTPUT);
  }
}

void loop() {
  boolean isWhite = true;
  for(int i=0; i < jumlahSensor; i++) {
    float v = digitalRead(pinSensor[i]);
    if(i < (jumlahSensor / 2)) { 
      if(v) val[i] = -1 * 1;
      else val[i] = 0;
    } else {
      if(v) val[i] = 1;
      else val[i] = 0;
    }
    error = error + val[i];
    if(v) isWhite = false;
  }
  lastError = error;
  error = pid(0.5,2,0.1, error, lastError);
  int motorR = 75-map(error,0,1, 0, 25);
  int motorL = 75+map(error,0,1, 0, 25);
  if(isWhite) {  
    motorR = 75-map(lastLastError,0,1, 0, 50);
    motorL = 75+map(lastLastError,0,1, 0, 50);
  } else {
    lastLastError = error;
  }
  analogWrite(pinMotor[0], 0);
  analogWrite(pinMotor[3], 0);
  analogWrite(pinMotor[1], motorR);
  analogWrite(pinMotor[2], motorL);
  delayMicroseconds(125);
}

void debug(int motorR, int motorL) {
  Serial.print("\t");
  Serial.print(motorR);
  Serial.print("\t");
  Serial.print(motorL);
  Serial.print("\t");
  Serial.print(error);
  Serial.println();
  delay(100);
}

float pid(float _kp, float _ki, float _kd, float _err, float _lastErr) {
  ts = 0.00000000625;
  _err = sp - _err;
  int p = _kp * error;
  int i = _ki * ((_err + _lastErr) / 2) * ts;
  int d = _kd * (error - _lastErr) / ts;
  return p + i + d;
}

