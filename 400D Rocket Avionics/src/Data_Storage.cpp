#include "Data_Storage.h"


bool Data_Storage::init()
{    
    if(!begin()) // attempt to start QSPI NOR Flash
    {
        Serial.println("NOR FLASH FAILED TO INITITALIZE");
        return 0; // Fail
    }
    else
    {
        Serial.println("NOR Flash Successfully Initialized");
        return 1; // Success  
    }
}


bool Data_Storage::initFiles()
{
    initFileBME();
    initFileIMU();
    initFileGPS();
    initFileFSM();
}


bool Data_Storage::initFileBME()
{
    // Initialize BME280 Data File
    char filename[13] = "BME.csv";
    String data;
    data = "uC Clock (us)";
    data += ",";
    data += "Altitude (m)";
    data += ",";
    data += "Temperature (C)";
    data += ",";
    data += "Humidity (%)";    

    bool createdFile = writeData(filename,data);
    
    if(createdFile)
    {   Serial.print("Successfully created ");  }
    else
    {   Serial.print("FAILED TO CREATE ");  }
    Serial.println(filename);
    
    return createdFile;
}


bool Data_Storage::initFileIMU()
{
    // Initialize IMU (ICM20649) Data File
    char filename[13] = "IMU.csv";
    String data;
    data = "uC Clock (us)";
    data += ",";
    data += "X Accel (m/s)";
    data += ",";
    data += "Y Accel (m/s)";
    data += ",";
    data += "Z Accel (m/s)"; 
    data += ",";
    data += "X Gyro (rad/s)";
    data += ",";
    data += "Y Gyro (rad/s)";
    data += ",";
    data += "Z Gyro (rad/s)";
    data += ",";
    data += "Temperature (C)";     

    bool createdFile = writeData(filename,data);
    
    if(createdFile)
    {   Serial.print("Successfully created ");  }
    else
    {   Serial.print("FAILED TO CREATE ");  }
    Serial.println(filename);
    
    return createdFile;
}


bool Data_Storage::initFileGPS()
{
    // Initialize GPS Data File
    char filename[13] = "GPS.csv";
    String data;
    data = "uC Clock (us)";
    data += ",";
    data += "Lattitude (DD)";
    data += ",";
    data += "Longitude (DD)";
    data += ",";
    data += "Altitude (m)"; 
    data += ",";
    data += "Speed (m/s)";
    data += ",";
    data += "Course (deg)";
    data += ",";
    data += "Year (UTC)";
    data += ",";
    data += "Month (UTC)";
    data += ",";
    data += "Day (UTC)";
    data += ",";
    data += "Hour (UTC)";
    data += ",";
    data += "Minute (UTC)";
    data += ",";
    data += "Second (UTC)";
    data += ",";
    data += "Sats";
    data += ",";
    data += "HDOP";
    data += ",";
    data += "Age";

    bool createdFile = writeData(filename,data);
    
    if(createdFile)
    {   Serial.print("Successfully created ");  }
    else
    {   Serial.print("FAILED TO CREATE ");  }
    Serial.println(filename);
    
    return createdFile;
}


bool Data_Storage::initFileFSM()
{
    // Initialize GPS Data File
    char filename[13] = "FSM.csv";
    String data;
    data = "uC Clock (us)";
    data += ",";
    data += "Event";

    bool createdFile = writeData(filename,data);
    
    if(createdFile)
    {   Serial.print("Successfully created ");  }
    else
    {   Serial.print("FAILED TO CREATE ");  }
    Serial.println(filename);
    
    return createdFile;
}


void Data_Storage::formatFlash()
{
    Serial.print("NOR Flash total Size = ");
    Serial.println(totalSize());

    Serial.print("NOR Flash used Size = ");
    Serial.println(usedSize());

    Serial.println("NOR Flash quick formatting");
    quickFormat();

    Serial.print("NOR Flash used size after format = ");
    Serial.println(usedSize());
}



bool Data_Storage::writeData(char filename[13], String data)
{
    File dataFile = open(filename, FILE_WRITE);
    if (dataFile) // check if file is available
    {
        dataFile.println(data); // write data line in file
        dataFile.close();
        return 1; // success
    }
    else // file isn't available, error message
    {
        Serial.print("error opening ");
        Serial.println(filename);
        return 0; // fail
    }
}


void Data_Storage::printData(char filename[13])
{
    File dataFile = open(filename, FILE_READ);

    if (dataFile) // check if file is available
    {
        while(dataFile.available()) // while more lines are available
        {
            Serial.println(dataFile.readString()); // print next line
        }
        dataFile.close();
    }
    else // file isn't available, error message
    {
        Serial.print("error opening ");
        Serial.println(filename);
    }
}


bool Data_Storage::dumpData()
{
    if(!SD.begin(BUILTIN_SDCARD)) // attempt to start QSPI NOR Flash
    {
        Serial.println("SD CARD FAILED TO INITITALIZE");
        return 0; // Fail
    }
    else
    {   Serial.println("SD Card Successfully Initialized"); }

    SD.remove("BME.csv");
    SD.remove("IMU.csv");
    SD.remove("GPS.csv");
    SD.remove("FSM.csv");

    dumpFile("BME.csv");
    dumpFile("IMU.csv");
    dumpFile("GPS.csv");
    dumpFile("FSM.csv");
    
    return 1;
}


bool Data_Storage::dumpFile(char filename[13])
{
    Serial.print("Dumping ");
    Serial.print(filename);
    Serial.print(" to SD Card.....");
    File dataFile = open(filename, FILE_READ);    
    File dumpFile = SD.open(filename, FILE_WRITE);
    while(dataFile.available())
    {        
        char data[13];
        dataFile.readBytes(data,1);
        dumpFile.printf(data);
    }
    dataFile.close();        
    dumpFile.close();
    Serial.println("Complete");

    return 1;
}