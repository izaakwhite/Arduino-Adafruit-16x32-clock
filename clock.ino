/**
* Authour: Izaak White
* Purpose: Makes A clock display to an adafruit 16x32 matrix
* planning to implement a way to make user press a button
* to change settings
* 
* Materials used: Arduino Mega, DS3231, Button, 10 kOhm resistor
*                 Adafruit protomatter shield
*/
#include <RGBmatrixPanel.h>
#include <string.h>

#define CLK  11  
#define OE   9
#define LAT 10
#define A   A0
#define B   A1
#define C   A2

RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

struct DateTime {
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t trueHour;
  uint8_t minute;
  uint8_t second;
  String date;

  DateTime(uint8_t m, uint8_t d, uint8_t h, uint8_t th, uint8_t min, uint8_t sec, const String& dt)
      : month(m), day(d), hour(h), trueHour(th), minute(min), second(sec), date(dt) {}
};


String getTime(const uint8_t &hour,const uint8_t &minute,const uint8_t &second){
  //00:00:00
  String str = "";
  if(hour<10)
    str += "0";
  str += String(hour);
  str += ":";
  if(minute<10)
    str += "0";
  str += String(minute);
  str += ":";
  if(second<10)
    str += "0";
  str += String(second);
  
  return str;
  
}

String getTime(const uint8_t &hour,const uint8_t &minute){
  //00:00
  String str = "";
  if(hour<10)
    str += "0";
  str += String(hour);
  str += ":";
  if(minute<10)
    str += "0";
  str += String(minute);

  return str;
}

String updateDate(uint8_t &day, uint8_t &month){
    if(day==28 & month==2){
        day = 1;
        month++;
    }
    else if(day==30 & (month==4||month==6||month==9||month==11)){
        day = 1;
        month++;
    }
    else if(day==31 & month==12){
        day=1;
        month=1;
    }
    else if(day == 31 & (month==1||month==3||month==5||month==7||month==8||month==10)){
        day = 1;
        month++;
    }
    else{
        day++;
    }
    String Offset = "";
    if(day<10)
      Offset = "0";
    return String(month) + "/" + Offset + String(day);
}

DateTime today(2, 5, 12, 24, 59, 30, "");

void setup() {
  //NeedDS3231 for RTC can update struct from this
  matrix.begin();
}

void loop() {
  uint64_t t1 = millis(); //Used to track millisecond delays as the arduino takes time to proccess instructions
  today.second++;
  if(today.second==60){
    today.second = 0;
    today.minute++;
  }
  if(today.minute==60){
    today.minute = 0;
    today.hour++;
    today.trueHour++;
  }
  if(today.hour==13){
    today.hour = 1;
  }
  if(today.trueHour==25){
    today.date=updateDate(today.day,today.month);
  }
  String second_accurate_time=getTime(today.hour,today.minute,today.second);
  String time_and_date = getTime(today.hour, today.minute) + " " + today.date;
  String military_time = getTime(today.trueHour, today.minute);
  String minute_accurate_time = getTime(today.hour, today.minute);
  matrix.setCursor(0, 0);  
  matrix.setTextSize(1);
  matrix.setTextColor(matrix.Color333(7,7,7));
  matrix.print(second_accurate_time);
  //depending on button state I plan on implementing, second accurate & date accurate time
  //as well as military time 
  uint64_t t2  = millis();
  delay(1000-(t2-t1));   
  matrix.fillScreen(matrix.Color333(0,0,0));
}
