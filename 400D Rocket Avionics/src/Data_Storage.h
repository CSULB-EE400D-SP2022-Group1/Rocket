#pragma once
//#include <Arduino.h>
#include <LittleFS.h>

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
    void dumpData(char filename[13]); // dump all data from NOR Flash to SD Card
};