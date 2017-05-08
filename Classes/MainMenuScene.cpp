#include "MainMenuScene.h"
#include "Definitions.h"
#include "SimpleAudioEngine.h"
#include "GameScene/GameScene.h"
#include "SplashScene/SplashScene.h"
#include "SonarFrameworks.h"

USING_NS_CC;


MainMenuScene::MainMenuScene()
{
	listTheme = UserDefault::getInstance()->getStringForKey(ACTIVE_THEME, "100000000000000000000000000000");
	currentTheme = UserDefault::getInstance()->getIntegerForKey(CURRENT_THEME, 0);
	isMusicOn = UserDefault::getInstance()->getBoolForKey(IS_MUSICON, true);
	isEffectOn = UserDefault::getInstance()->getBoolForKey(IS_EFFECTON, true);
	isMusicChange = false;
	isEffectChange = false;
}

Scene* MainMenuScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MainMenuScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	this->getActiveThemes();

	auto background = LayerColor::create(BACKGROUND_COLOR);
	this->addChild(background);

	this->initButton();
	this->createPlayerStyle();

	SonarCocosHelper::AdMob::showBannerAd(SonarCocosHelper::eBottom);

	return true;
}

void MainMenuScene::initButton()
{
	playButton = Button::create("play-button.png", "", "", TextureResType::LOCAL);
	//playButton->setBright(false);
	//playButton->set
	playButton->setTag(ButtonTag::playButtonTag);
	playButton->setPosition(Vec2(visibleSize.width*0.5 + origin.x, visibleSize.height*0.45 + origin.y));
	playButton->addTouchEventListener(CC_CALLBACK_2(MainMenuScene::onButtonClick, this));
	this->addChild(playButton);

	rateButton = Button::create("rate-button.png", "", "", TextureResType::LOCAL);
	rateButton->setTag(ButtonTag::rateButtonTag);
	rateButton->setPosition(Vec2(playButton->getPosition().x, playButton->getPosition().y - playButton->getContentSize().height*1.2));
	rateButton->addTouchEventListener(CC_CALLBACK_2(MainMenuScene::onButtonClick, this));
	this->addChild(rateButton);

	rankButton = Button::create("rank-button.png", "", "", TextureResType::LOCAL);
	rankButton->setTag(ButtonTag::rankButtonTag);
	rankButton->setPosition(Vec2(rateButton->getPosition().x + rankButton->getContentSize().width*0.75, rateButton->getPosition().y - rateButton->getContentSize().height*1.5));
	rankButton->addTouchEventListener(CC_CALLBACK_2(MainMenuScene::onButtonClick, this));
	this->addChild(rankButton);

	shareButton = Button::create("share-button.png", "", "", TextureResType::LOCAL);
	shareButton->setTag(ButtonTag::shareButtonTag);
	shareButton->setPosition(Vec2(rankButton->getPosition().x + rankButton->getContentSize().width*1.5, rankButton->getPosition().y));
	shareButton->addTouchEventListener(CC_CALLBACK_2(MainMenuScene::onButtonClick, this));
	this->addChild(shareButton);

	musicButton = Button::create("music-button.png", "", "", TextureResType::LOCAL);
	musicButton->setTag(ButtonTag::musicButtonTag);
	musicButton->setPosition(Vec2(rankButton->getPosition().x - rankButton->getContentSize().width*1.5, rankButton->getPosition().y));
	musicButton->addTouchEventListener(CC_CALLBACK_2(MainMenuScene::onButtonClick, this));
	musicButton->setBright(isMusicOn);
	this->addChild(musicButton);

	effectButton = Button::create("effect-button.png", "", "", TextureResType::LOCAL);
	effectButton->setTag(ButtonTag::effectButtonTag);
	effectButton->setPosition(Vec2(musicButton->getPosition().x - musicButton->getContentSize().width*1.5, musicButton->getPosition().y));
	effectButton->addTouchEventListener(CC_CALLBACK_2(MainMenuScene::onButtonClick, this));
	effectButton->setBright(isEffectOn);
	this->addChild(effectButton);

	this->changeMusicState();
	this->changeEffectState();
}

void MainMenuScene::onButtonClick(Ref *sender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
	{
		return;
	}

	if (isEffectOn)
	{
		SimpleAudioEngine::getInstance()->playEffect(EFFECT_PLAYER_LAND);
	}
	int tag = ((Button*)sender)->getTag();
	switch (tag)
	{
	case ButtonTag::playButtonTag:
	{
		this->onPlayButtonClick();
		break;
	}
	case ButtonTag::rateButtonTag:
	{
		this->gotoAppPage();
		break;
	}
	case ButtonTag::musicButtonTag:
	{
		isMusicChange = true;
		this->changeMusicState();
		break;
	}
	case ButtonTag::effectButtonTag:
	{
		isEffectChange = true;
		this->changeEffectState();
		break;
	}
	case ButtonTag::shareButtonTag:
	{
		this->onShareButtonClick();
		break;
	}

	default:
		break;
	}
}

void MainMenuScene::changeMusicState()
{
	if (isMusicChange)
	{
		isMusicOn = !isMusicOn;

		auto userDefault = UserDefault::getInstance();
		userDefault->setBoolForKey(IS_MUSICON, isMusicOn);
		userDefault->flush();
		isMusicChange = false;
	}

	if (false == isMusicOn)
	{
		musicButton->setBright(false);
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.0);
	}
	else
	{
		musicButton->setBright(true);
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0);
	}
}

void MainMenuScene::changeEffectState()
{
	if (isEffectChange)
	{
		isEffectOn = !isEffectOn;

		auto userDefault = UserDefault::getInstance();
		userDefault->setBoolForKey(IS_EFFECTON, isEffectOn);
		userDefault->flush();
		isEffectChange = false;
	}

	if (false == isEffectOn)
	{
		effectButton->setBright(false);
		SimpleAudioEngine::getInstance()->setEffectsVolume(0.0);
	}
	else
	{
		effectButton->setBright(true);
		SimpleAudioEngine::getInstance()->setEffectsVolume(1.0);
	}

}

void MainMenuScene::createPlayerStyle()
{
	auto playerBox = Sprite::create("player-box.png");
	playerBox->setPosition(playButton->getPosition().x, playButton->getPosition().y + playerBox->getContentSize().height*1.5);
	this->addChild(playerBox);

	auto title = Sprite::create("title-mainmenu.png");
	title->setPosition(playerBox->getPosition().x, playerBox->getPosition().y + playerBox->getContentSize().height*1.5);
	this->addChild(title);

	player = Sprite::create(this->nameOfPlayer());
	player->setPosition(playerBox->getPosition());
	this->addChild(player);

	auto nextPlayerThemeButton = Button::create("next-player.png", "", "", TextureResType::LOCAL);
	nextPlayerThemeButton->setPosition(Vec2(playerBox->getPositionX() + playerBox->getContentSize().width, playerBox->getPositionY()));
	nextPlayerThemeButton->addTouchEventListener(CC_CALLBACK_2(MainMenuScene::nextPlayerTheme, this));
	this->addChild(nextPlayerThemeButton);

	auto prePlayerThemeButton = Button::create("previous-player.png", "", "", TextureResType::LOCAL);
	prePlayerThemeButton->setPosition(Vec2(playerBox->getPositionX() - playerBox->getContentSize().width, playerBox->getPositionY()));
	prePlayerThemeButton->addTouchEventListener(CC_CALLBACK_2(MainMenuScene::prePlayerTheme, this));
	this->addChild(prePlayerThemeButton);
}

void MainMenuScene::nextPlayerTheme(Ref *sender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
	{
		return;
	}

	for (auto i = activeThemes.begin(); i != activeThemes.end(); i++)
	{
		if (*i == currentTheme)
		{
			if (++i == activeThemes.end())
			{
				break;
			}
			currentTheme = *i;
			player->setTexture(this->nameOfPlayer());
			break;
		}
	}
}

void MainMenuScene::prePlayerTheme(Ref *sender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
	{
		return;
	}

	for (auto i = activeThemes.begin(); i != activeThemes.end(); i++)
	{
		if (*i == currentTheme && i != activeThemes.begin())
		{
			currentTheme = *--i;
			player->setTexture(this->nameOfPlayer());
			break;
		}
	}
}

void MainMenuScene::getActiveThemes()
{
	for (int i = 0; i < listTheme.size(); i++)
	{
		int theme = listTheme.at(i);
		if (theme == 49/*1 in ascii*/)
		{
			activeThemes.push_back(i);
		}
	}
}

std::string MainMenuScene::nameOfPlayer()
{
	auto fullName = StringUtils::format("player_%i.png", currentTheme);
	return fullName;
}

void MainMenuScene::gotoGameScene()
{
	UserDefault::getInstance()->setIntegerForKey(CURRENT_THEME, currentTheme);
	UserDefault::getInstance()->flush();

	auto scene = GameScene::createScene();
	_director->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

void MainMenuScene::onPlayButtonClick()
{
	auto moveToPlayButton = MoveTo::create(0.5, Vec2(playButton->getPosition()));
	auto moveWithEase = EaseExponentialIn::create(moveToPlayButton);

	player->runAction(Spawn::create(
		moveToPlayButton,
		CallFunc::create([&]() {this->gotoGameScene(); }),
		nullptr));
}

void MainMenuScene::gotoAppPage()
{
	auto link = StringUtils::format("https://play.google.com/store/apps/details?id=com.glasses.doodleplane");
	//auto link = StringUtils::format("http://www.amazon.com/gp/mas/dl/android?p=com.glasses.keeptheballdown");
	//auto link = StringUtils::format("market://details?id=com.glasses.doodleplane");
	Application::getInstance()->openURL(link);
}

void MainMenuScene::onShareButtonClick()
{
	//auto link = StringUtils::format("https://play.google.com/store/apps/details?id=com.glasses.doodleplane");
	//auto link = StringUtils::format("http://www.amazon.com/gp/mas/dl/android?p=com.glasses.keeptheballdown");
	auto link = StringUtils::format("market://details?id=com.glasses.doodleplane");
	Application::getInstance()->openURL(link);
}