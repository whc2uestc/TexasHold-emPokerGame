#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Type.h"
#include "DoFrame.h"


int getMinCallNum()
{
	//返回投注前其他在玩选手本次最大的投注额
	int minNum = 0;
	int i=0;
	for(i=0;i<8;i++)
	{
		if((pHandle.otherPlayer[i].status ==PLAYING) && (pHandle.otherPlayer[i].pid != myID)){
			if(minNum < (pHandle.otherPlayer[i].bet - pHandle.otherPlayer[i].ebet)){
				minNum = pHandle.otherPlayer[i].bet - pHandle.otherPlayer[i].ebet;
			}
		}
	}
	
	return minNum ;
}
int getRanking(){
	int rankNum = 1;
	int myMoney = pHandle.otherPlayer[pHandle.mySeat].jetton + pHandle.otherPlayer[pHandle.mySeat].money;
	for(int i=0; i < 8; i++){
		if(pHandle.otherPlayer[i].status != ELIMINATE && (pHandle.otherPlayer[i].jetton+pHandle.otherPlayer[i].money) > myMoney)
			rankNum ++;
	}
	return rankNum;
}
STATUS doJudgeCheck(){
	if(2==pHandle.mySeat && 40==pHandle.minCallNum){
		return TRUE;
	}	
	for(int i=0;i<7;i++){
		if(pHandle.otherPlayer[(pHandle.mySeat+7-i)%8].status == PLAYING)
		{
		if(pHandle.otherPlayer[(pHandle.mySeat+7-i)%8].action == CHECK)
			return TRUE;
		}else{
			return FALSE;
		}

	}
}
void doRegisterMsg(char *message,char* playerID){
	strcpy(message,"reg: ");
	strcat(message,playerID);
	strcat(message," HelloFuture \n");
}
void doActionMsg(char *message){
	char raiseNum[6];
	switch(pHandle.action){
	case CHECK: strcpy(message,"check \n"); break;
	case CALL:  strcpy(message,"call \n"); break;
	case RAISE: 
		strcpy(message,"raise ");
		sprintf(raiseNum,"%d\n",pHandle.raiseNum); 
		strcat(message,raiseNum);
		break;
	case ALLIN: strcpy(message,"all_in \n"); break;
	case FOLD:  strcpy(message,"fold \n"); break;
	default:    strcpy(message,"fold \n"); break;
	}
}

char* StrLTrim(char *pStr){
	
	int i = 0;
	char *lStr = pStr;
	while(pStr[i] != '\0'){
		if(pStr[i] != ' ' && pStr[i] != '\t' && pStr[i] != '\n'){
			break;
		}else{
			lStr++;
		}
		i++;
	}
	return lStr;
}

char* StrTabToSpace(char *pStr){
	int i =0;
	char *ppStr = pStr;
	while(pStr[i] != '\0'){
		if(pStr[i] == '\t' || pStr[i] == '\n')
			ppStr[i] = ' ';
		i ++;
	}
	return ppStr;
}

void StrCharToSpace(char *pStr,char chr){
	int i = 0;
	while(pStr[i] != '\0'){
		if(pStr[i] == chr)
			pStr[i] = ' ';
		i++;
	}
}


void seatInfoMsgHandle(char *pStr){

	char *pButton = NULL;
	char *p[30] = {0};
	int i = 0;
	int pCount = 0;
	
	StrTabToSpace(pStr);
	pButton = strstr(pStr,"button:");
	strtok(pButton," ");

	while((p[i] = strtok(NULL, " "))){
		if(0 == strcmp(p[i],"/seat")){
			p[i] = NULL;
			break;
		} 
		if(0 == strcmp(p[i],"small") || 0 == strcmp(p[i],"big") || 0 == strcmp(p[i],"blind:"))
			continue;
		if(i > 25)
			break;
		i++;	
	}
	pCount = i / 3;
	for(i = 0; i<pCount; i++){
		pHandle.otherPlayer[i].pid = atoi(p[3*i]);
		pHandle.otherPlayer[i].jetton = atoi(p[3*i+1]);
		pHandle.otherPlayer[i].money = atoi(p[3*i+2]);
		pHandle.otherPlayer[i].status = PLAYING;
	}
	for(i = 0; i<pCount; i++){
		if(pHandle.otherPlayer[i].pid == myID){
			pHandle.mySeat = i;
			pHandle.myPlayer.jetton = pHandle.otherPlayer[i].jetton;	
			pHandle.myPlayer.money = pHandle.otherPlayer[i].money;
			pHandle.myPlayer.status = pHandle.otherPlayer[i].status;
		}
	}
	pHandle.alivePlayerNum = pCount;
	if(0 == pHandle.inning)
		pHandle.regPlayerNum = pCount;
}
void blindMsgHandle(char *pStr){
	char *ppStr = strstr(pStr,"blind/");
	char *pBlindMsg = StrTabToSpace(ppStr);
	char *pBlind[10] = {0};
	int i = 0;
	StrCharToSpace(pBlindMsg,':');
	strtok(pBlindMsg, " ");
	while((pBlind[i] = strtok(NULL, " "))){
		if(0 == strcmp(pBlind[i],"/blind")){
			pBlind[i] = NULL;
			break;
		} 
		if(i >= 4)
			break;
		i++;		
	}
	
	if(atoi(pBlind[0]) == pHandle.otherPlayer[1].pid)
		pHandle.otherPlayer[1].bet = atoi(pBlind[1]);
	else
		pHandle.otherPlayer[2].bet = atoi(pBlind[1]);

	if(NULL != pBlind[2]){
		if(atoi(pBlind[2]) == pHandle.otherPlayer[1].pid)
			pHandle.otherPlayer[1].bet = atoi(pBlind[3]);
		else
			pHandle.otherPlayer[2].bet = atoi(pBlind[3]);
	}

}
void holdCardsMsgHandle(char *pStr){
	char *ppStr = strstr(pStr,"hold/");
	char *pHoldCard = StrTabToSpace(ppStr);
	char *pCards[5] = {0};
	int i = 0;
	strtok(pHoldCard, " ");
	while((pCards[i] = strtok(NULL, " "))){
		if(0 == strcmp(pCards[i],"/hold")){
			pCards[i] = NULL;
			break;
		} 
		if(i >= 4)
			break;
		i++;		
	}
	for(i=0; i < 2; i++){
		if(0 == strcmp(pCards[i*2],"SPADES"))
			pHandle.holdCard[i].color = SPADES;
		else if(0 == strcmp(pCards[i*2],"HEARTS"))
			pHandle.holdCard[i].color = HEARTS;
		else if(0 == strcmp(pCards[i*2],"CLUBS"))
			pHandle.holdCard[i].color = CLUBS;
		else if(0 == strcmp(pCards[i*2],"DIAMONDS"))
			pHandle.holdCard[i].color = DIAMONDS;

		if(0 == strcmp(pCards[i*2+1],"J"))
			pHandle.holdCard[i].num = 11;
		else if(0 == strcmp(pCards[i*2+1],"Q"))
			pHandle.holdCard[i].num = 12;
		else if(0 == strcmp(pCards[i*2+1],"K"))
			pHandle.holdCard[i].num = 13;
		else if(0 == strcmp(pCards[i*2+1],"A"))
			pHandle.holdCard[i].num = 14;
		else
			pHandle.holdCard[i].num = atoi(pCards[i*2+1]);
	}
}
void inquireMsgHandle(char *ppStr){
	char *pInquire = strstr(ppStr,"inquire/");
	char *ppInquire[41] = {0};
	int i = 0,pCount = 0, pPotFlag = 0,j = 0, pFoldNum = 0;
	char *pPot = NULL;
	char *pStr = StrTabToSpace(pInquire);

	strtok(pStr," ");
	while(ppInquire[i] = strtok(NULL," ")){
		if(0 == strcmp(ppInquire[i],"total") || i >= 54){
			ppInquire[i] = NULL;
			break;
		}
		if(i >= 40)
			break;
		i++;
	}
	pCount = i / 5;
	strtok(NULL," ");
	pPot = strtok(NULL," ");
	for(j = 0; j < pCount; j++){
		for(i = 0; i < 8; i++){
			if(pHandle.otherPlayer[(pHandle.mySeat+7-i)%8].pid == atoi(ppInquire[j*5])){
				pHandle.otherPlayer[(pHandle.mySeat+7-i)%8].jetton = atoi(ppInquire[j*5+1]);
				pHandle.otherPlayer[(pHandle.mySeat+7-i)%8].money = atoi(ppInquire[j*5+2]);
				pHandle.otherPlayer[(pHandle.mySeat+7-i)%8].ebet = pHandle.otherPlayer[(pHandle.mySeat+7-i)%8].bet;	
				pHandle.otherPlayer[(pHandle.mySeat+7-i)%8].bet = atoi(ppInquire[j*5+3]);
				pHandle.otherPlayer[(pHandle.mySeat+7-i)%8].status = PLAYING;
				if(0 == strcmp(ppInquire[j*5+4],"blind"))
					pHandle.otherPlayer[(pHandle.mySeat+7-i)%8].action = BLIND;
				else if(0 == strcmp(ppInquire[j*5+4],"check"))
					pHandle.otherPlayer[(pHandle.mySeat+7-i)%8].action = CHECK;
				else if(0 == strcmp(ppInquire[j*5+4],"call"))
					pHandle.otherPlayer[(pHandle.mySeat+7-i)%8].action = CALL;
				else if(0 == strcmp(ppInquire[j*5+4],"raise"))
					pHandle.otherPlayer[(pHandle.mySeat+7-i)%8].action = RAISE;
				else if(0 == strcmp(ppInquire[j*5+4],"all_in"))
					pHandle.otherPlayer[(pHandle.mySeat+7-i)%8].action = ALLIN;
				else if(0 == strcmp(ppInquire[j*5+4],"fold")){
					fprintf(mylog,"into fold!!!\n");
					pHandle.otherPlayer[(pHandle.mySeat+7-i)%8].action = FOLD;
					pHandle.otherPlayer[(pHandle.mySeat+7-i)%8].status = FOLDED;
					pFoldNum ++;
				}
				break;
			}
		}
	}

	pHandle.totalPot = atoi(pPot);
	pHandle.remainPlayerNum  = pHandle.alivePlayerNum - pFoldNum;

	for(i = 0; i<pHandle.regPlayerNum; i++){
		if(pHandle.otherPlayer[i].pid == myID){
			pHandle.mySeat = i;
			pHandle.myPlayer.jetton = pHandle.otherPlayer[i].jetton;	
			pHandle.myPlayer.money = pHandle.otherPlayer[i].money;
			pHandle.myPlayer.status = pHandle.otherPlayer[i].status;
			pHandle.myPlayer.bet = pHandle.otherPlayer[i].bet;
			pHandle.myPlayer.ebet = pHandle.otherPlayer[i].ebet;
			pHandle.myPlayer.action = pHandle.otherPlayer[i].action;
		}
	}

}
void flopMsgHandle(char *pStr){
	char *pFlop = strstr(pStr,"flop/");
	char *pFlopCard = StrTabToSpace(pFlop);
	char *pCards[7] = {0};
	int i = 0;
	strtok(pFlopCard, " ");
	while((pCards[i] = strtok(NULL, " "))){
		if(0 == strcmp(pCards[i],"/flop")){
			pCards[i] = NULL;
			break;
		} 
		if(i >= 6)
			break;
		i++;		
	}
	for(i=0; i < 3; i++){
		if(0 == strcmp(pCards[i*2],"SPADES"))
			pHandle.flopCard[i].color = SPADES;
		else if(0 == strcmp(pCards[i*2],"HEARTS"))
			pHandle.flopCard[i].color = HEARTS;
		else if(0 == strcmp(pCards[i*2],"CLUBS"))
			pHandle.flopCard[i].color = CLUBS;
		else if(0 == strcmp(pCards[i*2],"DIAMONDS"))
			pHandle.flopCard[i].color = DIAMONDS;

		if(0 == strcmp(pCards[i*2+1],"J"))
			pHandle.flopCard[i].num = 11;
		else if(0 == strcmp(pCards[i*2+1],"Q"))
			pHandle.flopCard[i].num = 12;
		else if(0 == strcmp(pCards[i*2+1],"K"))
			pHandle.flopCard[i].num = 13;
		else if(0 == strcmp(pCards[i*2+1],"A"))
			pHandle.flopCard[i].num = 14;
		else
			pHandle.flopCard[i].num = atoi(pCards[i*2+1]);
	}
}
void turnMsgHandle(char *pStr){
	char *pTurn = strstr(pStr,"turn/");
	char *pTurnCard = StrTabToSpace(pTurn);
	char *pCards[3] = {0};
	int i = 0;
	strtok(pTurnCard, " ");
	while((pCards[i] = strtok(NULL, " "))){
		if(0 == strcmp(pCards[i],"/turn")){
			pCards[i] = NULL;
			break;
		} 
		if(i >= 2)
			break;
		i++;		
	}

	if(0 == strcmp(pCards[0],"SPADES"))
		pHandle.turnCard.color = SPADES;
	else if(0 == strcmp(pCards[0],"HEARTS"))
		pHandle.turnCard.color = HEARTS;
	else if(0 == strcmp(pCards[0],"CLUBS"))
		pHandle.turnCard.color = CLUBS;
	else if(0 == strcmp(pCards[0],"DIAMONDS"))
		pHandle.turnCard.color = DIAMONDS;

	if(0 == strcmp(pCards[1],"J"))
		pHandle.turnCard.num = 11;
	else if(0 == strcmp(pCards[1],"Q"))
		pHandle.turnCard.num = 12;
	else if(0 == strcmp(pCards[1],"K"))
		pHandle.turnCard.num = 13;
	else if(0 == strcmp(pCards[1],"A"))
		pHandle.turnCard.num = 14;
	else
		pHandle.turnCard.num = atoi(pCards[1]);

}
void riverMsgHandle(char *pStr){
	char *pRiver = strstr(pStr,"river/");
	char *pRiverCard = StrTabToSpace(pRiver);
	char *pCards[3] = {0};
	int i = 0;
	strtok(pRiverCard, " ");
	while((pCards[i] = strtok(NULL, " "))){
		if(0 == strcmp(pCards[i],"/river")){
			pCards[i] = NULL;
			break;
		} 
		if(i >= 2)
			break;
		i++;		
	}

	if(0 == strcmp(pCards[0],"SPADES"))
		pHandle.riverCard.color = SPADES;
	else if(0 == strcmp(pCards[0],"HEARTS"))
		pHandle.riverCard.color = HEARTS;
	else if(0 == strcmp(pCards[0],"CLUBS"))
		pHandle.riverCard.color = CLUBS;
	else if(0 == strcmp(pCards[0],"DIAMONDS"))
		pHandle.riverCard.color = DIAMONDS;

	if(0 == strcmp(pCards[1],"J"))
		pHandle.riverCard.num = 11;
	else if(0 == strcmp(pCards[1],"Q"))
		pHandle.riverCard.num = 12;
	else if(0 == strcmp(pCards[1],"K"))
		pHandle.riverCard.num = 13;
	else if(0 == strcmp(pCards[1],"A"))
		pHandle.riverCard.num = 14;
	else
		pHandle.riverCard.num = atoi(pCards[1]);
}
void showdownMsgHandle(char *pStr){

}
void potWinMsgHandle(char *pStr){

}

void gameOverMsgHandle(){
	pHandle.gameOverFlag = TRUE;
}

