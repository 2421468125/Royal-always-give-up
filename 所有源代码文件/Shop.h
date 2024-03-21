#ifndef __SHOP_H__
#define __SHOP_H__

#include "cocos2d.h"
#include "CharacterBase.h"
USING_NS_CC;

class Shop : public Scene
{
private:
	static const int common_price = 50;
	static const int value_price = 75;
	static const int rare_price = 150;
	static int remove_time;
	bool is_remove;
	Vector<Cards*> shop_card;
	std::vector<int> v_price;
public:
	static Scene* createScene();
	virtual bool init();
	void draw_panel_data();
	void RemoveService(Ref* pSender);
	void QuitShop(Ref* pSender, EventListenerTouchOneByOne* listener);
	CREATE_FUNC(Shop);
};

#endif // !__SHOP_H__
