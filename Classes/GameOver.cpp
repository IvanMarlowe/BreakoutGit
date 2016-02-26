#include "GameOver.h"
#include "MainMenu.h"
//#include "MainMenu.h"
#include <vector>  //for std::vector
#include <thread>
#include <string>
#include <iostream>
#include "SimpleAudioEngine.h";
#define COCOS2D_DEBUG 1
USING_NS_CC;
std::string score;
Scene* GameOver::createScene(std::string scoreTemp)
{
	score = scoreTemp;
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameOver::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}



//std::thread threadMovement(movementSnake);


// on "init" you need to initialize your instance
bool GameOver::init()
{
	//////////////////////////////
	// 1. super init first
	srand(time(NULL));
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto backgroundSprite = Sprite::create("gameoverbackground.jpg");
	backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	backgroundSprite->setScaleX((visibleSize.width / backgroundSprite->getContentSize().width) * 1);
	backgroundSprite->setScaleY((visibleSize.height / backgroundSprite->getContentSize().height) * 1);
	this->addChild(backgroundSprite);

	std::string stringScore = "";
	stringScore.append("Game Over! Your score is : ");
	stringScore.append(score);

	auto scoreText = Label::createWithTTF(stringScore, "shablagoo.ttf", 25);
	scoreText->setColor(ccc3(43, 88, 127));
	scoreText->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y + visibleSize.height * 0.10));
	this->addChild(scoreText);

	auto playAgain = MenuItemFont::create("Go To Main Menu?", CC_CALLBACK_1(GameOver::PlayAgain, this));
	playAgain->setFontSize(40);
	playAgain->setColor(ccc3(43, 88, 127));
	playAgain->setFontNameObj("shablagoo.ttf");
	playAgain->setPosition(Point(visibleSize.width / 2 + origin.x , (visibleSize.height / 2 + origin.y) - visibleSize.height * 0.1));

	auto exitGame = MenuItemFont::create("Exit", CC_CALLBACK_1(GameOver::exitNow, this));
	exitGame->setFontSize(40);
	exitGame->setColor(ccc3(43, 88, 127));
	exitGame->setFontNameObj("shablagoo.ttf");
	exitGame->setPosition(Point(visibleSize.width / 2 + origin.x, (visibleSize.height / 2 + origin.y) - visibleSize.height * 0.20));

	Menu *menu = Menu::create(exitGame, playAgain, NULL);
	menu->setPosition(Point(0, 0));
	this->addChild(playAgain);
	this->addChild(menu);
	return true;
}



void GameOver::PlayAgain(cocos2d::Ref *pSender) {
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playEffect("buttonClick.wav", false, 1.0f, 1.0f, 0.005f);
	auto director = Director::getInstance();
	//director->replaceScene(TransitionFade::create(1.0, MainMenu::createScene(std::to_string(score)), Color3B(0, 0, 0)));
}

void GameOver::exitNow(cocos2d::Ref *pSender) {
	auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playEffect("buttonClick.wav", false, 1.0f, 1.0f, 0.005f);
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
		MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
		return;
	#endif

		Director::getInstance()->end();

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
	#endif
}

void GameOver::update(float delta) {
	Node::update(delta);

}
