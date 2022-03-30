#include "myBME.h"

/*!
    @brief Initializes the sensor and prints error message(s) if any
*/
void myBME::start()
{
    unsigned status;
    
    //default settings
    status = mybme.begin();  
    // You can also pass in a Wire library object like &Wire2
    // status = bme.begin(0x76, &Wire2)
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(mybme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }
}

/*!
    @brief Have sensor start measuring data (temperature, altitude, humidity)
*/
void myBME::getData()
{
    currentMillis = millis();
    unsigned long interval = 1000/frequency; // time for one data cycle of sensor (ms)

    if (currentMillis - pastMillis >= interval)
    {
        temp = mybme.readTemperature();
        altitude = mybme.readAltitude(SEALEVELPRESSURE_HPA);
        humidity = mybme.readHumidity();
        
        pastMillis = currentMillis;
    }
}

/*!
    @brief Retrieves data from sensor
    @return temperature in Celscius
*/
double myBME::getTemp()
{
    return temp;
}

/*!
    @brief Retrieves data from sensor
    @return altitude in meters
*/
double myBME::getAltitude()
{
    return altitude;
}

/*!
    @brief Retrieves data from sensor
    @return humidity in %
*/
double myBME::getHumidity()
{
    return humidity;
}

/*!
    @brief Fills the data array
*/
void myBME::fillAvg()
{
    // shifts data right to make space for new data at index 0
    for (int i = SAMPLE_SIZE - 1; i > 0; --i)
    {
        alt_data[i] = alt_data[i-1]; 
    }

    // populate array with new data 
    alt_data[0] = altitude; 
}

/*!
    @brief Calculates the average of data array
    @return 3 second average as rounded integer
*/
int myBME::getAvg()
{
    double sum = 0.0;

    for (int i = 0; i <= SAMPLE_SIZE - 1; ++i)
    {
        sum += alt_data[i];
    }

    return round (sum / SAMPLE_SIZE);
}

/*!
    @brief Calculates the average of data array
    @return 2 second average as rounded integer
*/
int myBME::getAvgRecent()
{
    double sum = 0.0;

    for (int i = 0; i <= RECENT_SIZE - 1; ++i)
    {
        sum += alt_data[i];
    }

    return round(sum / RECENT_SIZE);
}

/*!
 *   @brief Sets the BME's output data rate in Hz
 *   @param rate desired rate in Hz (10,20,30,40,50,100)
*/
void myBME::setFrequency(int rate)
{
    if (rate == 10) {
        mybme.setSampling(  Adafruit_BME280::MODE_NORMAL,
                            Adafruit_BME280::SAMPLING_X16, // temperature
                            Adafruit_BME280::SAMPLING_X16, // pressure
                            Adafruit_BME280::SAMPLING_X16, // humidity
                            Adafruit_BME280::FILTER_OFF,
                            Adafruit_BME280::STANDBY_MS_0_5 );
        frequency = rate;
    }

    else if (rate == 20) {
        mybme.setSampling(  Adafruit_BME280::MODE_NORMAL,
                            Adafruit_BME280::SAMPLING_X1, // temperature
                            Adafruit_BME280::SAMPLING_X16, // pressure
                            Adafruit_BME280::SAMPLING_X8, // humidity
                            Adafruit_BME280::FILTER_OFF,
                            Adafruit_BME280::STANDBY_MS_0_5 ); 
        frequency = rate;
    }

    else if (rate == 30) {
        mybme.setSampling(  Adafruit_BME280::MODE_NORMAL,
                            Adafruit_BME280::SAMPLING_X1, // temperature
                            Adafruit_BME280::SAMPLING_X8, // pressure
                            Adafruit_BME280::SAMPLING_X8, // humidity
                            Adafruit_BME280::FILTER_OFF,
                            Adafruit_BME280::STANDBY_MS_0_5 );
        frequency = rate;
    }

    else if (rate == 40) {
        mybme.setSampling(  Adafruit_BME280::MODE_NORMAL,
                            Adafruit_BME280::SAMPLING_X1, // temperature
                            Adafruit_BME280::SAMPLING_X8, // pressure
                            Adafruit_BME280::SAMPLING_X4, // humidity
                            Adafruit_BME280::FILTER_OFF,
                            Adafruit_BME280::STANDBY_MS_0_5 );
        frequency = rate;
    }

    else if (rate == 50) {
        mybme.setSampling(  Adafruit_BME280::MODE_NORMAL,
                            Adafruit_BME280::SAMPLING_X1, // temperature
                            Adafruit_BME280::SAMPLING_X4, // pressure
                            Adafruit_BME280::SAMPLING_X4, // humidity
                            Adafruit_BME280::FILTER_OFF,
                            Adafruit_BME280::STANDBY_MS_0_5 );
        frequency = rate;
    }

    else if (rate == 100) {
        mybme.setSampling(  Adafruit_BME280::MODE_NORMAL,
                            Adafruit_BME280::SAMPLING_X1, // temperature
                            Adafruit_BME280::SAMPLING_X2, // pressure
                            Adafruit_BME280::SAMPLING_X1, // humidity
                            Adafruit_BME280::FILTER_OFF,
                            Adafruit_BME280::STANDBY_MS_0_5 );
        frequency = rate;
    }

    else {
        Serial.println("Invalid sensor rate. Options include:");
        Serial.println("10, 20, 30, 40, 50, 100 Hz");
    }
}
 