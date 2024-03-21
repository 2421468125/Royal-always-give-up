#include "GameStartMenu.h"
#include "GlobalValue.h"
#include "Cards.h"
USING_NS_CC;

Scene* GameStartMenu::createScene()
{
	Scene* scene = Scene::create();
	GameStartMenu* layer = GameStartMenu::create();
	scene->addChild(layer);
	return scene;
}

bool GameStartMenu::init() {
	
	if (!Scene::init())//ִ�л����init����������û������ִ�иó����ĳ�ʼ��
	{
		return false;
	}
	//��ȡ�ֱ��ʣ��������������ԭ�������Ϊ�ֱ��ʻ�����Ӧ���������Ǿ���λ��,���ڰ�����Ӧɾ�ˣ���Ȼ��û����
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto bg = Sprite::create("./imgs/bg/mainmenu.jpg");
	bg->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));//���ñ���λ��
	this->addChild(bg, -2);//�������ȼ��ϵ�


	//����ť
	auto button1 = MenuItemImage::create("./imgs/bg/startgame.png",
		"./imgs/bg/startgame.png", CC_CALLBACK_1(GameStartMenu::StartGame, this));
	button1->setAnchorPoint(Vec2(0, 0));//����ê�㣬���½Ǻ���һЩ
	button1->setPosition(0, 300);

	auto button2 = MenuItemImage::create("./imgs/bg/howtoplay.png",
		"./imgs/bg/howtoplay.png", CC_CALLBACK_1(GameStartMenu::HowToPlay, this));
	button2->setAnchorPoint(Vec2(0, 0));
	button2->setPosition(0, 220);

	auto button3 = MenuItemImage::create("./imgs/bg/aboutproject.png",
		"./imgs/bg/aboutproject.png", CC_CALLBACK_1(GameStartMenu::AboutProject, this));
	button3->setAnchorPoint(Vec2(0, 0));
	button3->setPosition(0, 140);

	auto button4 = MenuItemImage::create("./imgs/bg/quitgame.png",
		"./imgs/bg/quitgame.png", CC_CALLBACK_1(GameStartMenu::QuitGame, this));
	button4->setAnchorPoint(Vec2(0, 0));
	button4->setPosition(0, 60);

	auto menu = Menu::create(button1, button2, button3, button4, NULL);//����NULLӦ���ǲ��������ı�ʶ��
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 0);
	//auto card1 = Strike_R::create_card();
	//card1->setPosition(500, 500);
	//this->addChild(card1,0);
	return true;
}



void GameStartMenu::StartGame(Ref* pSender) {
	//�����˵��Ϸ���һ������
	auto new_scene = ChooseCharacter::createScene();
	auto change = TransitionCrossFade::create(0.8f, new_scene);
	Director::getInstance()->pushScene(change);
}

void GameStartMenu::HowToPlay(Ref* pSender) {
	auto scene = Scene::create();
	Size winSize = Director::getInstance()->getWinSize();
	auto back = MenuItemImage::create("./imgs/bg/charSelect/back.png",
		"./imgs/bg/charSelect/back.png", [](Ref* ref) {Director::getInstance()->popScene(); });
	auto menu = Menu::create(back, NULL);
	menu->setPosition(0, 0);
	back->setAnchorPoint(Vec2(0, 0));
	back->setPosition(0, 20);
	back->setScale(2.0f);
	scene->addChild(menu, 2);
	auto bg = Sprite::create("./imgs/bg/howtoplay.jpg");
	bg->setPosition(winSize.width / 2, winSize.height / 2);
	scene->addChild(bg, 0);

	auto label = Label::createWithTTF(FileUtils::getInstance()->getStringFromFile("./content/howtoplay.txt"),
		"./fonts/Myfont.ttf", 40);
	label->setTextColor(Color4B::WHITE);
	label->setDimensions(1328, int(864));
	label->setPosition(1728/2, 972/2);
	scene->addChild(label,1);
	auto change = TransitionProgressInOut::create(0.8f, scene);
	Director::getInstance()->pushScene(change);
}

void GameStartMenu::AboutProject(cocos2d::Ref* pSender) {
	auto scene = Scene::create();
	Size winSize = Director::getInstance()->getWinSize();
	Layer* c = Layer::create();
	int height = 2*winSize.height;
	int count = 0;
	auto back = MenuItemImage::create("./imgs/bg/charSelect/back.png",
		"./imgs/bg/charSelect/back.png", [](Ref* ref) {Director::getInstance()->popScene(); });
	auto menu = Menu::create(back, NULL);
	menu->setPosition(0, 0);
	back->setAnchorPoint(Vec2(0, 0));
	back->setPosition(0, 20);
	back->setScale(2.0f);
	scene->addChild(menu, 2);
	auto bg = Sprite::create("./imgs/bg/aboutgame.jpg");
	bg->setPosition(winSize.width / 2, winSize.height / 2);
	scene->addChild(bg, 0);

	auto label = Label::createWithTTF(FileUtils::getInstance()->getStringFromFile("./content/aboutgame.txt"),
		"./fonts/Myfont.ttf", 40);
	label->setTextColor(Color4B::BLACK);
	label->setAnchorPoint(Vec2(0, 0));
	label->setDimensions(1328, int(2*864));//�����ı���Χ
	label->setPosition(200,200);//�����Դ�õ���λ��
	c->addChild(label);
	ScrollView* view = ScrollView::create(winSize, c);
	view->setPosition(0, 0);
	scene->addChild(view, 1);

	view->setDirection(ScrollView::Direction::VERTICAL);
	//������ͼ�Ŀ�Ⱥ͸߶�
	view->setContentSize(Size(winSize.width, height));
	auto change = TransitionProgressInOut::create(0.8f, scene);
	Director::getInstance()->pushScene(change);
}

void GameStartMenu::QuitGame(Ref* pSender)
{
	Director::getInstance()->end();
}


Scene* ChooseCharacter::createScene() {
	auto scene = Scene::create();
	ChooseCharacter* layer = ChooseCharacter::create();
	scene->addChild(layer);
	return scene;
}

bool ChooseCharacter::init() {
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto ironclad = Sprite::create("./imgs/bg/charSelect/ironclad.jpg");
	ironclad->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(ironclad, -1,"bg");

	auto button1 = MenuItemImage::create("./imgs/bg/charSelect/start.png",
		"./imgs/bg/charSelect/start.png", CC_CALLBACK_1(ChooseCharacter::Start, this));
	button1->setAnchorPoint(Vec2(1, 0));
	button1->setPosition(1728, 20);
	button1->setScale(2.0f);

	auto button2 = MenuItemImage::create("./imgs/bg/charSelect/back.png",
		"./imgs/bg/charSelect/back.png", [](Ref* ref) {Director::getInstance()->popScene(); });
	//����lambda���ʽ���Ͳ����ٶ������Ա�����ˡ�
	button2->setAnchorPoint(Vec2(0, 0));//ê�����������½ǣ�����Գơ�
	button2->setPosition(0, 20);
	button2->setScale(2.0f);

	//3���л�����İ�ť
	auto ironcladButton = MenuItemImage::create("./imgs/bg/charSelect/ironcladButton.png",
		"./imgs/bg/charSelect/ironcladButton.png", 
		[=](Ref* ref) {
			if (hero == HeroBase::IRONCLAD)
				return;
			else {
				this->set_Hero(HeroBase::IRONCLAD);
				this->removeChildByName("bg");
				auto ironclad = Sprite::create("./imgs/bg/charSelect/ironclad.jpg");
				ironclad->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
				this->addChild(ironclad, -1, "bg");
			}
		});
	ironcladButton->setPosition(640, 260);

	auto silentButton = MenuItemImage::create("./imgs/bg/charSelect/silentButton.png",
		"./imgs/bg/charSelect/silentButton.png",
		[=](Ref* ref) {
			if (hero == HeroBase::SILENT)
				return;
			else {
				this->set_Hero(HeroBase::SILENT);
				this->removeChildByName("bg");
				auto ironclad = Sprite::create("./imgs/bg/charSelect/silent.jpg");
				ironclad->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
				this->addChild(ironclad, -1, "bg");
			}
		});
	silentButton->setPosition(840, 260);

	auto defectButton = MenuItemImage::create("./imgs/bg/charSelect/defectButton.png",
		"./imgs/bg/charSelect/defectButton.png",
		[=](Ref* ref) {
			if (hero == HeroBase::DEFECT)
				return;
			else {
				this->set_Hero(HeroBase::DEFECT);
				this->removeChildByName("bg");
				auto ironclad = Sprite::create("./imgs/bg/charSelect/defect.jpg");
				ironclad->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
				this->addChild(ironclad, -1, "bg");
			}
		});
	defectButton->setPosition(1040, 260);

	auto menu = Menu::create(button1, button2, ironcladButton, silentButton, defectButton, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 0);
	return this;
}

void ChooseCharacter::Start(cocos2d::Ref* pSender) {
	
	if (hero == HeroBase::IRONCLAD) {
		auto ic = Ironclad::create();
		ic->retain();//�ý�ɫ���𣬲���cocos�����
		GlobalValue::set_hero(ic);//���������������ȫ�ֱ���
	}
	else {//��ʾ������ɫ��δ����
		auto scene = Scene::create();
		auto dialogue = Sprite::create("./imgs/bg/dialogue.png");
		dialogue->setPosition(864, 486);
		scene->addChild(dialogue);
		auto content = Label::createWithTTF("This character isn't open currently.", "./fonts/Myfont.ttf", 30);
		content->setPosition(864, 500);
		scene->addChild(content);
		auto back = MenuItemImage::create("./imgs/bg/charSelect/back.png", "./imgs/bg/charSelect/back.png",
			[](Ref* ref)->void {Director::getInstance()->popScene(); });
		auto menu = Menu::create(back, NULL);
		menu->setPosition(864, 400);
		scene->addChild(menu, 1);
		Director::getInstance()->pushScene(scene);
		return;
	}
	auto map = GameMap::createScene();
	auto change = TransitionMoveInB::create(0.8f, map);
	Director::getInstance()->popToRootScene();
	Director::getInstance()->replaceScene(change);

}
