#include "Stepper.h"
#include "Time.h"

#define STEPS_PER_REVOLUTION (63.68395 * 32.0)  // here go the 2048 steps
Stepper stepper(2, 3, 10);
static TimeType time;
#define CHAIN_MODULES 48*2
#define PITCH 8
#define MOTOR_PINION_TOOTHS 11
#define MOTOR_PINION_PERIMETER (PITCH * MOTOR_PINION_TOOTHS) //(PITCH * MOTOR_PINION_TOOTHS) / PI)
#define CHAIN_LENGTH (CHAIN_MODULES * PITCH)
#define STEPS_PER_SECOND ((float)CHAIN_LENGTH / (12.0*60.0*60.0))
#define DIST_PER_STEP ((float)MOTOR_PINION_PERIMETER / (float)STEPS_PER_REVOLUTION)
//#define DEBUG
float currentDistance = 0.0, targetDistance;
uint32_t doneSteps_u32;

void setup() {
#ifdef DEBUG
  Serial.begin(38400); 
  Serial.print("dist per step: ");
  Serial.println(DIST_PER_STEP, 6);

  Serial.print("steps per sec: ");
  Serial.println(STEPS_PER_SECOND, 6);
#endif
  stepper.Init();
  time.hour = 0;
  time.minute = 0;
  time.second = 0;
  InitTime(time);        /* build up connection to rtc */
}


void loop() {  
  uint16_t currentTimeStamp;
  const float DistanceOfDay_f32 = getTimeStamp(24, 60, 60) * DIST_PER_STEP;
  const float MinDistanceForACycle_f32 = getTimeStamp(23, 59, 58) * DIST_PER_STEP;
  delay(1000); /* sleep for a second */
  GetTime(&time);
  
#ifdef DEBUG
  PrintTime(&time);
#endif

  currentTimeStamp = getTimeStamp(time.hour, time.minute, time.second);
  targetDistance = currentTimeStamp * STEPS_PER_SECOND;

  /* detect change between 23:59:59 to 00:00:00 but not after powercycle */
  if (   (currentTimeStamp == 0) /* 00:00:00 is detected */
      && (currentDistance > MinDistanceForACycle_f32)) {     
    currentDistance -= DistanceOfDay_f32; /* subtract distance of a day */
  }
  
#ifdef DEBUG
  printStatus(doneSteps_u32, currentDistance, targetDistance);
#endif

  while (currentDistance < targetDistance) {
    //doneSteps_u32 = stepper.DoStep();
    (void)stepper.DoStep();
    currentDistance += DIST_PER_STEP;
  }
}

void printStatus(uint32_t doneSteps_u32, float currentDistance, float targetDistance) {
  char totalBuff[30];
  char tempBuff[10];
  char tempBuffTarget[10];
  dtostrf(currentDistance, 5, 3, tempBuff);
  dtostrf(targetDistance, 5, 3, tempBuffTarget);
  sprintf(totalBuff, "%6lu: %s/%s\n", doneSteps_u32, tempBuff, tempBuffTarget);
  Serial.print(totalBuff);
}

uint32_t getTimeStamp(uint8_t hour, uint8_t minute, uint8_t second) {
  return (hour * 3600 + minute * 60 + second);
}

