#ifndef __GAMEMAP_H__
#define __GAMEMAP_H__
#include "cocos2d.h"
#include "cocos-ext.h"
#include <vector>
#include "CharacterBase.h"
#include "Cards.h"
#include "FightScene.h"
#include "Shop.h"
#include "Rest.h"
USING_NS_CC;
USING_NS_CC_EXT;
class GamePoint;
class HeroBase;

class GameMap :public cocos2d::Scene
{
private:
    std::vector<std::vector<GamePoint*> >list;//创建一个二维列表存放每层的关卡
    void draw_map(Sprite* map,Menu* icon_menu);
public:
    static int reward_type;//1:Monster, 2:Elite, 3:Boss
    static int layer;//记录目前角色所在位置
    static int order;
    static Scene* createScene();
    virtual bool init();
    void set_now_point();
    void draw_panel_data();//画血量，金钱，手牌数等
    void AttackBoss(cocos2d::Ref* pSender);//打boss时的回调函数
    void To_map(cocos2d::Ref* pSender) {
        if (Director::getInstance()->getRunningScene() == this)  return;
        Director::getInstance()->popToRootScene();//地图直接放在最底层吧
    }

    void Show_Cards(cocos2d::Ref* pSender);
    void Setting_Panel(cocos2d::Ref* pSender);
    CREATE_FUNC(GameMap);
};

//创建一个关卡节点类，用来生成地图（生成地图这部分也太难写了，qwq）
class GamePoint:public cocos2d::Node
{
public:
    enum type {
        MONSTER,
        ELITE,
        SHOP,
        REST,
        EVENT,
        CHEST
    };
private:
    int pos_x;
    int pos_y;
    int order;//从左至右的顺序，最左边为0
    int layer;//层级，最下方为0
    type point_type;
public:
    std::vector<int> next;
    std::vector<int> prev;
    void init_point(int l, int o, type t, int x, int y) { layer = l; order = o; point_type = t; pos_x = x; pos_y = y; };
    virtual bool init() { return true; };
    void add(GamePoint* p);//用于将节点间增加关系的函数
    Vec2 get_pos() { return Vec2(pos_x, pos_y); }
    void draw_point(Menu* node);
    void ClickIcon(cocos2d::Ref* pSender);
    CREATE_FUNC(GamePoint);
};


#endif 

