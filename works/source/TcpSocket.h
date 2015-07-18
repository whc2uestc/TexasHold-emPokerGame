#ifndef TCPSOCKET_H_
#define TCPSOCKET_H_
#include "Type.h"


enum STATUS connectToServer(char* serverIP,unsigned short serverPort,char* playerIP,unsigned short playerPort);
int writeMessageToServer(char *message);
int readMessageFromServer(char* buff);
void closeSocket();

#endif
