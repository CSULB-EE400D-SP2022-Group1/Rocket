#include "average.h"

/*
    @brief Creates a data array with proper size for averaging 
    *NOTE* THIS IS TO BE USED WITH THE FOLLOWING PARAMETERS 
    (CHANGES WILL NEED TO BE APPLIED TO THE FUNCTION DEFINITION):
    oversampling: temperature 1x | pressure 8x | humidity 0x
    standby time: 0.5ms
                
*/
void average::begin()
{
    int temp_os = 1;
    int pres_os = 8;
    int hum_os = 0;

    // Equation taken from BME280 datasheet (pg 51)
    // https://cdn-shop.adafruit.com/datasheets/BST-BME280_DS001-10.pdf

    int t_meas = 1 + (2*temp_os) + (2*pres_os + 0.5) + (2*hum_os + 0.5); // active measurement time in ms
    int t_sb = 0.5; // standby time in ms

    //Equation taken from BME280 datasheet (pg 52)
    int m_rate = 1000 / (t_meas + t_sb); // sampling rate in Hz

    arr_size = round(m_rate * dur); // calculate number of data points to store for the given duration and sampling rate
    read_array = new double[arr_size];
}

/*
    @brief Calculate the moving average of data
    @param newRead data to be stored in array
    @returns moving average of stored data
*/
int average::read(double newRead)
{
    // add new readings to sum until read_array is full
    if (count < arr_size) {
        ++count;
        sum += newRead;
    }

    // once filled (count > arr_size), start subtracting old data and adding new data
    else {
        sum = sum - read_array[next] + newRead;
    }

    read_array[next] = newRead;
    ++next;
    if (next >= arr_size){
        next = 0; // reset next index to start replacing oldest data with most recent data
    }
    
    Serial.print("Current data = ");
    Serial.println(read_array[next-1]);
    Serial.print("Current average = ");
    Serial.println(round(sum/count));

    return round(sum/count);
}
