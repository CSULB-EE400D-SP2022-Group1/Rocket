#pragma once

#include <Adafruit_BME280.h>
#include <Arduino.h>

#define SEALEVELPRESSURE_HPA (baseline)

const int FREQUENCY = 33;                       // proposed frequency at which we are logging data
const int SAMPLE_SIZE = 3 * FREQUENCY;          // duration of averager * frequency in Hz
const int RECENT_SIZE = 2 * FREQUENCY;          // duration of averager * frequency in Hz

class myBME : public Adafruit_BME280
{
    public:
        myBME(Adafruit_BME280 bme, float seaLv = 1013.25)
            : mybme{bme}, baseline{seaLv}, pastMillis{0} {}
        
        void start();
        void getData();
        double getTemp();
        double getAltitude();
        double getHumidity();

        void averagerInit(int rate);
        void fillAvg();
        int getAvg();
        int getAvgRecent();

        void setFrequency(int rate);
        

    private:
        Adafruit_BME280 mybme;              // Adafruit BME object
        float baseline;                     // baseline for ground lv pressure
        unsigned long pastMillis;           
        unsigned long currentMillis;    
        double temp, altitude, humidity;
        int frequency; 
        
        double alt_data[SAMPLE_SIZE] = {0.0};
        
        String data;
};
