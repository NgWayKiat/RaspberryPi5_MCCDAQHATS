#ifndef SRVSOK_HDR_FILE
#define SRVSOK_HDR_FILE

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <pthread.h>
#include <thread>
#include "MCC_DAQHATS.h"
#include "Global.h"
#include "Logger.h"
#include "Versions.h"

#define DEFAULT_BUFLEN 1024

using namespace std;

enum SRVSOK_CODE
{
    NOCODE, //No Specific Code
    A0001,  //Reboot Raspi5 [R0001]
    A0002,  //Check is Alive with return ProgramName,Version,ModifyDate [R0002]
    A0003,  //Get the send and read channel from giReadChannel [R0003]
    A0004,  
    A0005,  //Set the send and read channel to giReadChannel [R0005]
    A0006,
    A0007,
    A0008,
    A0009,
    A0010,
    A8888,
    A9999
};

void serverSok(int& retVal);
void handle_client_conn(int client_socket);
void handle_client_event(int* client_sock, string recData);
int getSRVSOK_CODE(string sCode);



#endif