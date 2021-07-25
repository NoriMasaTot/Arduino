#include <M5StickC.h>
float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;
float gyroX = 0.0F;
float gyroY = 0.0F;
float gyroZ = 0.0F;
float pitch = 0.0F;
float roll  = 0.0F;
float yaw   = 0.0F;
int mode = -1;
void setup() {
  M5.begin();
  M5.IMU.Init();
  M5.Lcd.setTextSize(2);
  M5.Lcd.setRotation(1);
}
void loop() {
  M5.update();
  // データ取得
  M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);
  M5.IMU.getAccelData(&accX, &accY, &accZ);
  M5.IMU.getAhrsData(&pitch, &roll, &yaw);
  // モードチェンジ
  if ( mode == -1 || M5.BtnA.wasReleased() ) {
    mode++;
    mode = mode % 3;
    // プロッタ用のタイトル出力
    M5.Lcd.setCursor(0, 0);
    if ( mode == 0 ) {
      M5.Lcd.printf("Gyro");
    } else if ( mode == 1 ) {
      M5.Lcd.printf("Acc");
    } else if ( mode == 2 ) {
      M5.Lcd.printf("Pitch");
    }
  }
  // データ出力
  M5.Lcd.setCursor(0, 30);
  if ( mode == 0 ) {
    M5.Lcd.printf("X:%.2f\nY:%.2f\nZ:%.2f", gyroX, gyroY, gyroZ);
  } else if ( mode == 1 ) {
    M5.Lcd.printf("X:%.2f\nY:%.2f\nZ:%.2f", accX, accY, accZ);
  } else if ( mode == 2 ) {
    M5.Lcd.printf("X:%.2f\nY:%.2f\nZ:%.2f", pitch, roll, yaw);
  }
  delay(500);
}
