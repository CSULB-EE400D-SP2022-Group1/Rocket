#include "myIMU.h"

/*!
    @brief initializes IMU, sets frequency of accelerometer and gyrometer at 1099Hz,
            and fills data buffer for acceleration (m/s^2) and angular vel. (rad/s)
    @param ICM_CS CS pin for IMU
    @return 1 (true) if initialization is successful, 0 (false) otherwise
*/
bool myIMU::start(int ICM_CS)
{
    dataFlag = 0;

    unsigned status =  begin_SPI(ICM_CS);
    elapsedMillis timePastMs;
    elapsedMicros timePastMicros;

    setAccelFrequency(IMU_FREQ);
    setGyroFrequency(IMU_FREQ);

    int i = 0;

    while (timePastMs <= 2000)
    {
        if (timePastMicros >= 1000000/IMU_LOGFREQ && i <= BUF_SIZE) 
        {   
            getEvent(&accel, &gyro, &temp);

            accX_buffer[i] = accel.acceleration.x;
            accX_buffer[i] = accel.acceleration.x;
            accX_buffer[i] = accel.acceleration.x;
            
            gyroX_buffer[i] = gyro.gyro.x;
            gyroY_buffer[i] = gyro.gyro.y;
            gyroZ_buffer[i] = gyro.gyro.z;
            
            timePastMicros -= 1000000/IMU_LOGFREQ;
            ++i;
        }
    }

    return status;
}

/*!
    @brief IMU starts measuring data
*/
bool myIMU::getData()
{
    if (timeSinceDataRead >= 1000000/IMU_FREQ)
    {
        getEvent(&accel, &gyro, &temp);
<<<<<<< Updated upstream
        timeSinceDataRead -= 1000000/highestSensorRate;
=======
        timeSinceDataRead -= 1000000/IMU_FREQ;

>>>>>>> Stashed changes
        dataFlag = 1;
    }
    
    if (timeSinceBufferUpdate >= 1e6/IMU_LOGFREQ) {
        updateBuffers();
        timeSinceBufferUpdate -= 1000000/IMU_LOGFREQ;
    }

    return dataFlag;
}

bool myIMU::resetDataFlag()
{
    dataFlag = 0;
    return dataFlag;
}

/*!
    @brief Update buffers for acceleration and angular velocity
*/
void myIMU::updateBuffers()
{
    getData();

    for (int i = BUF_SIZE - 1; i > 0; --i)
    {
        accX_buffer[i] = accX_buffer[i-1];
    }
    accX_buffer[0] = accel.acceleration.x;

    for (int i = BUF_SIZE - 1; i > 0; --i)
    {
        accY_buffer[i] = accY_buffer[i-1];
    }
    accY_buffer[0] = accel.acceleration.y;

    for (int i = BUF_SIZE - 1; i > 0; --i)
    {
        accZ_buffer[i] = accZ_buffer[i-1];
    }
    accZ_buffer[0] = accel.acceleration.z;

    for (int i = BUF_SIZE - 1; i > 0; --i)
    {
        gyroX_buffer[i] = gyroX_buffer[i-1];
    }
    gyroX_buffer[0] = gyro.gyro.x;

    for (int i = BUF_SIZE - 1; i > 0; --i)
    {
        gyroY_buffer[i] = gyroY_buffer[i-1];
    }
    gyroY_buffer[0] = gyro.gyro.y;

    for (int i = BUF_SIZE - 1; i > 0; --i)
    {
        gyroZ_buffer[i] = gyroZ_buffer[i-1];
    }
    gyroZ_buffer[0] = gyro.gyro.z;
}

/*!
    @brief Sets the accelerometer's frequency in Hz (maximum 1125)
*/
void myIMU::setAccelFrequency(int desiredRate)
{
    // equation taken from datasheet (page 65)
    // https://cdn-learn.adafruit.com/assets/assets/000/087/720/original/datasheet.pdf?1580177969

    uint16_t accel_divisor = 1125 / desiredRate - 1;
    setAccelRateDivisor(accel_divisor); 

    // Serial.print("Accel rate divisor set to: ");
    // Serial.println(getAccelRateDivisor());
}

/*!
    @brief Sets the gyrometer's frequency in Hz (maximum 1100)
*/
void myIMU::setGyroFrequency(int desiredRate)
{
    // equation taken from datasheet (page 61)
    // https://cdn-learn.adafruit.com/assets/assets/000/087/720/original/datasheet.pdf?1580177969

    uint16_t gyro_divisor = 1100 / desiredRate - 1;
    setGyroRateDivisor(gyro_divisor); 

    // Serial.print("Gyro rate divisor set to: ");
    // Serial.println(getGyroRateDivisor());
}   

/*!
    @brief Sets the accelerometer's range (+- 4, 8, 16, 30G)
*/
void myIMU::setAccelRange(int desiredRange)
{
    switch(desiredRange) {
    case 4:
        setAccelRange(ICM20649_ACCEL_RANGE_4_G);
        break;
    case 8:
        setAccelRange(ICM20649_ACCEL_RANGE_8_G);
        break;
    case 6:
        setAccelRange(ICM20649_ACCEL_RANGE_16_G);
        break;
    case 30:
        setAccelRange(ICM20649_ACCEL_RANGE_30_G);
        break;
    }
}

/*!
    @brief Sets the gyrometer's range (+- 500, 1000, 2000, 4000 DPS)
*/
void myIMU::setGyroRange(int desiredRange)
{
    switch(desiredRange) {
    case 500:
        setGyroRange(ICM20649_GYRO_RANGE_500_DPS);
        break;
    case 1000:
        setGyroRange(ICM20649_GYRO_RANGE_1000_DPS);
        break;
    case 2000:
        setGyroRange(ICM20649_GYRO_RANGE_2000_DPS);
        break;
    case 4000:
        setGyroRange(ICM20649_GYRO_RANGE_4000_DPS);
        break;
    }
}

/*!
    @brief Returns data measured from IMU
    @return temperature (deg C)
*/
<<<<<<< Updated upstream
float myIMU::getTemp()
{
    return temp.temperature;
=======
float myIMU::getTemp(int i)
{
    return temp_buffer[i];
>>>>>>> Stashed changes
}

/*!
    @brief Returns data measured from IMU
    @return acceleration (m/s^2) in x direction
*/
<<<<<<< Updated upstream
float myIMU::getAccelX()
{
    return accel.acceleration.x;
=======
float myIMU::getAccelX(int i)
{
    return accX_buffer[i];
>>>>>>> Stashed changes
}

/*!
    @brief returns measured 
    @return acceleration (m/s^2) in y direction
*/
<<<<<<< Updated upstream
float myIMU::getAccelY()
{
    return accel.acceleration.y;
=======
float myIMU::getAccelY(int i)
{
    return accY_buffer[i];
>>>>>>> Stashed changes
}

/*!
    @brief returns measured 
    @return acceleration (m/s^2) in z direction
*/
<<<<<<< Updated upstream
float myIMU::getAccelZ()
{
    return accel.acceleration.z;
=======
float myIMU::getAccelZ(int i)
{
    return accZ_buffer[i];
>>>>>>> Stashed changes
}

/*!
    @brief returns measured 
    @return angular velocity (rad/s) in x direction
*/
<<<<<<< Updated upstream
float myIMU::getGyroX()
{
    return gyro.gyro.x;
=======
float myIMU::getGyroX(int i)
{
    return gyroX_buffer[i];
>>>>>>> Stashed changes
}

/*!
    @brief returns measured 
    @return angular velocity (rad/s) in y direction
*/
<<<<<<< Updated upstream
float myIMU::getGyroY()
{
    return gyro.gyro.y;
=======
float myIMU::getGyroY(int i)
{
    return gyroY_buffer[i];
>>>>>>> Stashed changes
}

/*!
    @brief returns measured 
    @return angular velocity (rad/s) in z direction
*/
<<<<<<< Updated upstream
float myIMU::getGyroZ()
{
    return gyro.gyro.z;
}
=======
float myIMU::getGyroZ(int i)
{
    return gyroZ_buffer[i];
}

/*!
    @brief Returns time at which IMU measured data 
    @return time (us)
*/
uint32_t myIMU::getTime(int i)
{
    return timeMicros_buffer[i];
}
>>>>>>> Stashed changes
