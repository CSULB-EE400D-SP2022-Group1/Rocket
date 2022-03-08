#ifndef AVG_H
#define AVG_H

#include <Adafruit_BME280.h>

class average
{
    public:
        /*
            @brief Initialize all necessary variables for calculating the moving average
            @param array_size number of data points to be stored over a given time to calculate average for
        */
        average(int seconds)
            : dur {seconds}, sum{0}, count{0}, next{0} {}
        void begin();
        int read(double newRead);
    
    private:
        int dur;                // duration in seconds of averaging
        int arr_size;           // size of array
        double sum;             // sum of read_array
        int count;              // number of readings
        int next;               // index for next reading
        double* read_array;     // pointer to array, 

};


#endif