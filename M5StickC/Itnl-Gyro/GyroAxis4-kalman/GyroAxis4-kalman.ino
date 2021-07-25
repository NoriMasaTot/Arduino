#include <Kalman.h>
#include <M5StickC.h>
//x, y, zの順
float acc[3];
float accOffset[3];
float gyro[3];
float gyroOffset[3];
float kalAngleX;
float kalAngleY;
float kalAngleZ;
Kalman kalmanX;
Kalman kalmanY;
Kalman kalmanZ;
long lastMs = 0;
long tick = 0;
void setup() {
  M5.begin();
  M5.Lcd.setRotation(1);
  M5.MPU6886.Init();
  calibration();
}

void calibration(){
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("Calibrating...\n");
  delay(500);

  //補正値を求める
  float gyroSum[3];
  float accSum[3];
  int COUNTER = 500;
  for(int i = 0; i < COUNTER; i++){
    readGyro();
    gyroSum[0] += gyro[0];
    gyroSum[1] += gyro[1];
    gyroSum[2] += gyro[2];
    accSum[0] += acc[0];
    accSum[1] += acc[1];
    accSum[2] += acc[2];
    delay(10);
  }
  gyroOffset[0] = gyroSum[0]/COUNTER;
  gyroOffset[1] = gyroSum[1]/COUNTER;
  gyroOffset[2] = gyroSum[2]/COUNTER;
  accOffset[0] = accSum[0]/COUNTER;
  accOffset[1] = accSum[1]/COUNTER;
  accOffset[2] = accSum[2]/COUNTER - 1.0;//重力加速度1G

  M5.Lcd.println("    X       Y       Z\n");
  M5.Lcd.printf("%7.2f %7.2f %7.2f\n", gyroOffset[0], gyroOffset[1], gyroOffset[2]);
  M5.Lcd.printf("%7.2f %7.2f %7.2f\n", accOffset[0]*1000, accOffset[1]*1000, accOffset[2]*1000);
  delay(2000);  
  M5.Lcd.fillScreen(BLACK);

  readGyro();
  kalmanX.setAngle(getRoll());
  kalmanY.setAngle(getPitch());
  kalAngleZ = 0;
  lastMs = micros();
}

void loop() {
  // put your main code here, to run repeatedly:
  readGyro();
  applyCalibration();
  float dt = (micros() - lastMs) / 1000000.0;
  lastMs = micros();
  float roll = getRoll();
  float pitch = getPitch();
  float yaw = getYaw();
  
  kalAngleX = kalmanX.getAngle(roll, gyro[0], dt);
  kalAngleY = kalmanY.getAngle(pitch, gyro[1], dt);
  kalAngleZ += gyro[2] * dt;
  kalAngleZ = fmod(kalAngleZ, 360);
  //kalAngleZ = kalmanZ.getAngle(yaw, gyro[2], dt);

  //100回に1回だけ描画
  tick++;
  if(tick % 100 == 0){
    tick = 0;
    draw();

    if(digitalRead(M5_BUTTON_HOME) == LOW){
      calibration();
    }
  }
  
  delay(2);
}
void draw(){
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print("Deg\n");
  M5.Lcd.printf("X:%7.2f\nY:%7.2f\nZ:%7.2f\n", kalAngleX, kalAngleY, kalAngleZ);

  //M5.Lcd.setTextSize(1);
  //M5.Lcd.setCursor(0, 65);
  //M5.Lcd.printf("%7.2f %7.2f %7.2f", acc[0] * 1000, acc[1] * 1000, acc[2] * 1000);
  //M5.Lcd.printf("%7.2f %7.2f %7.2f", gyro[0], gyro[1], gyro[2]);
}
void readGyro(){
  M5.MPU6886.getGyroData(&gyro[0], &gyro[1], &gyro[2]);
  M5.MPU6886.getAccelData(&acc[0], &acc[1], &acc[2]);
}
void applyCalibration(){
  gyro[0] -= gyroOffset[0];
  gyro[1] -= gyroOffset[1];
  gyro[2] -= gyroOffset[2];
  acc[0] -= accOffset[0];
  acc[1] -= accOffset[1];
  acc[2] -= accOffset[2];
}
float getRoll(){
  return atan2(acc[1], acc[2]) * RAD_TO_DEG;
}
float getPitch(){
  return atan(-acc[0] / sqrt(acc[1]*acc[1] + acc[2]*acc[2])) * RAD_TO_DEG;
}
float getYaw(){
  return atan(acc[1] / acc[0]) * RAD_TO_DEG;
}
