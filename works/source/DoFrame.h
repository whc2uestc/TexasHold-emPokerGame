#ifndef DOFRAME_H_
#define DOFRAME_H_
char* StrLTrim(char *pStr);
void seatInfoMsgHandle(char *pStr);
void blindMsgHandle(char *pStr);
void holdCardsMsgHandle(char *pStr);
void inquireMsgHandle(char *pStr);
void flopMsgHandle(char *pStr);
void turnMsgHandle(char *pStr);
void riverMsgHandle(char *pStr);
void showdownMsgHandle(char *pStr);
void potWinMsgHandle(char *pStr);
char* StrTabToSpace(char *pStr);
void StrCharToSpace(char *pStr,char chr);
//void deFrame(char* recvMsg);
void doActionMsg(char *message);

void doRegisterMsg(char *message,char* playerID);
void gameOverMsgHandle();

int getMinCallNum();
int getRanking();
STATUS doJudgeCheck();
//char* doRegisterMsg(char* playerID);
#endif
