#pragma once

#include <Adafruit_BME280.h>
#include <Arduino.h>

const int ABOVE_BASELINE = 200;                         // meters above baseline for the rocket to be considered launched
<<<<<<< Updated upstream
<<<<<<< Updated upstream
static const int FREQUENCY = 33;                        // proposed frequency at which we are logging data
static const int SAMPLE_SIZE = 3 * FREQUENCY;           // duration for averaging * frequency in Hz
static const int RECENT_SIZE = 2 * FREQUENCY;           // duration for averaging * frequency in Hz
static const int BASELINE_SIZE = 2 *FREQUENCY;          // duration for averaging * frequency in Hz
=======
=======
>>>>>>> Stashed changes
const int BME_FREQ = 100;                               // frequency at which BME is reading data
static const int BME_LOGFREQ = 30;                    // proposed frequency at which we are logging data
static const int SAMPLE_SIZE = 3 * BME_LOGFREQ;           // duration for averaging * frequency in Hz
static const int RECENT_SIZE = 1 * BME_LOGFREQ;           // duration for averaging * frequency in Hz
static const int BASELINE_SIZE = 2 *BME_LOGFREQ;          // duration for averaging * frequency in Hz
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes

class myBME : public Adafruit_BME280

{
    public:
        using Adafruit_BME280::Adafruit_BME280; // credit to Brandon Summers

        bool start();
        bool getData();
        bool resetDataFlag();

<<<<<<< Updated upstream
<<<<<<< Updated upstream
        float getAltitude();
        float getTemp();
        float getHumidity();
=======
=======
>>>>>>> Stashed changes
        float getAltitude(int i);
        float getTemp(int i);
        float getHumidity(int i);
        uint32_t getTime(int i);
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
        
        void updateBuffers();
        int getAvg();
        int getAvgRecent();
        float getRecentData();

        void setFrequency(int rate);
        bool detectLaunch();
        

    private:
        // automatically keeps track of time elapsed in microseconds
        elapsedMicros timeSinceDataRead;       
<<<<<<< Updated upstream
=======
        elapsedMicros trueTime;
        elapsedMicros timeSinceBufferUpdate;
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes

        float temp, altitude, humidity;
        int recentAverage;
        int frequency; 
        
<<<<<<< Updated upstream
        float altitude_buffer[SAMPLE_SIZE] = {0.0}; // buffer for altitude
        float temp_buffer[SAMPLE_SIZE] = {0.0};     // buffer for temp
        float humidity_buffer[SAMPLE_SIZE] = {0.0}; // buffer for humidity  
        
=======
        float altitude_buffer[SAMPLE_SIZE] = {0.0};         // buffer for altitude (m)
        float temp_buffer[SAMPLE_SIZE] = {0.0};             // buffer for temp (deg C)
        float humidity_buffer[SAMPLE_SIZE] = {0.0};         // buffer for humidity  (%)
        uint32_t timeMicros_buffer[SAMPLE_SIZE] = {0};      // buffer for time (us)

>>>>>>> Stashed changes
        float baselinePressure;                     // baseline for ground lv pressure
        float baselineAltitude;                     // baseline for ground lv altitude

        bool launchDetect{0};                    // flag for launch detect, init at 0
        bool newDataDetect{0};                   // flag for new data detect, init at 0

};