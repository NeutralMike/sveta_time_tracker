#include <AnalogKey.h>
#include <GyverButton.h>
#include "GyverTM1637.h"
#include "GyverPower.h"
#define CLK 2
#define DIO 3


GButton workBTN(11);
GButton breakBTN(12);
GButton restBTN(13);
GyverTM1637 disp(CLK, DIO);

uint32_t workTime[] = {0, 0};
uint32_t restTime[] = {0, 0};
uint32_t breakStart = 0;
uint32_t breakDuration = 600000;
uint32_t restDuration = 14400000;
int mode = 0;

void setup() {
  workBTN.setTickMode(AUTO);
  breakBTN.setTickMode(AUTO); 
  restBTN.setTickMode(AUTO);
  disp.clear();
  disp.brightness(1);
  power.setSleepMode(IDLE_SLEEP);
  attachInterrupt(0, isr, FALLING);
}

void isr() {
}

void loop() {  
  switch (mode) {
    case 1: // work
      workTime[0] = workTime[0] + millis() - workTime[1];
      workTime[1] = millis();
      displayTime(workTime[0]);
      break;
    case 2: // takeBreak
      restTime[0] = restTime[0] + (millis() - restTime[1]);
      restTime[1] = millis();
      displayTimeSeconds(breakDuration - (millis() - breakStart));
      break;
    case 3: // rest
      restTime[0] = restTime[0] + (millis() - restTime[1]);
      restTime[1] = millis();
      displayTime(restDuration - restTime[0]); 
      break;
  }
  if (workBTN.isHolded()){
    mode = 0;
    workTime[0] = 0;
    restTime[0] = 0;
    breakStart = 0;
    disp.clear();
    disp.point(0);
    power.sleep(SLEEP_FOREVER);
  }
  if (workBTN.isClick())
  {
    mode = 1;
    workTime[1] = millis();  
  }
  
  if (breakBTN.isClick()){
    mode = 2;
    breakStart = millis();
    restTime[1] = millis();
  }
  if (restBTN.isClick()){
    mode = 3;
    restTime[1] = millis();
  }
}


void displayTime(uint32_t milliseconds){
    disp.point(true);
    int secs = milliseconds / 1000;
    int mins = secs / 60;
    int hrs = mins / 60;
    mins = mins % 60;
    disp.displayClock(hrs, mins);
}

void displayTimeSeconds(uint32_t milliseconds){
    disp.point(true);
    int secs = milliseconds / 1000 ;
    int mins = (secs / 60) % 60;
    secs = secs % 60;
    disp.displayClock(mins, secs); 
}
