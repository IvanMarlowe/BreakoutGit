#ifndef __GAMEOVER_SCENE_H__
#define __GAMEOVER_SCENE_H__
#include "cocos2d.h"
#include "GameOver.h"
USING_NS_CC;

class GameOver : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene(std::string score);
	void GameOver::PlayAgain(cocos2d::Ref *pSender);
	void GameOver::exitNow(cocos2d::Ref *pSender);
	virtual bool init();
	// implement the "static create()" method manually
	CREATE_FUNC(GameOver);

private:
	static std::map<cocos2d::EventKeyboard::KeyCode,
		std::chrono::high_resolution_clock::time_point> keys;
	cocos2d::Label * label;
public:
	virtual void update(float delta) override;
};

#endif // __HELLOWORLD_SCENE_H__
