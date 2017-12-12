#include <DS3231.h>

#define debug Serial.println

DS3231 rtc;

byte timeBuffer[12];
char time[12];

void setup() {
    Serial.begin(9600);
    rtc.begin();

    // Set time for the clock
    if (rtc.parseDateTime("12/12/2017 9:48:0", timeBuffer)) {
        rtc.setDateTime(timeBuffer);
        debug("DS3231 set to: ");
        char* setTime = rtc.timeToString(timeBuffer, time);
        debug(time);
    }
    else {
        debug("Error!");
    }
}

void loop() {
    debug("__________\n\n");
    rtc.getDateTime(timeBuffer);
    char day[4];
    strcpy(day, rtc.dayOfWeekToString(timeBuffer, time));
    char date[12];
    strcpy(date, rtc.dateToString(timeBuffer, time));
    char currentTime[12];
    strcpy(currentTime, rtc.timeToString(timeBuffer, time));
    debug("Day: ");
    debug(day);
    debug("Date: ");
    debug(date);
    debug("current time: ");
    debug(currentTime);
    delay(8000);
}
