#ifndef __GAMESTARTMENU_H__
#define __GAMESTARTMENU_H__
#include "cocos2d.h"
#include "CharacterBase.h"
#include "GameMap.h"
class GameStartMenu :public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    
    void StartGame(cocos2d::Ref* pSender);
    void HowToPlay(cocos2d::Ref* pSender);
    void AboutProject(cocos2d::Ref* pSender);
    void QuitGame(cocos2d::Ref* pSender);
    // implement the "static create()" method manually
    CREATE_FUNC(GameStartMenu);
};

//选择人物的场景
class ChooseCharacter : public cocos2d::Scene
{
private:
    HeroBase::Hero hero = HeroBase::IRONCLAD;
public:
    
    static cocos2d::Scene* createScene();
    virtual bool init();
    void set_Hero(HeroBase::Hero h) { hero = h; }
    void Start(cocos2d::Ref* pSender);
    CREATE_FUNC(ChooseCharacter);
    
};
#endif
