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
    std::vector<std::vector<GamePoint*> >list;//����һ����ά�б���ÿ��Ĺؿ�
    void draw_map(Sprite* map,Menu* icon_menu);
public:
    static int reward_type;//1:Monster, 2:Elite, 3:Boss
    static int layer;//��¼Ŀǰ��ɫ����λ��
    static int order;
    static Scene* createScene();
    virtual bool init();
    void set_now_point();
    void draw_panel_data();//��Ѫ������Ǯ����������
    void AttackBoss(cocos2d::Ref* pSender);//��bossʱ�Ļص�����
    void To_map(cocos2d::Ref* pSender) {
        if (Director::getInstance()->getRunningScene() == this)  return;
        Director::getInstance()->popToRootScene();//��ͼֱ�ӷ�����ײ��
    }

    void Show_Cards(cocos2d::Ref* pSender);
    void Setting_Panel(cocos2d::Ref* pSender);
    CREATE_FUNC(GameMap);
};

//����һ���ؿ��ڵ��࣬�������ɵ�ͼ�����ɵ�ͼ�ⲿ��Ҳ̫��д�ˣ�qwq��
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
    int order;//�������ҵ�˳�������Ϊ0
    int layer;//�㼶�����·�Ϊ0
    type point_type;
public:
    std::vector<int> next;
    std::vector<int> prev;
    void init_point(int l, int o, type t, int x, int y) { layer = l; order = o; point_type = t; pos_x = x; pos_y = y; };
    virtual bool init() { return true; };
    void add(GamePoint* p);//���ڽ��ڵ�����ӹ�ϵ�ĺ���
    Vec2 get_pos() { return Vec2(pos_x, pos_y); }
    void draw_point(Menu* node);
    void ClickIcon(cocos2d::Ref* pSender);
    CREATE_FUNC(GamePoint);
};


#endif 

