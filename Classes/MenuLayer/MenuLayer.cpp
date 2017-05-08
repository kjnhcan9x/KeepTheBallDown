#include "MenuLayer.h"
#include "GameScene/GameScene.h"

#define startButtonTag 20
#define musicButtonTag 21
#define rankButtonTag  22

extern bool isMusicOn;
GameScene *scene;

MenuLayer* MenuLayer::create(Ref *sender)
{
	scene = static_cast<GameScene*>(sender);
	MenuLayer *pRet = new(std::nothrow) MenuLayer();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}


MenuLayer::MenuLayer()
{
	isMusicOn = UserDefault::getInstance()->getBoolForKey(IS_MUSICON, false);
}

MenuLayer::~MenuLayer()
{
}

void MenuLayer::onEnter()
{
	Layer::onEnter();
}

// on "init" you need to initialize your instance
bool MenuLayer::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	scoreBackground = Sprite::createWithSpriteFrameName("5.png");
	scoreBackground->setPosition(visibleSize.width + scoreBackground->getContentSize().width*0.5f, 1078);
	this->addChild(scoreBackground);

	scoreText = Label::createWithTTF(bigFont, "000000");
	scoreText->setPosition(scoreBackground->getContentSize().width*0.5f, scoreBackground->getContentSize().height*0.5f);
	scoreBackground->addChild(scoreText);

	startButton = Button::create("3.png", "", "", TextureResType::PLIST);
	startButton->setTag(startButtonTag);
	startButton->setOpacity(0);
	startButton->setTouchEnabled(false);
	startButton->setPosition(startButtonPos);
	startButton->addTouchEventListener(CC_CALLBACK_2(MenuLayer::onButtonClick, this));
	startButton->setPositionX(visibleSize.width + startButton->getContentSize().width*0.5f);
	this->addChild(startButton);

	musicButton = Button::create("2.png", "", "", TextureResType::PLIST);
	musicButton->setTag(musicButtonTag);
	musicButton->setTouchEnabled(false);
	musicButton->setPosition(musicButtonPos);
	musicButton->addTouchEventListener(CC_CALLBACK_2(MenuLayer::onButtonClick, this));
	//musicButton->setPositionY(-musicButton->getContentSize().height * 0.5f);
	musicButton->setPositionX(-musicButton->getContentSize().height * 0.5f);
	musicButton->setBright(isMusicOn);
	this->addChild(musicButton);

	if (isMusicOn)
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0f);
	else
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.0f);
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/bgmusic.mp3", true);

	rankButton = Button::create("1.png", "", "", TextureResType::PLIST);
	rankButton->setTag(rankButtonTag);
	rankButton->setTouchEnabled(false);
	rankButton->setPosition(rankButtonPos);
	rankButton->addTouchEventListener(CC_CALLBACK_2(MenuLayer::onButtonClick, this));
	//rankButton->setPositionY(-rankButton->getContentSize().height *0.5f);
	rankButton->setPositionX(-rankButton->getContentSize().height *0.5f);
	this->addChild(rankButton);

	logo = Sprite::create("logo.png");
	logo->setPosition(visibleSize.width*0.5f, visibleSize.height + logo->getContentSize().height*0.5f);
	this->addChild(logo);

	startButtonOutPos = startButton->getPosition();
	musicButtonOutPos = musicButton->getPosition();
	rankButtonOutPos = rankButton->getPosition();
	logoOutPos = logo->getPosition();

	this->runAction(Sequence::create(
		CallFunc::create([&]() {this->MoveUIIntoScreen(); }),
		DelayTime::create(ANIM_TIME),
		CallFunc::create([&]() {scene->gameLoadFinishHandle(); }),
		NULL));
	return true;
}

void MenuLayer::updateScoreText(int score)
{
	//scoreText->setString(StringUtils::format("%i", score));
}

void MenuLayer::onButtonClick(Ref *sender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
	{
		return;
	}

	int tag = ((Button*)sender)->getTag();
	switch (tag)
	{
	case startButtonTag:
	{
		this->MoveUIOutScreen();
		this->CheckIsFirstGame();
		startButton->setTouchEnabled(false);
		rankButton->setTouchEnabled(false);
		musicButton->setTouchEnabled(false);

		break;
	}
	case musicButtonTag:
	{
		this->ChangeMusicState();
		break;
	}
	case rankButtonTag:
	{
		this->MoveUIOutScreen();
		this->ShowRankLayer();
		break;
	}

	default:
		break;
	}
}

void MenuLayer::ChangeMusicState()
{
	auto userDefault = UserDefault::getInstance();
	if (true == isMusicOn)
	{
		isMusicOn = false;
		musicButton->setBright(false);
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		SimpleAudioEngine::getInstance()->stopAllEffects();
	}
	else
	{
		isMusicOn = true;
		musicButton->setBright(true);
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0f);
	}
	userDefault->setBoolForKey(IS_MUSICON, isMusicOn);
	userDefault->flush();
}

void MenuLayer::MoveUIIntoScreen()
{
	auto fade = FadeTo::create(ANIM_TIME, 255);

	auto startButtonMoveIn = MoveTo::create(ANIM_TIME, startButtonPos);
	startButtonMoveIn->retain();
	auto moveWithEase = EaseExponentialOut::create(startButtonMoveIn);
	startButton->runAction(Spawn::createWithTwoActions(moveWithEase, fade));

	auto musicButtonMoveIn = MoveTo::create(ANIM_TIME*0.15f, musicButtonPos);
	musicButtonMoveIn->retain();
	//moveWithEase = EaseExponentialOut::create(musicButtonMoveIn);
	musicButton->runAction(Spawn::createWithTwoActions(fade, musicButtonMoveIn));

	auto rankButtonMoveIn = MoveTo::create(ANIM_TIME*0.15f*2.0f, rankButtonPos);
	rankButtonMoveIn->retain();
	//moveWithEase = EaseExponentialOut::create(rankButtonMoveIn);
	rankButton->runAction(Spawn::createWithTwoActions(fade, rankButtonMoveIn));

	auto logoMoveIn = MoveTo::create(ANIM_TIME, logoPos);
	logoMoveIn->retain();
	moveWithEase = EaseExponentialOut::create(logoMoveIn);
	logo->runAction(Sequence::create(
		moveWithEase,
		CallFunc::create([&]() {this->enableButton(); }),
		nullptr));

	/*auto scoreTextMove = MoveTo::create(ANIM_TIME, Vec2(visibleSize.width + scoreBackground->getContentSize().width*0.5f, 1078));
	auto easeMove = EaseExponentialOut::create(scoreTextMove);
	scoreBackground->runAction(easeMove);*/
	scoreBackground->setPosition(Vec2(visibleSize.width + scoreBackground->getContentSize().width*0.5f, 1078));
}

void MenuLayer::MoveUIOutScreen()
{
	auto fade = FadeTo::create(ANIM_TIME, 0);

	auto startButtonMoveIn = MoveTo::create(ANIM_TIME, startButtonOutPos);
	startButtonMoveIn->retain();
	auto moveWithEase = EaseExponentialOut::create(startButtonMoveIn);
	startButton->runAction(Spawn::createWithTwoActions(fade, moveWithEase));

	auto musicButtonMoveIn = MoveTo::create(ANIM_TIME, musicButtonOutPos);
	musicButtonMoveIn->retain();
	moveWithEase = EaseExponentialOut::create(musicButtonMoveIn);
	musicButton->runAction(Spawn::createWithTwoActions(fade, moveWithEase));

	auto rankButtonMoveIn = MoveTo::create(ANIM_TIME, rankButtonOutPos);
	rankButtonMoveIn->retain();
	moveWithEase = EaseExponentialOut::create(rankButtonMoveIn);
	rankButton->runAction(Spawn::createWithTwoActions(fade, moveWithEase));

	auto logoMoveIn = MoveTo::create(ANIM_TIME, logoOutPos);
	logoMoveIn->retain();
	moveWithEase = EaseExponentialOut::create(logoMoveIn);
	logo->runAction(moveWithEase);

	auto scoreTextMove = MoveTo::create(ANIM_TIME, scoreTextPos);
	auto easeMove = EaseExponentialOut::create(scoreTextMove);
	scoreBackground->runAction(easeMove);
}

void MenuLayer::CheckIsFirstGame()
{
	if (UserDefault::getInstance()->getBoolForKey(IS_FIRSTGAME, true))
	{
		UserDefault::getInstance()->setBoolForKey(IS_FIRSTGAME, false);
		/*auto guideLayer = GuideLayer::create();
		guideLayer->setPosition(-m_winSize.width, 0);
		this->addChild(guideLayer);

		guideLayer->layerFlyIn();*/

		CCLOG("Show guide!");
	}
	else
	{
		//NotificationCenter::getInstance()->postNotification(CMD_GAME_START);
		scene->gameStartHandle();

	}
}

void MenuLayer::ShowRankLayer()
{

}

void MenuLayer::gameOverHandle(Ref *sender)
{
	this->runAction(Sequence::create(
		CallFunc::create([&]() {this->MoveUIIntoScreen(); }),
		DelayTime::create(1.0f),
		/*CallFunc::create([&]() {NotificationCenter::getInstance()->postNotification(CMD_GAME_LOAD_FINISH); }),*/
		NULL));
}

void MenuLayer::enableButton()
{
	startButton->setTouchEnabled(true);
	musicButton->setTouchEnabled(true);
	rankButton->setTouchEnabled(true);
}

void MenuLayer::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

