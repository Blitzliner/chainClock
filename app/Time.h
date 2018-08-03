#ifndef TIME_H  
#define TIME_H

#include "DS3231.h"

/*************************************************/
/*         LOCAL VARIABLE DECLARATION            */
/*************************************************/
DS3231 Clock;

/*************************************************/
/*              TYPE DECLARATION                 */
/*************************************************/
typedef struct TimeType_Tag {
  byte second;
  byte minute;
  byte hour;
  byte day;
  byte month;
  byte year;
} TimeType;

/*************************************************/
/*         LOCAL FUNCTION DECLARATION            */
/*************************************************/
static void PrintTime(TimeType* t);
static void parseCompileTime(TimeType* t);

/*************************************************/
/*         GLOBAL FUNCTION DECLARATION           */
/*************************************************/
void InitTime(TimeType& time);
void SetTime(TimeType* t);
void GetTime(TimeType *t);

/*************************************************/
/*         LOCAL FUNCTION DEFINITIONS            */
/*************************************************/
void PrintTime(TimeType* t) { 
    char totalBuff[30];
    //sprintf(totalBuff, "%02d:%02d:%02d, %02d.%02d.%04d\n", t->hour, t->minute, t->second, t->day, t->month, t->year + 2000);
    sprintf(totalBuff, "%02d:%02d:%02d\n", t->hour, t->minute, t->second);
    Serial.print(totalBuff);
}

void parseCompileTime(TimeType* t) {
    char s_month[5];
    const char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
    uint16_t year;
    sscanf(__DATE__, "%s %c %d", s_month, &t->day, &year);
    t->year = (char)(year - 2000);
    t->month = (strstr(month_names, s_month) - month_names) / 3 + 1;
    Serial.println((strstr(month_names, s_month) - month_names));
    sscanf(__TIME__, "%2hhd %*c %2hhd %*c %2hhd", &t->hour, &t->minute, &t->second);
}
/*************************************************/
/*         GLOBAL FUNCTION DEFINITIONS            */
/*************************************************/
void InitTime(TimeType& time) {
    Wire.begin();
    //parseCompileTime(&time);
    SetTime(&time);
    PrintTime(&time);
}

void SetTime(TimeType* t) {
  Clock.setSecond(t->second);//Set the second 
  Clock.setMinute(t->minute);//Set the minute 
  Clock.setHour(t->hour);  //Set the hour 
  Clock.setDate(t->day);  //Set the day of the month
  Clock.setMonth(t->month);  //Set the month of the year
  Clock.setYear(t->year);// - 2000);  //Set the year (Last two digits of the year)
}

void GetTime(TimeType *t) {
  bool h12, PM, Century;
  t->hour = Clock.getHour(h12, PM); 
  t->minute = Clock.getMinute(); 
  t->second = Clock.getSecond();
  t->year = Clock.getYear();
  t->month = Clock.getMonth(Century); 
  t->day = Clock.getDate();
}

#endif
