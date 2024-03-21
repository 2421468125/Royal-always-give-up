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
	void Draw_card(int n);//�ѳ鿨����ʾ�ֿ�
	void ShowMonster();
	void ShowTemCards();
	void Use_card(Cards* card);
	void ShowDrawCards(Ref* pSender, Vector<Cards*>* cards);
	void ShowDiscards(Ref* pSender, Vector<Cards*>* cards);
	void ShowConsumeCards(Ref* pSender, Vector<Cards*>* cards);
	void TurnEnd(Ref* pSender);
	void Fight_end();
	void update_card_num();
	//��Ϊ������ѡ���ʼ���������俨����������Ĭ�ϳ�ʼ�����ǲ��޶�value�����俨���ǲ��޶�type
	static Cards* Choose_random_card(Cards::Cvalue value, Cards::Ctype type,Cards::Ccolour colour);
	void Choose_random_enemy();
	void Reward();//ս��������������,���һָ�����״̬���Ƶ�
	void Card_Reward(Ref* pSender, bool &has_chosen);//ѡ��������
	CREATE_FUNC(FightScene);
	
};
#endif 

