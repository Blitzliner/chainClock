#include "Stepper.h"
#include "Time.h"

#define PIN_STEP 2
#define PIN_DIR 3
#define PIN_ENABLE 4
#define STEPS_PER_REVOLUTION (63.68395 * 32.0)  // here go the 2048 steps
#define CHAIN_MODULES 48*2
#define PITCH 8
#define MOTOR_PINION_TOOTHS 11
#define MOTOR_PINION_PERIMETER (PITCH * MOTOR_PINION_TOOTHS) //(PITCH * MOTOR_PINION_TOOTHS) / PI)
#define CHAIN_LENGTH (CHAIN_MODULES * PITCH)
#define STEPS_PER_SECOND ((float)CHAIN_LENGTH / (12.0*60.0*60.0))
#define DIST_PER_STEP ((float)MOTOR_PINION_PERIMETER / (float)STEPS_PER_REVOLUTION)
#define DEBUG(func) func
//#define DEBUG(func) 

Stepper stepper(PIN_STEP, PIN_DIR, 10);
static TimeType time;
float currentDistance = 0.0, targetDistance;

void printInfos(void) {
  Serial.begin(38400); 
  Serial.print("dist per step: ");
  Serial.println(DIST_PER_STEP, 6);

  Serial.print("steps per sec: ");
  Serial.println(STEPS_PER_SECOND, 6);
}

void setup() {
  DEBUG(printInfos());
  
  (void)memset(&time, 0U, sizeof(TimeType));
  
  pinMode(PIN_ENABLE, OUTPUT);  
  digitalWrite(PIN_ENABLE, LOW);   
  
  stepper.Init();
  stepper.SetDirection(true);
  InitTime(time);
}


void loop() {  
  uint16_t currentTimeStamp, maxTimeStamp;
  delay(2000); /* sleep for a second */
  GetTime(&time);
  DEBUG(PrintTime(&time));

  currentTimeStamp = getTimeStamp(time.hour, time.minute, time.second);
  targetDistance = currentTimeStamp * STEPS_PER_SECOND;
  
  maxTimeStamp = getTimeStamp(23, 59, 59);
  
  /* prevent overflow so subtract a day distance */
  if (currentTimeStamp == maxTimeStamp) {     
    currentDistance -= DIST_PER_STEP * maxTimeStamp;
    targetDistance -= DIST_PER_STEP * maxTimeStamp; 
  }
  
  DEBUG(printStatus( currentDistance, targetDistance));

  while (currentDistance < targetDistance) {
    digitalWrite(PIN_ENABLE, LOW);   
    (void)stepper.DoStep();  
  
    currentDistance += DIST_PER_STEP;
    digitalWrite(PIN_ENABLE, HIGH); 
  }
}

void printStatus( float currentDistance, float targetDistance) {
  char totalBuff[30];
  char tempBuff[10];
  char tempBuffTarget[10];
  dtostrf(currentDistance, 5, 3, tempBuff);
  dtostrf(targetDistance, 5, 3, tempBuffTarget);
  sprintf(totalBuff, "%s/%s\n", tempBuff, tempBuffTarget);
  Serial.print(totalBuff);
}

uint32_t getTimeStamp(uint8_t hour, uint8_t minute, uint8_t second) {
  return (hour * 3600 + minute * 60 + second);
}

