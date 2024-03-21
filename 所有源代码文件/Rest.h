#ifndef __REST__H__
#define __REST__H__
#include "cocos2d.h"
#include "CharacterBase.h"

class RestScene : public Scene
{
private:
	bool has_done;
public:
	static Scene* createScene();
	virtual bool init();
	void Sleep(Ref* pSender);
	void Smith(Ref* pSender);
	CREATE_FUNC(RestScene);
};

#endif
