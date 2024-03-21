#ifndef __GLOBALVALUE_H__
#define __GLOBALVALUE_H__
#include "cocos2d.h"
#include "GameMap.h"
#include "Cards.h"
USING_NS_CC;
//专门用来存放全局变量，并调用静态方法
class GlobalValue : public Ref
{

public:
	virtual bool init() { return true; }
	static void set_hero(HeroBase* h);
	static void set_map(GameMap* m);
	static void card_vector_init();
	static void enemy_map_init();
	static void game_init() { Cards::card_init_(); card_vector_init(); enemy_map_init();}

	CREATE_FUNC(GlobalValue);
};
namespace CardVector {
	extern Vector<Cards*> red_attack_common;
	extern Vector<Cards*> red_attack_value;
	extern Vector<Cards*> red_attack_rare;
	extern Vector<Cards*> red_attack;
	extern Vector<Cards*> red_skill_common;
	extern Vector<Cards*> red_skill_value;
	extern Vector<Cards*> red_skill_rare;
	extern Vector<Cards*> red_skill;
	extern Vector<Cards*> red_power_value;//能力牌没有普通卡
	extern Vector<Cards*> red_power_rare;
	extern Vector<Cards*> red_power;
	extern Vector<Cards*> red_card;
	//白卡就不分稀有度了，懒了
	extern Vector<Cards*> grey_attack;
	extern Vector<Cards*> grey_skill;
	extern Vector<Cards*> grey_power;
	extern Vector<Cards*> grey_card;
};
#endif 

