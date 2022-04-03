#pragma once

#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

static const int RXPin = 0, TXPin = 1;
static const uint32_t GPSBaud = 9600;

class myGPS : public TinyGPSPlus
{
    public:
        using TinyGPSPlus::TinyGPSPlus; // credit to Brandon Summers

        boolean start();
        void getData();

        uint32_t numSatellites();
        float getLang();
        float getLat();
        uint8_t getDay();
        uint8_t getMonth();
        uint16_t getYear();
        uint8_t getHour();
        uint8_t getMinute();
        uint8_t getSecond();
        double getCourse();
        float getSpeed();
        float getGPSAltitude();
        float getHDOP();


    private:
        SoftwareSerial ss;
};