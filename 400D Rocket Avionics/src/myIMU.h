#pragma once

#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20649.h>

const int IMU_FREQ = 100;                      // proposed frequency at which we are logging data
const int SAMP_SIZE = 3 * IMU_FREQ;          // duration of averager * frequency in Hz
const int REC_SIZE = 2 * IMU_FREQ;          // duration of averager * frequency in Hz
const int BUF_SIZE = 2 * IMU_FREQ;

class myIMU : public Adafruit_ICM20649
{
    public:
        using Adafruit_ICM20649::Adafruit_ICM20649; // credit to Brandon Summers

        bool start(int ICM_CS, uint8_t accelRate, uint8_t gyroRate);
        bool getData();
        bool resetDataFlag();
        void updateBuffers();

        void setAccelFrequency(int desiredRate);
        void setGyroFrequency(int desiredRate);
        void setAccelRange(int desiredRange);
        void setGyroRange(int desiredRange);

        float getTemp(int index);
        float getAccelX(int index);
        float getAccelY(int index);
        float getAccelZ(int index);
        float getGyroX(int index);
        float getGyroY(int index);
        float getGyroZ(int index);
        uint32_t getTime(int index);
        
    private:
        elapsedMicros timeSinceDataRead;       
        elapsedMicros trueTime;
        bool dataFlag;
        uint8_t highestSensorRate;

        float accX_buffer[BUF_SIZE] = {0.0};
        float accY_buffer[BUF_SIZE] = {0.0};
        float accZ_buffer[BUF_SIZE] = {0.0};

        float gyroX_buffer[BUF_SIZE] = {0.0};
        float gyroY_buffer[BUF_SIZE] = {0.0};
        float gyroZ_buffer[BUF_SIZE] = {0.0};
    
        float temp_buffer[BUF_SIZE] = {0.0};

        uint32_t timeMicros_buffer[BUF_SIZE] = {0};
        
        sensors_event_t accel;                
        sensors_event_t gyro;
        sensors_event_t temp;
};



