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


    //����һ��������,�ò���ӽڵ����֮����
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
    //ΪGamePoint�Ľڵ㴴���˵���洢
    auto icon_menu = Menu::create();
    icon_menu->setPosition(0, 0);
    draw_map(map,icon_menu);
    map->addChild(icon_menu,1);

    auto boss = MenuItemImage::create("./imgs/map/boss/boss.png","./imgs/map/boss/boss.png", CC_CALLBACK_1(GameMap::AttackBoss, this));
    boss->setPosition(0, 2100);
    auto boss_menu = Menu::create(boss, NULL);
    map->addChild(boss_menu, 2);
    c->addChild(map);
    //��ͼ�����Ϸ�״̬��������map�ƶ�
    auto legend = Sprite::create("./imgs/map/legend.png");
    legend->setPosition(1550, 600);
    
    this->addChild(legend,1);
    auto panel = Sprite::create("./imgs/map/topPanel/bar.png");
    panel->setPosition(864, 915);
    
    //Ѫ��ͼ��
    auto heart = Sprite::create("./imgs/map/topPanel/panelHeart.png");
    heart->setPosition(100, 80);
    panel->addChild(heart, 1);
    //��Ǯͼ��
    auto money = Sprite::create("./imgs/map/topPanel/panelGoldBag.png");
    money->setPosition(250, 80);
    panel->addChild(money, 1);

    //��ͼͼ�ꡣ
    auto map_icon = MenuItemImage::create("./imgs/map/topPanel/map.png", 
        "./imgs/map/topPanel/map.png",CC_CALLBACK_1(GameMap::To_map,this));
    map_icon->setPosition(1400, 80);
    //����ͼ��
    auto card_icon = MenuItemImage::create("./imgs/map/topPanel/deck.png",
        "./imgs/map/topPanel/deck.png", CC_CALLBACK_1(GameMap::Show_Cards, this));
    card_icon->setPosition(1500, 80);
    //����ͼ��
    auto setting = MenuItemImage::create("./imgs/map/topPanel/settings.png",
        "./imgs/map/topPanel/settings.png", CC_CALLBACK_1(GameMap::Setting_Panel, this));
    setting->setPosition(1650, 80);
    auto panel_menu = Menu::create(map_icon, card_icon, setting, NULL);
    panel_menu->setPosition(0, 0);
    panel->addChild(panel_menu, 1);
    auto panel_data = Node::create();
    panel_data->setPosition(0, 0);
    //�������Ѫ�������ݵĽڵ㣬ͳһһ�����ڵ���ڸ���
    panel->addChild(panel_data, 1, "panel_data");
    this->addChild(panel, 2, "panel");//�����ʾ�����ȼ�����������
    draw_panel_data();
    //������ͼ
    //��һ����������ʾ����ͼ��С

    ScrollView* view = ScrollView::create(winSize, c);
    view->setPosition(0,0);
    this->addChild(view);

    //������ͼ�˶��ķ���Ϊˮƽ�˶�
    view->setDirection(ScrollView::Direction::VERTICAL);
  
    //������ͼ�Ŀ�Ⱥ͸߶�
    view->setContentSize(Size(winSize.width, winSize.height*3));


    return true;
}


void GameMap::draw_panel_data() {
    extern HeroBase* hero;//��ȡȫ�ֱ���
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

    //Ϊ��ʾ���ƶ��½�һ��������
    Layer* c = Layer::create();
    int height = 300 + ((hero->v_total_card.size()) + 4) / 5 * 384; //���㻭��߶�
    int count = 0;
    for (auto x : hero->v_total_card) {
        x->setAnchorPoint(Vec2(0,0));
        x->setPosition(228 + count % 5 * 300, height - 384 - count / 5 * 384);
        x->setScale(1.5);
        c->addChild(x,1);
        count++;
     }
    //���ؼ��������们��
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
    //������ͼ�Ŀ�Ⱥ͸߶�
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
    //�Ѿ��߹��ĵ㻭��x
    auto passed = Sprite::create("./imgs/map/mapCompleteIcon.png");
    passed->setAnchorPoint(Vec2(0, 0));
    passed->setPosition(pos_x,pos_y);
    this->addChild(passed, 1);
    if (this->point_type == MONSTER || this->point_type == ELITE) {
        GameMap::reward_type = point_type == MONSTER ? 1 : 2;
        auto new_scene = FightScene::createScene();
        auto sign = Node::create();//ר��Ϊս����������һ����ǣ���Cards�л��õ�
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

//��Ϊ���ĵ���MenuItem�࣬����ֻ�ܴ�Menu* ��Ϊ���ڵ�
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
    icon->setTag(layer * 10 + order);//����ÿ��ͼ���Tag�������ҽڵ㡣
    node->addChild(icon,1);
}



//�ص㣬���map���������
void GameMap::draw_map(Sprite* map,Menu* icon_menu) {
    
    const int Max_Point = 7;
    const int Min_Point = 2;
    const int Point_Layer = 15;
    const int Chest_Layer = rand() % 4 + 5;//�������5-9��֮��
    const float monster_poss = 0.55;
    const float elite_poss = 0.1;
    const float rest_poss = 0.15;
    const float shop_poss = 0.05;
    const float event_poss = 0.15;

    //��ÿ���ĸ�����λ�������ʼ��
    for (int layer = 0; layer < Point_Layer; ++layer) {
        int point_num = rand() % 6 + 2;//ÿ���ĸ���
        list.push_back(std::vector<GamePoint*>());

        for (int point = 0; point < point_num; ++point) {
            //���x���꣬���Ұڶ�����Ϊ80���أ����Ҷ�ƽ����400���ؿհײ���,�м����
            int x = 824 + 80 * CCRANDOM_0_1() + 928 / point_num * (point - (point_num-1) / 2.0f);
            //���y���꣬���°ڶ�����20���أ��Ϸ���500�������·���200����
            int y = 190 + 20 * CCRANDOM_0_1() + 2216 / Point_Layer * layer;
            //������õ�����
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
            gpoint->draw_point(icon_menu);//��Ҫ����draw�������ֱ���ͷŵ���
            map->addChild(gpoint);//draw��ֻ����MenuItem����GamePoint���Ǳ��ͷ��ˣ�����Ҫ�����������
            list[layer].push_back(gpoint);
        }
    }
    //�������Ǹ���Щ������
    for (int layer = 0; layer < Point_Layer - 1; ++layer) {
        std::vector<GamePoint*>& more = 
            (list[layer].size() > list[layer + 1].size() ? list[layer] : list[layer + 1]);
        std::vector<GamePoint*>& less =
            (list[layer].size() > list[layer + 1].size() ? list[layer + 1] : list[layer]);
        int less_index = 0;
        int linked = 1;//less�ڵ�����������Ŀ
        float aver = float(more.size()) / less.size(); //ƽ��һ���ڵ��������
        more[0]->add(less[0]);//��һ���ڵ����У�����������
        
        for (int index = 1; index < more.size(); ++index) {
            //���moreʣ��ڵ��less��Ҫ�٣���ʵ��һ���ࣩ����ô��������
            if (more.size() - index < less.size() - less_index) {
                less_index++;
                more[index]->add(less[less_index]);
                linked = 1;
            }
            else {
                //�����������ǣ�ƽ��������-����������*����0.5+������ӣ�0-1��
                float rand_num = CCRANDOM_0_1() + (aver - linked) * 0.5;
                if (rand_num > 0.7 || less_index == less.size() - 1) {
                    more[index]->add(less[less_index]);
                    linked++;
                }
                //������оͺ�less��һ���ڵ�����
                else {
                    less_index++;
                    more[index]->add(less[less_index]);
                    linked = 1;
                }
            }
        }        
    }

    // ������ר�Ÿ�һЩ�ط���������
    for (int layer = 0; layer < Point_Layer-1; ++layer)
        for (int point = 0; point < list[layer].size() - 1; ++point) 
            //���������ֻ��һ��·�ߵĵ㣬�����и��ʼ�һ��б��
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
                //���ߣ���Ϊê�������½ǣ������и�ƫ��
                draw->drawLine(orgin+Vec2(48,72), list[layer + 1][p]->get_pos()+Vec2(48,24), Color4F::WHITE);
        }
    //�����һ�е�boss����
    for (int point = 0; point < list[Point_Layer-1].size(); ++point)
        draw->drawLine(list[Point_Layer - 1][point]->get_pos() + Vec2(48, 72),Vec2(864,2420), Color4F::WHITE);
    
        map->addChild(draw,0);

}
