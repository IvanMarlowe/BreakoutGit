#ifndef __GAMEINTERFACE_SCENE_H__
#define __GAMEINTERFACE_SCENE_H__

#include "cocos2d.h"

class GameInterface : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameInterface);

private:
	void update(float) override;
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	cocos2d::SEL_SCHEDULE gameScheduler;
};

#endif // __GAMEINTERFACE_SCENE_H__
