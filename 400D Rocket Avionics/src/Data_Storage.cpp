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


bool Data_Storage::initFiles(myBME* ptr1,myIMU* ptr2,myGPS* ptr3,State* ptr4)
{
    initFileBME(ptr1);
    initFileIMU(ptr2);
    initFileGPS(ptr3);
    initFileFSM(ptr4);
    return 1;
}


bool Data_Storage::initFileBME(myBME* ptr)
{
    ptrBME = ptr;
    // Initialize BME280 Data File
    char filename[13] = "bme000.csv";
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


bool Data_Storage::initFileIMU(myIMU* ptr)
{
    ptrIMU = ptr;
    // Initialize IMU (ICM20649) Data File
    char filename[13] = "imu000.csv";
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

    bool createdFile = writeData(filename,data);
    
    if(createdFile)
    {   Serial.print("Successfully created ");  }
    else
    {   Serial.print("FAILED TO CREATE ");  }
    Serial.println(filename);
    
    return createdFile;
}


bool Data_Storage::initFileGPS(myGPS* ptr)
{
    ptrGPS = ptr;
    // Initialize GPS Data File
    char filename[13] = "gps000.csv";
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


bool Data_Storage::initFileFSM(State* ptr)
{
    ptrFSM = ptr;
    // Initialize GPS Data File
    char filename[13] = "fsm000.csv";
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
    dumpFile(bme_filename);
    dumpFile(imu_filename);
    dumpFile(gps_filename);
    dumpFile(fsm_filename);
    
    quickFormat();

    return 1;
}


bool Data_Storage::dumpFile(char filename[13])
{
    Serial.print("Dumping ");
    Serial.print(filename);
    Serial.print(" to SD Card ");
    File dataFile = open(filename, FILE_READ); 

    for (int i = 0; i < 1000; ++i)
    {   
        filename[3] = i/100 + '0';
        filename[4] = i/10 + '0';
        filename[5] = i%10 + '0';

        if(!SD.exists(filename)) 
        {
            break;
        }
    }

    File dumpFile = SD.open(filename, FILE_WRITE);
    Serial.print("as ");
    Serial.print(filename);

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


void Data_Storage::runLogs()
{
    logBME();
    logIMU();
    logGPS();
    logFSM();
}


void Data_Storage::logBME()
{
    if(ptrBME->getDataCount() >= bmeLastLogCount + bme_update_frequency)
    {
        bmeLastLogCount = ptrBME->getDataCount();
        String data;
        char filename[13] = "bme000.csv";

        for(int i = bme_update_frequency - 1; i >= 0 ; i--)
        {
            data += String(ptrBME->getTime(i));
            data += ",";
            data += String(ptrBME->getAltitude(i),1);
            data += ",";
            data += String(ptrBME->getTemp(i),0);
            data += ",";
            data += String(ptrBME->getHumidity(i),0);
            if(i > 0)
            {
                data += "\n";
            }
        }
        writeData(filename,data);
    }
}


void Data_Storage::logIMU()
{
    if(ptrIMU->getDataCount() >= imuLastLogCount + imu_update_frequency)
    {
        imuLastLogCount = ptrIMU->getDataCount();
        String data;    
        char filename[13] = "imu000.csv";
        
        for(int i = imu_update_frequency - 1; i >= 0 ; i--)
        {
            data += String(ptrIMU->getTime(i));
            data += ",";
            data += String(ptrIMU->getAccelX(i),2);
            data += ",";
            data += String(ptrIMU->getAccelY(i),2);
            data += ",";
            data += String(ptrIMU->getAccelZ(i),2);
            data += ",";
            data += String(ptrIMU->getGyroX(i),2);
            data += ",";
            data += String(ptrIMU->getGyroY(i),2);
            data += ",";
            data += String(ptrIMU->getGyroZ(i),2);
            if(i > 0)
            {
                data += "\n";
            }
        }
        writeData(filename,data);
    }
}


void Data_Storage::logGPS()
{
    if(millis() >= gpsLastLogTime + 1000)
    {
        gpsLastLogTime = millis();
        String data;    
        char filename[13] = "gps000.csv";
        data += String(micros());
        data += ",";
        data += String(ptrGPS->getLat(),5);
        data += ",";
        data += String(ptrGPS->getLng(),5);
        data += ",";
        data += String(ptrGPS->getGPSAltitude(),0);
        data += ",";
        data += String(ptrGPS->getSpeed(),0);
        data += ",";
        data += String(ptrGPS->getYear());
        data += ",";
        data += String(ptrGPS->getMonth());
        data += ",";
        data += String(ptrGPS->getDay());
        data += ",";
        data += String(ptrGPS->getHour());
        data += ",";
        data += String(ptrGPS->getMinute());    
        data += ",";
        data += String(ptrGPS->getSecond());    
        data += ",";
        data += String(ptrGPS->numSatellites());    
        data += ",";
        data += String(ptrGPS->getHDOP());    
        data += ",";
        data += String(ptrGPS->getAge());
        writeData(filename,data);
    }
}


void Data_Storage::logFSM()
{
    /*
    if(lastState < ptrFSM->getState())
    {
        lastState++;
        switch(ptrFSM->getState())
        {
        case 0:
            Serial.println("STATE Init");
            break;
        case 1:
            Serial.println("STATE Pad_Hold");
            break;
        case 2:
            Serial.println("STATE Pad_Idle");
            break;
        case 3:
            Serial.println("STATE Ascent");
            break;
        case 4:
            Serial.println("STATE Descent");
            break;
        case 5:
            Serial.println("STATE Landing");
            break;
        case 6:
            Serial.println("STATE Landing_Idle");
            break;
        }
    }\
    */
}