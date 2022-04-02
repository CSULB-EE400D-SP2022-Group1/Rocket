#pragma once
//#include <Arduino.h>
#include <LittleFS.h>
#include <SD.h>

class Data_Storage : public LittleFS_QSPIFlash
{
private:

public:
    bool init();

    bool initFiles(); // initialize all NOR Flash Files
    bool initFileBME();
    bool initFileIMU();
    bool initFileGPS();
    bool initFileFSM();

    void formatFlash();
    bool writeData(char filename[13], String data); // write a single line of data to a file
    void printData(char filename[13]); // print all data in a file
    bool dumpData(); // dump all data from NOR Flash to SD Card
    bool dumpFile(char filename[13]); // dumps specific files from NOR flash to SD card
};