#include "Shop.h"
#include "GameMap.h"

extern GameMap* map;
extern HeroBase* hero;
int Shop::remove_time = 0;
Scene* Shop::createScene() {
	Scene* scene = Scene::create();
	Shop* layer = Shop::create();
	scene->addChild(layer);
	return scene;
}



bool Shop::init() {
	if (!Scene::init()) {
		return false;
	}
	Size winSize = Director::getInstance()->getWinSize();
    auto tem_listener = EventListenerTouchOneByOne::create();//监听器
    is_remove = false;
	//背景图片与跳过键,卡牌移除功能
	auto bg = Sprite::create("./imgs/npcs/carpet.png");
	bg->setPosition(winSize.width / 2, winSize.height / 2);
	this->addChild(bg, -1);

	auto skip = MenuItemImage::create("./imgs/reward/skip.png", "./imgs/reward/skip.png",
        CC_CALLBACK_1(Shop::QuitShop,this, tem_listener));
    skip->setPosition(1100, 64);

    auto card_remove = MenuItemImage::create("imgs/npcs/remove_service.png", "imgs/npcs/remove_service.png",
        CC_CALLBACK_1(Shop::RemoveService, this));
    card_remove->setPosition(1428, 300);
    
    auto price = Label::createWithTTF(StringUtils::format("price: %d", 75+25*remove_time), "./fonts/Song.ttf", 30);
    price->setPosition(1428-30, 300-220);
    this->addChild(price);

    auto menu = Menu::create(skip,card_remove,NULL);
    menu->setPosition(0, 0);
    this->addChild(menu, 0, "menu");

    //基本直接照抄GameMap
    auto panel = Sprite::create("./imgs/map/topPanel/bar.png");
    panel->setPosition(864, 915);

    //血量图标
    auto heart = Sprite::create("./imgs/map/topPanel/panelHeart.png");
    heart->setPosition(100, 80);
    panel->addChild(heart, 1);
    //金钱图标
    auto money = Sprite::create("./imgs/map/topPanel/panelGoldBag.png");
    money->setPosition(250, 80);
    panel->addChild(money, 1);

    //卡牌图标
    auto card_icon = MenuItemImage::create("./imgs/map/topPanel/deck.png",
        "./imgs/map/topPanel/deck.png", CC_CALLBACK_1(GameMap::Show_Cards, map));
    card_icon->setPosition(1500, 80);
    //设置图标
    auto setting = MenuItemImage::create("./imgs/map/topPanel/settings.png",
        "./imgs/map/topPanel/settings.png", CC_CALLBACK_1(GameMap::Setting_Panel, map));
    setting->setPosition(1650, 80);
    auto panel_menu = Menu::create(card_icon, setting, NULL);
    panel_menu->setPosition(0, 0);
    panel->addChild(panel_menu, 1);
    auto panel_data = Node::create();
    panel_data->setPosition(0, 0);
    //用来存放血量等数据的节点，统一一个父节点便于更新
    panel->addChild(panel_data, 1, "panel_data");
    this->addChild(panel, 1, "panel");//这个显示的优先级比其他都高
    draw_panel_data();
    //画出随机选择的卡片
    shop_card.pushBack(FightScene::Choose_random_card(Cards::Cvalue::INITIAL, Cards::Ctype::ATTACK, Cards::Ccolour::RED)->clone(random(1,10)> 9));
    shop_card.pushBack(FightScene::Choose_random_card(Cards::Cvalue::INITIAL, Cards::Ctype::ATTACK, Cards::Ccolour::RED)->clone(random(1, 10) > 9));
    shop_card.pushBack(FightScene::Choose_random_card(Cards::Cvalue::INITIAL, Cards::Ctype::SKILL, Cards::Ccolour::RED)->clone(random(1, 10) > 9));
    shop_card.pushBack(FightScene::Choose_random_card(Cards::Cvalue::INITIAL, Cards::Ctype::SKILL, Cards::Ccolour::RED)->clone(random(1, 10) > 9));
    shop_card.pushBack(FightScene::Choose_random_card(Cards::Cvalue::INITIAL, Cards::Ctype::POWER, Cards::Ccolour::RED)->clone(random(1, 10) > 9));
    shop_card.pushBack(FightScene::Choose_random_card(Cards::Cvalue::INITIAL, Cards::Ctype::CURSE, Cards::Ccolour::GREY)->clone(random(1, 10) > 9));
    shop_card.pushBack(FightScene::Choose_random_card(Cards::Cvalue::INITIAL, Cards::Ctype::CURSE, Cards::Ccolour::GREY)->clone(random(1, 10) > 9));
    int discourse = random(0, 6);
    for (int i = 0; i < 7; ++i) {
        shop_card.at(i)->set_pos(228 + i % 5 * 300, 700 - 400 * (i / 5));
        this->addChild(shop_card.at(i));
        int cost;//设置价格，其中一个打5折
        if (i < 5) {
            if (shop_card.at(i)->get_value() == Cards::Cvalue::COMMON) cost = random(45, 55);
            else if (shop_card.at(i)->get_value() == Cards::Cvalue::VALUE) cost = random(67, 83);
            else cost = random(135, 165);
        }
        else {
            if (shop_card.at(i)->get_value() == Cards::Cvalue::COMMON) cost = random(55, 65);
            else if (shop_card.at(i)->get_value() == Cards::Cvalue::VALUE) cost = random(83, 98);
            else cost = random(165, 195);
        }
        if (i == discourse) cost /= 2;
        auto price = Label::createWithTTF(StringUtils::format("price: %d",cost), "./fonts/Song.ttf", 30);
        price->setPosition(228 + i % 5 * 300 -30, 700 - 400 * (i / 5) -220);
        this->addChild(price);
        v_price.push_back(cost);

        if (i == discourse) {
            price->setTextColor(Color4B::BLUE);
            auto icon = Sprite::create("./imgs/npcs/discourse.png");
            icon->setPosition(228 + i % 5 * 300+208, 700 - 400 * (i / 5)+176);

        }
    }

    
    tem_listener->onTouchBegan = [this,tem_listener](Touch* touch, Event* event)->bool
    {
        if (!Director::getInstance()->getRunningScene()->getChildByName("Shop")) return false;
        Vec2 pos = touch->getLocation();
        float ratio = 1.5;//卡牌的大小放缩系数,懒得再调用函数了，反正都是1.5
        for (int i = 0; i < v_price.size(); ++i) {
            auto card = shop_card.at(i);
            Vec2 card_pos = card->get_pos();//设置锚点卵用没有，这里它还是按照中心点为原点计算
            Rect range = Rect::Rect(card_pos.x - 180, card_pos.y - 190, 180 * ratio, 256 * ratio);
            if (range.containsPoint(pos) && hero->get_money() >= v_price[i]) {
                hero->add_card(card);
                card->removeFromParent();
                shop_card.erase(i);
                hero->gain_money(-v_price[i]);
                v_price.erase(v_price.begin() + i);
                draw_panel_data();
                break;
            }
        }
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(tem_listener, this);

}

void Shop::draw_panel_data() {
    
    auto health = Label::createWithTTF(StringUtils::format("%d / %d", hero->get_health(),
        hero->get_max_health()), "./fonts/Song.ttf", 30);
    health->setPosition(170, 80);
    auto money = Label::createWithTTF(StringUtils::format("%d", hero->get_money()), "./fonts/Song.ttf", 30);
    money->setPosition(320, 80);
    auto card_num = Label::createWithTTF(StringUtils::format("%d", hero->get_card_num()), "./fonts/Song.ttf", 30);
    card_num->setPosition(1570, 80);
    auto tem = this->getChildByName("panel")->getChildByName("panel_data");
    tem->removeAllChildren();
    tem->addChild(health, 1);
    tem->addChild(money, 1);
    tem->addChild(card_num, 1);
}

void Shop::QuitShop(Ref* pSender, EventListenerTouchOneByOne* listener) {
    _eventDispatcher->removeEventListener(listener);
    Director::getInstance()->popToRootScene(); 
    map->draw_panel_data();
}

void Shop::RemoveService(Ref* pSender) {
    if (hero->get_money() < 75 + 25 * remove_time || is_remove) return;
    auto scene = Scene::create();
    Size winSize = Director::getInstance()->getWinSize();
    Layer* c = Layer::create();
    scene->addChild(c, 1);
    int count = 0;
    for (auto x : hero->v_total_card) {
        x->set_pos(120 + count % 7 * 248, 820 - count / 7 * 384);//大约能存21张牌,应该够了
        c->addChild(x, 1);
        count++;
    }
    auto bg = Sprite::create("./imgs/bg/choosecardbg.jpg");
    bg->setPosition(winSize.width / 2, winSize.height / 2);
    scene->addChild(bg, 0);
    auto tem_listener = EventListenerTouchOneByOne::create();
    tem_listener->onTouchBegan = [this, tem_listener](Touch* touch, Event* event)->bool
    {
        bool flag = false;
        Vec2 pos = touch->getLocation();
        float ratio = 1.5;
        for (auto card : hero->v_total_card) {
            Vec2 card_pos = card->get_pos();
            Rect range = Rect::Rect(card_pos.x - 180, card_pos.y - 190, int(180 * ratio), int(256 * ratio));
            if (range.containsPoint(pos)) {
                if (card->handle_remove_buff()) {//死灵诅咒无法逃脱
                    hero->v_total_card.eraseObject(card);
                    hero->add_card_num(-1);
                }
                flag = true;
                break;
            }
        }
        if (flag) {
            hero->gain_money(-75 - 25 * remove_time);
            remove_time++;
            is_remove = true;
            auto picture = Sprite::create("./imgs/npcs/sold_out.png");
            picture->setPosition(1428, 300);
            this->addChild(picture, 1);
            this->draw_panel_data();
        }
        _eventDispatcher->removeEventListener(tem_listener);
        Director::getInstance()->popScene();
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(tem_listener, c);
    Director::getInstance()->pushScene(scene);
}
