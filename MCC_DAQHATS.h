#ifndef MCCDAQHATS_HDR_FILE_
#define MCCDAQHATS_HDR_FILE_

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <cmath> 
#include <gpiod.h>
#include <daqhats/daqhats.h>
#include <daqhats/mcc118.h>
#include "Global.h"
#include "Logger.h"

using namespace std;

#define DEFAULT_BUFLEN 1024

void MCC_DAQHATS_INIT(int* count, int& retVal);

#pragma region MCC118

void MCC118_Init_Struct(int address);

void MCC118(int address, int& retVal);

double MCC118_readChannel(int address, int channnel);

void MCC118_SCAN_RELEASE(int address);

#pragma endregion MCC118

#endif