#include "HUDLayer.h"

HUDLayer::HUDLayer()
{
	score = 0;
	bestScore = UserDefault::getInstance()->getIntegerForKey(BEST_SCORE, 0);
	speed = 1;
}

HUDLayer::~HUDLayer()
{
}

HUDLayer * HUDLayer::create()
{
	auto bRet = new HUDLayer();
	if (bRet && bRet->init())
	{
		bRet->autorelease();
		return bRet;
	}
	CC_SAFE_DELETE(bRet);
	return nullptr;
}

bool HUDLayer::init()
{
	if (!Sprite::initWithFile("hud-background.png"))
	{
		return false;
	}

	auto shadow = Sprite::create("hud-shadow.png");
	shadow->setPosition(this->getContentSize()*0.5);
	this->addChild(shadow);

	this->createHUDLabel();

	return true;
}

void HUDLayer::createHUDLabel()
{
	scoreLabel = Label::createWithTTF(bigFont, "0", TextHAlignment::CENTER);
	scoreLabel->setTextColor(Color4B::BLACK);
	scoreLabel->setPosition(Vec2(this->getContentSize())*0.5);
	this->addChild(scoreLabel);

	auto strBestScore = Label::createWithTTF(smallFont, "BEST", TextHAlignment::CENTER);
	strBestScore->setTextColor(Color4B::BLACK);
	strBestScore->setPosition(Vec2(origin.x + strBestScore->getContentSize().width*0.5 + 20, this->getContentSize().height - strBestScore->getContentSize().height));
	this->addChild(strBestScore);

	bestScoreLabel = Label::createWithTTF(normalFont, StringUtils::format("%i", bestScore), TextHAlignment::CENTER);
	bestScoreLabel->setTextColor(Color4B::BLACK);
	bestScoreLabel->setPosition(Vec2(strBestScore->getPosition().x, bestScoreLabel->getContentSize().height));
	this->addChild(bestScoreLabel);

	auto strSpeed = Label::createWithTTF(smallFont, "SPEED", TextHAlignment::CENTER);
	strSpeed->setTextColor(Color4B::BLACK);
	strSpeed->setPosition(Vec2(origin.x + visibleSize.width - strSpeed->getContentSize().width*0.5 - 20, this->getContentSize().height - strSpeed->getContentSize().height));
	this->addChild(strSpeed);

	speedLabel = Label::createWithTTF(normalFont, "1", TextHAlignment::CENTER);
	speedLabel->setTextColor(Color4B::BLACK);
	speedLabel->setPosition(Vec2(strSpeed->getPosition().x, speedLabel->getContentSize().height));
	this->addChild(speedLabel);
}

void HUDLayer::updateHUD(int score, int bestScore, int speed)
{
	scoreLabel->setString(StringUtils::format("%i", score));
	bestScoreLabel->setString(StringUtils::format("%i", bestScore));
	if (speed < 10)
	{
		speedLabel->setString(StringUtils::format("%i", speed));
	}
	else
	{
		speedLabel->setString("MAX");
	}
}