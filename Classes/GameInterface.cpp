#include "GameInterface.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include "MainMenu.h"
#include "SimpleAudioEngine.h";
#define PI 3.14159265
#define COCOS2D_DEBUG 1

USING_NS_CC;
using namespace std;
//Scene ELements
int x, y;
int width, height;

//Game Elements

float ballAngle;
int quad;
int ballSpeed;
int luck;
int score = 0;
int speed;
cocos2d::Size bodySize;
std::vector<Sprite*> wallSpritesLeft;
std::vector<Sprite*> wallSpritesRight;
std::vector<Sprite*> wallSpritesTop;
std::vector<Sprite*> bricks;
std::string stringScore;
cocos2d::Size barSize;
//Sprites
Sprite* ballSprite;
Sprite* wallSprite1;
Sprite* barSprite;
Label* scoreText;
//Movement
bool leftKey, rightKey;
bool isHit;
bool isHit2;
int barWidth;
int previousQuad;

int getQuadrant(int quadrant, int choice) {
	int returnedQuadrant = quadrant;

	switch (choice) {
	case 0:
		//case left
		switch (quadrant) {
		case 90:
			returnedQuadrant = 0;
			break;
		case 180:
			returnedQuadrant = 270;
			break;
		}
		break;
	case 1:
		switch (quadrant) {
		case 0:
			returnedQuadrant = 90;
			break;
		case 270:
			returnedQuadrant = 180;
			break;
		}
		break;
	case 2:
		switch (quadrant) {
		case 0:
			returnedQuadrant = 270;
			break;
		case 90:
			returnedQuadrant = 180;
			break;
		}
		break;
	case 3:
		switch (quadrant) {
		case 270:
			returnedQuadrant = 0;
			break;
		case 180:
			returnedQuadrant = 90;
			break;
		}
		break;
	}
	return returnedQuadrant;
}

Scene* GameInterface::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	score = 0;

	luck = rand() % 2;
	switch (luck) {
	case 0:
		previousQuad = 90;
		quad = 90;
		break;
	case 1:
		previousQuad = 0;
		quad = 0;
		break;
	}
	// 'layer' is an autorelease object
	auto layer = GameInterface::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameInterface::init()
{
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	srand(time(0));

	stringScore.clear();
	stringScore.append("Score: ");
	stringScore.append(std::to_string(score));
	scoreText = Label::createWithTTF(stringScore, "shablagoo.ttf", 20);
	scoreText->setColor(ccc3(245, 245, 220));
	scoreText->setPosition(Point(visibleSize.width / 2 + origin.x + 250, 50));



	auto backgroundSprite = Sprite::create("background.jpg");
	backgroundSprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(backgroundSprite);
	
	ballSprite = Sprite::create("ball.png");
	barSprite = Sprite::create("bar.jpg");


	barSize = barSprite->getContentSize();

	width = visibleSize.width;
	height = visibleSize.height;
	x = width / 2 + origin.x;
	y = height / 2 + origin.y;
	ballSpeed = 10;

	isHit = FALSE;
	isHit2 = FALSE;

	speed = 0.7;

	barWidth = barSize.width / 5;

	//initial vars
	ballAngle = 0;



	for (int x = 0; x <= 50; x++) {
		Sprite* wall = Sprite::create("wall.png");
		wall->setPosition(0, (x + 1) * 20);
		this->addChild(wall);
		wallSpritesLeft.push_back(wall);

		wall = Sprite::create("wall.png");
		wall->setPosition(visibleSize.width, (x + 1) * 20);
		this->addChild(wall);
		wallSpritesRight.push_back(wall);

		wall = Sprite::create("wall.png");
		wall->setPosition((x + 1) * 20, visibleSize.height);
		this->addChild(wall);
		wallSpritesTop.push_back(wall);
	}

	for (int x = 1; x <= 6; x++) {
		Sprite* wall = Sprite::create("lime-block.jpg");
		wall->setPosition(x * 100 + 100, visibleSize.height / 2 + 150);
		this->addChild(wall);
		bricks.push_back(wall);
	}

	for (int x = 1; x <= 8; x++) {
		Sprite* wall = Sprite::create("red-block.jpg");
		wall->setPosition(x * 100, visibleSize.height / 2 + 100);
		this->addChild(wall);
		bricks.push_back(wall);
	}

	int z = 150;
	for (int x = 0; x < 18; x++) {
		if (x % 2 == 0) {
			Sprite* wall = Sprite::create("blue-block.jpg");
			wall->setPosition(z + x * 50, visibleSize.height / 2 + 175);
			this->addChild(wall);
			bricks.push_back(wall);
		}
	}

	for (int x = 1; x <= 17; x++) {
		Sprite* wall = Sprite::create("green-block.jpg");
		wall->setPosition(x * 50, visibleSize.height / 2 + 120);
		this->addChild(wall);
		bricks.push_back(wall);
	}
	this->addChild(scoreText);
	CCLOG("wtf %f", scoreText->getPositionX());
	bodySize = bricks.at(0)->getContentSize();

	gameScheduler = schedule_selector(GameInterface::update);
	CCLOG("%d, size", bricks.size());

	ballSprite->setPosition(x, y - 200);
	this->addChild(ballSprite);

	barSprite->setPosition(x, y - 270);
	this->addChild(barSprite);

	leftKey = FALSE;
	rightKey = FALSE;

	auto eventListener = EventListenerKeyboard::create();



	eventListener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event) {
		Vec2 loc = event->getCurrentTarget()->getPosition();

		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:
			leftKey = TRUE;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:
			rightKey = TRUE;
			break;
		}
	};

	eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) {
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:
			leftKey = FALSE;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:
			rightKey = FALSE;
			break;
		}
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, barSprite);
	this->schedule(gameScheduler, speed);

	return true;
}

void GameInterface::update(float delta)
{
	float ballX = ballSprite->getPosition().x;
	float ballY = ballSprite->getPosition().y;
	int barX = barSprite->getPositionX();
	int barY = barSprite->getPositionY();
	boolean isLose = FALSE;

	for (int x = 0; x < wallSpritesLeft.size(); x++) {
		if (ballSprite->getBoundingBox().intersectsRect(wallSpritesLeft.at(x)->getBoundingBox())) {
			auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
			audio->playEffect("wallbump.wav", false, 1.0f, 1.0f, 0.005f);
			quad = getQuadrant(quad, 0);
		}
	}

	for (int x = 0; x < wallSpritesRight.size(); x++) {

		if (ballSprite->getBoundingBox().intersectsRect(wallSpritesRight.at(x)->getBoundingBox())) {
			auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
			audio->playEffect("wallbump.wav", false, 1.0f, 1.0f, 0.005f);
			quad = getQuadrant(quad, 1);
		}
	}

	for (int x = 0; x < wallSpritesTop.size(); x++) {
		if (ballSprite->getBoundingBox().intersectsRect(wallSpritesTop.at(x)->getBoundingBox())) {
			quad = getQuadrant(quad, 2);
			auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
			audio->playEffect("wallbump.wav", false, 1.0f, 1.0f, 0.005f);
		}
	}

	if (!isHit) {
		if (ballSprite->getBoundingBox().intersectsRect(barSprite->getBoundingBox())) {

			if (ballX >= 0 && ballX < barSprite->getPositionX() - 74 + barWidth * 1) {
				quad = 90;
				ballAngle = ballAngle / 4;
			}
			else if (ballX >= barSprite->getPositionX() - 74 + barWidth * 1 && ballX < barSprite->getPositionX() - 74 + barWidth * 2) {
				quad = 90;
				ballAngle = ballAngle / 2;
			}
			else if (ballX >= barSprite->getPositionX() - 74 + barWidth * 2 && ballX < barSprite->getPositionX() - 74 + barWidth * 3) {
				quad = getQuadrant(quad, 3);
			}
			else if (ballX >= barSprite->getPositionX() - 74 + barWidth * 3 && ballX < barSprite->getPositionX() - 74 + barWidth * 4) {
				quad = 0;
				ballAngle = ballAngle / 2;
			}
			else if (ballX >= barSprite->getPositionX() - 74 + barWidth * 4 && ballX <= barSprite->getPositionX() - 74 + barWidth * 5) {
				quad = 0;
				ballAngle = ballAngle / 4;
			}
			auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
			audio->playEffect("paddle.wav", false, 1.0f, 1.0f, 0.005f);
			
		}
		isHit = TRUE;
	}
	if (!(ballSprite->getBoundingBox().intersectsRect(barSprite->getBoundingBox()))) {
		isHit = FALSE;
	}

	//bumangga sa wall
	for (int x = 0; x < bricks.size(); x++) {
		if (!isHit2) {
			if (ballSprite->getBoundingBox().intersectsRect(bricks.at(x)->getBoundingBox())) {
				CCLOG("PASOK dito sdasdsa");
				if (ballY >= bricks.at(x)->getPositionY() + bodySize.height / 2) {
					CCLOG("PASOK dito down %d", quad);
					switch (quad) {
					case 0:
						quad = 270;
						break;
					case 90:
						quad = 180;
						break;
					case 180:
						quad = 90;
						break;
					case 270:
						quad = 0;
						break;
					}
				}
				else if (ballY <= bricks.at(x)->getPositionY() - bodySize.height / 2) {
					CCLOG("PASOK dito up %d", quad);
					//case 3 down
					switch (quad) {
					case 0:
						quad = 270;
						break;
					case 90:
						quad = 180;
						break;
					case 180:
						quad = 90;
						break;
					case 270:
						quad = 0;
						break;
					}
				}
				else if (ballX <= bricks.at(x)->getPositionX() - bodySize.width / 2) {
					//case 1 right
					CCLOG("PASOK dito %d", quad);
					quad = getQuadrant(quad, 1);
					switch (quad) {
					case 0:
						quad = 270;
						break;
					case 270:
						quad = 90;
						break;
					}

				}
				else if (ballX >= bricks.at(x)->getPositionX() + bodySize.width / 2) {
					CCLOG("PASOK dito left");
					switch (quad) {
					case 90:
						quad = 270;
						break;
					case 180:
						quad = 0;
						break;
					}
				}
				this->removeChild(bricks.at(x));
				bricks.erase(bricks.begin() + x);
				isHit2 = TRUE;
				score += 1;
				stringScore.clear();
				stringScore.append("Score: ");
				stringScore.append(std::to_string(score));
				scoreText->setString(stringScore);
				auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
				audio->playEffect("poof.wav", false, 1.0f, 1.0f, 0.005f);
				if (bricks.size() == 0) {
					//auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
					//audio->playEffect("snakeBump.wav", false, 1.0f, 1.0f, 0.005f);
					//CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
					if (isScheduled(gameScheduler))
						unschedule(gameScheduler);
					isLose = true;
				}
			}
		}

		isHit2 = FALSE;
	}
	//ballSprite->setVisible(false);
	ballAngle = 180 - ballAngle - 90;
	ballX = ballX + ballSpeed * cos((ballAngle + quad) * PI / 180);
	ballY = ballY + ballSpeed * sin((ballAngle + quad) * PI / 180);
	if (!isLose) {
		ballSprite->setPositionX(ballX);
		ballSprite->setPositionY(ballY);
	}
	if (leftKey) {
		if (!wallSpritesLeft.at(0)->getBoundingBox().intersectsRect(barSprite->getBoundingBox()))
			barSprite->setPosition((barX - 5), barY);
	}
	if (rightKey) {
		if (!wallSpritesRight.at(0)->getBoundingBox().intersectsRect(barSprite->getBoundingBox()))
			barSprite->setPosition((barX + 5), barY);
	}
	//ballSprite->setVisible(true);
	
	if (!isLose) {
		if (ballY <= 0) {
			auto director = Director::getInstance();
			if (isScheduled(gameScheduler))
				unschedule(gameScheduler);
			isLose = true;
		}
	}
	if (isLose) {
		stringScore.clear();
		stringScore.append("Score: ");
		stringScore.append(std::to_string(score));
		scoreText->setString(stringScore);
		auto director = Director::getInstance();
		director->replaceScene(TransitionFade::create(1.0, MainMenu::createScene(std::to_string(score)), Color3B(0, 0, 0)));
		isLose = true;
		auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
		audio->playEffect("defeat.wav", false, 1.0f, 1.0f, 0.005f);
		this->removeAllChildren();
		wallSpritesLeft.clear();
		wallSpritesRight.clear();
		wallSpritesTop.clear();
		bricks.clear();
		
	}
	if (!isLose)
		this->schedule(gameScheduler, speed);
}


void GameInterface::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
