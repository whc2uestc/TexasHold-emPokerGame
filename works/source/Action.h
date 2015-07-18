#ifndef _ACTION_H_
#define _ACTION_H_
#include<iostream>
#include<vector>
#include<algorithm>
#include"Type.h"
using namespace std;

ActionEnum HoldCard_Action(HandleMsg *handlemsg);
ActionEnum FlopCard_Action(HandleMsg *handlemsg);
ActionEnum TurnCard_Action(HandleMsg *handlemsg);
ActionEnum ReverCard_Action(HandleMsg *handlemsg);

STATUS MyAction();

//按大小顺序依次判断
//int King_Straight_Flust(vector<int> & number,vector<int> & coler);//皇家同花顺
//int Straight_Flust(vector<int> & number,vector<int> & coler); //同花顺
int King_Straight_Flust(vector<int> & number,vector<int> & coler,HandleMsg *handlemsg);//皇家同花顺
int Straight_Flust(vector<int> & number,vector<int> & coler,HandleMsg *handlemsg); //同花顺
int Four_OF_A_KIND(vector<int> & vec);//四条
int Full_House(vector<int> & vec);//葫芦
int Flust(vector<int> & vec);    //同花
int Straight(vector<int> & vec);  //顺子
int Three_OF_A_KIND(vector<int> & vec);//三条
int Two_Pair(vector<int> & vec);//两对
int One_Pair(vector<int> & vec);//一对
int High_Card(vector<int> & vec);//高牌

ActionEnum  pre_Check(HandleMsg *handlemsg,double coef);
ActionEnum  pre_Raise(HandleMsg *handlemsg,double coef1,double coef2);
int ProStraight(vector<int> & vec);//潜在顺子

#endif
