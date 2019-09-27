void tryingConnection() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("OK");
}
float akselerasi(float x, float y, float z) {
  float x_2 = pow(x,2),
          y_2 = pow(y,2),
          z_2 = pow(z,2);
  return sqrt(x_2 + y_2 + z_2);
}
float magnitude(float a) {
  return (log(a) + 1.6 * log(30));
}
void calibrateMagnitude() {
  mZeroPoint = 0;
  for(int i = 0; i < sampleLength; i++){
    double Ax, Ay, Az;
    Read_RawValue(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_XOUT_H);
    
    //divide each with their sensitivity scale factor
    Ax = (double)AccelX/AccelScaleFactor;
    Ay = (double)AccelY/AccelScaleFactor;
    Az = (double)AccelZ/AccelScaleFactor;
    float Accl = akselerasi(Ax, Ay, Az),
          M = magnitude(Accl);
    mZeroPoint += M;
  }
  mZeroPoint = mZeroPoint/sampleLength;
}
