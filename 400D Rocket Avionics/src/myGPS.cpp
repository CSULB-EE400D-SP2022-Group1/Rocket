#include "myGPS.h"

/*!
    @brief initializes software serial with Baud Rate of 9600
*/
void myGPS::start() {
    ss.begin(9600);
    dataFlag = 0;
}

boolean myGPS::getData() {
    while (ss.available() > 0)
    {
        encode(ss.read());
    }

    if (timeSinceDataRead >= 1000 && charsProcessed() < 10)
    {
        Serial.println(F("No GPS detected: check wiring."));
        timeSinceDataRead -= 1000;
    }

    dataFlag = 1;
    return dataFlag;
}

boolean myGPS::resetDataFlag() {
    dataFlag = 0;
    return dataFlag;
}

uint32_t myGPS::numSatellites() {
    return satellites.value();
}

float myGPS::getLang() {
    getData();
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