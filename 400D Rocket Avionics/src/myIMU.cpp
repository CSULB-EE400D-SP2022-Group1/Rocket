#include "myIMU.h"

/*!
    @brief initializes IMU, sets frequency of accelerometer and gyrometer,
            and fills data buffer for acceleration (m/s^2) and angular vel. (rad/s)
    @param ICM_CS CS pin for IMU
    @param accelRate desired accelerometer rate in Hz
    @param gyroRate desired gyrometer rate in Hz
    @return 1 (true) if initialization is successful, 0 (false) otherwise
*/
bool myIMU::start(int ICM_CS, int accelRate, int gyroRate)
{
    unsigned status =  begin_SPI(ICM_CS);

    elapsedMillis timePastMs;
    elapsedMicros timePastMicros;
    setAccelFrequency(accelRate);
    setGyroFrequency(gyroRate);
    int i = 0;

    
    while (timePastMs <= 2000)
    {
        if (timePastMicros >= 1000000/IMU_FREQ && i <= BUF_SIZE) 
        {   
            getEvent(&accel, &gyro, &temp);

            accX_buffer[i] = accel.acceleration.x;
            accX_buffer[i] = accel.acceleration.x;
            accX_buffer[i] = accel.acceleration.x;
            
            gyroX_buffer[i] = gyro.gyro.x;
            gyroY_buffer[i] = gyro.gyro.y;
            gyroZ_buffer[i] = gyro.gyro.z;
            
            Serial.println(accX_buffer[i]);
            timePastMicros -= 1000000/IMU_FREQ;
            ++i;
        }
    }
}

/*!
    @brief IMU starts measuring data
*/
void myIMU::getData()
{
    getEvent(&accel, &gyro, &temp);

    thistemp = temp.temperature;

    accX = accel.acceleration.x;
    accY = accel.acceleration.y;
    accZ = accel.acceleration.z;

    gyroX = gyro.gyro.x;
    gyroY = gyro.gyro.y;
    gyroZ = gyro.gyro.z;
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
    accX_buffer[0] = accX;

    for (int i = BUF_SIZE - 1; i > 0; --i)
    {
        accY_buffer[i] = accY_buffer[i-1];
    }
    accY_buffer[0] = accY;

    for (int i = BUF_SIZE - 1; i > 0; --i)
    {
        accZ_buffer[i] = accZ_buffer[i-1];
    }
    accZ_buffer[0] = accZ;

    for (int i = BUF_SIZE - 1; i > 0; --i)
    {
        gyroX_buffer[i] = gyroX_buffer[i-1];
    }
    gyroX_buffer[0] = gyroX;

    for (int i = BUF_SIZE - 1; i > 0; --i)
    {
        gyroY_buffer[i] = gyroY_buffer[i-1];
    }
    gyroY_buffer[0] = gyroY;

    for (int i = BUF_SIZE - 1; i > 0; --i)
    {
        gyroZ_buffer[i] = gyroZ_buffer[i-1];
    }
    gyroZ_buffer[0] = gyroZ;
}

/*!
    @brief Sets the accelerometer's frequency in Hz
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
    @brief Sets the gyrometer's frequency in Hz
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
float myIMU::getTemp()
{
    return thistemp;
}

/*!
    @brief Returns data measured from IMU
    @return acceleration (m/s^2) in x direction
*/
float myIMU::getAccelX()
{
    return accX;
}

/*!
    @brief returns measured 
    @return acceleration (m/s^2) in y direction
*/
float myIMU::getAccelY()
{
    return accY;
}

/*!
    @brief returns measured 
    @return acceleration (m/s^2) in z direction
*/
float myIMU::getAccelZ()
{
    return accZ;
}

/*!
    @brief returns measured 
    @return angular velocity (rad/s) in x direction
*/
float myIMU::getGyroX()
{
    return gyroX;
}

/*!
    @brief returns measured 
    @return angular velocity (rad/s) in y direction
*/
float myIMU::getGyroY()
{
    return gyroY;
}

/*!
    @brief returns measured 
    @return angular velocity (rad/s) in z direction
*/
float myIMU::getGyroZ()
{
    return gyroZ;
}