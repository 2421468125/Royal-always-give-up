#include"CharacterBase.h"
#include "GlobalValue.h"
extern HeroBase* hero;
extern Vector<MonsterBase*> v_enemy;
void CharacterBase::create_CharacterBase(int health, int max_health, int money,
	const std::string name, const std::string path) {
	_health = health;
	_max_health = max_health;
	_money = money;
	_name = name;
	_path = path;
	auto img = Sprite::create(path);
	this->addChild(img,0,"img");
	auto buff_node = Node::create();
	buff_node->setPosition(150,100);
	this->addChild(buff_node, 0, "buff");
	draw_health();
	draw_buff();
}

void CharacterBase::draw_health() {
	this->removeChildByName("health");
	auto draw = DrawNode::create(10);
	int length = 256.0f * _health / _max_health;
	draw->drawLine(Vec2(-128, -280), Vec2(length - 128, -280), Color4F::RED);
	auto health_label = Label::createWithTTF(StringUtils::format("%d/%d", _health, _max_health)
		, "./fonts/Myfont.ttf", 20);
	health_label->setTextColor(Color4B::RED);
	health_label->enableOutline(Color4B::WHITE, 2);
	health_label->setPosition(0, -250);
	draw->addChild(health_label);
	this->addChild(draw,0,"health");

}
void CharacterBase::add_state(std::string s, int x) {
	extern std::vector<std::string> debuff;
	if (search_power("artifact") && find(debuff.begin(), debuff.end(), s) != debuff.end()) {
		power_now["artifact"] -= 1;
		if (power_now["artifact"] == 0) power_now.erase("artifact");
	}
	else
		state_now[s] += x;
	//因为只做一个角色，就偷懒这么干了，不然要重写个虚函数
	if (search_power("sadisticnature") && _name != "Ironclad" && find(debuff.begin(), debuff.end(), s) != debuff.end())
		this->be_beaten(handle_beaten_buff(power_now["sadisticnature"]));
}
void CharacterBase::draw_buff() {
	auto node = this->getChildByName("buff");
	node->removeAllChildren();
	int cnt = 0;
	for (std::pair<std::string, int> state : state_now) {
		auto icon = Sprite::create("./imgs/state/" + state.first + ".png");
		auto label = Label::createWithTTF(StringUtils::format("%d", state.second), "./fonts/Myfont.ttf", 20);
		icon->setPosition(0, -32*cnt);
		label->setPosition(32,0);
		icon->addChild(label);
		node->addChild(icon);
		cnt++;
	}
	for (std::pair<std::string, int> power : power_now) {
		auto icon = Sprite::create("./imgs/state/" + power.first + ".png");
		auto label = Label::createWithTTF(StringUtils::format("%d", power.second), "./fonts/Myfont.ttf", 20);
		icon->setPosition(0, -32 * cnt);
		label->setPosition(32, 0);
		icon->addChild(label);
		node->addChild(icon);
		cnt++;
	}
}

void CharacterBase::gain_defend(int def) {

	state_now["next_turn_block"] += def;
	if (search_power("juggernaut")) {
		int index = random(0, int(v_enemy.size() - 1));
		v_enemy.at(index)->be_beaten(search_power("juggernaut"));
	}
}
void CharacterBase::be_beaten(int dam, CharacterBase* user) {
	dam = this->handle_beaten_buff(dam);
	_health -= dam;
	if(_health > 0 && user)
		this->handle_beat_over_buff(user);
}
void CharacterBase::lose_health(int n) {
	_health -= n; 
	if (_health < 1 && this == (CharacterBase*)hero) ((HeroBase*)this)->die();
	if (search_power("rupture"))
		add_power("strength", search_power("rupture"));
}

void CharacterBase::update_enemy() {
	for (int i = 0; i < v_enemy.size(); ++i) {
		if (v_enemy.at(i)->get_health() < 1) {
			v_enemy.at(i)->die();
			i--;
		}
	}
	if (v_enemy.size() == 0) {
		dynamic_cast<FightScene*>(hero->getParent())->Fight_end();
		return;
	}
}
int CharacterBase::handle_attack_buff(int dam) {
	dam += this->search_power("strength");
	if (this->search_state("weak"))
		dam = int(dam * 0.75);
	return dam;
}
int CharacterBase::handle_beaten_buff(int dam) {
	if (search_state("vulnerable"))
		dam = int(dam * 1.5);
	if (search_power("intangible")) {
		dam = 1;
		power_now["intangible"] -= 1;
		if (power_now["intangible"] == 0)
			power_now.erase("intangible");
	}
	//处理护盾
	int defend = search_state("next_turn_block");
	if (dam > defend) {
		if(defend) state_now["next_turn_block"] = 0;
		dam -= defend;
	}
	else {
		state_now["next_turn_block"] -= dam;
		dam = 0;
	}
	return dam;
}

int CharacterBase::handle_defend_buff(int def) {
	def += this->search_power("dexterity");
	if (this->search_state("frail"))
		def = int(def * 0.75);
	return def;
}
void CharacterBase::handle_beat_over_buff(CharacterBase* tar) {//tar是攻击方
	if (this == (CharacterBase*)hero) {//处理角色专有的能力
		if (search_state("flamebarrier"))
			tar->be_beaten(search_state("flamebarrier"));
	}
	else {
		if (search_power("curlup")) {
			gain_defend(power_now["curlup"]);
			power_now.erase("curlup");
		}
		if (search_power("angry"))
			add_power("strength", power_now["angry"]);
	}
	if(search_power("thorns"))
		tar->be_beaten(search_power("thorns"),nullptr);
}
void CharacterBase::update_buff() {
	if (search_state("flamebarrier"))//一些仅此回合有效但状态量不为1的要特判删除
		state_now["flamebarrier"] = 0;
	if (search_state("rage"))
		state_now["rage"] = 0;
	if (search_state("strengthdown")) {
		hero->add_power("strength", -state_now["strengthdown"]);
		state_now["strengthdown"] = 0;
	}
	if (search_state("next_turn_block"))
		state_now["next_turn_block"]++;
	for (auto state = state_now.begin(); state != state_now.end();) {
		if (state->second == 0 || state->second == 1)//反正要-1，不如直接判断
			state_now.erase(state++);//因为删除迭代器自身会使其无法与for循环中结束条件比较，会报错，所以这么写
		else {
			state->second -= 1;
			state++;
		}
	}
	handle_end_turn_buff();
}
void CharacterBase::handle_start_turn_buff() {
	if (!search_power("barricade") && search_state("next_turn_block"))//每回合护盾会自动消失
		state_now["next_turn_block"] = 0;
	if (this->search_power("ritual"))
		this->add_power("strength", search_power("ritual"));
}
void CharacterBase::handle_end_turn_buff() {
	if (search_power("combust")) {
		int dam = search_power("combust");
		lose_health(1);
		for (auto e : v_enemy)
			e->be_beaten(dam,hero);
	}
	if (search_power("metallicize"))
		gain_defend(power_now["metallicize"]);
}
void HeroBase::handle_start_turn_buff() {
	CharacterBase::handle_start_turn_buff();
	energy_now = std::max(energy, energy_now);
	if (search_power("berserk"))
		energy_now += search_power("berserk");
	if (search_power("demonform"))
		state_now["strength"] += search_power("demonform");
	if (search_power("magnetism"))
		for (int i = 0; i < power_now["magnetism"]; ++i)
			v_tem_card.pushBack(dynamic_cast<FightScene*>(this->getParent())->
				Choose_random_card(Cards::Cvalue::INITIAL, Cards::Ctype::CURSE, Cards::Ccolour::GREY)->clone());

}
void HeroBase::handle_consume_buff(){
	if (search_power("darkembrace"))
		tem_draw_card++;
	if (search_power("feelnopain"))
		gain_defend(handle_defend_buff(search_power("feelnopain")));
};
int HeroBase::handle_draw_card_buff(int num) {
	if (search_power("brutality")) {
		lose_health(search_power("brutality"));
		num += search_power("brutality");
	}
	return num;
}
void HeroBase::handle_card_buff(Cards* card) {
	if (search_state("doubletag") && card->get_type() == Cards::ATTACK) {
		auto new_card = card->clone();
		v_tem_card.pushBack(new_card);
		new_card->use();
		state_now["doubletag"]--;
	}
	if (search_state("rage") && card->get_type() == Cards::ATTACK)
		gain_defend(search_state("rage"));
	if (card->get_type() == Cards::Ctype::SKILL)
		for (auto e : v_enemy)
			if (e->search_power("enrage"))
				e->add_power("strength", e->search_power("enrage"));
}
void HeroBase::handle_use_card_buff() {
	if (search_power("panache") && apply_card_num % 5 == 0)
		for (auto e : v_enemy)
			e->be_beaten(handle_beaten_buff(power_now["panache"]));
	if (search_state("beatofdeath")) lose_health(1);
	if (search_state("normality")) {
		if (--state_now["normality"] == 0) {
			state_now.erase("normality");
			add_energe_now(-get_energy_now() - 1); //把能量设置为-1，完全出不了卡等价于回合结束了
		}
	}
}
void HeroBase::die() {
	//游戏结束
}
void HeroBase::Shuffle() {
	std::shuffle(v_discard.begin(), v_discard.end(), std::default_random_engine(time(0)));
	for (auto c : v_discard) v_draw_card.pushBack(c);
	v_discard.clear();
}
bool HeroBase::use_card(Cards* card) {
	int cost =  card->get_temcost();
	if (search_state("corruption") && card->get_type() == Cards::SKILL)
		energy_now = 0;
	if (cost > energy_now)
		return false;
	if (!card->available())
		return false;
	apply_card_num++;
	energy_now -= cost;
	handle_card_buff(card);
	if (v_enemy.empty())//不知道靠其他效果杀死单个敌人会不会引起第二次使用的对象为nullptr的bug
		return true;//如果靠card_buff一些内容就打赢了，那就不用接着打了
	card->use();//先使用，如果被消耗了就找不到，也不用进入弃牌堆
	if (card->get_cost_last_for() == 1) {
		card->set_temcost(card->get_upgrade() ? card->get_upcost() : card->get_cost());
		//设置temcost时会默认把cost_last_for设置为0
	}
	auto iter = v_tem_card.find(card);
	if (iter != v_tem_card.end()) {
		if (search_state("corruption") && card->get_type() == Cards::SKILL)
			card->consume();
		else {
			v_discard.pushBack(*iter);
			v_tem_card.erase(iter);
		}
	}
	//card->runAction(Sequence::create(MoveTo::create(0.8f, Vec2(1728, 0)), [card]() {card->set_pos(-1000,-1000); }, NULL));
	card->removeFromParent();
	if (tem_draw_card) {
		draw_card(tem_draw_card);
		tem_draw_card = 0;
	}
	handle_use_card_buff();
	update_enemy();
	//更新屏幕数据放在最后,如果敌人全部死亡，则结束
	if (!v_enemy.empty()) {
		dynamic_cast<FightScene*>(this->getParent())->update_card_num();
	}
	return true;
}

void HeroBase::draw_card(int n) {
	if (search_state("no_draw"))
		return;
	dynamic_cast<FightScene*>(this->getParent())->Draw_card(n);
}
void HeroBase::fight_init() {
	//清除卡牌区
	clear_v_card();
	this->energy_now = energy;
	lose_health_time = 0;
	//先处理普通卡牌，再处理固有的卡牌
	for (auto c : v_total_card)
		if (v_inherence_card.find(c) == v_inherence_card.end())
			v_draw_card.pushBack(c);
	std::shuffle(v_draw_card.begin(), v_draw_card.end(), std::default_random_engine(time(0)));
	//通过对固有牌堆的洗牌，避免进入抽牌堆后再洗牌
	std::shuffle(v_inherence_card.begin(), v_inherence_card.end(), std::default_random_engine(time(0)));
	for (auto c : v_inherence_card)
		v_draw_card.pushBack(c);
	//还有一些遗物的操作

	draw_buff();
}

void HeroBase::turn() {
	apply_card_num = 0;
	handle_start_turn_buff();//如仪式，生命恢复，与能量相关的buff等等
	auto scene = dynamic_cast<FightScene*>(this->getParent());
	scene->Draw_card(handle_draw_card_buff(draw_card_once));
	scene->update_card_num();
}
void HeroBase::turn_end() {
	update_buff();
	apply_card_num = 0;

		//把卡牌都移入弃牌堆
		for (auto c : v_tem_card) {
			int id = c->get_id();
			//特判一下是不是某些牌，实在想不到除了这个以外还有什么方法了。
			if (id == 39) { be_beaten(c->get_upgrade() ? 4 : 2); }
			else if (id == 73) { be_beaten(2); }
			else if (id == 89) { add_state("weak", 1); }
			else if (id == 272) { v_draw_card.pushBack(c->clone()); }
			else if (id == 200) { lose_health(v_tem_card.size()); }
			if (c->is_nihility())//判断卡牌属性是否有虚无
				v_consume_card.pushBack((Cards*)c);
			else
				v_discard.pushBack((Cards*)c);
			c->removeFromParent();
		}
	v_tem_card.clear();
}
void HeroBase::choose_card(Vector<Cards*>* from, Vector<Cards*>* to,int n) {
	if (from->size() == 0)
		return;
	auto scene = Scene::create();
	Size winSize = Director::getInstance()->getWinSize();
	Layer* c = Layer::create();//这里因为要监听鼠标点击，不能用滑动屏。
	scene->addChild(c, 1);
	int count = 0;
	for (auto x : *from) {
		x->set_pos(140 + count % 6 * 300, 820 - count / 6 * 384);//大约能存18张牌,应该够了
		c->addChild(x, 1);
		count++;
	}
	auto bg = Sprite::create("./imgs/bg/choosecardbg.jpg");
	bg->setPosition(winSize.width / 2, winSize.height / 2);
	scene->addChild(bg, 0);
	auto tem_listener = EventListenerTouchOneByOne::create();
	tem_listener->onTouchBegan = [this, from, to, n, tem_listener](Touch* touch, Event* event)->bool
	{
		bool flag = false;
		Vec2 pos = touch->getLocation();
		float ratio = 1.5;//卡牌的大小放缩系数,懒得再调用函数了，反正都是1.5
		for (auto card : *from) {
			Vec2 card_pos = card->get_pos();//设置锚点卵用没有，这里它还是按照中心点为原点计算
			Rect range = Rect::Rect(card_pos.x-180, card_pos.y-190, 180 * ratio, 256 * ratio);
			if (range.containsPoint(pos)) {
				to->pushBack(card);
				from->eraseObject(card);
				card->removeFromParent();
				flag = true;
				break;
			}
		}
		if (flag) {
			if (n > 0)//如果n>0就要抽卡，放在这里写是防止程序并行运行。
				this->draw_card(n);
			else
				dynamic_cast<FightScene*>(this->getParent())->ShowTemCards();
			this->end_choose_card(tem_listener);
			
		}
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(tem_listener, c);
	//设置视图的宽度和高度
	Director::getInstance()->pushScene(scene);
}

void HeroBase::end_choose_card(EventListenerTouchOneByOne* listener) {
	_eventDispatcher->removeEventListener(listener);
	Director::getInstance()->popScene();
}

void HeroBase::choose_tem_card(int dowhat) {
	auto scene = Scene::create();
	Size winSize = Director::getInstance()->getWinSize();
	Layer* c = Layer::create();
	scene->addChild(c, 1);
	int count = 0;
	for (auto x : v_choose_card) {
		x->set_pos(140 + count % 6 * 300, 820 - count / 6 * 384);//大约能存18张牌,应该够了
		c->addChild(x, 1);
		count++;
	}
	auto bg = Sprite::create("./imgs/bg/choosecardbg.jpg");
	bg->setPosition(winSize.width / 2, winSize.height / 2);
	scene->addChild(bg, 0);
	auto tem_listener = EventListenerTouchOneByOne::create();
	tem_listener->onTouchBegan = [this,tem_listener,dowhat](Touch* touch, Event* event)->bool
	{
		bool flag = false;
		Vec2 pos = touch->getLocation();
		float ratio = 1.5;
		for (auto card : v_choose_card) {
			Vec2 card_pos = card->get_pos();
			Rect range = Rect::Rect(card_pos.x - 180, card_pos.y - 190, 180 * ratio, 256 * ratio);
			if (range.containsPoint(pos)) {
				if (dowhat == 0) {
					//这里不能直接修改对应Vector中的值，因为这样新clone的卡牌不会retain，导致后面可能出现
					//内存释放问题，只有replace或者pushback，insert等会自动retain。
					v_tem_card.pushBack(card->clone(1));
					v_tem_card.eraseObject(card);
				}
				else if (dowhat < 3) {
					for (int i = 0; i < dowhat; ++i)
						v_tem_card.pushBack(card->clone());
				}
				else if (dowhat == 3) {
					v_total_card.pushBack(card->clone());
					add_card_num(1);
				}
				flag = true;
				break;
			}
		}
		if (flag) {
			v_choose_card.clear();
			dynamic_cast<FightScene*>(this->getParent())->ShowTemCards();
			this->end_choose_card(tem_listener);

		}
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(tem_listener, c);
	//奖励界面独有的返回键
	if (dowhat == 3) {
		auto skip = MenuItemImage::create("./imgs/reward/skip.png", "./imgs/reward/skip.png",
			[this, tem_listener](Ref* ref) {v_choose_card.clear();this->end_choose_card(tem_listener); });
		skip->setPosition(1000, 400);
		auto menu = Menu::create(skip, NULL);
		menu->setPosition(0, 0);
		scene->addChild(menu, 0, "menu");
	}
	//设置视图的宽度和高度
	Director::getInstance()->pushScene(scene);
}

bool Ironclad::init() {
	using CardVector::red_attack_rare;
	if (!HeroBase::init()) {
		return false;
	}
	this->create_CharacterBase(80, 80, 99, "Ironclad", "./imgs/characters/ironclad/main2.png");
	this->set_hero(IRONCLAD);
	this->energy_path = "./imgs/characters/ironclad/card_red_orb.png";
	this->set_pos(350, 600);
	for (int i = 0; i < 5; ++i) {
		v_total_card.pushBack(Strike_R::create_card());
		v_total_card.pushBack(Defend_R::create_card());
	}
	v_total_card.pushBack(Bash::create_card());
	total_card_num = v_total_card.size();
	return true;
	
}


bool MonsterBase::init() {
	if (!CharacterBase::init()) {
		return false;
	}
	auto intent_img = Sprite::create();
	intent_img->setPosition(0, 200);
	this->addChild(intent_img, 0, "intent");
	return true;
}

void MonsterBase::die() {
	hero->gain_money(this->get_money());
	auto iter = v_enemy.find(this);
	v_enemy.erase(iter);
	this->removeFromParent();
}

void MonsterBase::draw_intent(int damage,int time) {
	std::string path = "./imgs/intent/";
	switch (intent) {
		case Attack:
			path += "attack.png";
			break;
		case AttackBuff:
			path += "attackBuff.png";
			break;
		case AttackDebuff:
			path += "attackDebuff.png";
			break;
		case AttackDefend:
			path += "attackDefend.png";
			break;
		case Buff:
			path += "buff.png";
			break;
		case Debuff:
			path += "debuff.png";
			break;
		case Defend:
			path += "defend.png";
			break;
		case DefendBuff:
			path += "defendBuff.png";
			break;
		case Sleep:
			path += "sleep.png";
			break;
		case Stun:
			path += "stun.png";
			break;
		case Unkonwn:
			path += "unknown.png";
			break;
		case Special:
			path += "special.png";
			break;
	}
	auto* intent_img = (Sprite*)(this->getChildByName("intent"));
	intent_img->setTexture(path);
	intent_img->removeAllChildren();
	if (intent < 4) {//枚举和整数的隐式转换，这里判断敌人是否要攻击
		auto label = Label::createWithTTF(StringUtils::format("%d x %d", damage,time), "./fonts/Myfont.ttf",20);
		label->setTextColor(Color4B::RED);
		label->setPosition(80, 32);
		intent_img->addChild(label);
	}

}


bool Cultist::init() {
	if (!MonsterBase::init()) {
		return false;
	}
	int health = random(48, 54);
	this->create_CharacterBase(health, health, random(8, 12), "Cultist", "./imgs/monsters/cultist.png");
	draw_buff();
	DoNextTurn();
	return true;
}
void Cultist::DoNextTurn() {
	if (this->get_round() != 0) {
		this->set_intent(Attack);
		int dam = this->handle_attack_buff(6);
		this->draw_intent(dam, 1);
	}
	else {
		this->set_intent(Buff);
		this->draw_intent();
	}
}
void Cultist::turn() {
	handle_start_turn_buff();
	if (intent == Attack) {
		int dam = this->handle_attack_buff(3);
		hero->be_beaten(dam,this);
	}
	else
		this->add_power("ritual", 3);
	update_buff();
	draw_buff();
	DoNextTurn();
}

bool SmallSlime::init() {
	if (!MonsterBase::init()) {
		return false;
	}
	int health = random(8, 12);
	this->create_CharacterBase(health, health, random(4, 6), "SmallSlime", "./imgs/monsters/smallslime.png");
	draw_buff();
	DoNextTurn();
	return true;
}
void SmallSlime::DoNextTurn() {
	if (this->get_round() & 1) {
		this->set_intent(Attack);
		int dam = this->handle_attack_buff(4) * (hero->search_state("vulnerable") ? 1.5 : 1);
		this->draw_intent(dam, 1);
	}
	else {
		this->set_intent(Debuff);
		this->draw_intent();
	}
}
void SmallSlime::turn() {
	handle_start_turn_buff();
	if (intent == Attack) {
		int dam = this->handle_attack_buff(4);
		hero->be_beaten(dam, this);
	}
	else {
		int randnum = random(0, 1);
		hero->add_state(randnum ? "weak" : "frail", 1);
	}
	update_buff();
	draw_buff();
	DoNextTurn();
}

bool MidSlime::init() {
	if (!MonsterBase::init()) {
		return false;
	}
	int health = random(28, 32);
	this->create_CharacterBase(health, health, random(6, 8), "MidlSlime", "./imgs/monsters/midslime.png");
	draw_buff();
	DoNextTurn();
	return true;
}
void MidSlime::DoNextTurn() {
	int randnum = random(1, 10);
	if (randnum < 4) {
		this->set_intent(AttackDebuff);
		int dam = this->handle_attack_buff(7) * (hero->search_state("vulnerable") ? 1.5 : 1);
		this->draw_intent(dam, 1);
	}
	else if(randnum < 8) {
		this->set_intent(Attack);
		int dam = this->handle_attack_buff(12) * (hero->search_state("vulnerable") ? 1.5 : 1);
		this->draw_intent(dam, 1);
	}
	else {
		this->set_intent(Debuff);
		this->draw_intent();
	}
}
void MidSlime::turn() {
	handle_start_turn_buff();
	if (intent == AttackDebuff) {
		int dam = this->handle_attack_buff(7);
		hero->be_beaten(dam, this);
		hero->get_v_discard()->pushBack(Slimed::create_card());
	}
	else if (intent == Attack) {
		int dam = this->handle_attack_buff(12);
		hero->be_beaten(dam, this);
	}
	else {
		int randnum = random(0, 1);
		hero->add_state(randnum ? "weak" : "frail", 1);
	}
	update_buff();
	draw_buff();
	DoNextTurn();
}

bool BigSlime::init() {
	if (!MonsterBase::init()) {
		return false;
	}
	int health = random(65, 69);
	this->create_CharacterBase(health, health, 0, "BigSlime", "./imgs/monsters/bigslime.png");
	this->add_power("split", 1);
	draw_buff();
	DoNextTurn();
	return true;
}
void BigSlime::DoNextTurn() {
	int randnum = random(1, 10);
	if (randnum < 4) {
		this->set_intent(AttackDebuff);
		int dam = this->handle_attack_buff(12) * (hero->search_state("vulnerable") ? 1.5 : 1);
		this->draw_intent(dam, 1);
	}
	else if (randnum < 8) {
		this->set_intent(Attack);
		int dam = this->handle_attack_buff(16) * (hero->search_state("vulnerable") ? 1.5 : 1);
		this->draw_intent(dam, 1);
	}
	else {
		this->set_intent(Debuff);
		this->draw_intent();
	}
}
void BigSlime::turn() {
	handle_start_turn_buff();
	if (get_health() <= get_max_health() / 2) {
		int health = this->get_health();
		auto e1 = MidSlime::create();
		auto e2 = MidSlime::create();
		e1->add_max_health(health - e1->get_max_health());
		e2->add_max_health(health - e2->get_max_health());
		v_enemy.pushBack(e1);
		v_enemy.pushBack(e2);
		dynamic_cast<FightScene*>(hero->getParent())->ShowMonster();
		this->removeFromParent();
		v_enemy.eraseObject(this);
		return;
	}
	if (intent == AttackDebuff) {
		int dam = this->handle_attack_buff(12);
		hero->be_beaten(dam, this);
		hero->get_v_discard()->pushBack(Slimed::create_card());
		hero->get_v_discard()->pushBack(Slimed::create_card());
	}
	else if (intent == Attack) {
		int dam = this->handle_attack_buff(16);
		hero->be_beaten(dam, this);
	}
	else if (intent == Debuff) {
		int randnum = random(0, 1);
		hero->add_state(randnum ? "weak" : "frail", 2);
	}
		
	update_buff();
	draw_buff();
	DoNextTurn();
}

bool FatGremlin::init() {
	if (!MonsterBase::init()) {
		return false;
	}
	int health = random(13, 17);
	this->create_CharacterBase(health, health, random(3, 5), "FatGremlin", "./imgs/monsters/fatgremlin.png");
	draw_buff();
	DoNextTurn();
	return true;
}
void FatGremlin::DoNextTurn() {
	this->set_intent(AttackDebuff);
	int dam = this->handle_attack_buff(4) * (hero->search_state("vulnerable") ? 1.5 : 1);
	this->draw_intent(dam, 1);
}
void FatGremlin::turn() {
	handle_start_turn_buff();
	int dam = handle_attack_buff(4);
	hero->be_beaten(dam, this);
	hero->add_state("weak", 1);
	update_buff();
	draw_buff();
	DoNextTurn();
}

bool MadGremlin::init() {
	if (!MonsterBase::init()) {
		return false;
	}
	int health = random(20, 24);
	this->create_CharacterBase(health, health, random(4, 6), "MadGremlin", "./imgs/monsters/madgremlin.png");
	this->add_power("angry", 1);
	draw_buff();
	DoNextTurn();
	return true;
}
void MadGremlin::DoNextTurn() {
	this->set_intent(Attack);
	int dam = this->handle_attack_buff(4) * (hero->search_state("vulnerable") ? 1.5 : 1);
	this->draw_intent(dam, 1);
}
void MadGremlin::turn() {
	handle_start_turn_buff();
	int dam = handle_attack_buff(4);
	hero->be_beaten(dam, this);
	update_buff();
	draw_buff();
	DoNextTurn();
}

bool ShieldGremlin::init() {
	if (!MonsterBase::init()) {
		return false;
	}
	int health = random(12, 15);
	this->create_CharacterBase(health, health, random(4, 6), "ShieldGremlin", "./imgs/monsters/shieldgremlin.png");
	draw_buff();
	DoNextTurn();
	return true;
}
void ShieldGremlin::DoNextTurn() {
	int randnum = random(0,1);
	if (randnum) {
		this->set_intent(Attack);
		int dam = handle_attack_buff(6) * (hero->search_state("vulnerable") ? 1.5 : 1);
		this->draw_intent(dam, 1);
	}
	else {
		this->set_intent(Defend);
		this->draw_intent();
	}
}
void ShieldGremlin::turn() {
	handle_start_turn_buff();
	if (intent == Attack) {
		int dam = handle_attack_buff(6);
		hero->be_beaten(dam, this);
	}
	else 
		v_enemy.at(random(0, int(v_enemy.size() - 1)))->gain_defend(7);
	update_buff();
	draw_buff();
	DoNextTurn();
}

bool SneakyGremlin::init() {
	if (!MonsterBase::init()) {
		return false;
	}
	int health = random(10, 14);
	this->create_CharacterBase(health, health, random(4, 6), "SneakyGremlin", "./imgs/monsters/sneakygremlin.png");
	draw_buff();
	DoNextTurn();
	return true;
}
void SneakyGremlin::DoNextTurn() {
	this->set_intent(Attack);
	int dam = handle_attack_buff(9) * (hero->search_state("vulnerable") ? 1.5 : 1);
	this->draw_intent(dam, 1);
}
void SneakyGremlin::turn() {
	handle_start_turn_buff();
	int dam = handle_attack_buff(9);
	hero->be_beaten(dam, this);
	update_buff();
	draw_buff();
	DoNextTurn();
}

bool GremlinWizard::init() {
	if (!MonsterBase::init()) {
		return false;
	}
	int health = random(21, 25);
	this->create_CharacterBase(health, health, random(5, 7), "GremlinWizard", "./imgs/monsters/gremlinwizard.png");
	draw_buff();
	DoNextTurn();
	return true;
}
void GremlinWizard::DoNextTurn() {
	if (get_round() % 3 == 2) {
		this->set_intent(Attack);
		int dam = handle_attack_buff(25) * (hero->search_state("vulnerable") ? 1.5 : 1);
		this->draw_intent(dam, 1);
	}
	else {
		this->set_intent(Unkonwn);
		this->draw_intent();
	}
	
}
void GremlinWizard::turn() {
	handle_start_turn_buff();
	if (intent == Attack) {
		int dam = handle_attack_buff(9);
		hero->be_beaten(dam, this);
	}
	update_buff();
	draw_buff();
	DoNextTurn();
}

bool RedLouse::init() {
	if (!MonsterBase::init()) {
		return false;
	}
	int health = random(11, 17);
	this->create_CharacterBase(health, health, random(4, 6), "RedLouse", "./imgs/monsters/redlouse.png");
	add_power("curlup", 6);
	draw_buff();
	DoNextTurn();
	return true;
}
void RedLouse::DoNextTurn() {
	int randnum = random(1, 4);
	if (randnum == 1) {
		set_intent(Buff);
		draw_intent();
	}
	else if (randnum == 2) {
		set_intent(Debuff);
		draw_intent();
	}
	else {
		set_intent(Attack);
		int dam = handle_attack_buff(7) * (hero->search_state("vulnerable") ? 1.5 : 1);
		draw_intent(dam,1);
	}

}
void RedLouse::turn() {
	handle_start_turn_buff();
	if (intent == Attack) {
		int dam = handle_attack_buff(7);
		hero->be_beaten(dam, this);
	}
	else if (intent == Buff)
		add_power("strength", 3);
	else
		hero->add_state("weak", 2);
	update_buff();
	draw_buff();
	DoNextTurn();
}

bool RedSlaver::init() {
	if (!MonsterBase::init()) {
		return false;
	}
	int health = random(46, 50);
	this->create_CharacterBase(health, health, random(9, 13), "RedSlaver", "./imgs/monsters/redslaver.png");
	draw_buff();
	DoNextTurn();
	return true;
}
void RedSlaver::DoNextTurn() {
	int randnum = random(1, 20);
	if (get_round() != 0 && StrongDebuff) {
		set_intent(Debuff);
		draw_intent();
	}
	if (get_round() == 0 || randnum < 10) {
		set_intent(Attack);
		int dam = handle_attack_buff(13) * (hero->search_state("vulnerable") ? 1.5 : 1);
		draw_intent(dam, 1);
	}
	else {
		set_intent(AttackDebuff);
		int dam = handle_attack_buff(8) * (hero->search_state("vulnerable") ? 1.5 : 1);
		draw_intent(dam, 1);
	}
}
void RedSlaver::turn() {
	handle_start_turn_buff();
	if (intent == Attack) {
		int dam = handle_attack_buff(13);
		hero->be_beaten(dam, this);
	}
	else if(intent == AttackDebuff) {
		int dam = handle_attack_buff(8);
		hero->be_beaten(dam, this);
		hero->add_state("vulnerable", 1);
	}
	else 
		hero->add_state("entangled", 1);
	update_buff();
	draw_buff();
	DoNextTurn();
}

bool BlueSlaver::init() {
	if (!MonsterBase::init()) {
		return false;
	}
	int health = random(46, 50);
	this->create_CharacterBase(health, health, random(8, 12), "BlueSlaver", "./imgs/monsters/blueslaver.png");
	draw_buff();
	DoNextTurn();
	return true;
}
void BlueSlaver::DoNextTurn() {
	int randnum = random(1, 5);
	if (randnum <4){
		set_intent(Attack);
		int dam = handle_attack_buff(12) * (hero->search_state("vulnerable") ? 1.5 : 1);
		draw_intent(dam,1);
	}
	else{
		set_intent(AttackDebuff);
		int dam = handle_attack_buff(7) * (hero->search_state("vulnerable") ? 1.5 : 1);
		draw_intent(dam, 1);
	}
}
void BlueSlaver::turn() {
	handle_start_turn_buff();
	if (intent == Attack) {
		int dam = handle_attack_buff(13);
		hero->be_beaten(dam, this);
	}
	else{
		int dam = handle_attack_buff(7);
		hero->be_beaten(dam, this);
		hero->add_state("weak",1);
	}
	update_buff();
	draw_buff();
	DoNextTurn();
}

bool JawWorm::init() {
	if (!MonsterBase::init()) {
		return false;
	}
	int health = random(40, 44);
	this->create_CharacterBase(health, health, random(8, 12), "JawWorm", "./imgs/monsters/jawworm.png");
	draw_buff();
	DoNextTurn();
	return true;
}
void JawWorm::DoNextTurn() {
	int randnum = random(1, 20);
	if (randnum < 7 || get_round() == 0) {
		set_intent(Attack);
		int dam = handle_attack_buff(11) * (hero->search_state("vulnerable") ? 1.5 : 1);
		draw_intent(dam, 1);
	}
	else if(randnum < 14) {
		set_intent(AttackDefend);
		int dam = handle_attack_buff(7) * (hero->search_state("vulnerable") ? 1.5 : 1);
		draw_intent(dam, 1);
	}
	else {
		set_intent(DefendBuff);
		draw_intent();
	}
}
void JawWorm::turn() {
	handle_start_turn_buff();
	if (intent == Attack) {
		int dam = handle_attack_buff(11);
		hero->be_beaten(dam, this);
	}
	else if(intent == AttackDefend) {
		int dam = handle_attack_buff(7);
		hero->be_beaten(dam, this);
		this->gain_defend(5);
	}
	else {
		add_power("strength", 3);
		gain_defend(6);
	}
	update_buff();
	draw_buff();
	DoNextTurn();
}

bool FungiBeast::init() {
	if (!MonsterBase::init()) {
		return false;
	}
	int health = random(22, 28);
	this->create_CharacterBase(health, health, random(6, 10),  "FungiBeast", "./imgs/monsters/fungibeast.png");
	add_power("sporecloud", 2);
	draw_buff();
	DoNextTurn();
	return true;
}
void FungiBeast::DoNextTurn() {
	int randnum = random(1,5);
	if (randnum < 4) {
		set_intent(Attack);
		int dam = handle_attack_buff(6) * (hero->search_state("vulnerable") ? 1.5 : 1);
		draw_intent(dam, 1);
	}
	else{
		set_intent(Buff);
		draw_intent();
	}
}
void FungiBeast::turn() {
	handle_start_turn_buff();
	if (intent == Attack) {
		int dam = handle_attack_buff(6);
		hero->be_beaten(dam, this);
	}
	else {
		add_power("strength", 3);
		gain_defend(6);
	}
	update_buff();
	draw_buff();
	DoNextTurn();
}
void FungiBeast::die(){ 
	MonsterBase::die(); 
	hero->add_state("vulnerable", 2); 
}

bool Looter::init() {
	if (!MonsterBase::init()) {
		return false;
	}
	int health = random(44,48);
	this->create_CharacterBase(health, health, random(8, 12), "Looter", "./imgs/monsters/looter.png");
	add_power("thievery", 15);
	draw_buff();
	DoNextTurn();
	return true;
}
void Looter::DoNextTurn() {
	if (get_round() < 2) {
		set_intent(AttackDebuff);
		int dam = handle_attack_buff(10) * (hero->search_state("vulnerable") ? 1.5 : 1);
		draw_intent(dam, 1);
	}
	else if (get_round() == 2) {
		set_intent(Attack);
		int dam = handle_attack_buff(18) * (hero->search_state("vulnerable") ? 1.5 : 1);
		draw_intent(dam, 1);
	}
	else if (get_round() == 3) {
		set_intent(Defend);
		draw_intent();
	}
	else {
		set_intent(Special);
		draw_intent();
	}
}
void Looter::turn() {
	handle_start_turn_buff();
	if (intent == AttackDebuff || intent == Attack) {
		int dam = handle_attack_buff(intent == AttackDebuff ? 10 : 18);
		int lose = std::max(-hero->get_money(), -15);
		hero->gain_money(lose);
		this->gain_money(-lose);
		hero->be_beaten(dam, this);
	}
	else if(intent == Defend) 
		gain_defend(6);
	else {
		gain_money(-get_money());
		this->die();
		update_enemy();
		return;
	}
	update_buff();
	draw_buff();
	DoNextTurn();
}

bool GremlinNob::init() {
	if (!MonsterBase::init()) {
		return false;
	}
	int health = random(82, 86);
	this->create_CharacterBase(health, health, random(16, 20), "GremlinNob", "./imgs/monsters/gremlinnob.png");
	draw_buff();
	DoNextTurn();
	return true;
}
void GremlinNob::DoNextTurn() {
	int randnum = random(1, 5);
	if (get_round() == 0 ) {
		set_intent(Buff);
		draw_intent();
	}
	else if (randnum < 4) {
		set_intent(Attack);
		int dam = handle_attack_buff(14) * (hero->search_state("vulnerable") ? 1.5 : 1);
		draw_intent(dam, 1);
	}
	else {
		set_intent(AttackDebuff);
		int dam = handle_attack_buff(6) * (hero->search_state("vulnerable") ? 1.5 : 1);
		draw_intent(dam, 1);
	}
}
void GremlinNob::turn() {
	if (intent == Attack) {
		int dam = handle_attack_buff(14);
		hero->be_beaten(dam, this);
	}
	else if (intent == AttackDebuff) {
		int dam = handle_attack_buff(6);
		hero->be_beaten(dam, this);
		hero->add_state("vulnerable", 2);
	}
	else
		add_power("enrage", 2);
	update_buff();
	draw_buff();
	DoNextTurn();
}

bool Lagavulin::init() {
	if (!MonsterBase::init()) {
		return false;
	}
	int health = random(109, 111);
	this->create_CharacterBase(health, health, random(16, 20), "Lagavulin", "./imgs/monsters/lagavulin.png");
	add_power("metallicize", 8);
	add_state("next_turn_block", 8);
	draw_buff();
	DoNextTurn();
	return true;
}
void Lagavulin::DoNextTurn() {
	if (get_round() < 2 && sleep) {
		set_intent(Sleep);
		draw_intent();
	}
	else if (stun) {
		set_intent(Stun);
		draw_intent();
	}
	else if (attack % 3 == 2) {
		set_intent(Debuff);
		draw_intent();
		attack++;
	}
	else {
		set_intent(Attack);
		int dam = handle_attack_buff(18) * (hero->search_state("vulnerable") ? 1.5 : 1);
		draw_intent(dam, 1);
		attack++;
	}
}
void Lagavulin::turn() {
	handle_start_turn_buff();
	if (get_round() < 2 && get_health() < get_max_health() || !trans && get_round() == 2) {
		((Sprite*)this->getChildByName("img"))->setTexture("./imgs/monsters/lagavulin2.png");
		stun = get_health() < get_max_health() ? true : false;
		trans = true;
		sleep = false;
		power_now.erase("metallicize");
	}
	if (intent == Attack) {
		int dam = handle_attack_buff(18);
		hero->be_beaten(dam, this);
	}
	else if (intent == Debuff) {
		hero->add_power("strength", -1);
		hero->add_power("dexterity", -1);
	}
	else if (intent == Stun)
		stun = false;
	update_buff();
	draw_buff();
	DoNextTurn();
}

bool Sentry::init() {
	if (!MonsterBase::init()) {
		return false;
	}
	int health = random(38, 42);
	this->create_CharacterBase(health, health, random(8, 12), "Sentry", "./imgs/monsters/sentry.png");
	add_power("artifact", 1);
	draw_buff();
	DoNextTurn();
	return true;
}
void Sentry::DoNextTurn() {
	if (get_round() == 0) {
		if (random(0, 1)) {
			set_intent(Attack);
			int dam = hero->handle_beaten_buff(this->handle_attack_buff(9));
			draw_intent(dam, 1);
		}
		else {
			set_intent(Debuff);
			draw_intent();
		}
	}
	else if (intent == Debuff) {
		set_intent(Attack);
		int dam = hero->handle_beaten_buff(this->handle_attack_buff(9));
		draw_intent(dam, 1);
	}
	else {
		set_intent(Debuff);
		draw_intent();
	}
}
void Sentry::turn() {
	handle_start_turn_buff();
	if (intent == Attack) {
		int dam = handle_attack_buff(9);
		hero->be_beaten(dam, this);
	}
	else{
		hero->get_v_discard()->pushBack(Dazed::create_card());
		hero->get_v_discard()->pushBack(Dazed::create_card());
	}
	update_buff();
	draw_buff();
	DoNextTurn();
}

bool Hexaghust::init() {
	if (!MonsterBase::init()) {
		return false;
	}
	int health = 250;
	this->create_CharacterBase(health, health, random(50, 80), "Hexaghust", "./imgs/monsters/hexaghust.png");
	draw_buff();
	DoNextTurn();
	return true;
}
void Hexaghust::DoNextTurn() {
	int round = get_round();
	if (round > 1) round = (round - 2) % 7 + 2;
	switch (round) {
	
	case 0: {
		set_intent(Unkonwn);
		draw_intent();
	}
		  break;

	case 1: {
		set_intent(Attack);
		int dam = hero->handle_beaten_buff(this->handle_attack_buff(hero->get_health() / 12 + 1));
		draw_intent(dam, 6);
	}
		  break;

	case 2:
	case 4:
	case 6:{
		set_intent(AttackDebuff);
		int dam = hero->handle_beaten_buff(this->handle_attack_buff(6));
		draw_intent(dam, 1);
	}
		  break;

	case 3:
	case 5:{
		set_intent(Attack);
		int dam = hero->handle_beaten_buff(this->handle_attack_buff(5));
		draw_intent(dam, 2);
	}
		  break;
	
	case 7: {
		set_intent(DefendBuff);
		draw_intent();
	}
		  break;

	case 8: {
		set_intent(AttackDebuff);
		int dam = hero->handle_beaten_buff(this->handle_attack_buff(2));
		draw_intent(dam, 6);
	}
		  break;
	}
}
void Hexaghust::turn() {
	handle_start_turn_buff();
	int round = get_round();
	round--;//因为Monster在turn之前会先增加轮数，所以为了和DoNextTurn配对，让轮数--
	if (round > 1) round = (round - 2) % 7 + 2;
	switch (round) {

	case 0:
		  break;

	case 1: {
		int dam = hero->handle_beaten_buff(this->handle_attack_buff(hero->get_health() / 12 + 1));
		for (int i = 0; i < 6; ++i)
			hero->be_beaten(dam);
	}
		  break;

	case 2:
	case 4:
	case 6: {
		int dam = hero->handle_beaten_buff(this->handle_attack_buff(6));
		hero->be_beaten(dam);
		hero->get_v_discard()->pushBack(Burn::create_card());

	}
		  break;

	case 3:
	case 5: {
		int dam = hero->handle_beaten_buff(this->handle_attack_buff(5));
		for (int i = 0; i < 2; ++i)
			hero->be_beaten(dam);
	}
		  break;

	case 7: {
		add_power("strength", 2);
		gain_defend(12);
	}
		  break;

	case 8: {
		int dam = hero->handle_beaten_buff(this->handle_attack_buff(2));
		for (int i = 0; i < 6; ++i)
			hero->be_beaten(dam);
		for (int i = 0; i < 3; ++i)
			hero->get_v_discard()->pushBack(Burn::create_card(1));
	}
		  break;
	}
	update_buff();
	draw_buff();
	DoNextTurn();
}
