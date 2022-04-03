#include "myGPS.h"

boolean myGPS::start() {
    SoftwareSerial ss(RXPin, TXPin);
}

void myGPS::getData() {
    while (ss.available() > 0)
        encode(ss.read());
}

uint32_t myGPS::numSatellites() {
    return satellites.value();
}

float myGPS::getLang() {
    return location.lng();
}
float  myGPS::getLat() {
    return location.lat();
}
uint8_t  myGPS::getDay() {
    return date.day();
}
uint8_t  myGPS::getMonth() {
    return date.month();
}
uint16_t  myGPS::getYear() {
    return date.year();
}
uint8_t  myGPS::getHour() {
    return time.hour();
}
uint8_t  myGPS::getMinute() {
    return time.minute();
}
uint8_t  myGPS::getSecond() {
    return time.second();
}
float  myGPS::getSpeed() {
    return speed.mph(); 
}
float  myGPS::getGPSAltitude() {
    return altitude.meters();
}
float  myGPS::getHDOP() {
    return hdop.hdop();
}