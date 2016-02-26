#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__
#include "cocos2d.h"
#include "MainMenu.h"
USING_NS_CC;

class MainMenu : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene(std::string scoreTemp);
	void MainMenu::PlayNow(cocos2d::Ref *pSender);
	void MainMenu::ExitNow(cocos2d::Ref *pSender);
	virtual bool init();
	// implement the "static create()" method manually
	CREATE_FUNC(MainMenu);

};

#endif // __HELLOWORLD_SCENE_H__
