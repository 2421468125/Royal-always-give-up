#ifndef __CARDS_H__
#define __CARDS_H__

#include<string>
#include"cocos2d.h"
#include<vector>

USING_NS_CC;
//两个头文件不能相互包含，所以其中一个要前置声明
class CharacterBase;

class Cards : public cocos2d::Node{

public:
	enum Ctype {
		ATTACK,
		SKILL,
		POWER,
		CURSE,
		STATUS
	};//卡牌种类
	enum Cvalue {
		INITIAL,
		COMMON,
		VALUE,
		RARE
	};//稀有度
	enum Ccolour {
		RED,
		GREEN,
		BLUE,
		GREY,
		BLACK
	};//颜色，黑色算诅咒卡

private:
	int pos_x=0;
	int pos_y=0;
	int _id;
	int _cost; //耗能
	int _upcost;
	int _temcost;//暂时的耗能，退出战斗后重置
	int cost_last_for = 0;//判断_temcost是一整局还是一个回合，0：一整局，1：一回合
	Ctype _type;
	Cvalue _value;
	Ccolour _colour;
	int _upgrade;
	std::string _ename;
	CharacterBase* _owner;//拥有卡牌的对象
	CharacterBase* _target;//卡牌使用的受体
	virtual void set_description();//为了调用派生类而写的虚函数
	void redraw_cost();
public:
	// 虚无属性：回合结束时未被打出即消耗,没法用成员，因为指向派生类的基类指针无法访问派生类重载成员，只好用虚函数了
	virtual bool is_nihility() { return false; }
	// 固有属性
	virtual bool is_inherence() { return false; }
	const static int TOTAL_CARD_NUMBER = 273;
	virtual void create_card(int id, int cost, int upcost, Ctype type, Cvalue value,
		Ccolour colour, int upgrade, const std::string& ename,
		CharacterBase* owner = nullptr, CharacterBase* target= nullptr);
	virtual bool init() { return true; }
	virtual void use() {};
	
	void set_temcost(int x, int last_for = 0) { _temcost = x; cost_last_for = last_for; redraw_cost(); }
	void set_owner(CharacterBase* chara) { _owner = chara; }
	void set_target(CharacterBase* target) { _target = target; }
	void set_pos(int x, int y) { pos_x = x; pos_y = y; this->setPosition(x, y); }//主要是为了实现能让卡牌自动放回的功能
	Vec2 get_pos() { return Vec2(pos_x, pos_y); }
	int get_id() { return _id; }
	int get_cost() { return _cost; }
	int get_upcost() { return _upcost; }
	int get_temcost() { return _temcost; }
	int get_cost_last_for() { return cost_last_for; }
	Ctype get_type() { return _type; }
	Cvalue get_value() { return _value; }
	Ccolour get_color() { return _colour; }
	int get_upgrade() { return _upgrade; }
	std::string get_ename() { return _ename; }
	CharacterBase* get_owner() { return _owner; }
	CharacterBase* get_target() { return _target; }
	virtual void consume();//消耗卡牌
	void consume_power_card();
	bool handle_remove_buff();
	virtual bool available();//用于判断牌是否可用的函数，因为有些牌有各种要求
	static void card_init_();//总的静态方法
	//下面这些用来存储卡牌的中文名以及中文描述
	static std::vector<std::string> chinese_card_name;
	static void read_chinese_card_name();

	static std::vector<std::string> english_card_name;
	static void read_english_card_name();

	static std::vector<std::string> chinese_card_description;
	static void read_chinese_card_description();

	static std::vector<std::string> chinese_card_updescription;
	static void read_chinese_card_updescription();
	
	virtual Cards* clone(int upgrade = 0) { return nullptr; };//代替复制构造函数
	
	CREATE_FUNC(Cards);// 把card交给cocos内存管理
};
//这里使用了CRTP，创建一个中间类来写一些重复的虚函数模板
template<typename ConcreteCard>
class CloneCard : public Cards
{
public:
	virtual Cards* clone(int upgrade = 0) override;

};
//9
class Anger :public CloneCard<Anger>
{
	int damage = 6;
	int updamage = 8;
	virtual void set_description();
public:
	static Anger* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Anger);
};
//10
class Apotheosis :public CloneCard<Apotheosis>
{
	virtual void set_description();
public:
	static Apotheosis* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Apotheosis);
};

//11
class Armaments :public CloneCard<Armaments>
{
private:
	int defend = 5;
	virtual void set_description();
public:
	static Armaments* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Armaments);
};
//16
class BandageUp :public CloneCard<BandageUp>
{
private:
	int heal = 4;
	int upheal = 6;
	virtual void set_description();
public:
	static BandageUp* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(BandageUp);
};
//19
class Barricade :public CloneCard<Barricade>
{
private:
	virtual void set_description();
public:
	static Barricade* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Barricade);
};

//20
class Bash :public CloneCard<Bash>
{
private:
	int damage = 8;
	int updamage = 10;
	int vulnerable = 2;
	int upvulnerable = 3;
	virtual void set_description();
public:
	static Bash* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Bash);
};

//21
class BattleTrance :public CloneCard<BattleTrance>
{
private:
	int card_num = 3;
	int upcard_num = 4;
	virtual void set_description();
public:
	static BattleTrance* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(BattleTrance);
};

//23
class Berserk :public CloneCard<Berserk>
{
private:
	virtual void set_description();
public:
	static Berserk* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Berserk);
};
//25
class Bite :public CloneCard<Bite>
{
private:
	int damage = 7;
	int updamage = 8;
	int heal = 2;
	int upheal = 3;
	virtual void set_description();
public:
	static Bite* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Bite);
};
//27
class Blind :public CloneCard<Blind>
{
private:
	virtual void set_description();
public:
	static Blind* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Blind);
};

//29，或许可以用自定义事件分发器来控制其耗能
class BloodForBlood : public CloneCard<BloodForBlood>
{
private:
	int damage = 18;
	int updamage = 22;
	virtual void set_description();
public:
	static BloodForBlood* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(BloodForBlood);
};

//30
class BloodLetting : public CloneCard<BloodLetting>
{
private:
	virtual void set_description();
public:
	static BloodLetting* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(BloodLetting);
};
//31
class Blugeon :public CloneCard<Blugeon>
{
private:
	int damage = 32;
	int updamage = 42;
	virtual void set_description();
public:
	static Blugeon* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Blugeon);
};

//33
class BodySlam :public CloneCard<BodySlam>
{
private:
	virtual void set_description();
public:
	static BodySlam* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(BodySlam);
};

//36
class Brutality :public CloneCard<Brutality>
{
private:
	virtual void set_description();
public:
	static Brutality* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Brutality);
};
//39
class Burn :public CloneCard<Burn>
{
private:
	virtual void set_description();
public:
	static Burn* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use(){};
	virtual bool available() { return false; }//不能被打出
	CREATE_FUNC(Burn);
};

//40
class BurningPact : public CloneCard<BurningPact>
{
private:
	int card_num = 2;
	int upcard_num = 3;
	virtual void set_description();
public:
	static BurningPact* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(BurningPact);
};

//45
class Carnage :public CloneCard<Carnage>
{
private:
	int damage = 20;
	int updamage = 28;
	virtual void set_description();
public:
	virtual bool is_nihility() { return true; }
	static Carnage* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Carnage);
};

//50
class Clash :public CloneCard<Clash>
{
private:
	int damage = 14;
	int updamage = 18;
	virtual void set_description();
public:
	static Clash* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	virtual bool available();//条件为手牌都为攻击牌
	CREATE_FUNC(Clash);
};

//51
class Cleave :public CloneCard<Cleave>
{
private:
	int damage = 8;
	int updamage = 11;
	virtual void set_description();
public:
	static Cleave* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Cleave);
};

//53
class Clothesline :public CloneCard<Clothesline>
{
private:
	int damage = 12;
	int updamage = 14;
	int weak = 2;
	int upweak = 3;
	virtual void set_description();
public:
	static Clothesline* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Clothesline);
};
//54
class Clumsy :public CloneCard<Clumsy>
{
public:
	static Clumsy* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use() {};
	virtual bool available() { return false; }//不能被打出
	virtual bool is_nihility() { return true; }
	CREATE_FUNC(Clumsy);
};
//56
class Combust :public CloneCard<Combust>
{
private:
	int damage = 5;
	int updamage = 7;
	virtual void set_description();
public:
	static Combust* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Combust);
};
//62
class Corruption :public CloneCard<Corruption>
{
public:
	static Corruption* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Corruption);
};
//67
class DarkEmbrace :public CloneCard<DarkEmbrace>
{
private:
	virtual void set_description();
public:
	static DarkEmbrace* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(DarkEmbrace);
};
//71
class Dazed :public CloneCard<Dazed>
{
public:
	static Dazed* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use() {};
	virtual bool available() { return false; }//不能被打出
	virtual bool is_nihility() { return true; }
	CREATE_FUNC(Dazed);
};
//73
class Decay :public CloneCard<Decay>
{
public:
	static Decay* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use() {};
	virtual bool available() { return false; }//不能被打出
	CREATE_FUNC(Decay);
};
//74
class DeepBreath :public CloneCard<DeepBreath>
{
private:
	virtual void set_description();
public:
	static DeepBreath* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(DeepBreath);
};
//77
class Defend_R:public CloneCard<Defend_R>
{
private:
	int defend = 5;
	int updefend = 8;
	virtual void set_description();
public:
	static Defend_R* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Defend_R);
};
//80
class DemonForm :public CloneCard<DemonForm>
{
private:
	int strength = 2;
	int upstrength = 3;
	virtual void set_description();
public:
	static DemonForm* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(DemonForm);
};
//82
class Disarm :public CloneCard<Disarm>
{
private:
	int strength = 2;
	int upstrength = 3;
	virtual void set_description();
public:
	static Disarm* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Disarm);
};
//88
class DoubleTap :public CloneCard<DoubleTap>
{
private:
	virtual void set_description();
public:
	static DoubleTap* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(DoubleTap);
};
//89
class Doubt :public CloneCard<Doubt>
{
public:
	static Doubt* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use() {};
	virtual bool available() { return false; }//不能被打出
	CREATE_FUNC(Doubt);
};
//90
class DramaticEntrance :public CloneCard<DramaticEntrance>
{
private:
	int damage = 8;
	int updamage = 12;
	virtual void set_description();
public:
	virtual bool is_inherence() { return true; }
	static DramaticEntrance* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(DramaticEntrance);
};
//91
class Dropkick :public CloneCard<Dropkick>
{
private:
	int damage = 5;
	int updamage = 8;
	int gain_energy = 1;
	virtual void set_description();
public:
	static Dropkick* create_card(int upgrade = 0);//是否升级过的,默认没有
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Dropkick);
};
//92
class DualWield :public CloneCard<DualWield>
{
private:
	virtual void set_description();
public:
	static DualWield* create_card(int upgrade = 0);//是否升级过的,默认没有
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(DualWield);
};
//97
class Enlightment :public CloneCard<Enlightment>
{
private:
	virtual void set_description();
public:
	static Enlightment* create_card(int upgrade = 0);//是否升级过的,默认没有
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Enlightment);
};
//98
class Entrench : public CloneCard<Entrench>
{
private:
	virtual void set_description();
public:
	static Entrench* create_card(int upgrade = 0);//是否升级过的,默认没有
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Entrench);
};
//102
class Evolve : public CloneCard<Evolve>
{
private:
	virtual void set_description();
public:
	static Evolve* create_card(int upgrade = 0);//是否升级过的,默认没有
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Evolve);
};
//103
class Exhume : public CloneCard<Exhume>
{
private:
	virtual void set_description();
public:
	static Exhume* create_card(int upgrade = 0);//是否升级过的,默认没有
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Exhume);
};

//106
class Feed :public CloneCard<Feed>
{
private:
	int damage = 10;
	int updamage = 12;
	int gain_max_health = 3;
	int upgain_max_health = 4;
	virtual void set_description();
public:
	static Feed* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Feed);
};

//107
class FeelNoPain :public CloneCard<FeelNoPain>
{
private:
	int defend = 3;
	int updefend = 4;
	virtual void set_description();
public:
	static FeelNoPain* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(FeelNoPain);
};

//108
class FiendFire :public CloneCard<FiendFire>
{
private:
	int damage = 7;
	int updamage = 10;
	virtual void set_description();
public:
	static FiendFire* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(FiendFire);
};
//109
class Finesse :public CloneCard<Finesse>
{
private:
	int defend = 2;
	int updefend = 4;
	virtual void set_description();
public:
	static Finesse* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Finesse);
};
//111
class FireBreathing :public CloneCard<FireBreathing>
{
private:
	int damage = 6;
	int updamage = 10;
	virtual void set_description();
public:
	static FireBreathing* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(FireBreathing);
};

//113
class FlameBarrier : public CloneCard<FlameBarrier>
{
private:
	int defend = 12;
	int updefend = 16;
	int damage = 4;
	int updamage = 6;
	virtual void set_description();
public:
	static FlameBarrier* create_card(int upgrade = 0);//是否升级过的,默认没有
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(FlameBarrier);
};
//114
class FlashofSteel : public CloneCard<FlashofSteel>
{
private:
	int damage = 3;
	int updamage = 6;
	virtual void set_description();
public:
	static FlashofSteel* create_card(int upgrade = 0);//是否升级过的,默认没有
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(FlashofSteel);
};

//116
class Flex : public CloneCard<Flex>
{
private:
	int strength = 2;
	int upstrength = 4;
	virtual void set_description();
public:
	static Flex* create_card(int upgrade = 0);//是否升级过的,默认没有
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Flex);
};

//123
class Ghostly : public CloneCard<Ghostly>
{
private:
	virtual void set_description();
public:
	virtual bool is_nihility() { return !this->get_upgrade(); }
	static Ghostly* create_card(int upgrade = 0);//是否升级过的,默认没有
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Ghostly);
};
//124
class GhostlyArmor : public CloneCard<GhostlyArmor>
{
private:
	int defend = 10;
	int updefend = 13;
	virtual void set_description();
public:
	virtual bool is_nihility() { return true; }
	static GhostlyArmor* create_card(int upgrade = 0);//是否升级过的,默认没有
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(GhostlyArmor);
};
//128
class GoodInstincts : public CloneCard<GoodInstincts>
{
private:
	int defend = 6;
	int updefend = 9;
	virtual void set_description();
public:
	static GoodInstincts* create_card(int upgrade = 0);//是否升级过的,默认没有
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(GoodInstincts);
};
//130
class Havoc : public CloneCard<Havoc>
{
private:
	virtual void set_description();
public:
	static Havoc* create_card(int upgrade = 0);//是否升级过的,默认没有
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Havoc);
};
//131
class HeadButt :public CloneCard<HeadButt>
{
private:
	int damage = 9;
	int updamage = 12;
	virtual void set_description();
public:
	static HeadButt* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(HeadButt);
};
//133
class HeavyBlade: public CloneCard<HeavyBlade>
{
private:
	int damage = 14;
	int power = 3;
	int uppower = 5;
	virtual void set_description();
public:
	static HeavyBlade* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(HeavyBlade);
};

//136
class Hemokinesis: public CloneCard<Hemokinesis>
{
private:
	int damage = 15;
	int updamage = 20;
	virtual void set_description();
public:
	static Hemokinesis* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Hemokinesis);
};

//140
class Immolate : public CloneCard<Immolate>
{
private:
	int damage = 21;
	int updamage = 28;
	virtual void set_description();
public:
	static Immolate* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Immolate);
};
//141
class Impervious :public CloneCard<Impervious>
{
private:
	int defend = 30;
	int updefend = 40;
	virtual void set_description();
public:
	static Impervious* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Impervious);
};

//142
class InfernalBlade :public CloneCard<InfernalBlade>
{
private:
	virtual void set_description();
public:
	static InfernalBlade* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(InfernalBlade);
};
//144
class Inflame :public CloneCard<Inflame>
{
private:
	int strength = 2;
	int upstrength = 3;
	virtual void set_description();
public:
	static Inflame* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Inflame);
};
//145
class Injury :public CloneCard<Injury>
{
public:
	static Injury* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use() {};
	virtual bool available() { return false; }//不能被打出
	CREATE_FUNC(Injury);
};
//146
class Intimidate :public CloneCard<Intimidate>
{
private:
	int weak = 1;
	int upweak = 2;
	virtual void set_description();
public:
	static Intimidate* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Intimidate);
};

//147
class IronWave : public CloneCard<IronWave>
{
private:
	int damage = 5;
	int updamage = 7;
	int defend = 5;
	int updefend = 7;
	virtual void set_description();
public:
	static IronWave* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(IronWave);
};
//148
class Jax : public CloneCard<Jax>
{
private:
	int strength = 2;
	int upstrength = 3;
	virtual void set_description();
public:
	static Jax* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Jax);
};
//149
class JackOfAllTrades : public CloneCard<JackOfAllTrades>
{
private:
	virtual void set_description();
public:
	static JackOfAllTrades* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(JackOfAllTrades);
};
//150
class Juggernaut : public CloneCard<Juggernaut>
{
private:
	int damage = 5;
	int updamage = 7;
	virtual void set_description();
public:
	static Juggernaut* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Juggernaut);
};
//153
class LimitBreak :public CloneCard<LimitBreak>
{
private:
	virtual void set_description();
public:
	static LimitBreak* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(LimitBreak);
};
//157
class Madness :public CloneCard<Madness>
{
private:
	virtual void set_description();
public:
	static Madness* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Madness);
};
//158
class Magnetism :public CloneCard<Magnetism>
{
private:
	virtual void set_description();
public:
	static Magnetism* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Magnetism);
};
//160
class MasterofStrategy :public CloneCard<MasterofStrategy>
{
private:
	int card_num = 3;
	int upcard_num = 4;
	virtual void set_description();
public:
	static MasterofStrategy* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(MasterofStrategy);
};
//163
class Metallicize :public CloneCard<Metallicize>
{
private:
	int defend = 3;
	int updefend = 4;
	virtual void set_description();
public:
	static Metallicize* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Metallicize);
};
//165
class MindBlast :public CloneCard<MindBlast>
{
private:
	virtual void set_description();
public:
	virtual bool is_inherence() { return true; }
	static MindBlast* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(MindBlast);
};
//167
class Necronomicurse :public CloneCard<Injury>
{
public:
	static Necronomicurse* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use() {};
	virtual bool available() { return false; }//不能被打出
	CREATE_FUNC(Necronomicurse);
};
//170
class Normality :public CloneCard<Normality>
{
public:
	static Normality* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use() {};
	virtual bool available() { return false; }//不能被打出
	CREATE_FUNC(Normality);
};
//173
class Offering :public CloneCard<Offering>
{
private:
	int card_num = 2;
	int upcard_num = 4;
	virtual void set_description();
public:
	static Offering* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Offering);
};
//175,改了下，把原来敌人的死亡律动当成我方debuff了，有得有失吧
class Pain :public CloneCard<Pain>
{
public:
	static Pain* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use() {};
	virtual bool available() { return false; }//不能被打出
	CREATE_FUNC(Pain);
};
//176
class Panacea :public CloneCard<Panacea>
{
private:
	int artifact = 1;
	int upartifact = 2;
	virtual void set_description();
public:
	static Panacea* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Panacea);
};
//177
class Panache :public CloneCard<Panache>
{
private:
	int damage = 10;
	int updamage = 14;
	virtual void set_description();
public:
	static Panache* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Panache);
};
//178
class Parasite :public CloneCard<Parasite>
{
public:
	static Parasite* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use() {};
	virtual bool available() { return false; }//不能被打出
	CREATE_FUNC(Parasite);
};
//179 
class PerfectStrike : public CloneCard<PerfectStrike>
{
private:
	int damage = 6;
	int addi = 2;
	int upaddi = 3;
	virtual void set_description();
public:
	static PerfectStrike* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(PerfectStrike);
};
//183
class PommelStrike : public CloneCard<PommelStrike>
{
private:
	int damage = 9;
	int updamage = 10;
	int draw = 1;
	int updraw = 2;
	virtual void set_description();
public:
	static PommelStrike* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(PommelStrike);
};
//184
class PowerThrough : public CloneCard<PowerThrough>
{
private:
	int defend = 15;
	int updefend = 20;
	virtual void set_description();
public:
	static PowerThrough* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(PowerThrough);
};

//187
class Pummel : public CloneCard<Pummel>
{
private:
	int damage = 2;
	int time = 4;
	int uptime = 5;
	virtual void set_description();
public:
	static Pummel* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();

	CREATE_FUNC(Pummel);
};
//188,稍微改了改，原版的有点难写
class Purity : public CloneCard<Purity>
{
private:
	virtual void set_description();
public:
	static Purity* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Purity);
};

//190
class Rage : public CloneCard<Rage>
{
private:
	int defend = 3;
	int updefend = 5;
	virtual void set_description();
public:
	static Rage* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Rage);
};

//192
class Rampage : public CloneCard<Rampage>
{
private:
	int damage = 8;
	int addi = 5;
	int upaddi = 8;
	int usetime = 0;
	virtual void set_description();
public:
	static Rampage* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	virtual void fight_init() { usetime = 0; }
	CREATE_FUNC(Rampage);
};
//193
class Reaper : public CloneCard<Reaper>
{
private:
	int damage = 4;
	int updamage = 5;
	virtual void set_description();
public:
	static  Reaper* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Reaper);
};
//196
class RecklessCharge : public CloneCard<RecklessCharge>
{
private:
	int damage = 7;
	int updamage = 10;
	virtual void set_description();
public:
	static  RecklessCharge* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(RecklessCharge);
};
//200
class Regret :public CloneCard<Regret>
{
public:
	static Regret* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use() {};
	virtual bool available() { return false; }
	CREATE_FUNC(Regret);
};
//205
class Rupture : public CloneCard<Rupture>
{
private:
	int strength = 1;
	int upstrength = 2;
	virtual void set_description();
public:
	static  Rupture* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Rupture);
};
//206
class SadisticNature : public CloneCard<SadisticNature>
{
private:
	int damage = 5;
	int updamage = 7;
	virtual void set_description();
public:
	static SadisticNature* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(SadisticNature);
};
//208,功能不好改，就直接换了
class SearingBlow : public CloneCard<SearingBlow>
{
private:
	int damage = 12;
	int updamage = 22;
	virtual void set_description();
public:
	static  SearingBlow* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(SearingBlow);
};
//209
class SecondWind : public CloneCard<SecondWind>
{
private:
	int defend = 5;
	int updefend = 7;
	virtual void set_description();
public:
	static  SecondWind* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(SecondWind);
};
//210,这两个卡牌被我稍作修改了
class SecretTechnique : public CloneCard<SecretTechnique>
{
private:
	virtual void set_description();
public:
	static  SecretTechnique* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(SecretTechnique);
};
//211
class SecretWeapon : public CloneCard<SecretWeapon>
{
private:
	virtual void set_description();
public:
	static  SecretWeapon* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(SecretWeapon);
};
//212
class SeeingRed : public CloneCard<SeeingRed>
{
private:
	virtual void set_description();
public:
	static  SeeingRed* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(SeeingRed);
};
//215
class Sentinel : public CloneCard<Sentinel>
{
private:
	int defend = 5;
	int updefend = 8;
	virtual void set_description();
public:
	static  Sentinel* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	virtual void consume();
	CREATE_FUNC(Sentinel);
};
//217
class SeverSoul : public CloneCard<SeverSoul>
{
private:
	int damage = 16;
	int updamage = 22;
	virtual void set_description();
public:
	static  SeverSoul* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(SeverSoul);
};
//218
class Shiv : public CloneCard<Shiv>
{
private:
	int damage = 4;
	int updamage = 6;
	virtual void set_description();
public:
	static  Shiv* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Shiv);
};
//219
class ShockWave : public CloneCard<ShockWave>
{
private:
	int weak = 3;
	int upweak = 5;
	virtual void set_description();
public:
	static  ShockWave* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(ShockWave);
};
//220
class ShrugItOff : public CloneCard<ShrugItOff>
{
private:
	int defend = 8;
	int updefend = 11;
	virtual void set_description();
public:
	static  ShrugItOff* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(ShrugItOff);
};
//224
class Slimed : public CloneCard<Slimed>
{
private:
	virtual void set_description();
public:
	static  Slimed* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Slimed);
};
//225
class SpotWeakness : public CloneCard<SpotWeakness>
{
private:
	int strength = 3;
	int upstrength = 4;
	virtual void set_description();
public:
	static  SpotWeakness* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(SpotWeakness);
};
//235
class Strike_R :public CloneCard<Strike_R>
{
private:
	int damage = 6;
	int updamage = 9;
	virtual void set_description();
public:
	static Strike_R* create_card(int upgrade = 0);//是否升级过的,默认没有
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Strike_R);
};
//240
class SwiftStrike :public CloneCard<SwiftStrike>
{
private:
	int damage = 7;
	int updamage = 10;
	virtual void set_description();
public:
	static SwiftStrike* create_card(int upgrade = 0);//是否升级过的,默认没有
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(SwiftStrike);
};
//241
class SwordBoomerang : public CloneCard<SwordBoomerang>
{
private:
	int damage = 3;
	int time = 3;
	int uptime = 4;
	virtual void set_description();
public:
	static  SwordBoomerang* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(SwordBoomerang);
};
//245
class ThinkingAhead : public CloneCard<ThinkingAhead>
{
private:
	virtual void set_description();
public:
	static  ThinkingAhead* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(ThinkingAhead);
};
//247
class Thunderclap : public CloneCard<Thunderclap>
{
private:
	int damage = 4;
	int updamage = 7;
	virtual void set_description();
public:
	static  Thunderclap* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Thunderclap);
};
//249
class Transmutation : public CloneCard<Transmutation>
{
private:
	virtual void set_description();
public:
	static  Transmutation* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Transmutation);
};
//250
class Trip : public CloneCard<Trip>
{
private:
	int weak = 2;
	virtual void set_description();
public:
	static  Trip* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Trip);
};
//251
class TrueGrit : public CloneCard<TrueGrit>
{
private:
	int defend = 7;
	int updefend = 9;
	virtual void set_description();
public:
	static  TrueGrit* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(TrueGrit);
};
//257
class Uppercut : public CloneCard<Uppercut>
{
private:
	int damage = 13;
	int weak = 1;
	int upweak = 2;
	int vulnerable = 1;
	int upvulnerable = 2;
	virtual void set_description();
public:
	static  Uppercut* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Uppercut);
};
//259
class Void :public CloneCard<Void>
{
private:
	virtual void set_description();
public:
	static Void* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use() {};
	virtual bool available() { return false; }//不能被打出
	CREATE_FUNC(Void);
};
//260
class Warcry : public CloneCard<Warcry>
{
private:
	virtual void set_description();
public:
	static Warcry* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Warcry);
};
//262
class Whirlwind : public CloneCard<Whirlwind>
{
private:
	int damage = 5;
	int updamage = 8;
	virtual void set_description();
public:
	static  Whirlwind* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(Whirlwind);
};

//264
class WildStrike : public CloneCard<WildStrike>
{
private:
	int damage = 12;
	int updamage = 17;
	virtual void set_description();
public:
	static  WildStrike* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(WildStrike);
};
//265
class Wound :public CloneCard<Wound>
{
private:
	virtual void set_description();
public:
	static Wound* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use() {};
	virtual bool available() { return false; }//不能被打出
	CREATE_FUNC(Wound);
};
//267
class Writhe :public CloneCard<Writhe>
{
public:
	static Writhe* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use(){};
	virtual bool available() { return false; }
	virtual bool is_inherence() { return true; }
	CREATE_FUNC(Writhe);
};
//271
class RitualDagger :public CloneCard<RitualDagger>
{
private:
	int damage = 15;
	virtual void set_description();
public:
	static RitualDagger* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	CREATE_FUNC(RitualDagger);
};
//272
class Pride :public CloneCard<Pride>
{
public:
	static Pride* create_card(int upgrade = 0);
	virtual bool init() { return (Cards::init() ? true : false); }
	virtual void use();
	virtual bool is_inherence() { return true; }
	CREATE_FUNC(Pride);
};
#endif 


