#include "myGPS.h"

/*!
    @brief initializes software serial with Baud Rate of 9600
*/
void myGPS::start() {
    ss.begin(9600);
    dataFlag = 0;
}

/*!
    @brief GPS starts measuring data
    @return 1 (true) if data is successfuly retrieved, 0 otherwise
*/
boolean myGPS::getData() {
    while (ss.available() > 0)
    {
        if (encode(ss.read()))
            dataFlag = 1;
    }

    if (timeSinceDataRead >= 1000 && charsProcessed() < 10)
    {
        Serial.println(F("No GPS detected: check wiring."));
        timeSinceDataRead -= 1000;
    }

    return dataFlag;
}

/*!
    @brief Sets data flag to 0 (false)
    @return data flag (0)
*/
boolean myGPS::resetDataFlag() {
    dataFlag = 0;
    return dataFlag;
}

/*!
    @brief Returns number of satellites connected to GPS
    @return number of satellites
*/
uint32_t myGPS::numSatellites() {
    return satellites.value();
}

/*!
    @brief Returns longtitude
    @return longtitude (deg)
*/
float myGPS::getLng() {
    getData();
    return location.lng();
}

/*!
    @brief Returns latitude
    @return latitude (deg)
*/
float  myGPS::getLat() {
    return location.lat();
}

/*!
    @brief Returns current day of the month
    @return day
*/
uint8_t  myGPS::getDay() {
    return date.day();
}

/*!
    @brief Returns current month
    @return month
*/
uint8_t  myGPS::getMonth() {
    return date.month();
}

/*!
    @brief Returns current year
    @return year
*/
uint16_t  myGPS::getYear() {
    return date.year();
}

/*!
    @brief Returns current hour
    @return hour
*/
uint8_t  myGPS::getHour() {
    return time.hour();
}

/*!
    @brief Returns current minute
    @return minute
*/
uint8_t  myGPS::getMinute() {
    return time.minute();
}

/*!
    @brief Returns second
    @return second
*/
uint8_t  myGPS::getSecond() {
    return time.second();
}

/*!
    @brief Returns measured speed
    @return speed (meters per second)
*/
float  myGPS::getSpeed() {
    return speed.mps(); 
}

/*!
    @brief Returns measured altitude
    @return altitude (Meters)
*/
float  myGPS::getGPSAltitude() {
    return altitude.meters();
}

/*!
    @brief Returns measurement of GPS accuracy (between 1-2 is considered good)
    @return Horizontal Dilution of Precision
*/
float  myGPS::getHDOP() {
    return hdop.hdop();
}

/*!
    @brief Returns time since last GPS update
    @return time in milliseconds
*/
uint32_t myGPS::getAge() {
    return location.age();
}
