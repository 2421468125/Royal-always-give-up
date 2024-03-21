#include "GameMap.h"
#include "GlobalValue.h"
#include <algorithm>
USING_NS_CC;
USING_NS_CC_EXT;

int GameMap::layer = -1;
int GameMap::order = -1;
int GameMap::reward_type = 1;
Scene* GameMap::createScene() {
	Scene* scene = Scene::create();
	GameMap* layer = GameMap::create();
	scene->addChild(layer);
	return scene;
}

bool GameMap::init() {
    if (!Scene::init()) 
    {
        return false;
    }
    GlobalValue::set_map(this);

    Size winSize = Director::getInstance()->getWinSize();


    //创建一个滚动层,该层的子节点会随之滚动
    Layer* c = Layer::create();
    auto map = Sprite::create();
    auto top_map = Sprite::create("./imgs/map/mapTop.png");
    auto mid_map = Sprite::create("./imgs/map/mapMid.png");
    auto bot_map = Sprite::create("./imgs/map/mapBot.png");
    top_map->setPosition(winSize.width / 2, 5 * winSize.height / 2);
    mid_map->setPosition(winSize.width / 2, 3 * winSize.height / 2);
    bot_map->setPosition(winSize.width / 2, winSize.height / 2);
    map->addChild(top_map);
    map->addChild(mid_map);
    map->addChild(bot_map);
    //为GamePoint的节点创建菜单类存储
    auto icon_menu = Menu::create();
    icon_menu->setPosition(0, 0);
    draw_map(map,icon_menu);
    map->addChild(icon_menu,1);

    auto boss = MenuItemImage::create("./imgs/map/boss/boss.png","./imgs/map/boss/boss.png", CC_CALLBACK_1(GameMap::AttackBoss, this));
    boss->setPosition(0, 2100);
    auto boss_menu = Menu::create(boss, NULL);
    map->addChild(boss_menu, 2);
    c->addChild(map);
    //画图例和上方状态栏，不随map移动
    auto legend = Sprite::create("./imgs/map/legend.png");
    legend->setPosition(1550, 600);
    
    this->addChild(legend,1);
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

    //地图图标。
    auto map_icon = MenuItemImage::create("./imgs/map/topPanel/map.png", 
        "./imgs/map/topPanel/map.png",CC_CALLBACK_1(GameMap::To_map,this));
    map_icon->setPosition(1400, 80);
    //卡牌图标
    auto card_icon = MenuItemImage::create("./imgs/map/topPanel/deck.png",
        "./imgs/map/topPanel/deck.png", CC_CALLBACK_1(GameMap::Show_Cards, this));
    card_icon->setPosition(1500, 80);
    //设置图标
    auto setting = MenuItemImage::create("./imgs/map/topPanel/settings.png",
        "./imgs/map/topPanel/settings.png", CC_CALLBACK_1(GameMap::Setting_Panel, this));
    setting->setPosition(1650, 80);
    auto panel_menu = Menu::create(map_icon, card_icon, setting, NULL);
    panel_menu->setPosition(0, 0);
    panel->addChild(panel_menu, 1);
    auto panel_data = Node::create();
    panel_data->setPosition(0, 0);
    //用来存放血量等数据的节点，统一一个父节点便于更新
    panel->addChild(panel_data, 1, "panel_data");
    this->addChild(panel, 2, "panel");//这个显示的优先级比其他都高
    draw_panel_data();
    //滚动视图
    //第一个参数：显示的视图大小

    ScrollView* view = ScrollView::create(winSize, c);
    view->setPosition(0,0);
    this->addChild(view);

    //设置视图运动的方向为水平运动
    view->setDirection(ScrollView::Direction::VERTICAL);
  
    //设置视图的宽度和高度
    view->setContentSize(Size(winSize.width, winSize.height*3));


    return true;
}


void GameMap::draw_panel_data() {
    extern HeroBase* hero;//获取全局变量
    auto health = Label::createWithTTF(StringUtils::format("%d / %d",hero->get_health(),
        hero->get_max_health()),"./fonts/Song.ttf", 30);
    health->setPosition(170, 80);
    auto money = Label::createWithTTF(StringUtils::format("%d",hero->get_money()), "./fonts/Song.ttf", 30);
    money->setPosition(320, 80);
    auto card_num = Label::createWithTTF(StringUtils::format("%d", hero->get_card_num()), "./fonts/Song.ttf", 30);
    card_num->setPosition(1570, 80);
    auto tem = this->getChildByName("panel")->getChildByName("panel_data");
    tem->removeAllChildren();
    tem->addChild(health, 1);
    tem->addChild(money, 1);
    tem->addChild(card_num, 1);
}

void GameMap::Show_Cards(cocos2d::Ref* pSender) {
    extern HeroBase* hero;
    auto scene = Scene::create();
    Size winSize = Director::getInstance()->getWinSize();

    //为显示卡牌而新建一个滑动层
    Layer* c = Layer::create();
    int height = 300 + ((hero->v_total_card.size()) + 4) / 5 * 384; //计算画面高度
    int count = 0;
    for (auto x : hero->v_total_card) {
        x->setAnchorPoint(Vec2(0,0));
        x->setPosition(228 + count % 5 * 300, height - 384 - count / 5 * 384);
        x->setScale(1.5);
        c->addChild(x,1);
        count++;
     }
    //返回键，不随其滑动
    auto back = MenuItemImage::create("./imgs/bg/charSelect/back.png",
        "./imgs/bg/charSelect/back.png", [](Ref* ref) {Director::getInstance()->popScene(); });
    auto menu = Menu::create(back, NULL);
    menu->setPosition(0, 0);
    back->setAnchorPoint(Vec2(0, 0));
    back->setPosition(0, 20);
    back->setScale(2.0f);
    scene->addChild(menu, 1);

    auto bg = Sprite::create("./imgs/bg/showcardbg.jpg");
    bg->setPosition(winSize.width / 2, winSize.height / 2);
    scene->addChild(bg, 0);

    ScrollView* view = ScrollView::create(winSize, c);
    view->setPosition(0, 0);
    scene->addChild(view);

    view->setDirection(ScrollView::Direction::VERTICAL);
    //设置视图的宽度和高度
    view->setContentSize(Size(winSize.width,height));
    Director::getInstance()->pushScene(scene);
}

void GameMap::Setting_Panel(cocos2d::Ref* pSender) {
    Director::getInstance()->end();
}

void GameMap::AttackBoss(Ref* pSender) {
    if (layer != 14) return;
    reward_type = 3;
    auto new_scene = FightScene::createScene();
    auto sign = Node::create();
    sign->setName("FightScene");
    new_scene->addChild(sign);
    auto change = TransitionFadeBL::create(0.8f, new_scene);
    Director::getInstance()->pushScene(change);
}

void GamePoint::ClickIcon(cocos2d::Ref* pSender) {
    if(GameMap::layer+1 != layer || 
        (GameMap::order != -1 && std::find(prev.begin(),prev.end(),GameMap::order) == prev.end()))
        return;
    GameMap::layer++;
    GameMap::order = order;
    //已经走过的点画个x
    auto passed = Sprite::create("./imgs/map/mapCompleteIcon.png");
    passed->setAnchorPoint(Vec2(0, 0));
    passed->setPosition(pos_x,pos_y);
    this->addChild(passed, 1);
    if (this->point_type == MONSTER || this->point_type == ELITE) {
        GameMap::reward_type = point_type == MONSTER ? 1 : 2;
        auto new_scene = FightScene::createScene();
        auto sign = Node::create();//专门为战斗场景设置一个标记，在Cards中会用到
        sign->setName("FightScene");
        new_scene->addChild(sign);
        auto change = TransitionFadeBL::create(0.8f, new_scene);
        Director::getInstance()->pushScene(change);
    }
    else if (this->point_type == SHOP) {
        auto new_scene = Shop::createScene();
        auto change = TransitionProgressRadialCCW::create(0.8f, new_scene);
        auto sign = Node::create();
        sign->setName("Shop");
        new_scene->addChild(sign);
        Director::getInstance()->pushScene(change);
    }
    else if (this->point_type == REST) {
        auto new_scene = RestScene::createScene();
        auto change = TransitionTurnOffTiles::create(0.8f, new_scene);
        Director::getInstance()->pushScene(change);
    }
}
void GamePoint::add(GamePoint* p) {
    if (p->layer < layer) {
        p->next.push_back(order);
        prev.push_back(p->order);
    }
    else {
        p->prev.push_back(order);
        next.push_back(p->order);
    }
}

//因为画的点是MenuItem类，所以只能传Menu* 成为父节点
void GamePoint::draw_point(Menu* node) {
    std::string img_path = "./imgs/map";
    switch (point_type) {
        case MONSTER:
            img_path += "/monster.png";
            break;
        case EVENT:
            img_path += "/event.png";
            break;
        case REST:
            img_path += "/rest.png";
            break;
        case ELITE:
            img_path += "/elite.png";
            break;
        case SHOP:
            img_path += "/shop.png";
            break;
        case CHEST:
            img_path += "/chest.png";
            break;
    }
    auto icon = MenuItemImage::create(img_path,img_path, CC_CALLBACK_1(GamePoint::ClickIcon,this));
    icon->setAnchorPoint(Vec2(0, 0));
    icon->setPosition(pos_x, pos_y);
    icon->setTag(layer * 10 + order);//设置每个图标的Tag，方便找节点。
    node->addChild(icon,1);
}



//重点，完成map的随机生成
void GameMap::draw_map(Sprite* map,Menu* icon_menu) {
    
    const int Max_Point = 7;
    const int Min_Point = 2;
    const int Point_Layer = 15;
    const int Chest_Layer = rand() % 4 + 5;//宝箱层在5-9层之间
    const float monster_poss = 0.55;
    const float elite_poss = 0.1;
    const float rest_poss = 0.15;
    const float shop_poss = 0.05;
    const float event_poss = 0.15;

    //对每层点的个数和位置随机初始化
    for (int layer = 0; layer < Point_Layer; ++layer) {
        int point_num = rand() % 6 + 2;//每层点的个数
        list.push_back(std::vector<GamePoint*>());

        for (int point = 0; point < point_num; ++point) {
            //随机x坐标，左右摆动幅度为80像素，左右都平均隔400像素空白不画,中间对齐
            int x = 824 + 80 * CCRANDOM_0_1() + 928 / point_num * (point - (point_num-1) / 2.0f);
            //随机y坐标，上下摆动幅度20像素，上方隔500不画，下方隔200不画
            int y = 190 + 20 * CCRANDOM_0_1() + 2216 / Point_Layer * layer;
            //随机设置点的类别
            GamePoint::type t;
            if (layer == 14)
                t = GamePoint::REST;
            else if (layer == Chest_Layer)
                t = GamePoint::CHEST;
            else if (layer == 0)
                t = GamePoint::MONSTER;
            else{
                float rand_num = CCRANDOM_0_1();
                if (layer < 3) {
                    while (rand_num > monster_poss && rand_num < monster_poss + elite_poss + rest_poss)
                        rand_num = CCRANDOM_0_1();
                }
                if (rand_num < monster_poss) t = GamePoint::MONSTER;
                else if (rand_num < monster_poss + elite_poss) t = GamePoint::ELITE;
                else if (rand_num < monster_poss + elite_poss + rest_poss) t = GamePoint::REST;
                else if (rand_num < monster_poss + elite_poss + rest_poss + shop_poss) t = GamePoint::SHOP;
                else t = GamePoint::EVENT;
            }

            auto gpoint = GamePoint::create();
            gpoint->init_point(layer, point, t, x, y);
            gpoint->draw_point(icon_menu);//需要马上draw，否则就直接释放掉了
            map->addChild(gpoint);//draw了只保存MenuItem，但GamePoint还是被释放了，所以要这样保存才行
            list[layer].push_back(gpoint);
        }
    }
    //接下来是给这些点连线
    for (int layer = 0; layer < Point_Layer - 1; ++layer) {
        std::vector<GamePoint*>& more = 
            (list[layer].size() > list[layer + 1].size() ? list[layer] : list[layer + 1]);
        std::vector<GamePoint*>& less =
            (list[layer].size() > list[layer + 1].size() ? list[layer + 1] : list[layer]);
        int less_index = 0;
        int linked = 1;//less节点已相连的数目
        float aver = float(more.size()) / less.size(); //平均一个节点的连线数
        more[0]->add(less[0]);//第一个节点特判，必须相连。
        
        for (int index = 1; index < more.size(); ++index) {
            //如果more剩余节点比less的要少（其实是一样多），那么必须相连
            if (more.size() - index < less.size() - less_index) {
                less_index++;
                more[index]->add(less[less_index]);
                linked = 1;
            }
            else {
                //这里的随机数是（平均相连数-已相连数）*因子0.5+随机因子（0-1）
                float rand_num = CCRANDOM_0_1() + (aver - linked) * 0.5;
                if (rand_num > 0.7 || less_index == less.size() - 1) {
                    more[index]->add(less[less_index]);
                    linked++;
                }
                //如果不行就和less下一个节点相连
                else {
                    less_index++;
                    more[index]->add(less[less_index]);
                    linked = 1;
                }
            }
        }        
    }

    // 下面来专门给一些地方画交叉线
    for (int layer = 0; layer < Point_Layer-1; ++layer)
        for (int point = 0; point < list[layer].size() - 1; ++point) 
            //如果是两个只有一条路走的点，它们有概率加一条斜线
            if (list[layer][point]->next.size() == 1 && list[layer][point + 1]->next.back()
                - list[layer][point]->next.back() == 1) {
                auto tem = list[layer][point];
                if (CCRANDOM_0_1() > 0.85)
                    tem->add(list[layer + 1][tem->next[0] + 1]);
                else if (CCRANDOM_0_1() > 0.85)
                    list[layer][point + 1]->add(list[layer + 1][tem->next[0]]);
            }

    

    auto draw = DrawNode::create(5);
    for(int layer = 0;layer < Point_Layer-1;++layer)
        for (int point = 0; point < list[layer].size(); ++point) {
            auto orgin = list[layer][point]->get_pos();
            for (auto p : list[layer][point]->next)
                //画线，因为锚点在左下角，所以有个偏移
                draw->drawLine(orgin+Vec2(48,72), list[layer + 1][p]->get_pos()+Vec2(48,24), Color4F::WHITE);
        }
    //画最后一行到boss的线
    for (int point = 0; point < list[Point_Layer-1].size(); ++point)
        draw->drawLine(list[Point_Layer - 1][point]->get_pos() + Vec2(48, 72),Vec2(864,2420), Color4F::WHITE);
    
        map->addChild(draw,0);

}
