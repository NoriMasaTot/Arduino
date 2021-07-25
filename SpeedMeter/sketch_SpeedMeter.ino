#include <TimerOne.h>
#include <Wire.h>
#include <MultiFuncShield.h>

int cnt = 0;
int phase = 0;
unsigned long restart_wait = 0;

void flash() {
  cnt++;

  // Auto Clear
  if (phase == 2)
  {
    if (restart_wait == 0)
    {
      phase = 0;
      MFS.write((float)0.0);
    }
    else
    {
      restart_wait--;
    }
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("ProgramStart");

  Timer1.initialize();
  MFS.initialize(&Timer1);    // initialize multi-function shield library
  MFS.write((float)0.0);
}

void loop() {
  byte buttonNumber = 0;
  byte buttonAction = 0;

  MFS.manualButtonHandler();
  byte btn = MFS.getButton();
  MFS.userInterrupt = &flash;

  if (btn)
  {
    buttonNumber = btn & B00111111;
    buttonAction = btn & B11000000;

    if ((buttonNumber == 1) && (buttonAction == BUTTON_PRESSED_IND) && (phase == 0))
    //if (!bitRead(PINC, 1) && (phase == 0))
    {
      phase = 1;
      cnt = 0;
      MFS.beep(1, 1, 1);  // 100 milliseconds on / 100 off beep 1 times
      //Serial.println("Start");
    }

    if ((buttonNumber == 2) && (buttonAction == BUTTON_PRESSED_IND) && (phase == 1))
    //if (!bitRead(PINC, 2) && (phase == 1))
    {
      MFS.userInterrupt = NULL;
      phase = 2;

      float times = cnt;
      Serial.println(cnt);
      float spd = 3600 / times; 
      MFS.write(spd);
      Serial.println(spd);
      MFS.beep(1, 1, 1);  // 100 milliseconds on / 100 off beep 1 times

      restart_wait = 10000;  // Restart wait
    }

    // Manual Clear
    if ((buttonNumber == 3) && (buttonAction == BUTTON_PRESSED_IND) && (phase == 2))
    //if (!bitRead(PINC, 3) && (phase == 2))
    {
      phase = 0;
      MFS.write((float)0.0);
    }
  }
}
