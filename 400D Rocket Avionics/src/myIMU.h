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

        bool start(int ICM_CS, int accelRate, int gyroRate);
        void getData();
        void updateBuffers();

        void setAccelFrequency(int desiredRate);
        void setGyroFrequency(int desiredRate);
        void setAccelRange(int desiredRange);
        void setGyroRange(int desiredRange);

        float getTemp();
        float getAccelX();
        float getAccelY();
        float getAccelZ();
        float getGyroX();
        float getGyroY();
        float getGyroZ();
        
    private:
        float accX, accY, accZ;
        float gyroX, gyroY, gyroZ;
        float thistemp;

        float accX_buffer[BUF_SIZE] = {0.0};
        float accY_buffer[BUF_SIZE] = {0.0};
        float accZ_buffer[BUF_SIZE] = {0.0};

        float gyroX_buffer[BUF_SIZE] = {0.0};
        float gyroY_buffer[BUF_SIZE] = {0.0};
        float gyroZ_buffer[BUF_SIZE] = {0.0};

        sensors_event_t accel;                
        sensors_event_t gyro;
        sensors_event_t temp;
};



