#include "SplashScene.h"
#include "Definitions.h"
#include "SimpleAudioEngine.h"
#include "GameScene/GameScene.h"
#include "MainMenuScene.h"

USING_NS_CC;

Scene* SplashScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = SplashScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool SplashScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}


	auto bg = LayerColor::create(Color4B(255, 255, 255, 255));
	this->addChild(bg);

	auto logo3 = Sprite::create("glasses-logo3.png");
	logo3->setAnchorPoint(Vec2(0.5f, 0.5f));
	logo3->setPosition(visibleSize.width*0.5 + origin.x, visibleSize.height *0.5 + origin.y);
	this->addChild(logo3, 100);

	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.0f);
	this->runAction(Sequence::create(
		CallFunc::create(CC_CALLBACK_0(SplashScene::preloadResource, this)),
		DelayTime::create(3.0f),
		CallFunc::create(CC_CALLBACK_0(SplashScene::GotoMainMenuScene, this)),
		NULL));

	return true;
}

void SplashScene::preloadResource()
{
	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(BACKGROUND_MUSIC_LINK);
	SimpleAudioEngine::getInstance()->preloadEffect(EFFECT_PLAYER_DIE);
	SimpleAudioEngine::getInstance()->preloadEffect(EFFECT_PLAYER_LAND);

}

void SplashScene::GotoMainMenuScene()
{
	if (UserDefault::getInstance()->getBoolForKey(IS_MUSICON, false))
	{
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0);
	}
	else
	{
		SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.0);
	}
	SimpleAudioEngine::getInstance()->playBackgroundMusic(BACKGROUND_MUSIC_LINK, true);
	auto scene = MainMenuScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

