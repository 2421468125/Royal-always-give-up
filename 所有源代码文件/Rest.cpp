#include "Rest.h"
#include "GameMap.h"

extern GameMap* map;
extern HeroBase* hero;
Scene* RestScene::createScene() {
	Scene* scene = Scene::create();
	RestScene* layer = RestScene::create();
	scene->addChild(layer);
	return scene;
}

bool RestScene::init() {
	if (!Scene::init()) {
		return false;
	}
    has_done = false;
	auto bg = Sprite::create("./imgs/bg/rest.jpg");
    bg->setPosition(1728 / 2, 972 / 2);
	this->addChild(bg,-1);

	auto sleep = MenuItemImage::create("./imgs/bg/sleep.png", "./imgs/bg/sleep.png",
		CC_CALLBACK_1(RestScene::Sleep,this));
	sleep->setPosition(700, 650);
	auto smith = MenuItemImage::create("./imgs/bg/smith.png", "./imgs/bg/smith.png",
		CC_CALLBACK_1(RestScene::Smith, this));
	auto menu = Menu::create(sleep, smith, NULL);
    menu->setPosition(0,0);
	smith->setPosition(1050, 650);
	this->addChild(menu, 1);

    //仍然是照抄GameMap,但这里不用重复调用draw_panel_data了
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

    auto health = Label::createWithTTF(StringUtils::format("%d / %d", hero->get_health(),
        hero->get_max_health()), "./fonts/Song.ttf", 30);
    health->setPosition(170, 80);
    auto money_num = Label::createWithTTF(StringUtils::format("%d", hero->get_money()), "./fonts/Song.ttf", 30);
    money_num->setPosition(320, 80);
    auto card_num = Label::createWithTTF(StringUtils::format("%d", hero->get_card_num()), "./fonts/Song.ttf", 30);
    card_num->setPosition(1570, 80);
    panel_data->addChild(health, 1);
    panel_data->addChild(money_num, 1);
    panel_data->addChild(card_num, 1);
}

void RestScene::Sleep(Ref* pSender) {
    int heal = hero->get_max_health() * 0.3;
    hero->add_health(heal);
    map->draw_panel_data();
    Director::getInstance()->popScene();
}
void RestScene::Smith(Ref* pSender) {
    auto scene = Scene::create();
    Size winSize = Director::getInstance()->getWinSize();
    Layer* c = Layer::create();
    scene->addChild(c, 1);
    int count = 0;
    for (auto x : hero->v_total_card) {
        if (x->get_upgrade()) continue;
        x->set_pos(120 + count % 7 * 248, 820 - count / 7 * 384);//大约能存21张牌,应该够了
        c->addChild(x, 1);
        count++;
    }
    auto bg = Sprite::create("./imgs/bg/choosecardbg.jpg");
    bg->setPosition(winSize.width / 2, winSize.height / 2);
    scene->addChild(bg, 0);
    auto tem_listener = EventListenerTouchOneByOne::create();
    tem_listener->onTouchBegan = [this,tem_listener](Touch* touch, Event* event)->bool
    {
        bool flag = false;
        Vec2 pos = touch->getLocation();
        float ratio = 1.5;
        for (auto card : hero->v_total_card) {
            if (card->get_upgrade()) continue;
            Vec2 card_pos = card->get_pos();
            Rect range = Rect::Rect(card_pos.x - 180, card_pos.y - 190, int(180 * ratio), int(256 * ratio));
            if (range.containsPoint(pos)){
                hero->v_total_card.pushBack(card->clone(1));
                hero->v_total_card.eraseObject(card);
                flag = true;
                break;
            }
        }
        _eventDispatcher->removeEventListener(tem_listener);
        if (flag) Director::getInstance()->popToRootScene();
        else Director::getInstance()->popScene();
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(tem_listener, c);
    Director::getInstance()->pushScene(scene);
}
