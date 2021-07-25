#include <M5StickC.h>

float roll, pitch, yaw;
long tick = 0;

void setup() {
  M5.begin();
  M5.MPU6886.Init();
  M5.Lcd.setRotation(1);
}

void loop() {
  M5.MPU6886.getAhrsData(&pitch, &roll, &yaw);


  //100回に1回だけ描画
  tick++;
  if(tick % 100 == 0){
    tick = 0;
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.print("Deg\n");
    M5.Lcd.printf("X:%5.1f\nY:%5.1f\nZ:%5.1f\n",pitch,roll,yaw);
  }
  delay(2);
}
