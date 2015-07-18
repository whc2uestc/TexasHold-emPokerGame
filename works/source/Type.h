
#ifndef TYPE_H_
#define TYPE_H_

enum STATUS{FALSE,TRUE};
enum ActionEnum {NOACTION,CHECK,CALL,RAISE,FOLD,BLIND,ALLIN}; 
enum ColorEnum{SPADES,HEARTS,CLUBS,DIAMONDS};
enum PlayerSatus{PLAYING,FOLDED,ELIMINATE};
typedef struct PlayerInformation{
	int pid;//ID
	int seat;//座位信息，按逆时针座次由近及远排列1、2、3、4、5、6、7，上家排在第一个
	int jetton;//手中筹码
	int money;//剩余金币
	int bet;//本手牌累计投注额
	int ebet;
	enum ActionEnum action;//行动
	enum PlayerSatus status;
}PlayerInfo;

typedef struct CardInformation {
	int num;
	enum ColorEnum color;
}CardInfo;


typedef struct HandleMessage{
	int state;//这一局中当前进行到第几个阶段  0、1、2、3...... 
	int count;//每一轮的喊注下注的次数累加。
	PlayerInfo otherPlayer[8];
	PlayerInfo myPlayer;
	CardInfo holdCard[2];
	CardInfo flopCard[3];
	CardInfo turnCard;
	CardInfo riverCard;
	enum ActionEnum action;
	int raiseNum;
	int remainPlayerNum;
	int alivePlayerNum;
	int mySeat;//我的位置
	int totalPot;
	int regPlayerNum;//注册玩家数
	int minCallNum; //本次跟注最小金额
	int ranking;
	int inning;//一场比赛中第几局
	enum STATUS gameOverFlag;
}HandleMsg;



extern HandleMsg pHandle;
extern int myID;
extern FILE *mylog;

#endif
