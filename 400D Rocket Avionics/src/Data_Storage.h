#pragma once
#include <LittleFS.h>
#include <SD.h>
#include "myBME.h"
#include "myIMU.h"
#include "myGPS.h"
//#include "state.hpp"

const uint8_t bme_update_frequency = 30; // Hz
const uint8_t imu_update_frequency = 100; // Hz
const uint8_t gps_update_frequency = 1; // Hz



class Data_Storage : public LittleFS_QSPIFlash
{
private:
    uint32_t lastLog = 0; // milliseconds

    myGPS* ptrGPS;
    myIMU* ptrIMU;
    myBME* ptrBME;

    uint8_t gpsCounter = 0;
    uint8_t imuCounter = 0;
    uint8_t bmeCounter = 0;
public:
    bool init();

    bool initFiles(myBME* ptr1,myIMU* ptr2,myGPS* ptr3); // initialize all NOR Flash Files
    bool initFileBME(myBME* ptr);
    bool initFileIMU(myIMU* ptr);
    bool initFileGPS(myGPS* ptr);
    bool initFileFSM();

    void formatFlash();
    bool writeData(char filename[13], String data); // write a single line of data to a file
    void printData(char filename[13]); // print all data in a file
    bool dumpData(); // dump all data from NOR Flash to SD Card
    bool dumpFile(char filename[13]); // dumps specific files from NOR flash to SD card
    
    void runLogs();

    void logBME();
    void logIMU();
    void logGPS();
    void logFSM();
};


// DATA LOGGING NOTES
/*
1. Allow sensor buffers to fill up for 1 second(keep track of this timer/counter)
2. Open a file and start writing the buffers to it, and then close it. This significantly lowers the
amount of times that we open and close files which takes up a significant amount of time. Limited us to 20 Hz
3. Ensure that the saved data still follows the proper CSV format. 
*/