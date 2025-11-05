#include "MCC_DAQHATS.h"

//MCC118HatInfo structMCC118HatInfo;

void MCC_DAQHATS_INIT(int* count, int& retVal)
{
    char buf[DEFAULT_BUFLEN] = {0};
    int id = 0;
    int address = 0;
    struct HatInfo* list = (struct HatInfo*)malloc(*count * sizeof(struct HatInfo));
    hat_list(HAT_ID_ANY, list);

    id = list->id;
    address = list->address;

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "MCC HATS Info: Id[%d], Address[%d], ProductName[%s], Version[%d]", id, address, list->product_name, list->version);
    writeToLog(INFO, buf);

    free(list);

    switch (id)
    {
        case HAT_ID_MCC_118:           
            MCC118(address, retVal);
            break;
        
        case HAT_ID_MCC_118_BOOTLOADER:
            break;

        case HAT_ID_MCC_128:
            break;
        
        case HAT_ID_MCC_134:
            break;
        
        case HAT_ID_MCC_152:
            break;
        
        case HAT_ID_MCC_172:
            break;

        default:
            break;
    }
}

#pragma region "MCC118"

void MCC118_Init_Struct()
{
    char buf[DEFAULT_BUFLEN] = {0};
    char temp[DEFAULT_BUFLEN] = {0};

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "The initiate value for the MCC118 Structur List as below:");
    writeToLog(INFO, buf);

    for (int i=0; i < structMCC118HatInfo.iTotalChannel; i++)
    {
        structMCC118HatInfo.subInfo[i].iChannel = i;
        memset(temp, 0, sizeof(temp));
        sprintf(temp, "Channel %d" , i);
        structMCC118HatInfo.subInfo[i].sName = temp;
        memset(temp, 0, sizeof(temp));
        sprintf(temp, "MCC118 DAQ HAT - Channel %d" , i);
        structMCC118HatInfo.subInfo[i].sDescription = temp;
        structMCC118HatInfo.subInfo[i].bStatus = true;
        structMCC118HatInfo.subInfo[i].value = 0.0;

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "CHN[%d], Name:%s, Description:%s, Status:%s, Value:%d", structMCC118HatInfo.subInfo[i].iChannel,  structMCC118HatInfo.subInfo[i].sName.c_str(),
        structMCC118HatInfo.subInfo[i].sDescription.c_str(), structMCC118HatInfo.subInfo[i].bStatus ? "Enable":"Disable", structMCC118HatInfo.subInfo[i].value);
        writeToLog(INFO, buf);
    }
}

void MCC118(int address, int& retVal)
{
    char buf[DEFAULT_BUFLEN] = {0};
    char bufTemp[DEFAULT_BUFLEN] = {0};
    int result = RESULT_SUCCESS;
    int iChannel = 0;
    struct MCC118DeviceInfo* deviceInfo = (struct MCC118DeviceInfo*)malloc(sizeof(struct MCC118DeviceInfo));
    double scan_rate = 1000.0;
    double actual_scan_rate = 0.0;
    double voltage = 0.0;
       
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "Selected MCC 118 device at address[%d]", address);
    writeToLog(INFO, buf);

    if (!mcc118_is_open(address)) {
        result = mcc118_open(address);
    }
            
    if (result == RESULT_SUCCESS)
    {
        MCC118_SCAN_RELEASE(address);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "Success to open a connection to the MCC 118 device at address[%d]", address);
        writeToLog(INFO, buf);

        deviceInfo = mcc118_info();
        iChannel = deviceInfo->NUM_AI_CHANNELS;

        //Read the actual sample rate per channelresult == RESULT_SUCCE for a requested sample rate.
        mcc118_a_in_scan_actual_rate(deviceInfo->NUM_AI_CHANNELS, scan_rate, &actual_scan_rate);

        //Initiate and Assign value to the structure MCC118
        structMCC118HatInfo.address = address;
        structMCC118HatInfo.iTotalChannel = iChannel;
        structMCC118HatInfo.scanRate = scan_rate;
        structMCC118HatInfo.actScanRate = actual_scan_rate;
        structMCC118HatInfo.minADCCode = deviceInfo->AI_MIN_CODE;
        structMCC118HatInfo.maxADCCode = deviceInfo->AI_MAX_CODE;
        structMCC118HatInfo.minVoltage = deviceInfo->AI_MIN_VOLTAGE;
        structMCC118HatInfo.maxVoltage = deviceInfo->AI_MAX_VOLTAGE;
        structMCC118HatInfo.minRange = deviceInfo->AI_MIN_RANGE;
        structMCC118HatInfo.maxRange = deviceInfo->AI_MAX_RANGE;

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "The MCC118 Device Information as below:");
        writeToLog(INFO, buf);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "No. of Channel = %d", iChannel);
        writeToLog(INFO, buf);
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "Requested scan rate: %-10.2f", structMCC118HatInfo.scanRate);
        writeToLog(INFO, buf);
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "Actual scan rate: %-10.2f", structMCC118HatInfo.actScanRate);
        writeToLog(INFO, buf);
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "MIN ADC Code = %d", structMCC118HatInfo.minADCCode);
        writeToLog(INFO, buf);
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "MAX ADC Code = %d", structMCC118HatInfo.maxADCCode);
        writeToLog(INFO, buf);
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "MIN Voltage = %d", structMCC118HatInfo.minVoltage);
        writeToLog(INFO, buf);
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "MAX Voltage = %d", structMCC118HatInfo.maxVoltage);
        writeToLog(INFO, buf);
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "MIN Range = %d", structMCC118HatInfo.minRange);
        writeToLog(INFO, buf);
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "MAX Range = %d", structMCC118HatInfo.maxRange);  
        writeToLog(INFO, buf);

        MCC118_Init_Struct();
        
        while (true){
            memset(buf, 0, sizeof(buf));
            string sMsg = "Voltage Result: ";
            for (int i = 0; i < structMCC118HatInfo.iTotalChannel; i++)
            {            
                voltage = MCC118_readChannel(structMCC118HatInfo.address, i);
                memset(bufTemp, 0, sizeof(bufTemp));
                sprintf(bufTemp, "%5.5f", voltage);
                sMsg += " CH[" + to_string(i) +"]" + " =" + bufTemp + " V";
                if(structMCC118HatInfo.subInfo[i].bStatus)
                {
                    structMCC118HatInfo.subInfo[i].value = voltage;
                }
            }
            memset(buf, 0, sizeof(buf));
            sprintf(buf, sMsg.c_str());
            writeToLog(INFO, buf);
            fflush(stdout);
            
            usleep(giMSScan);
        }
    }
    else
    {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "Failed to open a connection to the MCC 118 device at address %d\n", address);
        writeToLog(INFO, buf);
        retVal = 1;
        goto STOP;
    }

STOP:
    MCC118_SCAN_RELEASE(address);
    mcc118_close(address);
}

double MCC118_readChannel(int address, int channnel)
{
    int result = RESULT_SUCCESS;
    char buf[DEFAULT_BUFLEN] = {0};
    double voltage = 0.0;
    
    result = mcc118_a_in_read(address, channnel, OPTS_DEFAULT, &voltage);

    if (result != RESULT_SUCCESS) {
        voltage = 0.0;
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "The MCC118_readChannel failed. CH[%d]=%3.3f V", channnel, voltage);
        writeToLog(INFO, buf);
    }

    return voltage;
}

void MCC118_SCAN_RELEASE(int address)
{
    mcc118_a_in_scan_stop(address);
    mcc118_a_in_scan_cleanup(address);
}

#pragma endregion

#pragma region "HAT_ID_MCC_118_BOOTLOADER"
#pragma endregion

#pragma region "MCC128"
#pragma endregion

#pragma region "HAT_ID_MCC_134"
#pragma endregion

#pragma region "HAT_ID_MCC_152"
#pragma endregion

#pragma region "HAT_ID_MCC_172"
#pragma endregion
