#pragma once
#include <LittleFS.h>
#include <SD.h>
#include "myBME.h"
#include "myIMU.h"
#include "myGPS.h"
//#include "state.hpp"

const uint8_t bme_update_frequency = 30;
const uint8_t imu_update_frequency = 100;
const uint8_t gps_update_frequency = 1;


class Data_Storage : public LittleFS_QSPIFlash
{
private:
    myGPS* ptrGPS;
    myIMU* ptrIMU;
    myBME* ptrBME;
public:
    bool init();

    uint32_t bmeLastLogCount = 0;
    uint32_t imuLastLogCount = 0;
    uint32_t gpsLastLogTime = 0; // milliseconds

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