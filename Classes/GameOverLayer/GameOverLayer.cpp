#include "GameOverLayer.h"
#include "GameScene/GameScene.h"
#include "MainMenuScene.h"
#include "SonarFrameworks.h"

int score;
int bestScore;

GameOverLayer::GameOverLayer()
{
	listTheme = UserDefault::getInstance()->getStringForKey(ACTIVE_THEME, "100000000000000000000000000000");
	currentTheme = UserDefault::getInstance()->getIntegerForKey(CURRENT_THEME, 0);
	isEffectOn = UserDefault::getInstance()->getBoolForKey(IS_EFFECTON, true);
	bestScore = UserDefault::getInstance()->getIntegerForKey(BEST_SCORE, 0);
	score = UserDefault::getInstance()->getIntegerForKey(SCORE, 0);
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

	this->getActiveThemeList();

	auto background = LayerColor::create(BACKGROUND_COLOR);
	this->addChild(background);

	playButton = Button::create("play-button.png", "", "", TextureResType::LOCAL);
	playButton->setTag(ButtonTag::playButtonTag);
	playButton->setPosition(Vec2(visibleSize.width*0.5 + origin.x, visibleSize.height*0.45 + origin.y));
	playButton->addTouchEventListener(CC_CALLBACK_2(GameOverLayer::onButtonClick, this));
	this->addChild(playButton);

	auto mainmenuButton = Button::create("mainmenu-button.png", "", "", TextureResType::LOCAL);
	mainmenuButton->setTag(ButtonTag::mainmenuButtonTag);
	mainmenuButton->setPosition(Vec2(playButton->getPosition().x, playButton->getPosition().y - playButton->getContentSize().height*1.2));
	mainmenuButton->addTouchEventListener(CC_CALLBACK_2(GameOverLayer::onButtonClick, this));
	this->addChild(mainmenuButton);

	this->createPlayerStyle();

	auto shareButton = Button::create("share-button-gameover.png", "", "", TextureResType::LOCAL);
	shareButton->setTag(ButtonTag::shareButtonTag);
	shareButton->setPosition(Vec2(
		visibleSize.width + origin.x - shareButton->getContentSize().width*0.65,
		visibleSize.height + origin.y - shareButton->getContentSize().height*0.65));
	shareButton->addTouchEventListener(CC_CALLBACK_2(GameOverLayer::onButtonClick, this));
	this->addChild(shareButton);

	SonarCocosHelper::AdMob::showBannerAd(SonarCocosHelper::eBottom);

	return true;
}

std::string GameOverLayer::nameOfPlayer()
{
	auto fullName = StringUtils::format("player_%i.png", currentTheme);
	return fullName;
}


void GameOverLayer::createPlayerStyle()
{
	auto playerBox = Sprite::create("player-box.png");
	playerBox->setPosition(playButton->getPosition().x, playButton->getPosition().y + playerBox->getContentSize().height*1.5);
	this->addChild(playerBox);

	player = Sprite::create(this->nameOfPlayer());
	player->setPosition(playerBox->getPosition());
	this->addChild(player);

	auto nextPlayerThemeButton = Button::create("next-player.png", "", "", TextureResType::LOCAL);
	nextPlayerThemeButton->setPosition(Vec2(playerBox->getPositionX() + playerBox->getContentSize().width, playerBox->getPositionY()));
	nextPlayerThemeButton->addTouchEventListener(CC_CALLBACK_2(GameOverLayer::nextPlayerTheme, this));
	this->addChild(nextPlayerThemeButton);

	auto prePlayerThemeButton = Button::create("previous-player.png", "", "", TextureResType::LOCAL);
	prePlayerThemeButton->setPosition(Vec2(playerBox->getPositionX() - playerBox->getContentSize().width, playerBox->getPositionY()));
	prePlayerThemeButton->addTouchEventListener(CC_CALLBACK_2(GameOverLayer::prePlayerTheme, this));
	this->addChild(prePlayerThemeButton);

	auto bestScoreLabel = Label::createWithTTF(normalFont, StringUtils::format("BEST %i",bestScore), TextHAlignment::CENTER);
	bestScoreLabel->setPosition(Vec2(playerBox->getPosition().x, playerBox->getPosition().y + playerBox->getContentSize().height));
	this->addChild(bestScoreLabel);

	auto scoreLabel = Label::createWithTTF(superbigFont, StringUtils::format("%i", score), TextHAlignment::CENTER);
	scoreLabel->setScale(0.5);
	//scoreLabel->setPosition(Vec2(playerBox->getPosition().x, bestScoreLabel->getPosition().y + bestScoreLabel->getContentSize().height + scoreLabel->getContentSize().height*0.5));
	scoreLabel->setPosition(Vec2(visibleSize.width*0.5 + origin.x, visibleSize.height + origin.y - scoreLabel->getContentSize().height));
	this->addChild(scoreLabel);

	auto moveTo = MoveTo::create(0.3f, Vec2(playerBox->getPosition().x, bestScoreLabel->getPosition().y + bestScoreLabel->getContentSize().height + scoreLabel->getContentSize().height*0.5));
	auto scaleTo = ScaleTo::create(0.3f, 1.0f);
	//auto ease = EaseBackOut::create(scaleTo);
	scoreLabel->runAction(Spawn::createWithTwoActions(moveTo, scaleTo));

}

void GameOverLayer::nextPlayerTheme(Ref *sender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
	{
		return;
	}

	for (auto i = listActive.begin(); i != listActive.end(); i++)
	{
		if (*i == currentTheme)
		{
			if (++i == listActive.end())
			{
				break;
			}
			currentTheme = *i;
			player->setTexture(this->nameOfPlayer());
			break;
		}
	}
}

void GameOverLayer::prePlayerTheme(Ref *sender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
	{
		return;
	}

	for (auto i = listActive.begin(); i != listActive.end(); i++)
	{
		if (*i == currentTheme && i != listActive.begin())
		{
			currentTheme = *--i;
			player->setTexture(this->nameOfPlayer());
			break;
		}
	}
}

void GameOverLayer::getActiveThemeList()
{
	for (int i = 0; i < listTheme.size(); i++)
	{
		int theme = listTheme.at(i);
		if (theme == 49/*1 in ascii*/)
		{
			listActive.push_back(i);
		}
	}
}

void GameOverLayer::gotoGameScene()
{
	UserDefault::getInstance()->setIntegerForKey(CURRENT_THEME, currentTheme);
	UserDefault::getInstance()->flush();

	auto scene = GameScene::createScene();
	_director->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

void GameOverLayer::onButtonClick(Ref *sender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::ENDED)
	{
		return;
	}

	if (isEffectOn)
	{
		SimpleAudioEngine::getInstance()->playEffect(EFFECT_PLAYER_LAND);
	}

	if (3==UserDefault::getInstance()->getIntegerForKey(COUNT_TO_SHOW_ADS,0))
	{
		UserDefault::getInstance()->setIntegerForKey(COUNT_TO_SHOW_ADS, 0);
		UserDefault::getInstance()->flush();

		SonarCocosHelper::AdMob::showPreLoadedFullscreenAd();
	}

	int tag = ((Button*)sender)->getTag();
	switch (tag)
	{
	case ButtonTag::playButtonTag:
	{
		auto moveToPlayButton = MoveTo::create(0.5, Vec2(playButton->getPosition()));
		auto moveWithEase = EaseExponentialIn::create(moveToPlayButton);

		player->runAction(Spawn::create(
			moveToPlayButton,
			CallFunc::create([&]() {this->gotoGameScene(); }),
			nullptr));
		break;
	}
	case ButtonTag::mainmenuButtonTag:
	{
		//utils::captureScreen(CC_CALLBACK_2(GameOverLayer::showScreenShot, this), "ScreenShot.png");
		this->gotoMainMenuScene();
		break;
	}
	case ButtonTag::shareButtonTag:
	{
		CCLOG("Share");
		break;
	}
	default:
		break;
	}
}

void GameOverLayer::gotoMainMenuScene()
{
	auto scene = MainMenuScene::createScene();
	_director->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

void GameOverLayer::showScreenShot(bool succeed, const std::string &outputFile)
{
	if (succeed)
	{
		auto screenShot = Sprite::create(outputFile);
		this->addChild(screenShot);
		screenShot->setPosition(visibleSize.width*0.5 + origin.x, visibleSize.height*0.5 + origin.y);
		//screenShot->setScale(1.5);
	}
	else
	{
		CCLOG("Shot failed!");
	}
}
