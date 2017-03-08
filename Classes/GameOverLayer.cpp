#include "GameOverLayer.h"

GameOverLayer::GameOverLayer()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("CountLayer/BraveMan0.plist", "CountLayer/BraveMan0.png");
}

GameOverLayer::~GameOverLayer()
{
}

void GameOverLayer::onEnter()
{
	Layer::onEnter();
}


// on "init" you need to initialize your instance
bool GameOverLayer::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	
/*

	auto backgroundLayer1 = LayerColor::create(Color4B(212, 0, 0, 255));
	this->addChild(backgroundLayer1, 0);
*/

	scoreBoard = Sprite::createWithSpriteFrameName("9.png");
	scoreBoard->setAnchorPoint(Vec2::ZERO);
	scoreBoard->setPosition(151, 457);
	this->addChild(scoreBoard, 30);

	auto scoreBoardRope = Sprite::createWithSpriteFrameName("8.png");
	scoreBoardRope->setAnchorPoint(Vec2(0.5,0.1));
	scoreBoardRope->setPosition(scoreBoard->getContentSize().width*0.25f,
		scoreBoard->getContentSize().height);
	scoreBoard->addChild(scoreBoardRope, -1);

	auto scoreBoardRope2 = Sprite::createWithSpriteFrameName("8.png");
	scoreBoardRope2->setAnchorPoint(Vec2(0.5, 0.1));
	scoreBoardRope2->setPosition(scoreBoard->getContentSize().width*0.75f,
		scoreBoard->getContentSize().height);
	scoreBoard->addChild(scoreBoardRope2, -1);

	auto desPos = scoreBoard->getPosition();
	scoreBoard->setPositionY(visibleSize.height);

	auto scoreBoardMove = MoveTo::create(1.0f, desPos);
	scoreBoardMove->retain();
	scoreBoard->runAction(EaseBounceOut::create(scoreBoardMove));

/*
	auto layer = Sprite::create("gameover-layer.png");
	layer->setPosition(visibleSize / 2);
	this->addChild(layer);*/

	auto backButton = Button::create("6.png", "", "", TextureResType::PLIST);
	backButton->setPosition(Vec2(100, 100));
	backButton->addTouchEventListener(CC_CALLBACK_2(GameOverLayer::reviveTheBall, this));
	this->addChild(backButton);

	auto playButton = Button::create("play-button.png", "", "", TextureResType::LOCAL);
	playButton->setPosition(Vec2(300, 150));
	playButton->addTouchEventListener(CC_CALLBACK_2(GameOverLayer::gotoGameScene, this));
	this->addChild(playButton);

	auto action = ScaleBy::create(0.5f, 1.1f, 1.1f);
	auto ease = EaseElasticIn::create(action);
	auto action2 = ScaleTo::create(0.1f, 1.0f, 1.0f);
	playButton->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1.0f), ease, /*DelayTime::create(0.2f),*/ action2, nullptr)));

	return true;
}

void GameOverLayer::reviveTheBall(Ref *sender, Widget::TouchEventType type)
{
	if (type!=Widget::TouchEventType::ENDED)
	{
		return;
	}

}

void GameOverLayer::gotoGameScene(Ref *sender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
	{
		return;
	}

	NotificationCenter::getInstance()->postNotification(CMD_RESUME_ALL_LAND);
}


