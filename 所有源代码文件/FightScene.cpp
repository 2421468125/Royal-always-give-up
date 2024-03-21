#include "FightScene.h"
#include "GameMap.h"
#include "GlobalValue.h"
USING_NS_CC;
extern HeroBase* hero;
extern Vector<MonsterBase*> v_enemy;
Scene* FightScene::createScene() {
	Scene* scene = Scene::create();
	FightScene* layer = FightScene::create();
	scene->addChild(layer);
	return scene;
}

bool FightScene::init(){
	
	if (!Scene::init()) {
		return false;
	}
	hero->fight_init();
	Size winSize = Director::getInstance()->getWinSize();
	//ս��ʱ�ı���ͼƬ
	auto bg = Sprite::create("./imgs/bg/FightScene.jpg");
	bg->setPosition(winSize.width / 2, winSize.height / 2);
	this->addChild(bg, -1);
	//����ɫ
	hero->set_pos(350, 600);
	this->addChild(hero);
	hero->init_round();
	hero->add_round();
	//��������ս���õ�������ͼƬҲ����ȥ
	auto energy_img = Sprite::create(hero->energy_path);
	energy_img->setPosition(150, 250);
	this->addChild(energy_img, 0, "energy_img");
	

	//��������������������������,����ʾ������
	auto draw_card_icon = MenuItemImage::create("./imgs/cards/drawcard.png","./imgs/cards/drawcard.png",
		CC_CALLBACK_1(FightScene::ShowDrawCards,this,hero->get_v_draw_card()));
	draw_card_icon->setPosition(50, 100);
	auto discard_icon = MenuItemImage::create("./imgs/cards/discard.png", "./imgs/cards/discard.png",
		CC_CALLBACK_1(FightScene::ShowDiscards, this, hero->get_v_discard()));
	discard_icon->setPosition(1550, 100);
	auto consume_card_icon = MenuItemImage::create("./imgs/cards/consumecard.png", "./imgs/cards/consumecard.png",
		CC_CALLBACK_1(FightScene::ShowConsumeCards, this, hero->get_v_consume_card()));
	consume_card_icon->setPosition(1550, 200);
	//��ſ��������Ľڵ�
	auto number = Node::create();
	number->setPosition(0, 0);
	this->addChild(number, 1, "number");

	//���������
	auto use_cards = Node::create();//������г鵽�Ŀ��ƵĽڵ�
	use_cards->setPosition(0, 0);
	this->addChild(use_cards, 1, "use_cards");
	hero->turn();

	//������
	auto enemy = Node::create(); //  ����������˵Ľڵ�
	this->addChild(enemy, 0, "enemy");
	Choose_random_enemy();
	
	//�غϽ�����
	auto end_turn = MenuItemImage::create("./imgs/bg/turnend.png",
		"./imgs/bg/turnend.png", CC_CALLBACK_1(FightScene::TurnEnd, this));
	end_turn->setPosition(1500, 300);
	auto menu = Menu::create(end_turn,draw_card_icon,discard_icon,consume_card_icon,NULL);
	menu->setPosition(0, 0);
	this->addChild(menu, 1);
}

void FightScene::Draw_card(int n) {
	Vector<Cards*>* tem_draw = hero->get_v_draw_card();
	Vector<Cards*>* tem_use = hero->get_v_tem_card();
	for (int i = 0; i < n; ++i) {
		auto card = tem_draw->back();
		
		//��push��pop����Ϊ���Ƴ�������ʱ���ƺ����ױ�cocos�ĳ��������ʱ��Ҫ������
		tem_use->pushBack(card);
		if (card->get_type() == Cards::STATUS) {
			if(hero->search_power("evolve"))
				n += hero->search_power("evolve");
			if (hero->search_power("firebreathing")) {
				int dam = hero->search_power("firebreathing");
				for (auto e : v_enemy)
					e->be_beaten(dam,hero);
				if (v_enemy.size() == 0)
					break;
			}
			if (card->get_id() == 259)//�ж��ǲ������
				hero->add_energe_now(-1);
			else if (card->get_id() == 170)
				hero->add_state("beatofdeath", 1);
		}
			
		tem_draw->popBack();
		if (tem_draw->size() == 0)
			hero->Shuffle();
		if (tem_draw->size() == 0)
			break;
	}
	
	this->ShowTemCards();
}

void FightScene::ShowMonster() {
	auto enemy = this->getChildByName("enemy");
	enemy->removeAllChildren();
	int len = v_enemy.size();
	int count = 0;
	for (auto e : v_enemy) {
		e->set_pos(1036 - ((int(len * 0.5 - 0.5) - count) * 972 / len),600);
		enemy->addChild(e);
		count++;
	}

}

void FightScene::ShowTemCards() {
	Vector<Cards*>* tem_use = hero->get_v_tem_card();
	auto node = this->getChildByName("use_cards");
	int len = tem_use->size();
	for (int i = 0; i < len; ++i) {
		auto card = tem_use->at(i);
		card->removeFromParent();
		float coe = (i - (len - 1) / 2.0f);
		//���Ǿ�����ʾ�����Ҹ���200
		card->setAnchorPoint(Vec2(0.5, 0));
		int x = int(864 + coe * ((1128 / len > 150) ? 150 : 1328 / len));
		int y = int(160 - coe * coe * 4);
		card->set_pos(x, y);//����ת�Ļ������ƶ�һ��,�Ʋ����ʱ�򿿽�һЩ
		card->setRotation(coe * 6);//����һ����ת�����⵲̫��
		node->addChild(card, 1);
	}
}

void FightScene::TurnEnd(Ref* Psender) {
	hero->turn_end();
	//Ȼ���ǵ��˵Ļغ�
	for (int i = v_enemy.size() - 1; i > -1; --i) {
		v_enemy.at(i)->add_round();
		v_enemy.at(i)->turn();
	}
	if (v_enemy.size() == 0)
		Fight_end();

	//�ٵ���ɫ�غ�
	hero->add_round();
	hero->turn();
}
void FightScene::update_card_num() {
	//�ȸ�������
	auto node = this->getChildByName("number");
	node->removeAllChildren();
	auto label1 = Label::createWithTTF(StringUtils::format("%d", hero->get_v_draw_card()->size()),
		"./fonts/Myfont.ttf", 24);
	label1->setTextColor(Color4B::WHITE);
	label1->setPosition(100, 100);
	node->addChild(label1);
	auto label2 = Label::createWithTTF(StringUtils::format("%d", hero->get_v_discard()->size()),
		"./fonts/Myfont.ttf", 24);
	label2->setTextColor(Color4B::WHITE);
	label2->setPosition(1600, 100);
	node->addChild(label2);
	auto label3 = Label::createWithTTF(StringUtils::format("%d", hero->get_v_consume_card()->size()),
		"./fonts/Myfont.ttf", 24);
	label3->setTextColor(Color4B::WHITE);
	label3->setPosition(1550, 200);
	node->addChild(label3);

	//�ٸ��½�ɫ����
	auto energy_img = this->getChildByName("energy_img");
	energy_img->removeAllChildren();
	auto energy_num = Label::createWithTTF(StringUtils::format("%d/%d", hero->get_energy_now(),
		hero->get_energy()), "./fonts/Myfont.ttf", 48);
	energy_num->setTextColor(Color4B::WHITE);
	energy_num->enableOutline(Color4B::BLACK, 5);
	energy_num->setPosition(80, 75);
	energy_img->addChild(energy_num, 1, "energy_num");
	
	//�ٸ���Ѫ����buff
	for (auto e : v_enemy) {
		e->draw_buff();
		e->draw_health();
	}
	hero->draw_buff();
	hero->draw_health();

}

void FightScene::Fight_end() {
	//�������״̬
	hero->clear_buff();
	//��������������ŵ�ս����ʼ�ȽϺã���Ϊ�����������Կ��ܻ������������use���ܣ�
	for (auto c : hero->v_total_card)//��ԭ����ԭʼ����
		c->set_temcost(c->get_upgrade() ? c->get_cost() : c->get_upcost());
	//�������棬�ص���ͼ
	Reward();
}


//��GameMap����ʾ�������һ��,��Ϊ���иĶ�������ʵ�ڲ�֪���ص�������ģ��Ӧ�ó�ɶ���������ظ�д��4��
void FightScene::ShowDrawCards(Ref* pSender,Vector<Cards*>* cards) {
	auto scene = Scene::create();
	Size winSize = Director::getInstance()->getWinSize();
	Layer* c = Layer::create();
	//����һ�����ƶѸ��������������ʾ
	int height = 300 + (cards->size() + 4) / 5 * 384;
	int count = 0;
	Vector<Cards*> tem = *cards;
	std::shuffle(tem.begin(), tem.end(), std::default_random_engine(time(0)));//ʵ�����ϴ��
	for (auto x : tem) {
		x->setAnchorPoint(Vec2(0, 0));
		x->setPosition(228 + count % 5 * 300, height - 384 - count / 5 * 384);
		c->addChild(x, 1);
		count++;
	}
	//���ؼ��ͱ����������们��
	auto back = MenuItemImage::create("./imgs/bg/charSelect/back.png",
		"./imgs/bg/charSelect/back.png", [](Ref* ref) {Director::getInstance()->popScene(); });
	auto menu = Menu::create(back, NULL);
	menu->setPosition(0, 0);
	back->setAnchorPoint(Vec2(0, 0));
	back->setPosition(0, 20);
	back->setScale(2.0f);
	scene->addChild(menu, 2);

	auto bg = Sprite::create("./imgs/bg/drawcardbg.jpg");
	bg->setPosition(winSize.width / 2, winSize.height / 2);
	scene->addChild(bg, 0);
	ScrollView* view = ScrollView::create(winSize, c);
	view->setPosition(0, 0);
	scene->addChild(view,1);

	view->setDirection(ScrollView::Direction::VERTICAL);
	//������ͼ�Ŀ�Ⱥ͸߶�
	view->setContentSize(Size(winSize.width, height));
	Director::getInstance()->pushScene(scene);
}

void FightScene::ShowDiscards(Ref* pSender, Vector<Cards*>* cards) {
	auto scene = Scene::create();
	Size winSize = Director::getInstance()->getWinSize();
	Layer* c = Layer::create();
	int height = 300 + (cards->size() + 4) / 5 * 384;
	int count = 0;
	for (auto x : *cards) {
		x->setAnchorPoint(Vec2(0, 0));
		x->setPosition(228 + count % 5 * 300, height - 384 - count / 5 * 384);
		c->addChild(x, 1);
		count++;
	}
	//���ؼ��ͱ����������们��
	auto back = MenuItemImage::create("./imgs/bg/charSelect/back.png",
		"./imgs/bg/charSelect/back.png", [](Ref* ref) {Director::getInstance()->popScene(); });
	auto menu = Menu::create(back, NULL);
	menu->setPosition(0, 0);
	back->setAnchorPoint(Vec2(0, 0));
	back->setPosition(0, 20);
	back->setScale(2.0f);
	scene->addChild(menu, 2);

	auto bg = Sprite::create("./imgs/bg/discardbg.jpg");
	bg->setPosition(winSize.width / 2, winSize.height / 2);
	scene->addChild(bg, 0);
	ScrollView* view = ScrollView::create(winSize, c);
	view->setPosition(0, 0);
	scene->addChild(view, 1);

	view->setDirection(ScrollView::Direction::VERTICAL);
	//������ͼ�Ŀ�Ⱥ͸߶�
	view->setContentSize(Size(winSize.width, height));
	Director::getInstance()->pushScene(scene);
}

void FightScene::ShowConsumeCards(Ref* pSender, Vector<Cards*>* cards) {
	auto scene = Scene::create();
	Size winSize = Director::getInstance()->getWinSize();
	Layer* c = Layer::create();
	int height = 300 + (cards->size() + 4) / 5 * 384;
	int count = 0;
	for (auto x : *cards) {
		x->setAnchorPoint(Vec2(0, 0));
		x->setPosition(228 + count % 5 * 300, height - 384 - count / 5 * 384);
		c->addChild(x, 1);
		count++;
	}
	//���ؼ��ͱ����������们��
	auto back = MenuItemImage::create("./imgs/bg/charSelect/back.png",
		"./imgs/bg/charSelect/back.png", [](Ref* ref) {Director::getInstance()->popScene(); });
	auto menu = Menu::create(back, NULL);
	menu->setPosition(0, 0);
	back->setAnchorPoint(Vec2(0, 0));
	back->setPosition(0, 20);
	back->setScale(2.0f);
	scene->addChild(menu, 2);

	auto bg = Sprite::create("./imgs/bg/consumecardbg.jpg");
	bg->setPosition(winSize.width / 2, winSize.height / 2);
	scene->addChild(bg, 0);
	ScrollView* view = ScrollView::create(winSize, c);
	view->setPosition(0, 0);
	scene->addChild(view, 1);

	view->setDirection(ScrollView::Direction::VERTICAL);
	//������ͼ�Ŀ�Ⱥ͸߶�
	view->setContentSize(Size(winSize.width, height));
	Director::getInstance()->pushScene(scene);
}

void FightScene::Choose_random_enemy() {
	if (GameMap::reward_type == 3) {
		v_enemy.pushBack(Hexaghust::create());
		ShowMonster();
		return;
	}//Bossս������
	extern GameMap* map;
	extern std::unordered_map<std::string, MonsterBase* (*)()> enemy_map;
	extern std::vector<std::pair<const char*, int> > v_difficulty;
	int difficulty_tot = 45 + 3 * map->layer;//�Ѷ�ϵ����layer����
	if (GameMap::reward_type == 2) difficulty_tot += 25;
	int difficulty_now = 0;//Ŀǰ���Ѷ�ϵ��
	int max_num = v_difficulty.size() - 1;
	while (difficulty_tot - difficulty_now > 10) {
		int randnum = random(0, max_num);
		int excess = v_difficulty[randnum].second + difficulty_now - difficulty_tot;
		int reselect_poss = excess * 10;
		while (random(1, 100) < reselect_poss) {
			randnum = random(0, randnum);
			excess = v_difficulty[randnum].second + difficulty_now - difficulty_tot;
			reselect_poss = excess * 10;
			if (randnum == 0) {
				randnum = (random(1, 100) < reselect_poss ? -1 : 0);
				break;
			}
		}
		if(randnum > -1)
			v_enemy.pushBack(enemy_map[v_difficulty[randnum].first]());
		difficulty_now += v_difficulty[randnum].second;
	}
	ShowMonster();
}

void FightScene::Reward() {
	extern GameMap* map;
	auto scene = Scene::create();
	auto bg = Sprite::create("./imgs/reward/rewardScreenSheet.png");
	bg->setPosition(864, 600);
	scene->addChild(bg);
	//��ÿ��Ƶİ�ť
	bool has_chosen = false;
	auto getcard = MenuItemImage::create("./imgs/reward/getcard.png",
		"./imgs/reward/getcard.png", CC_CALLBACK_1(FightScene::Card_Reward, this, has_chosen));
	getcard->setPosition(864, 700);
	//�����İ�ť
	auto skip = MenuItemImage::create("./imgs/reward/skip.png", "./imgs/reward/skip.png",
		[](Ref* ref) {Director::getInstance()->popToRootScene(); map->draw_panel_data(); });
	skip->setPosition(1000, 400);
	auto menu = Menu::create(getcard, skip, NULL);
	menu->setPosition(0, 0);
	scene->addChild(menu, 0,"menu");
	Director::getInstance()->pushScene(scene);
}

void FightScene::Card_Reward(Ref* pSender, bool &has_chosen) {
	if (has_chosen)
		return;
	extern GameMap* map;
	int upgrade_poss = map->layer;//��ÿ���Ϊ�����ĸ��ʼ�Ϊ�㼶
	int rare_poss = std::max(1,std::min(int(map->layer * map->layer * 0.04), 25));
	int common_poss = std::max(20, 75 - map->layer * 3);
	int value_poss = 100 - rare_poss - common_poss;
	//��ʱֻ���쿨
	auto reward_card = hero->get_v_choose_card();
	for(int i=0;i<3;++i){//չʾ���ſ������������ٸ�
		int rand_num = random(0, 99);
		Cards::Cvalue value;
		if (rand_num < rare_poss)  value = Cards::Cvalue::RARE;
		else if (rand_num < rare_poss + value_poss) value = Cards::Cvalue::VALUE;
		else value = Cards::Cvalue::COMMON;
		reward_card->pushBack(this->Choose_random_card(value, Cards::Ctype::CURSE, Cards::Ccolour::RED)->clone());
	}
	hero->choose_tem_card(3);
	has_chosen = true;
}

Cards* FightScene::Choose_random_card(Cards::Cvalue value, Cards::Ctype type, Cards::Ccolour colour) {
	using namespace CardVector;
	Cards* rand_card;
	switch (value) {

	case Cards::Cvalue::INITIAL: {
		switch (type) {
		case Cards::Ctype::CURSE:
			if (colour == Cards::Ccolour::RED) rand_card = red_card.at(random(0, int(red_card.size() - 1)));
			else if (colour == Cards::Ccolour::GREY) rand_card = grey_card.at(random(0, int(grey_card.size() - 1)));
			break;
		case Cards::Ctype::ATTACK:
			if (colour == Cards::Ccolour::RED) rand_card = red_attack.at(random(0, int(red_attack.size() - 1)));
			else if (colour == Cards::Ccolour::GREY) rand_card = grey_attack.at(random(0, int(grey_attack.size() - 1)));
			break;
		case Cards::Ctype::SKILL:
			if (colour == Cards::Ccolour::RED) rand_card = red_skill.at(random(0, int(red_skill.size() - 1)));
			else if (colour == Cards::Ccolour::GREY) rand_card = grey_skill.at(random(0, int(grey_skill.size() - 1)));
			break;
		case Cards::Ctype::POWER:
			if (colour == Cards::Ccolour::RED) rand_card = red_power.at(random(0, int(red_power.size() - 1)));
			else if (colour == Cards::Ccolour::GREY) rand_card = grey_power.at(random(0, int(grey_power.size() - 1)));
			break;
		default:break;
		}
	}
							   break;
	case Cards::Cvalue::COMMON: {
		switch (type) {
		case Cards::Ctype::CURSE:
			if (colour == Cards::Ccolour::RED) {
				int max_num = red_attack_common.size() + red_skill_common.size();
				int rand_num = random(0, int(max_num - 1));
				if (rand_num < red_attack_common.size()) rand_card = red_attack_common.at(rand_num);
				else rand_card = red_skill_common.at(rand_num - red_attack_common.size());
			}
			else if (colour == Cards::Ccolour::GREY) rand_card = grey_card.at(random(0, int(grey_card.size() - 1)));
			break;
		case Cards::Ctype::ATTACK:
			if (colour == Cards::Ccolour::RED)
				rand_card = red_attack_common.at(random(0, int(red_attack_common.size() - 1)));
			else if (colour == Cards::Ccolour::GREY) rand_card = grey_attack.at(random(0, int(grey_attack.size() - 1)));
			break;
		case Cards::Ctype::SKILL:
			if (colour == Cards::Ccolour::RED)
				rand_card = red_skill_common.at(random(0, int(red_skill_common.size() - 1)));
			else if (colour == Cards::Ccolour::GREY) rand_card = grey_skill.at(random(0, int(grey_skill.size() - 1)));
			break;
			//power��û��common
		default:break;
		}
	}
							  break;
	case Cards::Cvalue::VALUE: {
		switch (type) {
		case Cards::Ctype::CURSE:
			if (colour == Cards::Ccolour::RED) {
				int max_num = red_attack_value.size() + red_skill_value.size() + red_power_value.size();
				int rand_num = random(0, int(max_num - 1));
				if (rand_num < red_attack_value.size()) rand_card = red_skill_value.at(rand_num);
				else if (rand_num < red_attack_value.size() + red_skill_value.size())
					rand_card = red_skill_value.at(rand_num - red_attack_value.size());
				else rand_card = red_power_value.at(rand_num - red_attack_value.size() - red_skill_value.size());
			}
			else if (colour == Cards::Ccolour::GREY) rand_card = grey_card.at(random(0, int(grey_card.size() - 1)));
			break;
		case Cards::Ctype::ATTACK:
			if (colour == Cards::Ccolour::RED)
				rand_card = red_attack_value.at(random(0, int(red_attack_value.size() - 1)));
			else if (colour == Cards::Ccolour::GREY) rand_card = grey_attack.at(random(0, int(grey_attack.size() - 1)));
			break;
		case Cards::Ctype::SKILL:
			if (colour == Cards::Ccolour::RED)
				rand_card = red_skill_value.at(random(0, int(red_skill_value.size() - 1)));
			else if (colour == Cards::Ccolour::GREY) rand_card = grey_skill.at(random(0, 12));
			break;
		case Cards::Ctype::POWER:
			if (colour == Cards::Ccolour::RED)
				rand_card = red_power_value.at(random(0, int(red_power_value.size() - 1)));
			else if (colour == Cards::Ccolour::GREY) rand_card = grey_skill.at(random(0, int(grey_skill.size() - 1)));
			break;
		default:break;
		}
	}
							 break;
	case Cards::Cvalue::RARE: {
		switch (type) {
		case Cards::Ctype::CURSE:
			if (colour == Cards::Ccolour::RED) {
				int max_num = red_attack_rare.size() + red_skill_rare.size() + red_power_rare.size();
				int rand_num = random(0, int(max_num - 1));
				if (rand_num < red_attack_rare.size()) rand_card = red_skill_rare.at(rand_num);
				else if (rand_num < red_attack_rare.size() + red_skill_rare.size())
					rand_card = red_skill_rare.at(rand_num - red_attack_rare.size());
				else rand_card = red_power_rare.at(rand_num - red_attack_rare.size() - red_skill_rare.size());
			}
			else if (colour == Cards::Ccolour::GREY) rand_card = grey_card.at(random(0, int(grey_card.size() - 1)));
			break;
		case Cards::Ctype::ATTACK:
			if (colour == Cards::Ccolour::RED)
				rand_card = red_attack_rare.at(random(0, int(red_attack_rare.size() - 1)));
			else if (colour == Cards::Ccolour::GREY) rand_card = grey_attack.at(random(0, int(grey_attack.size() - 1)));
			break;
		case Cards::Ctype::SKILL:
			if (colour == Cards::Ccolour::RED)
				rand_card = red_skill_rare.at(random(0, int(red_skill_rare.size() - 1)));
			else if (colour == Cards::Ccolour::GREY) rand_card = grey_skill.at(random(13, int(grey_skill.size() - 1)));
			break;
		case Cards::Ctype::POWER:
			if (colour == Cards::Ccolour::RED)
				rand_card = red_power_rare.at(random(0, int(red_power_rare.size() - 1)));
			else if (colour == Cards::Ccolour::GREY) rand_card = grey_skill.at(random(0, int(grey_skill.size() - 1)));
			break;
		default:break;
		}
	}
							break;
	}
	return rand_card;
}
