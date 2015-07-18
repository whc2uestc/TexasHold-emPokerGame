#include <stdio.h>
#include <stdlib.h>
#include "Type.h"
#include "DoFrame.h"
#include "TcpSocket.h"
#include <string.h>
#include "Action.h"

HandleMsg pHandle;
int myID;
FILE *mylog;

int main(int argc,char *argv[]){
	char *serverIP = NULL;
	unsigned short serverPort;
	char *playerIP = NULL;
	unsigned short playerPort;
	char *playerID = NULL;
	char recvMsg[1024];
	char handleMsg[1024];
	char *headMsg = NULL;
	char headHandleMsg[1024];
	char *head = NULL;
	char sendMsg[50];
	char fileName[12];
	int i = 0;
	enum STATUS headFlag = FALSE;
	
	if(argc < 6){
		printf("input parameter error!");
		exit(1);
	}
	else {
		serverIP = argv[1];
		serverPort = atoi(argv[2]);
		playerIP = argv[3];
		playerPort = atoi(argv[4]);
		playerID = argv[5];
		myID = atoi(playerID);
	
	}


	fileName[0] = 'l';
	fileName[1] = 'o';
	fileName[2] = 'g';
	fileName[3] = *playerID;
	fileName[4] = *(playerID+1);
	fileName[5] = *(playerID+2);
	fileName[6] = *(playerID+3);
	fileName[7] = '.';
	fileName[8] = 't';
	fileName[9] = 'x';
	fileName[10] = 't';
	fileName[11] = '\0';
	mylog = fopen(fileName,"w+");
	if(NULL != mylog)
		fprintf(mylog,"open file success!!");
	fprintf(mylog,"%s\n",serverIP);
	fprintf(mylog,"%d\n",serverPort);
	fprintf(mylog,"%s\n",playerIP);
	fprintf(mylog,"%d\n",playerPort);
	fprintf(mylog,"%s\n",playerID);
	
	if(connectToServer(serverIP,serverPort,playerIP,playerPort)){
		memset(sendMsg,'\0',50);
		doRegisterMsg(sendMsg,playerID);
		fprintf(mylog,"%s\n",sendMsg);
		int len;
		len = writeMessageToServer(sendMsg);
		fprintf(mylog,"%d\n",len);		
	}
	
	while(FALSE == pHandle.gameOverFlag){
		memset(recvMsg,'\0',1024);
		memset(sendMsg,'\0',50);
		readMessageFromServer(recvMsg);
		fprintf(mylog,"%s",recvMsg);
		
		headMsg = recvMsg;
		head = headHandleMsg;
		for(i = 0; i < 10; i++){
			strcpy(headHandleMsg,headMsg);
			head = strtok(headHandleMsg,"/");	
			//exit(0);
			if(NULL != strstr(head,"seat")){
				headMsg = strstr(recvMsg,"/seat");
				headMsg += 5;
				headFlag = TRUE;

				strcpy(handleMsg,recvMsg);
				for(int k = 0; k < 8; k++){
					pHandle.otherPlayer[k].bet = 0;
					pHandle.otherPlayer[k].ebet = 0;
					pHandle.otherPlayer[k].action = NOACTION;
					pHandle.otherPlayer[k].status = ELIMINATE;
					pHandle.otherPlayer[k].pid = 0;
				}
				pHandle.action = CHECK;
				pHandle.state = 0;
				pHandle.count = 0;
				seatInfoMsgHandle(handleMsg);
				
				fprintf(mylog,"into seatMsg handle!%d\n",pHandle.alivePlayerNum);
				MyAction();
				pHandle.ranking = getRanking();
				pHandle.inning ++;
			//exit(0);
			}	
			else if(NULL != strstr(head,"blind")){
				headMsg = strstr(recvMsg,"/blind");
				headMsg += 6;
				headFlag = TRUE;

				strcpy(handleMsg,recvMsg);
				blindMsgHandle(handleMsg);
				fprintf(mylog,"into blindMsg hangle!\n");
				//exit(0);
			}
			else if(NULL != strstr(head,"hold")){
				headMsg = strstr(recvMsg,"/hold");
				headMsg += 5;
				headFlag = TRUE;

				strcpy(handleMsg,recvMsg);
				holdCardsMsgHandle(handleMsg);
				fprintf(mylog,"into holdCards handle!\n");
				pHandle.state = 1;
				pHandle.count = 0;
				for(int k = 0; k < 8; k++){
					pHandle.otherPlayer[k].bet = 0;
					pHandle.otherPlayer[k].ebet = 0;
				}	
				fprintf(mylog,"hold card:%d %d %d %d\n",pHandle.holdCard[0].num,pHandle.holdCard[0].color,pHandle.holdCard[1].num,pHandle.holdCard[1].color);
				//exit(0);
			}
			else if(NULL != strstr(head,"inquire")){
				headMsg = strstr(recvMsg,"/inquire");
				headMsg += 8;
				headFlag = TRUE;

				strcpy(handleMsg,recvMsg);
				inquireMsgHandle(handleMsg);
				//fprintf(mylog,"into inquire handle!%d %d \n",pHandle.otherPlayer[0].ebet,pHandle.otherPlayer[0].ebet);
				pHandle.minCallNum = getMinCallNum();
				
				fprintf(mylog,"regNum:%d,inning:%d\n",pHandle.regPlayerNum,pHandle.inning);

				for(int kk = 0; kk < 8; kk ++){
					fprintf(mylog,"id:%d,jetton:%d,money:%d,bet:%d,ebet:%d,action:%d,status:%d\n",pHandle.otherPlayer[kk].pid,pHandle.otherPlayer[kk].jetton,pHandle.otherPlayer[kk].money,pHandle.otherPlayer[kk].bet,pHandle.otherPlayer[kk].ebet,pHandle.otherPlayer[kk].action,pHandle.otherPlayer[kk].status);
				}
				fprintf(mylog,"id:%d,jetton:%d,money:%d,bet:%d,ebet:%d,action:%d,status:%d\n",pHandle.myPlayer.pid,pHandle.myPlayer.jetton,pHandle.myPlayer.money,pHandle.myPlayer.bet,pHandle.myPlayer.ebet,pHandle.myPlayer.action,pHandle.myPlayer.status);
				fprintf(mylog,"minCall:%d,mySeat:%d,Rank:%d\n",pHandle.minCallNum,pHandle.mySeat,pHandle.ranking);
			
				
				if(pHandle.action != ALLIN && pHandle.action != FOLD){
					MyAction();
					if((TRUE==doJudgeCheck()) && (FOLD==pHandle.action))	
						pHandle.action = CHECK;
					fprintf(mylog,"%d\n",pHandle.action);
					doActionMsg(sendMsg);
					fprintf(mylog,"%s\n",sendMsg);
					int len;
					len = writeMessageToServer(sendMsg);
					fprintf(mylog,"%d\n",len);
				}	
				pHandle.count ++;
				//exit(0);
			}
			else if(NULL != strstr(head,"flop")){
				headMsg = strstr(recvMsg,"/flop");
				headMsg += 5;
				headFlag = TRUE;

				strcpy(handleMsg,recvMsg);
				flopMsgHandle(handleMsg);
				fprintf(mylog,"into flopCards handle!\n");
				pHandle.state = 2;
				pHandle.count = 0;
				for(int k = 0; k < 8; k++){
					pHandle.otherPlayer[k].bet = 0;
					pHandle.otherPlayer[k].ebet = 0;
				}	
				fprintf(mylog,"flop card:%d %d %d %d %d %d\n",pHandle.flopCard[0].num,pHandle.flopCard[0].color,pHandle.flopCard[1].num,pHandle.flopCard[1].color,pHandle.flopCard[2].num,pHandle.flopCard[2].color);
				//exit(0);
			}
			else if(NULL != strstr(head,"turn")){
				headMsg = strstr(recvMsg,"/turn");
				headMsg += 5;
				headFlag = TRUE;

				strcpy(handleMsg,recvMsg);
				turnMsgHandle(handleMsg);
				fprintf(mylog,"into turnCards handle!\n");
				pHandle.state = 3;
				pHandle.count = 0;
				for(int k = 0; k < 8; k++){
					pHandle.otherPlayer[k].bet = 0;
					pHandle.otherPlayer[k].ebet = 0;
				}	
				fprintf(mylog,"turn card:%d %d\n",pHandle.turnCard.num,pHandle.turnCard.color);
				//exit(0);
			}
			else if(NULL != strstr(head,"river")){
				headMsg = strstr(recvMsg,"/river");
				headMsg += 6;
				headFlag = TRUE;

				strcpy(handleMsg,recvMsg);
				riverMsgHandle(handleMsg);
				fprintf(mylog,"into riverCards handle!\n");
				pHandle.state = 4;
				pHandle.count = 0;
				for(int k = 0; k < 8; k++){
					pHandle.otherPlayer[k].bet = 0;
					pHandle.otherPlayer[k].ebet = 0;
				}	
				fprintf(mylog,"river card:%d %d\n",pHandle.riverCard.num,pHandle.riverCard.color);
			}
			else if(NULL != strstr(head,"showdown")){
				headMsg = strstr(recvMsg,"/showdown");
				headMsg += 8;
				headFlag = TRUE;

				strcpy(handleMsg,recvMsg);
				showdownMsgHandle(handleMsg);
				fprintf(mylog,"into showdown handle!\n");
			}
			else if(NULL != strstr(head,"pot-win")){
				headMsg = strstr(recvMsg,"/pot-win");
				headMsg += 8;
				headFlag = TRUE;

				strcpy(handleMsg,recvMsg);
				potWinMsgHandle(handleMsg);
				fprintf(mylog,"into pot-win handle!\n");
			}
			if(FALSE == headFlag)
				break;
		}
		if(NULL != strstr(recvMsg,"game-over")){
			gameOverMsgHandle();
			fprintf(mylog,"into game-over handle!\n");
			if(TRUE == pHandle.gameOverFlag)
				fprintf(mylog,"gameOverFlag = TRUE");
			closeSocket();
			exit(0);
		}
		
	}
	
	fclose(mylog);
	exit(0);
	return 0;
}
