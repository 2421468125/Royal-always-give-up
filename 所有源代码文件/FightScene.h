#ifndef __FIGHTSCENE_H__
#define __FIGHTSCENE_H__
#include "cocos2d.h"
#include "cocos-ext.h"
#include "CharacterBase.h"
#include <cstdlib>
USING_NS_CC;
USING_NS_CC_EXT;
class FightScene :public Scene
{
public:
	static Scene* createScene();
	virtual bool init();
	void Draw_card(int n);//把抽卡和显示分开
	void ShowMonster();
	void ShowTemCards();
	void Use_card(Cards* card);
	void ShowDrawCards(Ref* pSender, Vector<Cards*>* cards);
	void ShowDiscards(Ref* pSender, Vector<Cards*>* cards);
	void ShowConsumeCards(Ref* pSender, Vector<Cards*>* cards);
	void TurnEnd(Ref* pSender);
	void Fight_end();
	void update_card_num();
	//因为不可能选择初始卡或者诅咒卡，所以这里默认初始卡就是不限定value，诅咒卡就是不限定type
	static Cards* Choose_random_card(Cards::Cvalue value, Cards::Ctype type,Cards::Ccolour colour);
	void Choose_random_enemy();
	void Reward();//战斗结束奖励界面,并且恢复所有状态卡牌等
	void Card_Reward(Ref* pSender, bool &has_chosen);//选择奖励卡牌
	CREATE_FUNC(FightScene);
	
};
#endif 

