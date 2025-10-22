#include "ServerSocket.h"

void handle_client_conn(int client_socket) {
    char buf[DEFAULT_BUFLEN] = {0};
    char recData[DEFAULT_BUFLEN] = {0};
    int ret = 0;
    struct sockaddr_in peer_addr;
    socklen_t peer_addr_len = sizeof(peer_addr);

    while (true)
    {
        // Read data from client connection
        memset(recData, 0, sizeof(recData));
        ret = read(client_socket, recData, sizeof(recData));

        getpeername(client_socket, (struct sockaddr*)&peer_addr, &peer_addr_len);
        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(peer_addr.sin_addr), ip_str, INET_ADDRSTRLEN);
        int port = ntohs(peer_addr.sin_port);

        if (ret > 0)
        {
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "Server Socket Data Receive from Clint_IP[%s], Client_Port[%d]. RecData: %s", ip_str, port, recData);
            writeToLog(INFO, buf);
            
            handle_client_event(&client_socket, recData);
        }
        else{                     
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "Client Disconnected. Client_IP[%s], Client_Port[%d]", ip_str, port);
            writeToLog(WARNING, buf);

            close(client_socket);
            pthread_exit(0);
        }
    }
}

void serverSok(int& retVal)
{
    int server_fd, client_socket;
    int opt = 1;
    struct sockaddr_in host_address;
    struct sockaddr_in client_address;
    socklen_t clientAddressLen = sizeof(client_address);
    char buf[DEFAULT_BUFLEN] = {0};
    char buffer[DEFAULT_BUFLEN] = {0};

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "Start server socket function. SERVER_PORT [%d]", gsHostPort);
    writeToLog(INFO, buf);

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "Failed to create Server Socket file descriptor");
        writeToLog(INFO, buf);
        retVal = 1;
    }

    // Set socket options for address and port reuse (optional but recommended)
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "Failed to set Server Socket options for address and port reuse");
        writeToLog(INFO, buf);
        retVal = 1;
    }

    host_address.sin_family = AF_INET;
    host_address.sin_addr.s_addr = INADDR_ANY;
    host_address.sin_port = htons(gsHostPort);

     // Bind the socket to the specified IP and port
    if (bind(server_fd, (struct sockaddr *)&host_address, sizeof(host_address)) < 0) {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "Failed bind the server socket with specific IP and port");
        writeToLog(INFO, buf);
        retVal = 1;
    }

    // Listen for incoming connections with allow 10 connection
    if (listen(server_fd, 10) < 0) {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "The server socket failed to linsten incoming connection. client_socketMax Listen Connection: 10");
        writeToLog(INFO, buf);
        retVal = 1;
    }

    while (true) {
        // Accept a new connection
        client_socket = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t*)&clientAddressLen);

        // Get client IP address
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_address.sin_addr), client_ip, INET_ADDRSTRLEN);

        // Get client port number
        int client_port = ntohs(client_address.sin_port);

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "Server Socket Client Connected. IP [%s] PORT [%d]", client_ip, client_port);
        writeToLog(INFO, buf);
        
        // Create a new thread to handle the client
        thread client_handler(handle_client_conn, client_socket);
        client_handler.detach(); // Detach the thread to run independently
    }

    // Close sockets
    close(server_fd);
}

void handle_client_event(int* client_sock, string recData)
{
    MCC118HatInfo MCC118HatInfo;
    char buf[DEFAULT_BUFLEN] = {0};
    char sendData[DEFAULT_BUFLEN] = {0};
    bool isSpecialCode = false;
    double vol = 0.0;
    int iDelToken = 0;
    string delimeter = "|:|";
    string sTemp = "";

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s", recData.c_str());

    string firstToken = recData.substr(0, recData.find(delimeter));

    const int iCode = getSRVSOK_CODE(firstToken);

    switch (iCode)
    {
    case A0001:
        memset(sendData, 0, sizeof(sendData));
        sprintf(sendData, "R0001|:|The Raspi5 is rebooting.");
        system("sudo reboot");
        break;
    
    case A0002:
        memset(sendData, 0, sizeof(sendData));
        sprintf(sendData, "R0002|:|NAME=%s;VERSION=%s;MODIFY_DATE=%s", sProgramName.c_str(), sVersion.c_str(), sModifyDate.c_str());
        break;
    
    case A8888:
        for(int i =0; i < structMCC118HatInfo.iTotalChannel; i++)
        {
            vol = MCC118_readChannel(structMCC118HatInfo.address, i);
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%d=%3.3f;", i, vol);
            sTemp += buf;
        }
        memset(sendData, 0, sizeof(sendData));
        sprintf(sendData, "%s", sTemp.c_str());
        break;
    
    case A9999:
        vol = MCC118_readChannel(structMCC118HatInfo.address, 0);
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "The Address[%d] -> Channel[%d] -> Voltage[%3.3f]", 0, 0, vol);
        writeToLog(INFO, buf);

        memset(sendData, 0, sizeof(sendData));
        sprintf(sendData, "valid=1;value=%3.3f;", vol);
        break;

    default:
        vol = MCC118_readChannel(structMCC118HatInfo.address, 0);
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "The Address[%d] -> Channel[%d] -> Voltage[%3.3f]", 0, 0, vol);
        writeToLog(INFO, buf);

        memset(sendData, 0, sizeof(sendData));
        sprintf(sendData, "valid=1;value=%3.3f;", vol);
        break;
    }

    memset(buf, 0, sizeof(buf));
    sprintf(buf, "SendData: [%s]", sendData);
    writeToLog(INFO, buf);
    send(*client_sock, sendData, sizeof(sendData), 0);
}

int getSRVSOK_CODE(string sCode)
{
    int ret = NOCODE;

    if (sCode == "A0001")
    {
        ret = A0001;
    }
    else if(sCode == "A0002")
    {
        ret = A0002;
    }
    else if(sCode == "A0003")
    {
        ret = A0003;
    }
    else if(sCode == "A0004")
    {
        ret = A0004;
    }
    else if(sCode == "A0005")
    {
        ret = A0005;
    }
    else if(sCode == "A8888")
    {
        ret = A8888;
    }
    else if(sCode == "pcmeasure.lpt1.1")
    {
        ret = A9999;
    }

    return ret;
}

