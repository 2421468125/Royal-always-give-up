#ifndef __CHARACTERBASE_H__
#define __CHARACTERBASE_H__
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <cstdlib>
#include <algorithm>
#include "cocos2d.h"
#include "Cards.h"
#include "FightScene.h"
USING_NS_CC;


class Cards;
class CharacterBase: public Node
{
private:
	int _health; 
	int _max_health;
	int _money;//���������ɫӵ��
	int pos_x=0;
	int pos_y=0;
	int round = 0;//�غ���
	std::string _name;
	std::string _path;//ͼƬ��ַ
protected:
	std::unordered_map<std::string, int> state_now;//�ŵ�ǰ��ɫĿǰ�е�״̬��״̬����غ�������
	std::unordered_map<std::string, int> power_now;//�ŵ�ǰ��ɫĿǰ�е�����������������ʧ
public:
	virtual bool init() { return true; }
	void create_CharacterBase(int health, int max_health, int money,const std::string name, const std::string path);
	void set_pos(int x, int y) { pos_x = x; pos_y = y; this->setPosition(x, y); }
	void lose_health(int n);
	void add_health(int n) { _health = std::min(_health + n, _max_health); }
	void add_max_health(int n) { _max_health += n; _health = std::min(_health + n, _max_health); }
	void add_round() { round++; }
	void init_round() { round = 0; }
	int get_round() { return round; }
	Vec2 get_pos() { return Vec2(pos_x, pos_y); }
	int get_health() { return _health; }
	int get_max_health() { return _max_health; }
	int get_money() { return _money; }
	const std::string& get_name() { return _name; }
	int search_state(const char* s) { return (state_now.count(s) ? state_now[s] : 0); }
	int search_power(const char* s) { return (power_now.count(s) ? power_now[s] : 0); }
	void add_state(std::string s, int x);
	void add_power(std::string s, int x) { power_now[s] += x; }
	void gain_money(int m) { _money += m; }
	void gain_defend(int def);
	void draw_health();
	void be_beaten(int dam, CharacterBase* user = nullptr);//֮���һ��
	virtual void die() {}
	//��ʱ�������ַ�ʽ��������buff�жϼ��ϡ�
	int handle_attack_buff(int dam);
	int handle_beaten_buff(int dam);
	int handle_defend_buff(int def); 
	void handle_beat_over_buff(CharacterBase* tar);//�����������ȹ������buff
	void handle_start_turn_buff();
	void handle_end_turn_buff();//�����ֱ�ӷŵ�update_buff����
	void update_buff();
	void draw_buff();
	static void update_enemy();
	CREATE_FUNC(CharacterBase);
};

//�������Ǹ��м��࣬ûɶ̫�����ã���������create��
class HeroBase : public CharacterBase
{
public:     
	enum Hero {
		IRONCLAD,
		SILENT,
		DEFECT
	};
private:
	Hero hero;
	int energy;//�ж���
	int energy_now;
	int kill_enemy_num;
	int draw_card_once;
	int tem_draw_card;//�����ӳٳ鿨�ļ���������������һ�߳鿨һ�����Ŀ��ĳ���
	int apply_card_num;//һ�غ��ڴ���Ŀ�������
	Vector<Cards*> v_tem_card;//ս��ʱ��ʱ����ӵ�еĿ����б�
	Vector<Cards*> v_draw_card;//���ƶ�
	Vector<Cards*> v_discard;//���ƶ�
	Vector<Cards*> v_consume_card;//�����ƶ�
	Vector<Cards*> v_inherence_card;//�������ԵĿ�
	Vector<Cards*> v_choose_card;//����ѡ���Ƶ���ʱ�Ŷѣ�ÿ����������
	std::unordered_set<std::string> relics;
	//����ר��״̬��,Ҳ������state��
protected:
	int total_card_num;
public:
	int lose_health_time = 0;//ʧȥ�����Ĵ���
	std::string energy_path;
	Vector<Cards*> v_total_card;//ƽʱ��ӵ�еĿ����б�

	virtual bool init() {
		energy = 3;
		energy_now = energy;
		draw_card_once = 5;
		tem_draw_card = 0;
		return (CharacterBase::init() ? true : false); }

	void set_hero(Hero h) { hero = h; }
	Hero get_hero() { return hero; }
	int get_card_num() { return total_card_num; }
	void add_card_num(int n) { total_card_num += n; }
	int get_energy() { return energy; }
	int get_energy_now() { return energy_now; }
	void add_energe_now(int n) { energy_now += n; }
	int get_draw_card_once() { return draw_card_once; }
	Vector<Cards*>* get_v_draw_card() { return &v_draw_card; }
	Vector<Cards*>* get_v_discard() { return &v_discard; }
	Vector<Cards*>* get_v_consume_card() { return &v_consume_card; }
	Vector<Cards*>* get_v_tem_card() { return &v_tem_card; }
	Vector<Cards*>* get_v_inherence_card() { return &v_inherence_card; }
	Vector<Cards*>* get_v_choose_card() { return &v_choose_card; }
	void clear_v_card() { v_tem_card.clear(); v_discard.clear(); v_consume_card.clear(); v_draw_card.clear(); }
	void beat_anime() { kill_enemy_num++; }
	void clear_buff() { state_now.clear(); power_now.clear(); }
	void lose_health(int n) { lose_health_time++; ((CharacterBase*)this)->lose_health(n); }//ʧȥ����
	void Shuffle();//ϴ��
	//�̵�����ʱ���õ����ӿ��������ĺ���
	void add_card(Cards* card) { v_total_card.pushBack(card); total_card_num++; }
	//ս��ʱ�õ�
	bool use_card(Cards* card);//��ս����صķ�������Ҫ�������á�
	//�̵��Ƴ�����ʱ�õ�
	void remove_card(Cards* card) { v_total_card.erase(v_total_card.find(card)); total_card_num--; }
	void draw_card(int n);
	//�� from ��ѡ��һ�ſ��Ʒ���to�У�Ĭ�Ϸŵ���λ��
	//����д�ü����ã�n�ǳ鿨�������������������ƺ�鿨Ҳʹ����������ˣ����Կ��źܳ�
	void choose_card(Vector<Cards*>* from, Vector<Cards*>* to, int n = 0);
	void end_choose_card(EventListenerTouchOneByOne* listener);
	/*
	* ר������ѡ�����Ƶĺ�����dowhat��ʾ��������
	* 0��������
	* 1-2������1-2�ţ�
	* 3��ѡ�񿨵�������
	*/
	void choose_tem_card(int dowhat);
	void handle_consume_buff();
	int handle_draw_card_buff(int num);
	void handle_start_turn_buff();
	void handle_card_buff(Cards* card);//����������һ�ſ���xxx�ĺ���
	void handle_use_card_buff();//���꿨�ƺ��buff
	//ս����ʼ�ĳ�ʼ��
	void fight_init();
	virtual void die();
	virtual void turn();
	void turn_end();
	CREATE_FUNC(CharacterBase);
};

class Ironclad : public HeroBase
{
public:
	
	virtual bool init();
	
	CREATE_FUNC(Ironclad)
};
//��ΪҪ�����˵���ͼ�����Լ��˸��м���
class MonsterBase : public CharacterBase
{
protected:
	enum Intent
	{
		Attack,
		AttackBuff,
		AttackDebuff,
		AttackDefend,
		Buff,
		Debuff,
		Defend,
		DefendBuff,
		Sleep,
		Special,
		Stun,
		Unkonwn
	};
	Intent intent;
public:
	virtual bool init();
	void set_intent(Intent t) { intent = t; }
	Intent get_intent() { return intent; }
	void draw_intent(int damage = 0,int time = 1);
	virtual void die();
	virtual void turn(){};
	virtual void DoNextTurn() {};//�����»غ���ʲô���麯����ÿ�����˲�һ��
	CREATE_FUNC(MonsterBase);
};
//˵һ���Ѷȣ�difficulty���ı�׼��10+ƽ��Ѫ��/2+ƽ���˺�+��ʼbuff*2(ȡ����buffǿ��),�Լ����������������debuffʵ���ϻ���֣�
class Cultist : public MonsterBase
{
public:
	static const int difficulty = 44;
	virtual bool init();
	virtual void DoNextTurn();
	virtual void turn();
	CREATE_FUNC(Cultist);
};

class SmallSlime : public MonsterBase
{
public:
	static const int difficulty = 17;
	virtual bool init();
	virtual void DoNextTurn();
	virtual void turn();
	CREATE_FUNC(SmallSlime);
};

class MidSlime : public MonsterBase
{
public:
	static const int difficulty = 35;
	virtual bool init();
	virtual void DoNextTurn();
	virtual void turn();
	CREATE_FUNC(MidSlime);
};

class BigSlime : public MonsterBase
{
public:
	static const int difficulty = 60;
	virtual bool init();
	virtual void DoNextTurn();
	virtual void turn();
	CREATE_FUNC(BigSlime);
};

class FatGremlin : public MonsterBase
{
public:
	static const int difficulty = 21;
	virtual bool init();
	virtual void DoNextTurn();
	virtual void turn();
	CREATE_FUNC(FatGremlin);
};

class MadGremlin : public MonsterBase
{
public:
	static const int difficulty = 28;
	virtual bool init();
	virtual void DoNextTurn();
	virtual void turn();
	CREATE_FUNC(MadGremlin);
};

class ShieldGremlin : public MonsterBase
{
public:
	static const int difficulty = 23;
	virtual bool init();
	virtual void DoNextTurn();
	virtual void turn();
	CREATE_FUNC(ShieldGremlin);
};

class SneakyGremlin : public MonsterBase
{
public:
	static const int difficulty = 25;
	virtual bool init();
	virtual void DoNextTurn();
	virtual void turn();
	CREATE_FUNC(SneakyGremlin);
};

class GremlinWizard : public MonsterBase
{
public:
	static const int difficulty = 32;
	virtual bool init();
	virtual void DoNextTurn();
	virtual void turn();
	CREATE_FUNC(GremlinWizard);
};

class RedLouse : public MonsterBase
{
public:
	static const int difficulty = 25;
	virtual bool init();
	virtual void DoNextTurn();
	virtual void turn();
	CREATE_FUNC(RedLouse);
};

class RedSlaver : public MonsterBase
{
private:
	bool StrongDebuff = true;
public:
	static const int difficulty = 50;
	virtual bool init();
	virtual void DoNextTurn();
	virtual void turn();
	CREATE_FUNC(RedSlaver);
};

class BlueSlaver : public MonsterBase
{
public:
	static const int difficulty = 45;
	virtual bool init();
	virtual void DoNextTurn();
	virtual void turn();
	CREATE_FUNC(BlueSlaver);
};

class JawWorm : public MonsterBase
{
public:
	static const int difficulty = 44;
	virtual bool init();
	virtual void DoNextTurn();
	virtual void turn();
	CREATE_FUNC(JawWorm);
};

class FungiBeast : public MonsterBase
{
public:
	static const int difficulty = 35;
	virtual bool init();
	virtual void DoNextTurn();
	virtual void turn();
	virtual void die();
	CREATE_FUNC(FungiBeast);
};

class Looter : public MonsterBase
{
public:
	static const int difficulty = 35;
	virtual bool init();
	virtual void DoNextTurn();
	virtual void turn();
	CREATE_FUNC(Looter);
};

class GremlinNob : public MonsterBase
{
public:
	static const int difficulty = 70;
	virtual bool init();
	virtual void DoNextTurn();
	virtual void turn();
	CREATE_FUNC(GremlinNob);
};

class Lagavulin : public MonsterBase
{
private:
	int attack = 0;
	bool trans = false;
	bool sleep = true;
	bool stun = false;
public:
	static const int difficulty = 79;
	virtual bool init();
	virtual void DoNextTurn();
	virtual void turn();
	CREATE_FUNC(Lagavulin);
};

class Sentry : public MonsterBase
{
public:
	static const int difficulty = 37;
	virtual bool init();
	virtual void DoNextTurn();
	virtual void turn();
	CREATE_FUNC(Sentry);
};

class Hexaghust : public MonsterBase
{
public:
	virtual bool init();
	virtual void DoNextTurn();
	virtual void turn();
	CREATE_FUNC(Hexaghust);
};
#endif 
