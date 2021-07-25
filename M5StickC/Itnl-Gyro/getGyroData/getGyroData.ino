#include <M5StickC.h>

float gyroX, gyroY, gyroZ;  // ジャイロデータ取得　→回転(左90 0/-50/0, 右90 0/50/0)
float ax, ay, az;  // 加速度データ取得　→傾き(X/Y/Z: 正 0/0/1, 左90 1/0/0, 右90 -1/0/0, 上90 0/-1/0, 下90 0/1/0)

void setup() {
  //Serial.begin(115200);
  M5.begin();
  M5.MPU6886.Init();
}
void loop() {
  M5.MPU6886.getGyroData(&gyroX, &gyroY, &gyroZ);
  //M5.MPU6886.getAccelData(&ax, &ay, &az);
  M5.Lcd.setCursor(0, 30);

  M5.Lcd.printf("X:%7.2f\n\nY:%7.2f\n\nZ:%7.2f", gyroX * M5.MPU6886.gRes, gyroY * M5.MPU6886.gRes, gyroZ * M5.MPU6886.gRes);
  //Serial.printf("%7.2f,%7.2f,%7.2f\n", gyroX * M5.MPU6886.gRes, gyroY * M5.MPU6886.gRes, gyroZ * M5.MPU6886.gRes);
  //M5.Lcd.printf("X:%7.2f\nY:%7.2f\nZ:%7.2f", ax , ay , az );
  //Serial.printf("%7.2f,%7.2f,%7.2f\n", ax , ay , az);

  delay(500);
}
