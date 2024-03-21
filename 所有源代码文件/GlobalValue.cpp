#include "GlobalValue.h"

HeroBase* hero;
GameMap* map;
Vector<MonsterBase*> v_enemy;//怪物列表，在判断卡牌点时需要
//卡牌列表其实每种保存一个就够了，用下标数字分割不同类型也可以，不过懒改了
std::vector<std::string> debuff{
	"frail","vulnerable","weak","beatofdeath"
};

//一个用于创建怪兽的模板函数，和函数指针一起用可以构造map映射
template<typename T> MonsterBase* createMonster() { return T::create(); }
//一个用于将名称和构造函数指针建立映射的map，这样可以简单实现RTTI
std::unordered_map<std::string, MonsterBase* (*)()> enemy_map;
//一个存放名称和难度的数组
std::vector<std::pair<const char*, int> > v_difficulty = {
	{"Cultist",44},
	{"SmallSlime",17},
	{"MidSlime",35},
	{"BigSlime",60},
	{"FatGremlin",21},
	{"MadGremlin",28},
	{"ShieldGremlin",23},
	{"SneakyGremlin",25},
	{"GremlinWizard",32},
	{"RedLouse",25},
	{"RedSlaver",50},
	{"BlueSlaver",45},
	{"JawWorm",44},
	{"FungiBeast",35},
	{"Looter",35},
	{"GremlinNob",70},
	{"Lagavulin",79},
	{"Sentry",37}
};

void GlobalValue::set_hero(HeroBase* h) {
	::hero = h;
}

void GlobalValue::set_map(GameMap* m) {
	::map = m;
}
namespace CardVector {
	Vector<Cards*> red_attack_common;
	Vector<Cards*> red_attack_value;
	Vector<Cards*> red_attack_rare;
	Vector<Cards*> red_attack;
	Vector<Cards*> red_skill_common;
	Vector<Cards*> red_skill_value;
	Vector<Cards*> red_skill_rare;
	Vector<Cards*> red_skill;
	Vector<Cards*> red_power_value;//能力牌没有普通卡
	Vector<Cards*> red_power_rare;
	Vector<Cards*> red_power;
	Vector<Cards*> red_card;
	//白卡就不分稀有度了，懒了
	Vector<Cards*> grey_attack;
	Vector<Cards*> grey_skill;
	Vector<Cards*> grey_power;
	Vector<Cards*> grey_card;
};
void GlobalValue::card_vector_init() {
	using namespace CardVector;
	Vector<Cards*> tem1 = {
	Anger::create_card(),
	BodySlam::create_card(),
	Clash::create_card(),
	Cleave::create_card(),
	Clothesline::create_card(),
	HeadButt::create_card(),
	HeavyBlade::create_card(),
	IronWave::create_card(),
	PerfectStrike::create_card(),
	PommelStrike::create_card(),
	SwordBoomerang::create_card(),
	Thunderclap::create_card(),
	WildStrike::create_card()
	};
	Vector<Cards*> tem2 = {
	BloodForBlood::create_card(),
	Carnage::create_card(),
	Dropkick::create_card(),
	Hemokinesis::create_card(),
	Pummel::create_card(),
	Rampage::create_card(),
	RecklessCharge::create_card(),
	SearingBlow::create_card(),
	SeverSoul::create_card(),
	Uppercut::create_card(),
	Whirlwind::create_card()
	};
	Vector<Cards*> tem3 = {
	Blugeon::create_card(),
	Feed::create_card(),
	FiendFire::create_card(),
	Immolate::create_card(),
	Reaper::create_card()
	};
	red_attack_common = tem1;
	red_attack_value = tem2;
	red_attack_rare = tem3;
	red_attack = tem1;
	for (auto c : tem2) red_attack.pushBack(c);
	for (auto c : tem3) red_attack.pushBack(c);

	Vector<Cards*> tem4 = {
	Armaments::create_card(),
	Flex::create_card(),
	Havoc::create_card(),
	ShrugItOff::create_card(),
	TrueGrit::create_card(),
	Warcry::create_card(),
	};
	Vector<Cards*> tem5 = {
	BattleTrance::create_card(),
	BloodLetting::create_card(),
	BurningPact::create_card(),
	Disarm::create_card(),
	DualWield::create_card(),
	Entrench::create_card(),
	FlameBarrier::create_card(),
	GhostlyArmor::create_card(),
	InfernalBlade::create_card(),
	Intimidate::create_card(),
	PowerThrough::create_card(),
	Rage::create_card(),
	SecondWind::create_card(),
	SeeingRed::create_card(),
	Sentinel::create_card(),
	ShockWave::create_card(),
	SpotWeakness::create_card(),
	};
	Vector<Cards*> tem6 = {
	DoubleTap::create_card(),
	Exhume::create_card(),
	Impervious::create_card(),
	LimitBreak::create_card(),
	Offering::create_card(),
	};
	red_skill_common = tem4;
	red_skill_value = tem5;
	red_skill_rare = tem6;
	red_skill = tem4;
	for (auto c : tem5) red_skill.pushBack(c);
	for (auto c : tem6) red_skill.pushBack(c);
	Vector<Cards*> tem7 = {
	Combust::create_card(),
	DarkEmbrace::create_card(),
	Evolve::create_card(),
	FeelNoPain::create_card(),
	FireBreathing::create_card(),
	Inflame::create_card(),
	Metallicize::create_card(),
	Rupture::create_card(),
	};
	Vector<Cards*> tem8 = {
	Barricade::create_card(),
	Berserk::create_card(),
	Brutality::create_card(),
	Corruption::create_card(),
	DemonForm::create_card(),
	Juggernaut::create_card(),
	};
	red_power_value = tem7;
	red_power_rare = tem8;
	red_power = tem7;
	for (auto c : tem8) red_power.pushBack(c);

	red_card = red_attack;
	for (auto c : red_skill) red_card.pushBack(c);
	for (auto c : red_power) red_card.pushBack(c);
	
	Vector<Cards*> tem9 = {
	Bite::create_card(),
	DramaticEntrance::create_card(),
	FlashofSteel::create_card(),
	MindBlast::create_card(),
	Shiv::create_card(),
	SwiftStrike::create_card(),
	RitualDagger::create_card(),
	};
	grey_attack = tem9;
	Vector<Cards*> tem10 = {
	//value
	BandageUp::create_card(),
	Blind::create_card(),
	DeepBreath::create_card(),
	Enlightment::create_card(),
	Finesse::create_card(),
	Ghostly::create_card(),
	GoodInstincts::create_card(),
	Jax::create_card(),
	JackOfAllTrades::create_card(),
	Madness::create_card(),
	Panacea::create_card(),
	Purity::create_card(),
	Trip::create_card(),
	//rare
	Apotheosis::create_card(),
	MasterofStrategy::create_card(),
	SecretTechnique::create_card(),
	SecretWeapon::create_card(),
	ThinkingAhead::create_card(),
	Transmutation::create_card()
	};
	grey_skill = tem10;
	Vector<Cards*> tem11 = {
	Magnetism::create_card(),
	Panache::create_card(),
	SadisticNature::create_card()
	};
	grey_power = tem11;
	grey_card = grey_attack;
	for (auto c : grey_skill) grey_card.pushBack(c);
	for (auto c : grey_power) grey_card.pushBack(c);
	
}

void GlobalValue::enemy_map_init() {
	using std::pair;
	enemy_map["Cultist"] = &createMonster<Cultist>;
	enemy_map["SmallSlime"] = &createMonster<SmallSlime>;
	enemy_map["MidSlime"] = &createMonster<MidSlime>;
	enemy_map["BigSlime"] = &createMonster<BigSlime>;
	enemy_map["FatGremlin"] = &createMonster<FatGremlin>;
	enemy_map["MadGremlin"] = &createMonster<MadGremlin>;
	enemy_map["ShieldGremlin"] = &createMonster<ShieldGremlin>;
	enemy_map["SneakyGremlin"] = &createMonster<SneakyGremlin>;
	enemy_map["GremlinWizard"] = &createMonster<GremlinWizard>;
	enemy_map["RedLouse"] = &createMonster<RedLouse>;
	enemy_map["BlueSlaver"] = &createMonster<BlueSlaver>;
	enemy_map["RedSlaver"] = &createMonster<RedSlaver>;
	enemy_map["JawWorm"] = &createMonster<JawWorm>;
	enemy_map["FungiBeast"] = &createMonster<FungiBeast>;
	enemy_map["Looter"] = &createMonster<Looter>;
	enemy_map["GremlinNob"] = &createMonster<GremlinNob>;
	enemy_map["Lagavulin"] = &createMonster<Lagavulin>;
	enemy_map["Sentry"] = &createMonster<Sentry>;
	
	//为怪物难度数组从小到大排序
	std::sort(v_difficulty.begin(), v_difficulty.end(), [](pair<const char*, int>& a, pair<const char*, int>& b)
		{return a.second < b.second; });
}
