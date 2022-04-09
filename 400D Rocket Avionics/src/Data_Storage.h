#pragma once
#include <LittleFS.h>
#include <SD.h>
#include "myBME.h"
#include "myIMU.h"
#include "myGPS.h"
#include "state_class.hpp"

const uint8_t bme_update_frequency = 32;
const uint8_t imu_update_frequency = 100;
const uint8_t gps_update_frequency = 1;

static char bme_filename[13] = "bme000.csv";
static char imu_filename[13] = "imu000.csv";
static char gps_filename[13] = "gps000.csv";
static char fsm_filename[13] = "fsm000.csv";

class Data_Storage : public LittleFS_QSPIFlash
{
private:
    myGPS* ptrGPS;
    myIMU* ptrIMU;
    myBME* ptrBME;
    State* ptrFSM;
    
    uint32_t bmeLastLogCount = 0;
    uint32_t imuLastLogCount = 0;
    uint32_t gpsLastLogTime = 0; // milliseconds

    int8_t lastState = -1;

public:
    bool init();

    bool initFiles(myBME* ptr1,myIMU* ptr2,myGPS* ptr3,State* ptr4); // initialize all NOR Flash Files
    bool initFileBME(myBME* ptr);
    bool initFileIMU(myIMU* ptr);
    bool initFileGPS(myGPS* ptr);
    bool initFileFSM(State* ptr);

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