#pragma once

#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20649.h>

const int IMU_FREQ = 1099;                          // sampling rate of accelerometer AND gyrometer 
const int IMU_LOGFREQ = 100;                        // proposed frequency at which we are logging data
const int BUF_SIZE = 2 * IMU_LOGFREQ;               // duration of buffer (s) * logging frequency (hz)

class myIMU : public Adafruit_ICM20649
{
    public:
        using Adafruit_ICM20649::Adafruit_ICM20649; // credit to Brandon Summers

        bool start(int ICM_CS);
        bool getData();
        bool resetDataFlag();
        void updateBuffers();

        void setAccelFrequency(int desiredRate);
        void setGyroFrequency(int desiredRate);
        void setAccelRange(int desiredRange);
        void setGyroRange(int desiredRange);

<<<<<<< Updated upstream
<<<<<<< Updated upstream
        float getTemp();
        float getAccelX();
        float getAccelY();
        float getAccelZ();
        float getGyroX();
        float getGyroY();
        float getGyroZ();
        
    private:
        elapsedMicros timeSinceDataRead;       
=======
        float getTemp(int i);
        float getAccelX(int i);
        float getAccelY(int i);
        float getAccelZ(int i);
        float getGyroX(int i);
        float getGyroY(int i);
        float getGyroZ(int i);
        uint32_t getTime(int i);
        
    private:
=======
        float getTemp(int i);
        float getAccelX(int i);
        float getAccelY(int i);
        float getAccelZ(int i);
        float getGyroX(int i);
        float getGyroY(int i);
        float getGyroZ(int i);
        uint32_t getTime(int i);
        
    private:
>>>>>>> Stashed changes
        elapsedMicros timeSinceDataRead; 
        elapsedMicros timeSinceBufferUpdate;      
        elapsedMicros trueTime;
>>>>>>> Stashed changes
        bool dataFlag;

        float accX_buffer[BUF_SIZE] = {0.0};
        float accY_buffer[BUF_SIZE] = {0.0};
        float accZ_buffer[BUF_SIZE] = {0.0};

        float gyroX_buffer[BUF_SIZE] = {0.0};
        float gyroY_buffer[BUF_SIZE] = {0.0};
        float gyroZ_buffer[BUF_SIZE] = {0.0};

<<<<<<< Updated upstream
<<<<<<< Updated upstream
=======
=======
>>>>>>> Stashed changes
        uint32_t timeMicros_buffer[BUF_SIZE] = {0};
        
>>>>>>> Stashed changes
        sensors_event_t accel;                
        sensors_event_t gyro;
        sensors_event_t temp;
};


