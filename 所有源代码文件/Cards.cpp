#include<fstream>
#include<iostream>
#include"Cards.h"
#include"GlobalValue.h"
#include"CharacterBase.h"

USING_NS_CC;
extern HeroBase* hero;
extern Vector<MonsterBase*> v_enemy;
std::vector<std::string> Cards::chinese_card_name = {};
std::vector<std::string> Cards::english_card_name = {};
std::vector<std::string> Cards::chinese_card_description={};
std::vector<std::string> Cards::chinese_card_updescription = {};
//给静态成员变量赋值
void Cards::read_chinese_card_name() {
	std::string data = FileUtils::getInstance()->getStringFromFile("./content/chinese_card_name.txt");
	auto iter = data.begin();
	for (auto i = data.begin(); i != data.end(); ++i) {
		if (*i =='\n') {
			chinese_card_name.push_back(std::string(iter, i-1));
			iter = i+1;
		}
	}
}
void Cards::read_chinese_card_description() {
	std::string data = FileUtils::getInstance()->getStringFromFile("./content/chinese_card_description.txt");
	auto iter = data.begin();
	for (auto i = data.begin(); i != data.end(); ++i) {
		if (*i == '\n') {
			chinese_card_description.push_back(std::string(iter, i-1));
			iter = i + 1;
		}
	}
}
void Cards::read_chinese_card_updescription() {
	std::string data = FileUtils::getInstance()->getStringFromFile("./content/chinese_card_updescription.txt");
	auto iter = data.begin();
	for (auto i = data.begin(); i != data.end(); ++i) {
		if (*i == '\n') {
			chinese_card_updescription.push_back(std::string(iter, i-1));
			iter = i + 1;
		}
	}
}
void Cards::read_english_card_name() {
	std::string data = FileUtils::getInstance()->getStringFromFile("./content/english_card_name.txt");
	auto iter = data.begin();
	for (auto i = data.begin(); i != data.end(); ++i) {
		if (*i == '\n') {
			english_card_name.push_back(std::string(iter, i-1));
			iter = i + 1;
		}
	}
}

//静态成员总方法
void Cards::card_init_() {
	Cards::read_chinese_card_name();
	Cards::read_chinese_card_description();
	Cards::read_chinese_card_updescription();
	Cards::read_english_card_name();
	srand(time(0));
}
//重新画耗能
void Cards::redraw_cost() {
	this->getChildByName("img")->removeChildByName("cost");
	auto label2 = Label::createWithTTF(StringUtils::format("%d", (_upgrade ? _temcost : _cost)), "./fonts/Marker Felt.ttf", 24);
	label2->setPosition(Vec2(61, 224));
	label2->setTextColor(Color4B::WHITE);
	label2->enableOutline(Color4B::BLACK, 2);
	this->getChildByName("img")->addChild(label2, 0, "cost");
}

//给对象初始化的方法，并且在里边设置卡牌的图片
void Cards::create_card(int id, int cost, int upcost, Ctype type, Cvalue value,
	Ccolour colour, int upgrade, const std::string& ename,
	CharacterBase* owner, CharacterBase* target) {
	_id = id;
	_cost = cost;
	_upcost = upcost;
	_upgrade = upgrade;
	_temcost = (_upgrade ? upcost : cost);
	_type = type;
	_value = value;
	_colour = colour;
	_ename = ename;
	_owner = owner;
	_target = target;
	//将图片加到card的子节点
	std::string img_path;
	switch (_colour) {

	case RED:
		switch (_value) {

		case INITIAL:
		case COMMON:
			img_path = "./imgs/cards/red_com_card.png";
			break;
		case VALUE:
			img_path = "./imgs/cards/red_value_card.png";
			break;
		case RARE:
			img_path = "./imgs/cards/red_rare_card.png";
			break;
		}
		break;
	case GREEN:
		switch (_value) {

		case INITIAL:
		case COMMON:
			img_path = "./imgs/cards/green_com_card.png";
			break;
		case VALUE:
			img_path = "./imgs/cards/green_value_card.png";
			break;
		case RARE:
			img_path = "./imgs/cards/green_rare_card.png";
			break;
		}
		break;
	case BLUE:
		switch (_value) {

		case INITIAL:
		case COMMON:
			img_path = "./content/cards/blue_com_card.png";
			break;
		case VALUE:
			img_path = "./content/cards/blue_value_card.png";
			break;
		case RARE:
			img_path = "./content/cards/blue_rare_card.png";
			break;
		}
		break;
	case GREY:
		switch (_value) {

		case INITIAL:
		case COMMON:
			img_path = "./imgs/cards/grey_com_card.png";
			break;
		case VALUE:
			img_path = "./imgs/cards/grey_value_card.png";
			break;
		case RARE:
			img_path = "./imgs/cards/grey_rare_card.png";
			break;
		}
		break;
	case BLACK:
		img_path = "./imgs/cards/black_card.png";
		break;
	}
	//卡牌贴图的地址
	std::string img2_path = "./imgs/cardimg";
	std::string card_type = "(State)";//卡牌类型字符串
	if (this->get_type() == CURSE)
		img2_path += "/curse";
	else if (this->get_type() == STATUS)
		img2_path += "/status";
	else {
		switch (this->get_color()) {
		case RED:
			img2_path += "/red";
			break;
		case GREEN:
			img2_path += "/green";
			break;
		case BLUE:
			img2_path += "/blue";
			break;
		case GREY:
			img2_path += "/grey";
			break;
		}
		switch (this->get_type()) {
		case ATTACK:
			img2_path += "/attack";
			card_type = "(Attack)";
			break;
		case POWER:
			img2_path += "/power";
			card_type = "(Power)";
			break;
		case SKILL:
			img2_path += "/skill";
			card_type = "(Skill)";
			break;
		}
	}
	auto img = Sprite::create(img_path);

	//把卡牌名字写上去,同时加上卡牌类型吧
	auto label1 = Label::createWithTTF(Cards::chinese_card_name[_id] + (upgrade ? "+" : "") + card_type, "./fonts/Myfont.ttf", 18);
	label1->setPosition(Vec2(128, 216));
	if(_upgrade)
		label1->setTextColor(Color4B::GREEN);
	else
		label1->setTextColor(Color4B::BLACK);

	auto label2 = Label::createWithTTF(StringUtils::format("%d", _temcost), "./fonts/Marker Felt.ttf", 24);
	label2->setPosition(Vec2(61, 224));
	label2->setTextColor(Color4B::WHITE);
	label2->enableOutline(Color4B::BLACK, 2);

	//description需要在具体的类中设置，因为数字可能不同。
	img->addChild(label1, 0, "name");//把文字加到img的子节点上，这样就一起显示了。
	img->addChild(label2, 0, "cost");// 创建子节点时加上名字，方便查找
	this->addChild(img, 0, "img");
	this->set_description();
	this->setScale(1.5);

	//把卡牌插图放上去。

	img2_path = img2_path + "/" + this->get_ename() + ".png";
	auto img2 = Sprite::create(img2_path);
	img2->setPosition(128, 160);
	this->getChildByName("img")->addChild(img2, 0, "ename");
	//接下来设置事件监听器
	auto listener = EventListenerTouchOneByOne::create();
	//触摸发生时的函数
	listener->onTouchBegan = [this](Touch* touch, Event* event)->bool
	{//不知道为什么用相对坐标原点一直不在左下角，设置锚点也没用
		if (Director::getInstance()->getRunningScene()->getChildByName("FightScene") == nullptr)
			return false;
		Vec2 pos = touch->getLocation();//还是绝对坐标系用得舒服
		float ratio = this->getScale();
		//Vec2 pos = this->convertTouchToNodeSpace(touch);//把鼠标点击的点转到卡牌的坐标系下
		//pos += Vec2(int(this->getScale() * 256), 0); // 因为卡牌的锚点被设置在了（0.5,0）
		Rect range = Rect::Rect(pos_x - 128 * ratio, pos_y, 256 * ratio, 256 * ratio);
		if (range.containsPoint(pos))//判断是否在卡牌里
			return true;
	};
	//鼠标按住移动时的函数
	listener->onTouchMoved = [this](Touch* touch, Event* event)->void
	{//用touch->getDelta()时老是有偏移，不懂
		this->setPosition(touch->getLocation());
	};
	//鼠标松开时的函数
	listener->onTouchEnded = [this](Touch* touch, Event* event)->void
	{//tnnd就是不能用Action，就各种偏移，回不到设置的点！花了3个小时各种调式也没用，我tm真服了
		auto pos = touch->getLocation();
		bool flag = false;
		for (auto enemy : v_enemy) {
			auto e_pos = enemy->get_pos();
			if (e_pos.x - 150 < pos.x && e_pos.x + 150 > pos.x && e_pos.y - 256 < pos.y && e_pos.y + 100 > pos.y) {
				this->set_target(enemy);
				if (hero->use_card(this))//查看能量是否够用
					flag = true;
				else
					this->getChildByName("img")->getChildByName("cost")->runAction(RotateBy::create(1.0f, 360.0f));
				//否则就搞一个动画，显示能量不够
				break;
			}
			if(!flag)
				this->setPosition(pos_x, pos_y);
		}
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

inline bool Cards::available() {
	if (hero->search_state("entangled") && _type == ATTACK) return false;
	return true;
}

void Cards::consume() {
	hero->handle_consume_buff();
	hero->get_v_consume_card()->pushBack(this);
	hero->get_v_tem_card()->eraseObject(this);
	this->removeFromParent();

}

void Cards::consume_power_card() {//使用完能力牌就直接消失
	hero->get_v_tem_card()->eraseObject(this);
	this->removeFromParent();
}

void Cards::set_description(){
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}

bool Cards::handle_remove_buff() {
	if (_id == 167) return false;
	else if (_id == 178) hero->add_max_health(-3);
	return true;
}

template<typename ConcreteCard>
Cards* CloneCard<ConcreteCard>::clone(int upgrade) {
	auto new_card = ConcreteCard::create();
	((Cards*)new_card)->create_card(this->get_id(), this->get_cost(), this->get_temcost(), this->get_type()
		, this->get_value(), this->get_color(), upgrade|this->get_upgrade(), this->get_ename(), this->get_owner(), this->get_target());
	//虽然直接给了指针地址，但应该问题不大，毕竟所有卡牌的拥有人都是空或者角色本人。
	return new_card;
}



//9
Anger* Anger::create_card(int upgrade) {
	auto ptr = Anger::create();
	((Cards*)ptr)->create_card(9, 0, 0, Ctype::ATTACK, Cvalue::COMMON, Ccolour::RED,
		upgrade, Cards::english_card_name[9]);
	return ptr;
}
void Anger::set_description() {
	std::string tem;//判断一下用哪个描述
	if (!this->get_upgrade())
		tem= StringUtils::format(chinese_card_description[this->get_id()].c_str(), damage);
	else if(chinese_card_updescription[this->get_id()].size() < 5)
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), updamage);
	else
		tem = StringUtils::format(chinese_card_updescription[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Anger::use() {
	int dam = hero->handle_attack_buff((this->get_upgrade() ? updamage : damage));
	this->get_target()->be_beaten(dam,hero);
	hero->get_v_discard()->pushBack(this->clone());
}
//10
Apotheosis* Apotheosis::create_card(int upgrade) {
	auto ptr = Apotheosis::create();
	((Cards*)ptr)->create_card(10, 2, 1, Ctype::SKILL, Cvalue::RARE, Ccolour::GREY,
		upgrade, Cards::english_card_name[10]);
	return ptr;
}
void Apotheosis::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Apotheosis::use(){
	auto tem = hero->get_v_tem_card();
	for (int i = 0; i < hero->get_v_tem_card()->size(); ++i) {
		if (tem->at(i)->get_upgrade())
			continue;
		tem->at(i)->removeFromParent();
		auto new_card = tem->at(i)->clone(1);
		tem->replace(i, new_card);
	}
	tem = hero->get_v_discard();
	for (int i = 0; i < hero->get_v_discard()->size(); ++i) {
		if (tem->at(i)->get_upgrade())
			continue;
		auto new_card = tem->at(i)->clone(1);
		tem->replace(i, new_card);
	}
	tem = hero->get_v_draw_card();
	for (int i = 0; i < hero->get_v_draw_card()->size(); ++i) {
		if (tem->at(i)->get_upgrade())
			continue;
		auto new_card = tem->at(i)->clone(1);
		tem->replace(i, new_card);
	}
	tem = hero->get_v_consume_card();
	for (int i = 0; i < hero->get_v_consume_card()->size(); ++i) {
		if (tem->at(i)->get_upgrade())
			continue;
		auto new_card = tem->at(i)->clone(1);
		tem->replace(i, new_card);
	}
	dynamic_cast<FightScene*>(hero->getParent())->ShowTemCards();
	this->consume();
}
//11
Armaments* Armaments::create_card(int upgrade) {
	auto ptr = Armaments::create();
	((Cards*)ptr)->create_card(11, 1, 1, Ctype::SKILL, Cvalue::COMMON, Ccolour::RED,
		upgrade, Cards::english_card_name[11]);

	return ptr;
}
void Armaments::set_description() {
	std::string tem;//判断一下用哪个描述
	if (!this->get_upgrade())
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), defend);
	else
		tem = StringUtils::format(chinese_card_updescription[this->get_id()].c_str(),defend);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Armaments::use() {//这里不考虑卡牌已经升级的情况，即可能会无效（如果刚好随机到了已经升级的牌）。
	int def = hero->handle_defend_buff(defend);
	hero->gain_defend(def);
	this->retain();
	auto tem = hero->get_v_tem_card();
	tem->eraseObject(this);
	int len = hero->get_v_tem_card()->size();
	if (len < 1)
		return;
	else {
		if (this->get_upgrade()) {
			for (int i = 0; i < len; ++i) {
				if (tem->at(i)->get_upgrade())
					continue;
				tem->at(i)->removeFromParent();
				auto new_card = tem->at(i)->clone(1);
				tem->replace(i, new_card);
			}
			dynamic_cast<FightScene*>(hero->getParent())->ShowTemCards();
			
		}
		else {
			auto v_card = hero->get_v_choose_card();
			for (auto c : *hero->get_v_tem_card()) {
				c->removeFromParent();
				v_card->pushBack(c);
			}
			hero->choose_tem_card(0);
		}
	}
	tem->pushBack(this);
	this->release();
}
//16
BandageUp* BandageUp::create_card(int upgrade) {
	auto ptr = BandageUp::create();
	((Cards*)ptr)->create_card(16, 0, 0, Ctype::SKILL, Cvalue::VALUE, Ccolour::GREY,
		upgrade, Cards::english_card_name[16]);
	return ptr;
}
void BandageUp::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? upheal : heal);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void BandageUp::use() {
	hero->add_health(this->get_upgrade() ? upheal : heal);
	this->consume();
}
//19
Barricade* Barricade::create_card(int upgrade) {
	auto ptr = Barricade::create();
	((Cards*)ptr)->create_card(19, 3, 2, Ctype::POWER, Cvalue::RARE, Ccolour::RED,
		upgrade, Cards::english_card_name[19]);

	return ptr;
}
void Barricade::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Barricade::use() {
	hero->add_power("barricade",1);
	this->consume_power_card();
}

//20
Bash* Bash::create_card(int upgrade) {
	auto ptr = Bash::create();
	((Cards*)ptr)->create_card(20, 2, 2, Ctype::ATTACK, Cvalue::INITIAL, Ccolour::RED,
		upgrade, Cards::english_card_name[20]);
	return ptr;
}
void Bash::set_description() {
	std::string tem;
	if (!this->get_upgrade())
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), damage,vulnerable);
	else if (chinese_card_updescription[this->get_id()].size() < 5)
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), updamage,upvulnerable);
	else
		tem = StringUtils::format(chinese_card_updescription[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Bash::use() {
	int dam = hero->handle_attack_buff((this->get_upgrade() ? updamage : damage));
	this->get_target()->be_beaten(dam,hero);
	if(this->get_target()->get_health()>0)
		this->get_target()->add_state("vulnerable", (this->get_upgrade() ? upvulnerable : vulnerable));
}

//21
BattleTrance* BattleTrance::create_card(int upgrade) {
	auto ptr = BattleTrance::create();
	((Cards*)ptr)->create_card(21, 0, 0, Ctype::SKILL, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[21]);
	return ptr;
}
void BattleTrance::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? upcard_num : card_num);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void BattleTrance::use() {
	hero->draw_card(this->get_upgrade() ? upcard_num : card_num);
	hero->add_state("no_draw", 1);
}
//23
Berserk* Berserk::create_card(int upgrade) {
	auto ptr = Berserk::create();
	((Cards*)ptr)->create_card(23, 0, 0, Ctype::POWER, Cvalue::RARE, Ccolour::RED,
		upgrade, Cards::english_card_name[23]);
	return ptr;
}
void Berserk::set_description() {
	std::string tem;
	if (!this->get_upgrade())
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str());
	else
		tem = StringUtils::format(chinese_card_updescription[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Berserk::use() {
	hero->add_state("vulnerable", this->get_upgrade() ? 1 : 2);
	hero->add_power("berserk", 1);
	this->consume_power_card();
}
//25
Bite* Bite::create_card(int upgrade) {
	auto ptr = Bite::create();
	((Cards*)ptr)->create_card(25, 1, 1, Ctype::ATTACK, Cvalue::COMMON, Ccolour::GREY,
		upgrade, Cards::english_card_name[25]);
	return ptr;
}
void Bite::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? updamage : damage, this->get_upgrade() ? upheal : heal);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Bite::use() {
	int dam = hero->handle_attack_buff(this->get_upgrade() ? updamage : damage);
	this->get_target()->be_beaten(dam,hero);
	hero->add_health(this->get_upgrade() ? upheal : heal);
}
//27
Blind* Blind::create_card(int upgrade) {
auto ptr = Blind::create();
((Cards*)ptr)->create_card(27, 0, 0, Ctype::SKILL, Cvalue::VALUE, Ccolour::GREY,
	upgrade, Cards::english_card_name[27]);
return ptr;
}
void Blind::set_description() {
	std::string tem;
	if(this->get_upgrade())
		tem = StringUtils::format(chinese_card_updescription[this->get_id()].c_str(), 1);
	else
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), 1);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Blind::use() {
	if (this->get_upgrade())
		for(auto e: v_enemy)
			e->add_state("weak", 1);
	else
		this->get_target()->add_state("weak", 1);
}
//29
BloodForBlood* BloodForBlood::create_card(int upgrade) {
	auto ptr = BloodForBlood::create();
	((Cards*)ptr)->create_card(29, 4, 4 ,Ctype::ATTACK, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[29]);
	return ptr;
}
void BloodForBlood::set_description() {
	std::string tem;
	if (!this->get_upgrade())
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), damage);
	else if (chinese_card_updescription[this->get_id()].size() < 5)
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), updamage);
	else
		tem = StringUtils::format(chinese_card_updescription[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void BloodForBlood::use() {
	int dam = hero->handle_attack_buff((this->get_upgrade() ? updamage : damage));
	this->get_target()->be_beaten(dam,hero);
}

//30
BloodLetting* BloodLetting::create_card(int upgrade) {
	auto ptr = BloodLetting::create();
	((Cards*)ptr)->create_card(30, 0, 0, Ctype::SKILL, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[30]);
	return ptr;
}
void BloodLetting::set_description() {
	std::string tem;
	if (this->get_upgrade())
		tem = StringUtils::format(chinese_card_updescription[this->get_id()].c_str());
	else
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void BloodLetting::use() {
	hero->lose_health(3);
	if(hero->get_health()>0)
		hero->add_energe_now(this->get_upgrade() ? 3 : 2);
	
}
//31
Blugeon* Blugeon::create_card(int upgrade) {
	auto ptr = ::Blugeon::create();
	((Cards*)ptr)->create_card(31, 3, 3, Ctype::ATTACK, Cvalue::RARE, Ccolour::RED,
		upgrade, Cards::english_card_name[31]);
	return ptr;
}
void Blugeon::set_description() {
	std::string tem;
	if (!this->get_upgrade())
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), damage);
	else if (chinese_card_updescription[this->get_id()].size() < 5)
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), updamage);
	else
		tem = StringUtils::format(chinese_card_updescription[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Blugeon::use() {
	int dam = hero->handle_attack_buff((this->get_upgrade() ? updamage : damage));
	this->get_target()->be_beaten(dam,hero);
}
//33
BodySlam* BodySlam::create_card(int upgrade) {
	auto ptr = BodySlam::create();
	((Cards*)ptr)->create_card(33, 1, 0, Ctype::ATTACK, Cvalue::COMMON, Ccolour::RED,
		upgrade, Cards::english_card_name[33]);
	return ptr;
}
void BodySlam::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void BodySlam::use() {
	int damage = hero->search_state("next_turn_block");
	int dam = hero->handle_attack_buff(damage);
	this->get_target()->be_beaten(dam,hero);
}
//36
Brutality* Brutality::create_card(int upgrade) {
	auto ptr = Brutality::create();
	((Cards*)ptr)->create_card(36, 0, 0, Ctype::POWER, Cvalue::RARE, Ccolour::RED,
		upgrade, Cards::english_card_name[36]);
	return ptr;
}
void Brutality::set_description() {
	std::string tem;
	if(this->get_upgrade())
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str());
	else 
		tem = StringUtils::format(chinese_card_updescription[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Brutality::use() {
	hero->add_power("brutality", 1);
	this->consume_power_card();
}
//39
Burn* Burn::create_card(int upgrade) {
	auto ptr = Burn::create();
	((Cards*)ptr)->create_card(39, 0, 0, Ctype::STATUS, Cvalue::COMMON, Ccolour::GREY,
		upgrade, Cards::english_card_name[39]);
	return ptr;
}
void Burn::set_description() {
	std::string tem;
	if (!this->get_upgrade())
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str());
	else
		tem = StringUtils::format(chinese_card_updescription[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
//40
BurningPact* BurningPact::create_card(int upgrade) {
	auto ptr = BurningPact::create();
	((Cards*)ptr)->create_card(40, 1, 1, Ctype::SKILL, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[40]);
	return ptr;
}
void BurningPact::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? upcard_num : card_num);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void BurningPact::use() {
	this->retain();
	hero->get_v_tem_card()->eraseObject(this);
	if (hero->get_v_tem_card()->size() > 0);
		hero->handle_consume_buff();
	for (auto card : *hero->get_v_tem_card())
		card->removeFromParent();
	hero->choose_card(hero->get_v_tem_card(), hero->get_v_consume_card(),this->get_upgrade()?upcard_num:card_num);
	hero->get_v_tem_card()->pushBack(this);
	this->release();//考虑到卡牌可能是临时的，所以要放止这个被释放掉
	//画目前拥有的卡(抽卡)在end_choose_card()中，因为这两边实际上是同时进行的，如果我在这里直接画卡了，
	//那么另一边就显示不了.
	
}

//45
Carnage* Carnage::create_card(int upgrade) {
	auto ptr = Carnage::create();
	((Cards*)ptr)->create_card(45, 2, 2, Ctype::ATTACK, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[45]);
	return ptr;
}
void Carnage::set_description() {
	std::string tem;
	if (!this->get_upgrade())
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), damage);
	else if (chinese_card_updescription[this->get_id()].size() < 5)
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), updamage);
	else
		tem = StringUtils::format(chinese_card_updescription[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Carnage::use() {
	int dam = hero->handle_attack_buff((this->get_upgrade() ? updamage : damage));
	this->get_target()->be_beaten(dam,hero);
}
//50
Clash* Clash::create_card(int upgrade) {
	auto ptr = Clash::create();
	((Cards*)ptr)->create_card(50, 0, 0, Ctype::ATTACK, Cvalue::COMMON, Ccolour::RED,
		upgrade, Cards::english_card_name[50]);
	return ptr;
}
void Clash::set_description() {
	std::string tem;
	if (!this->get_upgrade())
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), damage);
	else if (chinese_card_updescription[this->get_id()].size() < 5)
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), updamage);
	else
		tem = StringUtils::format(chinese_card_updescription[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Clash::use() {
	int dam = hero->handle_attack_buff((this->get_upgrade() ? updamage : damage));
	this->get_target()->be_beaten(dam,hero);
}
bool Clash::available() {
	if (Cards::available())
		return false;
	for (auto c : *hero->get_v_tem_card()) 
		if (c->get_type() != ATTACK) 
			return false;
	return true;
}
//51
Cleave* Cleave::create_card(int upgrade) {
	auto ptr = Cleave::create();
	((Cards*)ptr)->create_card(51, 1, 1, Ctype::ATTACK, Cvalue::COMMON, Ccolour::RED,
		upgrade, Cards::english_card_name[51]);
	return ptr;
}
void Cleave::set_description() {
	std::string tem;
	if (!this->get_upgrade())
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), damage);
	else if (chinese_card_updescription[this->get_id()].size() < 5)
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), updamage);
	else
		tem = StringUtils::format(chinese_card_updescription[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Cleave::use() {
	int dam = hero->handle_attack_buff((this->get_upgrade() ? updamage : damage));
	for (auto e : v_enemy) {
		e->be_beaten(dam,hero);
	}
		
}
//53
Clothesline* Clothesline::create_card(int upgrade) {
	auto ptr = Clothesline::create();
	((Cards*)ptr)->create_card(53, 2, 2, Ctype::ATTACK, Cvalue::COMMON, Ccolour::RED,
		upgrade, Cards::english_card_name[53]);
	return ptr;
}
void Clothesline::set_description() {
	std::string tem;
	if (!this->get_upgrade())
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), damage,weak);
	else if (chinese_card_updescription[this->get_id()].size() < 5)
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), updamage,upweak);
	else
		tem = StringUtils::format(chinese_card_updescription[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Clothesline::use() {
	int dam = hero->handle_attack_buff((this->get_upgrade() ? updamage : damage));
	this->get_target()->be_beaten(dam,hero);
	if (this->get_target()->get_health() > 0)
		this->get_target()->add_state("weak", (this->get_upgrade() ? upweak : weak));
}
//54
Clumsy* Clumsy::create_card(int upgrade) {
	auto ptr = Clumsy::create();
	((Cards*)ptr)->create_card(54, 0, 0, Ctype::CURSE, Cvalue::COMMON, Ccolour::BLACK,
		upgrade, Cards::english_card_name[54]);
	return ptr;
}
//56
Combust* Combust::create_card(int upgrade) {
	auto ptr = Combust::create();
	((Cards*)ptr)->create_card(56, 1, 1, Ctype::POWER, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[56]);
	return ptr;
}
void Combust::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? updamage : damage);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Combust::use() {
	hero->add_power("combust", this->get_upgrade() ? updamage : damage);
	this->consume_power_card();
}
//62
Corruption* Corruption::create_card(int upgrade) {
	auto ptr = Corruption::create();
	((Cards*)ptr)->create_card(62, 3, 2, Ctype::POWER, Cvalue::RARE, Ccolour::RED,
		upgrade, Cards::english_card_name[62]);
	return ptr;
}
void Corruption::use() {
	hero->add_power("corruption", 1);
	this->consume_power_card();
}
//67
DarkEmbrace* DarkEmbrace::create_card(int upgrade) {
	auto ptr = DarkEmbrace::create();
	((Cards*)ptr)->create_card(67, 2, 1, Ctype::POWER, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[67]);
	return ptr;
}
void DarkEmbrace::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void DarkEmbrace::use() {
	hero->add_power("darkembrace", 1);
	this->consume_power_card();
}
//71
Dazed* Dazed::create_card(int upgrade) {
	auto ptr = Dazed::create();
	((Cards*)ptr)->create_card(71, 0, 0, Ctype::STATUS, Cvalue::COMMON, Ccolour::GREY,
		upgrade, Cards::english_card_name[71]);
	return ptr;
}
//73
Decay* Decay::create_card(int upgrade) {
	auto ptr = Decay::create();
	((Cards*)ptr)->create_card(73, 0, 0, Ctype::CURSE, Cvalue::COMMON, Ccolour::BLACK,
		upgrade, Cards::english_card_name[73]);
	return ptr;
}
//74
DeepBreath* DeepBreath::create_card(int upgrade) {
	auto ptr = DeepBreath::create();
	((Cards*)ptr)->create_card(74, 0, 0, Ctype::SKILL, Cvalue::VALUE, Ccolour::GREY,
		upgrade, Cards::english_card_name[74]);
	return ptr;
}
void DeepBreath::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? 2 : 1);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void DeepBreath::use() {
	hero->Shuffle();
	hero->draw_card(this->get_upgrade() ? 2 : 1);
}
//77
Defend_R* Defend_R::create_card(int upgrade) {
	auto ptr = Defend_R::create();
	((Cards*)ptr)->create_card(77, 1, 1, Ctype::SKILL, Cvalue::INITIAL, Ccolour::RED,
		upgrade, Cards::english_card_name[77]);

	return ptr;
}
void Defend_R::set_description() {
	std::string tem;
	if (!this->get_upgrade())
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), defend);
	else if (chinese_card_updescription[this->get_id()].size() < 5)
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), updefend);
	else
		tem = StringUtils::format(chinese_card_updescription[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Defend_R::use() {
	int def = hero->handle_defend_buff(this->get_upgrade() ? updefend : defend);
	hero->gain_defend(def);
}
//80
DemonForm* DemonForm::create_card(int upgrade) {
	auto ptr = DemonForm::create();
	((Cards*)ptr)->create_card(80, 3, 3, Ctype::POWER, Cvalue::RARE, Ccolour::RED,
		upgrade, Cards::english_card_name[80]);

	return ptr;
}
void DemonForm::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
			this->get_upgrade() ? upstrength : strength);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void DemonForm::use() {
	hero->add_power("demonform", this->get_upgrade() ? upstrength : strength);
	this->consume_power_card();
}
//82
Disarm* Disarm::create_card(int upgrade) {
	auto ptr = Disarm::create();
	((Cards*)ptr)->create_card(82, 1, 1, Ctype::SKILL, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[82]);

	return ptr;
}
void Disarm::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), 
			this->get_upgrade()?upstrength:strength);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Disarm::use() {
	this->get_target()->add_power("strength", this->get_upgrade() ? -upstrength : -strength);
	this->consume();
}

//88
DoubleTap* DoubleTap::create_card(int upgrade) {
	auto ptr = DoubleTap::create();
	((Cards*)ptr)->create_card(88, 1, 1, Ctype::SKILL, Cvalue::RARE, Ccolour::RED,
		upgrade, Cards::english_card_name[88]);

	return ptr;
}
void DoubleTap::set_description() {
	std::string tem;
	if (!this->get_upgrade())
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str());
	else
		tem = StringUtils::format(chinese_card_updescription[this->get_id()].c_str(),2);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void DoubleTap::use() {
	hero->add_state("doubletag", 1);
	this->consume();
}
//89
Doubt* Doubt::create_card(int upgrade) {
	auto ptr = Doubt::create();
	((Cards*)ptr)->create_card(89, 0, 0, Ctype::CURSE, Cvalue::COMMON, Ccolour::BLACK,
		upgrade, Cards::english_card_name[89]);
	return ptr;
}
//90
DramaticEntrance* DramaticEntrance::create_card(int upgrade) {
	auto ptr = DramaticEntrance::create();
	((Cards*)ptr)->create_card(90, 0, 0, Ctype::ATTACK, Cvalue::VALUE, Ccolour::GREY,
		upgrade, Cards::english_card_name[90]);
	return ptr;
}
void DramaticEntrance::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? updamage : damage);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void DramaticEntrance::use() {
	int dam = hero->handle_attack_buff(this->get_upgrade() ? updamage : damage);
	for (auto e : v_enemy)
		e->be_beaten(dam,hero);
	this->consume();
}
//91
Dropkick* Dropkick::create_card(int upgrade) {
	auto ptr = Dropkick::create();
	((Cards*)ptr)->create_card(91, 1, 1, Ctype::ATTACK, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[91]);
	return ptr;
}
void Dropkick::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		(this->get_upgrade() ? updamage : damage), gain_energy);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Dropkick::use() {
	//懒得从逻辑上改了，因为当最后一个敌人死亡后就直接跳到奖励了，所以没法进行下一步抽卡什么的，故我把顺序交换了一下
	if (this->get_target()->search_state("vulnerable")) {
		hero->add_energe_now(1);
		hero->draw_card(1);
	}
	int dam = hero->handle_attack_buff((this->get_upgrade() ? updamage : damage));
	this->get_target()->be_beaten(dam,hero);
}
//92
DualWield* DualWield::create_card(int upgrade) {
	auto ptr = DualWield::create();
	((Cards*)ptr)->create_card(92, 1, 1, Ctype::SKILL, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[92]);
	return ptr;
}
void DualWield::set_description() {
	std::string tem;
	if (!this->get_upgrade())
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str());
	else
		tem = StringUtils::format(chinese_card_updescription[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void DualWield::use() {
	auto v_card = hero->get_v_choose_card();
	for (auto card : *hero->get_v_tem_card())
		if(card->get_type() == ATTACK || card->get_type()== POWER)
			v_card->pushBack(card->clone());
	if (v_card->size() == 0)
		return;
	hero->choose_tem_card(this->get_upgrade() ? 2 : 1);
}
//97
Enlightment* Enlightment::create_card(int upgrade) {
	auto ptr = Enlightment::create();
	((Cards*)ptr)->create_card(97, 0, 0, Ctype::SKILL, Cvalue::VALUE, Ccolour::GREY,
		upgrade, Cards::english_card_name[97]);
	return ptr;
}
void Enlightment::set_description() {
	std::string tem;
	if (!this->get_upgrade())
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str());
	else
		tem = StringUtils::format(chinese_card_updescription[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Enlightment::use() {
	int last_for = this->get_upgrade() ? 0 : 1;
	for (auto c : (*hero->get_v_tem_card()))
		if (c != (Cards*)this) c->set_temcost(1, last_for);
}
//98
Entrench* Entrench::create_card(int upgrade) {
	auto ptr = Entrench::create();
	((Cards*)ptr)->create_card(98, 2, 1, Ctype::SKILL, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[98]);
	return ptr;
}
void Entrench::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Entrench::use() {
	int defend = hero->search_state("next_turn_block");
	if(defend>0)
		hero->gain_defend(defend);
}
//102
Evolve* Evolve::create_card(int upgrade) {
	auto ptr = Evolve::create();
	((Cards*)ptr)->create_card(102, 1, 1, Ctype::POWER, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[102]);
	return ptr;
}
void Evolve::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade()?2:1);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Evolve::use() {
	hero->add_power("evolve", this->get_upgrade() ? 2 : 1);
	this->consume_power_card();
}
//103
Exhume* Exhume::create_card(int upgrade) {
	auto ptr = Exhume::create();
	((Cards*)ptr)->create_card(103, 1, 0, Ctype::SKILL, Cvalue::RARE, Ccolour::RED,
		upgrade, Cards::english_card_name[103]);
	return ptr;
}
void Exhume::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Exhume::use() {
	if (hero->get_v_consume_card()->size() > 0)
		hero->choose_card(hero->get_v_consume_card(), hero->get_v_tem_card());
	this->consume();
}

//106
Feed* Feed::create_card(int upgrade) {
	auto ptr = Feed::create();
	((Cards*)ptr)->create_card(106, 1, 1, Ctype::ATTACK, Cvalue::RARE, Ccolour::RED,
		upgrade, Cards::english_card_name[106]);
	return ptr;
}
void Feed::set_description() {
	std::string tem;
	if (!this->get_upgrade())
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), damage,gain_max_health);
	else if (chinese_card_updescription[this->get_id()].size() < 5)
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), updamage,upgain_max_health);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Feed::use() {
	int dam = hero->handle_attack_buff((this->get_upgrade() ? updamage : damage));
	this->get_target()->be_beaten(dam,hero);
	//卡片消耗
	this->consume();
	if (this->get_target()->get_health() < 1)
		hero->add_max_health(this->get_upgrade() ? gain_max_health : upgain_max_health);
}

//107
FeelNoPain* FeelNoPain::create_card(int upgrade) {
	auto ptr = FeelNoPain::create();
	((Cards*)ptr)->create_card(107, 1, 1, Ctype::POWER, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[107]);
	return ptr;
}
void FeelNoPain::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade()?updefend:defend);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void FeelNoPain::use() {
	hero->add_power("feelnopain", this->get_upgrade() ? updefend : defend);
	this->consume_power_card();
}
//108
FiendFire* FiendFire::create_card(int upgrade) {
	auto ptr = FiendFire::create();
	((Cards*)ptr)->create_card(108, 2, 2, Ctype::ATTACK, Cvalue::RARE, Ccolour::RED,
		upgrade, Cards::english_card_name[108]);
	return ptr;
}
void FiendFire::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),this->get_upgrade()? damage:updamage);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void FiendFire::use() {
	auto v_card = hero->get_v_tem_card();
	int num = v_card->size();
	int dam = hero->handle_attack_buff((this->get_upgrade() ? num*updamage : num*damage));
	this->get_target()->be_beaten(dam,hero);
	//卡片消耗,这里注意敌人全部死亡后v_card会clear
	while (v_card->size())
		v_card->back()->consume();

}
//109
Finesse* Finesse::create_card(int upgrade) {
	auto ptr = Finesse::create();
	((Cards*)ptr)->create_card(109, 0, 0, Ctype::SKILL, Cvalue::VALUE, Ccolour::GREY,
		upgrade, Cards::english_card_name[109]);
	return ptr;
}
void Finesse::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), this->get_upgrade() ? defend : updefend);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Finesse::use() {
	int def = hero->handle_defend_buff(this->get_upgrade() ? updefend : defend);
	hero->gain_defend(def);
	hero->draw_card(1);
}
//111
FireBreathing* FireBreathing::create_card(int upgrade) {
	auto ptr = FireBreathing::create();
	((Cards*)ptr)->create_card(111, 1, 1, Ctype::POWER, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[111]);
	return ptr;
}
void FireBreathing::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), 
		this->get_upgrade() ? damage : updamage);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void FireBreathing::use() {
	hero->add_power("firebreathing", this->get_upgrade() ? updamage : damage);
	this->consume_power_card();

}
//113
FlameBarrier* FlameBarrier::create_card(int upgrade) {
	auto ptr = FlameBarrier::create();
	((Cards*)ptr)->create_card(113, 2, 2, Ctype::SKILL, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[113]);
	return ptr;
}
void FlameBarrier::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void FlameBarrier::use() {
	int def = hero->handle_defend_buff(this->get_upgrade() ? updefend : defend);
	hero->gain_defend(def);
	hero->add_state("flamebarrier", this->get_upgrade()?updamage:damage);
}
//114
FlashofSteel* FlashofSteel::create_card(int upgrade) {
	auto ptr = FlashofSteel::create();
	((Cards*)ptr)->create_card(114, 0, 0, Ctype::ATTACK, Cvalue::VALUE, Ccolour::GREY,
		upgrade, Cards::english_card_name[114]);
	return ptr;
}
void FlashofSteel::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? updamage : damage);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void FlashofSteel::use() {
	int dam = hero->handle_attack_buff(this->get_upgrade() ? updamage : damage);
	this->get_target()->be_beaten(dam,hero);
	hero->draw_card(1);
}
//116
Flex* Flex::create_card(int upgrade) {
	auto ptr = Flex::create();
	((Cards*)ptr)->create_card(116, 0, 0, Ctype::SKILL, Cvalue::COMMON, Ccolour::RED,
		upgrade, Cards::english_card_name[116]);
	return ptr;
}
void Flex::set_description() {
	int _strength = this->get_upgrade() ? upstrength : strength;
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		_strength,_strength);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Flex::use() {//这里我把buff效果消失放在下回合开始的时候，我觉得目前没有太大问题，懒得再写函数了。
	int _strength = this->get_upgrade() ? upstrength : strength;
	hero->add_power("strength", _strength);
	hero->add_state("strengthdown", _strength);
}
//123
Ghostly* Ghostly::create_card(int upgrade) {
	auto ptr = Ghostly::create();
	((Cards*)ptr)->create_card(123, 1, 0, Ctype::SKILL, Cvalue::COMMON, Ccolour::GREY,
		upgrade, Cards::english_card_name[123]);
	return ptr;
}
void Ghostly::set_description() {
	std::string tem = StringUtils::format(this->get_upgrade()? chinese_card_updescription[this->get_id()].c_str():
		chinese_card_description[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Ghostly::use() {
	hero->add_power("intangible", 1);
	this->consume();
}
//124
GhostlyArmor* GhostlyArmor::create_card(int upgrade) {
	auto ptr = GhostlyArmor::create();
	((Cards*)ptr)->create_card(124, 1, 1, Ctype::SKILL, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[124]);
	return ptr;
}
void GhostlyArmor::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? updefend : defend);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void GhostlyArmor::use() {
	int def = hero->handle_defend_buff(this->get_upgrade() ? updefend : defend);
	hero->gain_defend(def);
}
//128
GoodInstincts* GoodInstincts::create_card(int upgrade) {
	auto ptr = GoodInstincts::create();
	((Cards*)ptr)->create_card(128, 0, 0, Ctype::SKILL, Cvalue::VALUE, Ccolour::GREY,
		upgrade, Cards::english_card_name[128]);
	return ptr;
}
void GoodInstincts::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? updefend : defend);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void GoodInstincts::use() {
	int def = hero->handle_defend_buff(this->get_upgrade() ? updefend : defend);
	hero->gain_defend(def);
}
//130
Havoc* Havoc::create_card(int upgrade) {
	auto ptr = Havoc::create();
	((Cards*)ptr)->create_card(130, 1, 0, Ctype::SKILL, Cvalue::COMMON, Ccolour::RED,
		upgrade, Cards::english_card_name[130]);
	return ptr;
}
void Havoc::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Havoc::use() {
	/*这个功能目前还是有些许不足，因为runAction的执行与后面的代码貌似是并行的，
	* 所以一些会更换场景的卡牌会在显示前先调用，于是你就不知道用了什么牌，
	*感觉也没法改，就这样吧*/
	auto card = hero->get_v_draw_card()->back()->clone();
	hero->get_v_tem_card()->pushBack(card);//复制一份卡牌，并且不再利用旧牌了
	if (card->get_type() == ATTACK)
		card->set_target(v_enemy.at(random(0, int(v_enemy.size() - 1))));
	else
		card->set_target(hero);
	auto old_card = hero->get_v_draw_card()->back();
	dynamic_cast<FightScene*>(hero->getParent())->addChild(old_card, 1);//旧的牌在目标上显示一下
	old_card->set_pos(card->get_target()->get_pos().x, card->get_target()->get_pos().y);
	old_card->runAction(Sequence::create(RotateBy::create(1.0f, 0),
		CallFunc::create([old_card]() {
			old_card->removeFromParent();//显示完直接删除
			})
		, nullptr));
	hero->get_v_draw_card()->popBack();
	card->set_temcost(0,1);
	hero->use_card(card);
	if (v_enemy.empty())
		return;
	auto v_card = hero->get_v_discard();
	//如果这个牌本身不消耗，use之后它会出现在弃牌堆里，因为consume有buff处理，所以再把它移到手牌里消耗
	if (v_card->find(card) != v_card->end()) {
		hero->get_v_tem_card()->pushBack(card);
		v_card->eraseObject(card);
		card->consume();
	}
}
//131
HeadButt* HeadButt::create_card(int upgrade) {
	auto ptr = HeadButt::create();
	((Cards*)ptr)->create_card(131, 1, 1, Ctype::ATTACK, Cvalue::COMMON, Ccolour::RED,
		upgrade, Cards::english_card_name[131]);
	return ptr;
}
void HeadButt::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), this->get_upgrade() ? damage : updamage);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void HeadButt::use() {
	int dam = hero->handle_attack_buff((this->get_upgrade() ? updamage : damage));
	this->get_target()->be_beaten(dam,hero);
	if(!v_enemy.empty())
		hero->choose_card(hero->get_v_discard(),hero->get_v_draw_card());
}

//133
HeavyBlade* HeavyBlade::create_card(int upgrade) {
	auto ptr = HeavyBlade::create();
	((Cards*)ptr)->create_card(133, 2, 2, Ctype::ATTACK, Cvalue::COMMON, Ccolour::RED,
		upgrade, Cards::english_card_name[133]);
	return ptr;
}
void HeavyBlade::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
	damage,	this->get_upgrade() ? uppower : power);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void HeavyBlade::use() {
	int addition = hero->search_state("strength");//原来是3和5，现在先改为2和4，然后计算力量时再统一加1
	int dam = hero->handle_attack_buff((this->get_upgrade() ? 4 : 2) * addition + damage);
	this->get_target()->be_beaten(dam,hero);
}
//136
Hemokinesis* Hemokinesis::create_card(int upgrade) {
	auto ptr = Hemokinesis::create();
	((Cards*)ptr)->create_card(136, 1, 1, Ctype::ATTACK, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[136]);
	return ptr;
}
void Hemokinesis::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		2, this->get_upgrade() ? damage : updamage);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Hemokinesis::use() {
	hero->lose_health(2);
	if (hero->get_health() > 0) {
		int dam = hero->handle_attack_buff((this->get_upgrade() ? updamage : damage));
		this->get_target()->be_beaten(dam,hero);
	}
}
//140
Immolate* Immolate::create_card(int upgrade) {
	auto ptr = Immolate::create();
	((Cards*)ptr)->create_card(140, 2, 2, Ctype::ATTACK, Cvalue::RARE, Ccolour::RED,
		upgrade, Cards::english_card_name[140]);
	return ptr;
}
void Immolate::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? updamage : damage);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Immolate::use() {
	int dam = hero->handle_attack_buff((this->get_upgrade() ? updamage : damage));
	for(auto e: v_enemy)
		e->be_beaten(dam,hero);
	hero->get_v_discard()->pushBack(Burn::create_card());
}
//141
Impervious* Impervious::create_card(int upgrade) {
	auto ptr = Impervious::create();
	((Cards*)ptr)->create_card(141, 2, 2, Ctype::SKILL, Cvalue::RARE, Ccolour::RED,
		upgrade, Cards::english_card_name[141]);

	return ptr;
}
void Impervious::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? updefend : defend);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Impervious::use() {
	int def = hero->handle_defend_buff(this->get_upgrade() ? updefend : defend);
	hero->gain_defend(def);
	this->consume();
}
//142
InfernalBlade* InfernalBlade::create_card(int upgrade) {
	auto ptr = InfernalBlade::create();
	((Cards*)ptr)->create_card(142, 1, 0, Ctype::SKILL, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[142]);
	return ptr;
}
void InfernalBlade::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void InfernalBlade::use() {
	using CardVector::red_attack;
	int upgrade = random(1, 10) > 9 ? 1 : 0;
	auto card = red_attack.at(random(0, int(red_attack.size() - 1)))->clone(upgrade);
	int cost = card->get_temcost();
	card->set_temcost(0, 1);
	hero->get_v_tem_card()->pushBack(card);
	dynamic_cast<FightScene*>(hero->getParent())->ShowTemCards();
}
//144
Inflame* Inflame::create_card(int upgrade) {
	auto ptr = Inflame::create();
	((Cards*)ptr)->create_card(144, 1, 1, Ctype::POWER, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[144]);
	return ptr;
}
void Inflame::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? upstrength : strength);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Inflame::use() {
	hero->add_power("strength", this->get_upgrade() ? upstrength : strength);
	this->consume_power_card();
}
//145
Injury* Injury::create_card(int upgrade) {
	auto ptr = Injury::create();
	((Cards*)ptr)->create_card(145, 0, 0, Ctype::CURSE, Cvalue::COMMON, Ccolour::BLACK,
		upgrade, Cards::english_card_name[145]);
	return ptr;
}
//146
Intimidate* Intimidate::create_card(int upgrade) {
	auto ptr = Intimidate::create();
	((Cards*)ptr)->create_card(146, 0, 0, Ctype::SKILL, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[146]);
	return ptr;
}
void Intimidate::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? upweak : weak);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Intimidate::use() {
	int _weak = this->get_upgrade() ? upweak : weak;
	for (auto e : v_enemy) {
		e->add_state("weak", _weak);
	}
	this->consume();
}

//147
IronWave* IronWave::create_card(int upgrade) {
	auto ptr = IronWave::create();
	((Cards*)ptr)->create_card(147, 1, 1, Ctype::ATTACK, Cvalue::COMMON, Ccolour::RED,
		upgrade, Cards::english_card_name[147]);
	return ptr;
}
void IronWave::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? damage : updamage, this->get_upgrade() ? updefend : defend);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void IronWave::use() {
	int def = hero->handle_defend_buff(this->get_upgrade() ? updefend : defend);
	hero->gain_defend(def);
	int dam = hero->handle_attack_buff((this->get_upgrade() ? updamage : damage));
	this->get_target()->be_beaten(dam,hero);

}
//148
Jax* Jax::create_card(int upgrade) {
	auto ptr = Jax::create();
	((Cards*)ptr)->create_card(148, 0, 0, Ctype::SKILL, Cvalue::VALUE, Ccolour::GREY,
		upgrade, Cards::english_card_name[148]);
	return ptr;
}
void Jax::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? upstrength : strength);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Jax::use() {
	hero->lose_health(3);
	hero->add_power("strength", this->get_upgrade() ? upstrength : strength);
	this->consume();
}
//149
JackOfAllTrades* JackOfAllTrades::create_card(int upgrade) {
	auto ptr = JackOfAllTrades::create();
	((Cards*)ptr)->create_card(149, 0, 0, Ctype::SKILL, Cvalue::VALUE, Ccolour::GREY,
		upgrade, Cards::english_card_name[149]);
	return ptr;
}
void JackOfAllTrades::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? 2 : 1);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void JackOfAllTrades::use() {
	for (int i = 0; i < this->get_upgrade() ? 2 : 1; ++i) {
		auto new_card = dynamic_cast<FightScene*>(hero->getParent())->Choose_random_card(INITIAL, CURSE, GREY);
		hero->get_v_tem_card()->pushBack(new_card);
	}
	dynamic_cast<FightScene*>(hero->getParent())->ShowTemCards();
	this->consume();
}
//150
Juggernaut* Juggernaut::create_card(int upgrade) {
	auto ptr = Juggernaut::create();
	((Cards*)ptr)->create_card(150, 2, 2, Ctype::POWER, Cvalue::RARE, Ccolour::RED,
		upgrade, Cards::english_card_name[150]);
	return ptr;
}
void Juggernaut::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? damage : updamage);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Juggernaut::use() {
	hero->add_power("juggernaut", this->get_upgrade() ? updamage : damage);
	this->consume_power_card();

}

//153
LimitBreak* LimitBreak::create_card(int upgrade) {
	auto ptr = LimitBreak::create();
	((Cards*)ptr)->create_card(153, 1, 1, Ctype::SKILL, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[153]);
	return ptr;
}
void LimitBreak::set_description() {
	std::string tem = StringUtils::format(this->get_upgrade() ? chinese_card_updescription
		[this->get_id()].c_str():chinese_card_description[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void LimitBreak::use() {
	if (hero->search_power("strength")) 
		hero->add_power("strength", hero->search_power("strength"));
	if (!this->get_upgrade())
		this->consume();
}
//157
Madness* Madness::create_card(int upgrade) {
	auto ptr = Madness::create();
	((Cards*)ptr)->create_card(157, 1, 0, Ctype::SKILL, Cvalue::VALUE, Ccolour::GREY,
		upgrade, Cards::english_card_name[157]);
	return ptr;
}
void Madness::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Madness::use() {
	auto cards = hero->get_v_tem_card();
	this->retain();
	cards->eraseObject(this);
	int len = cards->size();
	if(len) hero->get_v_tem_card()->at(random(0,len - 1))->set_temcost(0);
	cards->pushBack(this);
	this->release();
	this->consume();
}
//158
Magnetism* Magnetism::create_card(int upgrade) {
	auto ptr = Magnetism::create();
	((Cards*)ptr)->create_card(158, 2, 1, Ctype::POWER, Cvalue::RARE, Ccolour::GREY,
		upgrade, Cards::english_card_name[158]);
	return ptr;
}
void Magnetism::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Magnetism::use() {
	hero->add_power("magnetism", 1);
	this->consume_power_card();
}
//160
MasterofStrategy* MasterofStrategy::create_card(int upgrade) {
	auto ptr = MasterofStrategy::create();
	((Cards*)ptr)->create_card(160, 0, 0, Ctype::SKILL, Cvalue::RARE, Ccolour::GREY,
		upgrade, Cards::english_card_name[160]);
	return ptr;
}
void MasterofStrategy::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? upcard_num : card_num);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void MasterofStrategy::use() {
	hero->draw_card(this->get_upgrade() ? upcard_num : card_num);
	this->consume();
}
//163
Metallicize* Metallicize::create_card(int upgrade) {
	auto ptr = Metallicize::create();
	((Cards*)ptr)->create_card(163, 1, 1, Ctype::POWER, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[163]);
	return ptr;
}
void Metallicize::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? updefend : defend);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Metallicize::use() {
	hero->add_power("metallicize", this->get_upgrade() ? updefend : defend);
	this->consume_power_card();
}
//165
MindBlast* MindBlast::create_card(int upgrade) {
	auto ptr = MindBlast::create();
	((Cards*)ptr)->create_card(165, 2, 1, Ctype::ATTACK, Cvalue::VALUE, Ccolour::GREY,
		upgrade, Cards::english_card_name[165]);
	return ptr;
}
void MindBlast::set_description() {
	std::string tem = StringUtils::format(chinese_card_updescription[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void MindBlast::use() {
	int dam = hero->handle_attack_buff(hero->get_v_draw_card()->size());
	this->get_target()->be_beaten(dam,hero);
}
//167
Necronomicurse* Necronomicurse::create_card(int upgrade) {
	auto ptr = Necronomicurse::create();
	((Cards*)ptr)->create_card(167, 0, 0, Ctype::CURSE, Cvalue::COMMON, Ccolour::BLACK,
		upgrade, Cards::english_card_name[167]);
	return ptr;
}
//170
Normality* Normality::create_card(int upgrade) {
	auto ptr = Normality::create();
	((Cards*)ptr)->create_card(170, 0, 0, Ctype::CURSE, Cvalue::COMMON, Ccolour::BLACK,
		upgrade, Cards::english_card_name[170]);
	return ptr;
}
//173
Offering* Offering::create_card(int upgrade) {
	auto ptr = Offering::create();
	((Cards*)ptr)->create_card(173, 0, 0, Ctype::SKILL, Cvalue::RARE, Ccolour::RED,
		upgrade, Cards::english_card_name[173]);
	return ptr;
}
void Offering::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? upcard_num : card_num);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Offering::use() {
	hero->lose_health(4);
	if (hero->get_health() > 0) {
		hero->add_energe_now(2);
		hero->draw_card(this->get_upgrade() ? upcard_num : card_num);
	}
}
//175
Pain* Pain::create_card(int upgrade) {
	auto ptr = Pain::create();
	((Cards*)ptr)->create_card(175, 0, 0, Ctype::CURSE, Cvalue::COMMON, Ccolour::BLACK,
		upgrade, Cards::english_card_name[175]);
	return ptr;
}
//176
Panacea* Panacea::create_card(int upgrade) {
	auto ptr = Panacea::create();
	((Cards*)ptr)->create_card(176, 0, 0, Ctype::SKILL, Cvalue::VALUE, Ccolour::GREY,
		upgrade, Cards::english_card_name[176]);
	return ptr;
}
void Panacea::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? upartifact : artifact);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Panacea::use() {
	hero->add_power("artifact", this->get_upgrade() ? upartifact : artifact);
	this->consume();
}
//177
Panache* Panache::create_card(int upgrade) {
	auto ptr = Panache::create();
	((Cards*)ptr)->create_card(177, 0, 0, Ctype::POWER, Cvalue::RARE, Ccolour::GREY,
		upgrade, Cards::english_card_name[177]);
	return ptr;
}
void Panache::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? updamage : damage);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Panache::use() {
	hero->add_power("panache", this->get_upgrade() ? updamage : damage);
	this->consume_power_card();
}
//178
Parasite* Parasite::create_card(int upgrade) {
	auto ptr = Parasite::create();
	((Cards*)ptr)->create_card(178, 0, 0, Ctype::CURSE, Cvalue::COMMON, Ccolour::BLACK,
		upgrade, Cards::english_card_name[178]);
	return ptr;
}
//179
PerfectStrike* PerfectStrike::create_card(int upgrade) {
	auto ptr = PerfectStrike::create();
	((Cards*)ptr)->create_card(179, 2, 2, Ctype::ATTACK, Cvalue::COMMON, Ccolour::RED,
		upgrade, Cards::english_card_name[179]);
	return ptr;
}
void PerfectStrike::set_description() {
	std::string tem;
	if (!this->get_upgrade())
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), damage);
	else
		tem = StringUtils::format(chinese_card_updescription[this->get_id()].c_str(),damage);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void PerfectStrike::use() {//如果消耗了就不算在这里面
	int dam = 6;
	int add = this->get_upgrade() ? upaddi : addi;
	for (auto card : *hero->get_v_draw_card()) {
		int id = card->get_id();
		if (id == 179 || id == 183 || id == 235 || id == 240 || id == 264)
			dam += add;
	}
	for (auto card : *hero->get_v_tem_card()) {
		int id = card->get_id();
		if (id == 179 || id == 183 || id == 235 || id == 240 || id == 264)
			dam += add;
	}
	for (auto card : *hero->get_v_discard()) {
		int id = card->get_id();
		if (id == 179 || id == 183 || id == 235 || id == 240 || id == 264)
			dam += add;
	}
	dam = hero->handle_attack_buff(dam);
	this->get_target()->be_beaten(dam,hero);
	
}
//183
PommelStrike* PommelStrike::create_card(int upgrade) {
	auto ptr = PommelStrike::create();
	((Cards*)ptr)->create_card(183, 1, 1, Ctype::ATTACK, Cvalue::COMMON, Ccolour::RED,
		upgrade, Cards::english_card_name[183]);
	return ptr;
}
void PommelStrike::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? updamage : damage, this->get_upgrade() ? updraw:draw);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void PommelStrike::use() {
	int dam = hero->handle_attack_buff((this->get_upgrade() ? updamage : damage));
	this->get_target()->be_beaten(dam,hero);
	if(!v_enemy.empty())
		dynamic_cast<FightScene*>(hero->getParent())->Draw_card(this->get_upgrade() ? updraw:draw);
}
//184
PowerThrough* PowerThrough::create_card(int upgrade) {
	auto ptr = PowerThrough::create();
	((Cards*)ptr)->create_card(184, 1, 1, Ctype::SKILL, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[184]);
	return ptr;
}
void PowerThrough::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? updefend:defend);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void PowerThrough::use() {
	for (int i = 0; i < 2; ++i) 
		hero->get_v_tem_card()->pushBack(Wound::create_card());
	dynamic_cast<FightScene*>(hero->getParent())->ShowTemCards();
	int def = hero->handle_defend_buff(this->get_upgrade() ? updefend : defend);
	hero->add_state("next_turn_block", def);
}
//187
Pummel* Pummel::create_card(int upgrade) {
	auto ptr = Pummel::create();
	((Cards*)ptr)->create_card(187, 1, 1, Ctype::ATTACK, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[187]);
	return ptr;
}
void Pummel::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
	2, this->get_upgrade() ? uptime : time);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Pummel::use() {
	int t = (this->get_upgrade() ? uptime : time);
	auto enemy = this->get_target();
	int dam = hero->handle_attack_buff(damage);
	for (int i = 0; i < t; ++i) {
		enemy->be_beaten(dam,hero);
		if (enemy->get_health() < 1)
			break;
	}
	this->consume();
}
//188
Purity* Purity::create_card(int upgrade) {
	auto ptr = Purity::create();
	((Cards*)ptr)->create_card(188, 0, 0, Ctype::SKILL, Cvalue::VALUE, Ccolour::GREY,
		upgrade, Cards::english_card_name[188]);
	return ptr;
}
void Purity::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? 2 : 1);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Purity::use() {
	this->retain();
	hero->get_v_tem_card()->eraseObject(this);
	for(auto c: *hero->get_v_tem_card())
		this->consume();
	for (int i = 0; i < this->get_upgrade() ? 2 : 1; ++i)
		if(hero->get_v_consume_card()->size())
			hero->choose_card(hero->get_v_consume_card(), hero->get_v_tem_card());
	hero->get_v_tem_card()->pushBack(this);
	this->release();
	this->consume();
}
//190
Rage* Rage::create_card(int upgrade) {
	auto ptr = Rage::create();
	((Cards*)ptr)->create_card(190, 0, 0, Ctype::SKILL, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[190]);
	return ptr;
}
void Rage::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		 this->get_upgrade() ? updefend : defend);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Rage::use() {
	hero->add_state("rage", this->get_upgrade() ? updefend : defend);
}

//192
Rampage* Rampage::create_card(int upgrade) {
	auto ptr = Rampage::create();
	((Cards*)ptr)->create_card(192, 1, 1, Ctype::ATTACK, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[192]);
	return ptr;
}
void Rampage::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		damage, this->get_upgrade() ? upaddi : addi);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Rampage::use() {
	int dam = damage + (this->get_upgrade() ? upaddi : addi) * usetime;
	dam = hero->handle_attack_buff(dam);
	this->get_target()->be_beaten(dam,hero);
	usetime ++;
}
//193
Reaper* Reaper::create_card(int upgrade) {
	auto ptr = Reaper::create();
	((Cards*)ptr)->create_card(193, 2, 2, Ctype::ATTACK, Cvalue::RARE, Ccolour::RED,
		upgrade, Cards::english_card_name[193]);
	return ptr;
}
void Reaper::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? updamage : damage);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Reaper::use() {
	int dam = hero->handle_attack_buff((this->get_upgrade() ? updamage : damage));
	for (auto e : v_enemy) {
		int prev_health = e->get_health();
		e->be_beaten(dam,hero);
		int now_health = std::max(0,e->get_health());
		hero->add_health(prev_health - now_health);
	}
	this->consume();
}
//196
RecklessCharge* RecklessCharge::create_card(int upgrade) {
	auto ptr = RecklessCharge::create();
	((Cards*)ptr)->create_card(196, 0, 0, Ctype::ATTACK, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[196]);
	return ptr;
}
void RecklessCharge::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? updamage : damage);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void RecklessCharge::use() {
	int max = hero->get_v_draw_card()->size();
	hero->get_v_draw_card()->insert(random(0, max), Dazed::create_card());//把眩晕牌随机放到抽牌堆里
	int dam = hero->handle_attack_buff((this->get_upgrade() ? updamage : damage));
	this->get_target()->be_beaten(dam,hero);
}
//200
Regret* Regret::create_card(int upgrade) {
	auto ptr = Regret::create();
	((Cards*)ptr)->create_card(200, 0, 0, Ctype::CURSE, Cvalue::COMMON, Ccolour::BLACK,
		upgrade, Cards::english_card_name[200]);
	return ptr;
}
//205
Rupture* Rupture::create_card(int upgrade) {
	auto ptr = Rupture::create();
	((Cards*)ptr)->create_card(205, 1, 1, Ctype::POWER, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[205]);
	return ptr;
}
void Rupture::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? upstrength : strength);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Rupture::use() {
	hero->add_power("rupture", this->get_upgrade() ? upstrength : strength);
	this->consume_power_card();
}
//206
SadisticNature* SadisticNature::create_card(int upgrade) {
	auto ptr = SadisticNature::create();
	((Cards*)ptr)->create_card(206, 0, 0, Ctype::POWER, Cvalue::RARE, Ccolour::GREY,
		upgrade, Cards::english_card_name[206]);
	return ptr;
}
void SadisticNature::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? updamage : damage);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void SadisticNature::use() {
	hero->add_power("sadisticnature", this->get_upgrade() ? updamage : damage);
	this->consume_power_card();
}
//208
SearingBlow* SearingBlow::create_card(int upgrade) {
	auto ptr = SearingBlow::create();
	((Cards*)ptr)->create_card(208, 2, 2, Ctype::ATTACK, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[208]);
	return ptr;
}
void SearingBlow::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade()? updamage:damage);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void SearingBlow::use() {
	int dam = hero->handle_attack_buff((this->get_upgrade() ? updamage : damage));
	this->get_target()->be_beaten(dam,hero);
}
//209
SecondWind* SecondWind::create_card(int upgrade) {
	auto ptr = SecondWind::create();
	((Cards*)ptr)->create_card(209, 1, 1, Ctype::SKILL, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[209]);
	return ptr;
}
void SecondWind::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? updefend : defend);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void SecondWind::use() {
	int def = 0;
	auto v_card = hero->get_v_tem_card();
	for (int i = 0; i < v_card->size();) {
		auto card = v_card->at(i);
		if (card->get_type() != ATTACK && card != this) {
			card->consume();
			def += this->get_upgrade() ? updefend : defend;
		}
		else
			i++;
	}
	if (def) {
		def = hero->handle_defend_buff(def);
		hero->gain_defend(def);
	}
}
//210
SecretTechnique* SecretTechnique::create_card(int upgrade) {
	auto ptr = SecretTechnique::create();
	((Cards*)ptr)->create_card(210, 0, 0, Ctype::SKILL, Cvalue::RARE, Ccolour::GREY,
		upgrade, Cards::english_card_name[210]);
	return ptr;
}
void SecretTechnique::set_description() {
	std::string tem = StringUtils::format(this->get_upgrade() ? chinese_card_updescription[this->get_id()].c_str() :
		chinese_card_description[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void SecretTechnique::use() {
	auto cards = hero->get_v_choose_card();
	//默认抽红卡，毕竟我不打算写别的卡了
	for (int i = 0; i < 3; ++i)
		cards->pushBack(dynamic_cast<FightScene*>(hero->getParent())->Choose_random_card(INITIAL, SKILL, RED));
	hero->choose_tem_card(1);
	if (!this->get_upgrade())
		this->consume();
}
//211
SecretWeapon* SecretWeapon::create_card(int upgrade) {
	auto ptr = SecretWeapon::create();
	((Cards*)ptr)->create_card(211, 0, 0, Ctype::SKILL, Cvalue::RARE, Ccolour::GREY,
		upgrade, Cards::english_card_name[211]);
	return ptr;
}
void SecretWeapon::set_description() {
	std::string tem = StringUtils::format(this->get_upgrade() ? chinese_card_updescription[this->get_id()].c_str() :
		chinese_card_description[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void SecretWeapon::use() {
	auto cards = hero->get_v_choose_card();
	for (int i = 0; i < 3; ++i)
		cards->pushBack(dynamic_cast<FightScene*>(hero->getParent())->Choose_random_card(INITIAL, ATTACK, RED));
	hero->choose_tem_card(1);
	if (!this->get_upgrade())
		this->consume();
}
//212
SeeingRed* SeeingRed::create_card(int upgrade) {
	auto ptr = SeeingRed::create();
	((Cards*)ptr)->create_card(212, 1, 0, Ctype::SKILL, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[212]);
	return ptr;
}
void SeeingRed::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),2);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void SeeingRed::use() {
	hero->add_energe_now(2);
	this->consume();
}
//215
Sentinel* Sentinel::create_card(int upgrade) {
	auto ptr = Sentinel::create();
	((Cards*)ptr)->create_card(215, 1, 1, Ctype::SKILL, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[215]);
	return ptr;
}
void Sentinel::set_description() {
	std::string tem;
	if (!this->get_upgrade())
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), defend);
	else
		tem = StringUtils::format(chinese_card_updescription[this->get_id()].c_str(),updefend);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Sentinel::use() {
	int def = hero->handle_defend_buff(this->get_upgrade() ? updefend : defend);
	hero->gain_defend(def);
}
void Sentinel::consume() {
	hero->add_energe_now(this->get_upgrade() ? 3 : 2); 
	((Cards*)this)->consume(); 
}
//217
SeverSoul* SeverSoul::create_card(int upgrade) {
	auto ptr = SeverSoul::create();
	((Cards*)ptr)->create_card(217, 2, 2, Ctype::ATTACK, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[217]);
	return ptr;
}
void SeverSoul::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), this->get_upgrade() ? damage : updamage);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void SeverSoul::use() {
	//不知道为什么不能用迭代器，所以只能用下标来写了
	auto v_card = hero->get_v_tem_card();
	for (int i = 0; i < v_card->size();) {
		if (v_card->at(i)->get_type() != ATTACK)
			v_card->at(i)->consume();
		else
			i++;
	}
	int dam = hero->handle_attack_buff((this->get_upgrade() ? updamage : damage));
	this->get_target()->be_beaten(dam,hero);

}
//218
Shiv* Shiv::create_card(int upgrade) {
	auto ptr = Shiv::create();
	((Cards*)ptr)->create_card(218, 0, 0, Ctype::ATTACK, Cvalue::COMMON, Ccolour::GREY,
		upgrade, Cards::english_card_name[218]);
	return ptr;
}
void Shiv::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), this->get_upgrade() ? damage : updamage);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Shiv::use() {
	int dam = hero->handle_attack_buff((this->get_upgrade() ? updamage : damage));
	this->get_target()->be_beaten(dam,hero);
	this->consume();
}
//219
ShockWave* ShockWave::create_card(int upgrade) {
	auto ptr = ShockWave::create();
	((Cards*)ptr)->create_card(219, 2, 2, Ctype::SKILL, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[219]);
	return ptr;
}
void ShockWave::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? upweak : weak);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void ShockWave::use() {
	int buff = this->get_upgrade() ? upweak : weak;
	for (auto e : v_enemy) {
		e->add_state("weak", buff);
		e->add_state("vulnerable", buff);
	}
	this->consume();
}
//220
ShrugItOff* ShrugItOff::create_card(int upgrade) {
	auto ptr = ShrugItOff::create();
	((Cards*)ptr)->create_card(220, 1, 1, Ctype::SKILL, Cvalue::COMMON, Ccolour::RED,
		upgrade, Cards::english_card_name[220]);
	return ptr;
}
void ShrugItOff::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? updefend : defend);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void ShrugItOff::use() {
	int def = hero->handle_defend_buff(this->get_upgrade() ? updefend : defend);
	hero->gain_defend(def);
	hero->draw_card(1);
}
//224
Slimed* Slimed::create_card(int upgrade) {
	auto ptr = Slimed::create();
	((Cards*)ptr)->create_card(224, 1, 0, Ctype::STATUS, Cvalue::COMMON, Ccolour::GREY,
		upgrade, Cards::english_card_name[224]);
	return ptr;
}
void Slimed::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Slimed::use() {
	this->consume();
}
//225
SpotWeakness* SpotWeakness::create_card(int upgrade) {
	auto ptr = SpotWeakness::create();
	((Cards*)ptr)->create_card(225, 1, 1, Ctype::SKILL, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[225]);
	return ptr;
}
void SpotWeakness::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? upstrength:strength);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void SpotWeakness::use() {
	auto intent = dynamic_cast<MonsterBase*>(this->get_target())->get_intent();
	if (intent < 4)
		hero->add_power("strength", this->get_upgrade() ? upstrength : strength);
}
//235
Strike_R* Strike_R::create_card(int upgrade) {
	auto ptr = Strike_R::create();
	((Cards*)ptr)->create_card(235, 1, 1, Ctype::ATTACK, Cvalue::INITIAL, Ccolour::RED,
		upgrade,Cards::english_card_name[235]);
	
	return ptr;
}
void Strike_R::set_description() {
	//计算一些状态影响，这个之后也会做成函数。
	int dam = (this->get_upgrade() ? updamage : damage);
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), dam);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0,"description");
}
void Strike_R::use() {
	int dam = hero->handle_attack_buff((this->get_upgrade() ? updamage : damage));
	this->get_target()->be_beaten(dam,hero);
}
//240
SwiftStrike* SwiftStrike::create_card(int upgrade) {
	auto ptr = SwiftStrike::create();
	((Cards*)ptr)->create_card(240, 0, 0, Ctype::ATTACK, Cvalue::VALUE, Ccolour::GREY,
		upgrade, Cards::english_card_name[240]);
	return ptr;
}
void SwiftStrike::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? updamage : damage);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void SwiftStrike::use() {
	int dam = hero->handle_attack_buff((this->get_upgrade() ? updamage : damage));
	this->get_target()->be_beaten(dam,hero);
}

//241
SwordBoomerang* SwordBoomerang::create_card(int upgrade) {
	auto ptr = SwordBoomerang::create();
	((Cards*)ptr)->create_card(241, 1, 1, Ctype::ATTACK, Cvalue::COMMON, Ccolour::RED,
		upgrade, Cards::english_card_name[241]);
	return ptr;
}
void SwordBoomerang::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		damage, this->get_upgrade() ? uptime : time);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void SwordBoomerang::use() {
	int dam = hero->handle_attack_buff(damage);
	for (int i = 0; i < this->get_upgrade() ? uptime : time; ++i) {
		int enemy = random(0, int(v_enemy.size() - 1));
		v_enemy.at(enemy)->be_beaten(dam,hero);
	}
}
//245
ThinkingAhead* ThinkingAhead::create_card(int upgrade) {
	auto ptr = ThinkingAhead::create();
	((Cards*)ptr)->create_card(245, 0, 0, Ctype::SKILL, Cvalue::RARE, Ccolour::GREY,
		upgrade, Cards::english_card_name[245]);
	return ptr;
}
void ThinkingAhead::set_description() {
	std::string tem = StringUtils::format(this->get_upgrade() ? chinese_card_updescription[this->get_id()].c_str() :
		chinese_card_description[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void ThinkingAhead::use() {
	hero->draw_card(2);
	auto v_card = hero->get_v_tem_card();
	this->retain();
	v_card->eraseObject(this);
	for (auto card : *v_card)
		card->removeFromParent();
	hero->choose_card(v_card, hero->get_v_draw_card());
	v_card->pushBack(this);
	this->release();
	if(!this->get_upgrade())
		this->consume();
}
//247
Thunderclap* Thunderclap::create_card(int upgrade) {
	auto ptr = Thunderclap::create();
	((Cards*)ptr)->create_card(247, 1, 1, Ctype::ATTACK, Cvalue::COMMON, Ccolour::RED,
		upgrade, Cards::english_card_name[247]);
	return ptr;
}
void Thunderclap::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? updamage : damage);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Thunderclap::use() {
	int dam = hero->handle_attack_buff((this->get_upgrade() ? updamage : damage));
	for (auto e : v_enemy) {
		e->be_beaten(dam,hero);
		if (e->get_health() > 0)
			e->add_state("vulnerable", 1);
	}
}
//249
Transmutation* Transmutation::create_card(int upgrade) {
	auto ptr = Transmutation::create();
	((Cards*)ptr)->create_card(249, 0, 0, Ctype::SKILL, Cvalue::RARE, Ccolour::GREY,
		upgrade, Cards::english_card_name[249]);
	return ptr;
}
void Transmutation::set_description() {
	std::string tem = StringUtils::format(this->get_upgrade() ? chinese_card_updescription[this->get_id()].c_str() :
		chinese_card_description[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Transmutation::use() {
	int time = hero->get_energy_now();
	hero->add_energe_now(-time);
	auto scene = dynamic_cast<FightScene*>(hero->getParent());
	Cards* new_card;
	for (int i = 0; i < time; ++i) {
		new_card = scene->Choose_random_card(INITIAL, CURSE, GREY)->clone(this->get_upgrade() ? 1 : 0);
		hero->get_v_tem_card()->pushBack(new_card);
	}
	this->consume();
	scene->ShowTemCards();
}
//250
Trip* Trip::create_card(int upgrade) {
	auto ptr = Trip::create();
	((Cards*)ptr)->create_card(250, 0, 0, Ctype::SKILL, Cvalue::VALUE, Ccolour::GREY,
		upgrade, Cards::english_card_name[250]);
	return ptr;
}
void Trip::set_description() {
	std::string tem = StringUtils::format(this->get_upgrade() ? chinese_card_updescription[this->get_id()].c_str() :
		chinese_card_description[this->get_id()].c_str(),weak);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Trip::use() {
	if (this->get_upgrade())
		for (auto e : v_enemy)
			e->add_state("weak", weak);
	else
		this->get_target()->add_state("weak", weak);
}
//251
TrueGrit* TrueGrit::create_card(int upgrade) {
	auto ptr = TrueGrit::create();
	((Cards*)ptr)->create_card(251, 1, 1, Ctype::SKILL, Cvalue::COMMON, Ccolour::RED,
		upgrade, Cards::english_card_name[251]);
	return ptr;
}
void TrueGrit::set_description() {
	std::string tem;
	if (!this->get_upgrade())
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),defend);
	else
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),updefend);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void TrueGrit::use() {
	int def = hero->handle_defend_buff(this->get_upgrade() ? updefend : defend);
	hero->gain_defend(def);
	auto v_card = hero->get_v_tem_card();
	if (this->get_upgrade()) {
		this->retain();
		v_card->eraseObject(this);
		if (v_card->size() > 0);
		hero->handle_consume_buff();
		for (auto card : *v_card)
			card->removeFromParent();
		hero->choose_card(v_card, hero->get_v_consume_card());
		v_card->pushBack(this);
		this->release();
	}
	else if (v_card->size() > 1) {
		v_card->eraseObject(this);
		v_card->at(random(0, int(v_card->size() - 1)))->consume();
		v_card->pushBack(this);
	}
}
//257
Uppercut* Uppercut::create_card(int upgrade) {
	auto ptr = Uppercut::create();
	((Cards*)ptr)->create_card(257, 2, 2, Ctype::ATTACK, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[257]);
	return ptr;
}
void Uppercut::set_description() {
	std::string tem;
	if (!this->get_upgrade())
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), damage,weak, vulnerable);
	else
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), damage,upweak, upvulnerable);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Uppercut::use() {
	auto e = this->get_target();
	int dam = hero->handle_attack_buff(damage);
	e->be_beaten(dam,hero);
	if (e->get_health() > 0) {
		e->add_state("weak", this->get_upgrade() ? upweak : weak);
		e->add_state("vulnerable", this->get_upgrade() ? upvulnerable : vulnerable);
	}
}
//259
Void* Void::create_card(int upgrade) {
	auto ptr = Void::create();
	((Cards*)ptr)->create_card(259, 0, 0, Ctype::STATUS, Cvalue::COMMON, Ccolour::GREY,
		upgrade, Cards::english_card_name[259]);
	return ptr;
}
void Void::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
//260
Warcry* Warcry::create_card(int upgrade) {
	auto ptr = Warcry::create();
	((Cards*)ptr)->create_card(260, 0, 0, Ctype::SKILL, Cvalue::COMMON, Ccolour::RED,
		upgrade, Cards::english_card_name[260]);
	return ptr;
}
void Warcry::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? 2 : 1);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Warcry::use() {
	hero->draw_card(this->get_upgrade() ? 2 : 1);
	auto v_card = hero->get_v_tem_card();
	this->retain();
	v_card->eraseObject(this);
	for (auto card : *v_card)
		card->removeFromParent();
	hero->choose_card(v_card, hero->get_v_draw_card());
	v_card->pushBack(this);
	this->release();
	this->consume();
}
//262
Whirlwind* Whirlwind::create_card(int upgrade) {
	auto ptr = Whirlwind::create();
	((Cards*)ptr)->create_card(262, 0, 0, Ctype::ATTACK, Cvalue::VALUE, Ccolour::RED,
		upgrade, Cards::english_card_name[262]);
	return ptr;
}
void Whirlwind::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? updamage : damage);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void Whirlwind::use() {
	int time = hero->get_energy_now();
	hero->add_energe_now(-time);
	int dam = hero->handle_attack_buff((this->get_upgrade() ? updamage : damage));
	for (int i = 0; i < time; ++i) {
		for (auto e : v_enemy)
			e->be_beaten(dam,hero);
	}
}
//264
WildStrike* WildStrike::create_card(int upgrade) {
	auto ptr = WildStrike::create();
	((Cards*)ptr)->create_card(264, 1, 1, Ctype::ATTACK, Cvalue::COMMON, Ccolour::RED,
		upgrade, Cards::english_card_name[264]);
	return ptr;
}
void WildStrike::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(),
		this->get_upgrade() ? updamage : damage);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void WildStrike::use() {
	int max = hero->get_v_draw_card()->size();
	hero->get_v_draw_card()->insert(random(0, max), Wound::create_card());
	int dam = hero->handle_attack_buff((this->get_upgrade() ? updamage : damage));
	this->get_target()->be_beaten(dam,hero);
}
//265
Wound* Wound::create_card(int upgrade) {
	auto ptr = Wound::create();
	((Cards*)ptr)->create_card(265, 0, 0, Ctype::STATUS, Cvalue::COMMON, Ccolour::GREY,
		upgrade, Cards::english_card_name[265]);
	return ptr;
}
void Wound::set_description() {
	std::string tem;
	if (!this->get_upgrade())
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str());
	else if (chinese_card_updescription[this->get_id()].size() < 5)
		tem = StringUtils::format(chinese_card_description[this->get_id()].c_str());
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
//267
Writhe* Writhe::create_card(int upgrade) {
	auto ptr = Writhe::create();
	((Cards*)ptr)->create_card(267, 0, 0, Ctype::CURSE, Cvalue::COMMON, Ccolour::BLACK,
		upgrade, Cards::english_card_name[267]);
	return ptr;
}
//271
RitualDagger* RitualDagger::create_card(int upgrade) {
	auto ptr = RitualDagger::create();
	((Cards*)ptr)->create_card(271, 1, 1, Ctype::ATTACK, Cvalue::VALUE, Ccolour::GREY,
		upgrade, Cards::english_card_name[271]);
	return ptr;
}
void RitualDagger::set_description() {
	std::string tem = StringUtils::format(chinese_card_description[this->get_id()].c_str(), damage,
		this->get_upgrade() ? 5 : 3);
	auto label3 = Label::createWithTTF(tem, "./fonts/Song.ttf", 15);
	label3->setDimensions(120, 80);
	label3->setPosition(130, 70);
	this->getChildByName("img")->addChild(label3, 0, "description");
}
void RitualDagger::use() {
	int dam = hero->handle_attack_buff(damage);
	this->get_target()->be_beaten(dam,hero);
	if (this->get_target()->get_health() < 1)
		damage += this->get_upgrade() ? 5 : 3;
	this->consume();
}

//272
Pride* Pride::create_card(int upgrade) {
	auto ptr = Pride::create();
	((Cards*)ptr)->create_card(272, 1, 1, Ctype::CURSE, Cvalue::COMMON, Ccolour::BLACK,
		upgrade, Cards::english_card_name[272]);
	return ptr;
}
void Pride::use() {
	this->consume();
}