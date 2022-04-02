#pragma once

#include <Adafruit_BME280.h>
#include <Arduino.h>

const int ABOVE_BASELINE = 200;                         // meters above baseline for the rocket to be considered launched
static const int FREQUENCY = 33;                        // proposed frequency at which we are logging data
static const int SAMPLE_SIZE = 3 * FREQUENCY;           // duration for averaging * frequency in Hz
static const int RECENT_SIZE = 2 * FREQUENCY;           // duration for averaging * frequency in Hz
static const int BASELINE_SIZE = 2 *FREQUENCY;          // duration for averaging * frequency in Hz

class myBME : public Adafruit_BME280

{
    public:
        using Adafruit_BME280::Adafruit_BME280; // credit to Brandon Summers

        boolean start(int rate);
        boolean getData();
        boolean resetDataFlag();

        void averagerInit(int rate);
        void updateBuffers();
        int getAvg();
        int getAvgRecent();
        float getRecentData();

        float getAltitude();
        float getTemp();
        float getHumidity();

        void setFrequency(int rate);
        boolean detectLaunch();
        

    private:
        // automatically keeps track of time elapsed in microseconds
        elapsedMicros timeSinceDataRead;       

        float temp, altitude, humidity;
        int frequency; 
        
        float altitude_buffer[SAMPLE_SIZE] = {0.0}; // buffer for altitude
        float temp_buffer[SAMPLE_SIZE] = {0.0};     // buffer for temp
        float humidity_buffer[SAMPLE_SIZE] = {0.0}; // buffer for humidity  
        
        float baselinePressure;                     // baseline for ground lv pressure
        float baselineAltitude;                     // baseline for ground lv altitude

        boolean launchDetect{0};                    // flag for launch detect, init at 0
        boolean newDataDetect{0};                   // flag for new data detect, init at 0

};