#include "GameScene.h"
#include "SonarFrameworks.h"

USING_NS_CC;

#define backgroundZOder	-255
#define cloudZOder		  10
#define landZOder		  20
#define playerZOder		  30
#define ceilingZOder	  40
#define uiZOder			 255
GameScene::GameScene()
	:player(nullptr),
	gameState(GAME_OVER),
	curScore(0),
	curLevel(0),
	curLayerIndex(0),
	preLayerIndex(0),
	ySpeed(150),
	world(nullptr),
	skinNumber(0)
{
	listOfLand.clear();
	listOfDestroy.clear();
	themeNumber = UserDefault::getInstance()->getIntegerForKey(CURRENT_THEME, 1);
	listTheme = UserDefault::getInstance()->getStringForKey(ACTIVE_THEME, "100000000000000000000000000000");
	isEffectOn = UserDefault::getInstance()->getBoolForKey(IS_EFFECTON, true);
	bestScore = UserDefault::getInstance()->getIntegerForKey(BEST_SCORE, 0);

}

GameScene::~GameScene()
{
	/*CC_SAFE_DELETE(world);
	listOfLand.clear();
	listOfDestroy.clear();*/
}

void GameScene::onEnter()
{
	Layer::onEnter();

}

Scene* GameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameScene::create();
	layer->setTag(gameSceneTag);

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	deadBackground = LayerColor::create(Color4B(212, 0, 0, 255));
	//this->addChild(deadBackground, -1000);

	auto background = LayerColor::create(backgroundColor[themeNumber]);
	background->setLocalZOrder(backgroundZOder);
	this->addChild(background);

	hudLayer = HUDLayer::create();
	hudLayer->setColor(Color3B(backgroundColor[themeNumber]));
	hudLayer->setPosition(Vec2(visibleSize.width*0.5 + origin.x, visibleSize.height + origin.y - hudLayer->getContentSize().height*0.5));
	this->addChild(hudLayer, 500);

	botBorder = Sprite::create("bot-border.png");
	botBorder->setAnchorPoint(Vec2(0.5, 1.0));
	botBorder->setColor(Color3B(backgroundColor[themeNumber]));
	botBorder->setPosition(visibleSize.width*0.5 + origin.x, botBorder->getContentSize().height*0.5);
	this->addChild(botBorder, 500);

	this->initPhysics();
	this->addEventListener();

	landsFile = LandsFile::getInstance();
	landsFile->initLand(world, themeNumber);

	player = Player::create(world, themeNumber);
	this->addChild(player, playerZOder);
	auto playerBody = player->getBody();
	playerBody->SetActive(false);

	this->gameLoadFinishHandle();

	this->scheduleUpdate();

	SonarCocosHelper::AdMob::hideBannerAd();

	if (2==UserDefault::getInstance()->getIntegerForKey(COUNT_TO_SHOW_ADS,0))
	{
		SonarCocosHelper::AdMob::preLoadFullscreenAd();
	}

	return true;
}

void GameScene::addEventListener()
{
	touchEventListener = EventListenerTouchOneByOne::create();
	touchEventListener->retain();
	touchEventListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	touchEventListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchEventListener, this);

	keyBoardListener = EventListenerKeyboard::create();
	keyBoardListener->retain();
	keyBoardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	keyBoardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoardListener, this);
}

void GameScene::initPhysics()
{
	b2Vec2 gravity;
	gravity.Set(0, -9.8 * 5);
	world = new b2World(gravity);
	world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);
	world->SetContactListener(this);

	_debugDraw = new (std::nothrow) GLESDebugDraw(PTM_RATIO);
	world->SetDebugDraw(_debugDraw);

	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	flags += b2Draw::e_jointBit;
	flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_pairBit;
	flags += b2Draw::e_centerOfMassBit;
	_debugDraw->SetFlags(flags);

	b2BodyDef bodyDef;
	b2EdgeShape edgeShape;
	b2PolygonShape polygonShape;
	b2FixtureDef fixtureDef;


	//////////////////////////////////////////////////////////////////////////
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0, 0);
	auto deadLineBody = world->CreateBody(&bodyDef);
	deadLineBody->SetGravityScale(0);
	/*auto deadLineBodyNode = Sprite::create();
	deadLineBodyNode->setTag(deadlineTag);*/

	polygonShape.SetAsBox(
		(visibleSize.width *0.5f + origin.x) / PTM_RATIO, g_borderSize *0.5f / PTM_RATIO,
		b2Vec2((visibleSize.width *0.5f + origin.x) / PTM_RATIO, (origin.x - 200) / (2 * PTM_RATIO)),
		0);
	fixtureDef.shape = &polygonShape;
	fixtureDef.isSensor = true;
	fixtureDef.filter.categoryBits = DEAD_LINE;
	fixtureDef.filter.maskBits = PLAYER | LAND;
	deadLineBody->CreateFixture(&fixtureDef);
	fixtureDef.isSensor = false;

	//deadline collision land out of screen and delete it
	polygonShape.SetAsBox(
		(visibleSize.width + origin.x) / (2 * PTM_RATIO),
		g_borderSize / (2 * PTM_RATIO),
		b2Vec2(
			(visibleSize.width *0.5 + origin.x) / PTM_RATIO,
			(visibleSize.height + origin.x + 200) / PTM_RATIO), 0);
	fixtureDef.shape = &polygonShape;
	fixtureDef.isSensor = true;
	fixtureDef.filter.categoryBits = DEAD_LINE;
	fixtureDef.filter.maskBits = LAND;
	deadLineBody->CreateFixture(&fixtureDef);
	fixtureDef.isSensor = false;

	/*auto worldBodyNode = Sprite::create();
	worldBodyNode->retain();
	worldBodyNode->setTag(screenBorderTag);
*/

	bodyDef.type = b2_staticBody;
	//border left
	auto borderLeft = Sprite::create("border.png");
	this->addChild(borderLeft);

	bodyDef.position.Set(
		(borderLeft->getContentSize().width*0.5) / PTM_RATIO,
		(visibleSize.height*0.5 + origin.y) / PTM_RATIO);
	polygonShape.SetAsBox(
		borderLeft->getContentSize().width*0.5 / PTM_RATIO,
		borderLeft->getContentSize().height*0.5 / PTM_RATIO);
	fixtureDef.shape = &polygonShape;
	fixtureDef.friction = 0;
	fixtureDef.filter.categoryBits = SCREEN_BORDER;
	fixtureDef.filter.maskBits = PLAYER;
	auto borderLeftBody = world->CreateBody(&bodyDef);
	borderLeftBody->CreateFixture(&fixtureDef);
	borderLeftBody->SetUserData(borderLeft);

	//border right
	auto borderRight = Sprite::create("border.png");
	this->addChild(borderRight);

	bodyDef.position.Set(
		(visibleSize.width + origin.x * 2 - borderRight->getContentSize().width*0.5) / PTM_RATIO,
		(visibleSize.height*0.5 + origin.y) / PTM_RATIO);
	polygonShape.SetAsBox(
		borderRight->getContentSize().width*0.5 / PTM_RATIO,
		borderRight->getContentSize().height*0.5 / PTM_RATIO);
	fixtureDef.shape = &polygonShape;
	fixtureDef.friction = 0;
	fixtureDef.filter.categoryBits = SCREEN_BORDER;
	fixtureDef.filter.maskBits = PLAYER;
	auto borderRightBody = world->CreateBody(&bodyDef);
	borderRightBody->CreateFixture(&fixtureDef);
	borderRightBody->SetUserData(borderRight);
	//////////////////////////////////////////////////////////////////////////
	//ceiling
	ceiling = Sprite::create("ceiling.png");
	ceiling->setTag(ceilingTag);
	this->addChild(ceiling, ceilingZOder);

	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(
		(visibleSize.width *0.5 + origin.x) / PTM_RATIO,
		(hudLayer->getPosition().y - hudLayer->getContentSize().height*0.5 - ceiling->getContentSize().height *0.5) / PTM_RATIO);

	polygonShape.SetAsBox(
		ceiling->getContentSize().width / (PTM_RATIO * 2),
		ceiling->getContentSize().height / (PTM_RATIO * 2));
	fixtureDef.shape = &polygonShape;
	fixtureDef.filter.categoryBits = CEILLING;
	fixtureDef.filter.maskBits = PLAYER;
	auto ceilingBody = world->CreateBody(&bodyDef);
	ceilingBody->CreateFixture(&fixtureDef);
	ceilingBody->SetUserData(ceiling);
}

void GameScene::BeginContact(b2Contact *contact)
{
	auto fixtureA = contact->GetFixtureA();
	auto fixtureB = contact->GetFixtureB();

	int addMask = fixtureA->GetFilterData().categoryBits + fixtureB->GetFilterData().categoryBits;
	if (addMask == (PLAYER + DEAD_LINE)/* || addMask == (PLAYER + CEILLING) || addMask == (PLAYER + DEAD_LAND)*/)
	{
		if (isEffectOn)
		{
			SimpleAudioEngine::getInstance()->playEffect(EFFECT_PLAYER_DIE);
		}
		gameState = GAME_OVER;

		UserDefault::getInstance()->setIntegerForKey(BEST_SCORE, bestScore);
		UserDefault::getInstance()->flush();
		UserDefault::getInstance()->setIntegerForKey(SCORE, curScore);
		UserDefault::getInstance()->flush();

		int countToShowAds = UserDefault::getInstance()->getIntegerForKey(COUNT_TO_SHOW_ADS);
		countToShowAds++;
		UserDefault::getInstance()->setIntegerForKey(COUNT_TO_SHOW_ADS, countToShowAds);
		UserDefault::getInstance()->flush();

		player->breakAnimation();

		this->runAction(Sequence::create(
			CallFunc::create([&]() {this->stopAllLand(); }),
			DelayTime::create(0.5),
			CallFunc::create([&]() {this->showGameOverLayer(); }),
			nullptr));
		//this->showGameOverLayer();
		//this->deadBackground->setLocalZOrder(0);

		_eventDispatcher->removeEventListener(touchEventListener);
	}
	else if (addMask == (LAND + PLAYER))
	{
		if (contact->GetManifold()->localNormal.y > 0)
		{
			Land* land = (fixtureA->GetFilterData().categoryBits == LAND) ?
				((Land*)fixtureA->GetBody()->GetUserData()) :
				((Land*)fixtureB->GetBody()->GetUserData());
			if (gameState == GAME_START && !land->getColliend())
			{
				if (isEffectOn)
				{
					SimpleAudioEngine::getInstance()->playEffect(EFFECT_PLAYER_LAND);
				}
				land->setColliend(true);

				curScore += (land->getLayerIndex() - preLayerIndex)*scorePerLayer;

				if (curScore > bestScore)
				{
					bestScore = curScore;
				}

				preLayerIndex = land->getLayerIndex();

				this->speedControl();
				hudLayer->updateHUD(curScore, bestScore, speed);
				//////////////////////////////////////////////////////////////////////////

			}
		}
	}
	else if (addMask == (LAND + DEAD_LINE) || addMask == (DEAD_LAND + DEAD_LINE))
	{
		Land* land = (fixtureA->GetFilterData().categoryBits == 16) ?
			((Land *)fixtureA->GetBody()->GetUserData()) :
			((Land *)fixtureB->GetBody()->GetUserData());
		if (land != nullptr)
		{
			listOfDestroy.push_back(land);
			for (std::vector<Land *>::iterator iter = listOfLand.begin(); iter != listOfLand.end(); iter++)
			{
				if (*iter != nullptr && *iter == land)
				{
					listOfLand.erase(iter);
					break;
				}
			}
		}
	}
	else if (addMask == (PLAYER + PLAYER_GIFT))
	{
		if (isEffectOn)
		{
			SimpleAudioEngine::getInstance()->playEffect(EFFECT_PLAYER_LAND);
		}

		if (fixtureA->GetFilterData().categoryBits == PLAYER_GIFT)
		{
			playerSkinsDestroy.push_back(fixtureA->GetBody());
		}
		else
		{
			playerSkinsDestroy.push_back(fixtureB->GetBody());
		}
		listActive.push_back(skinNumber);
		this->setActiveThemeList();
	}


}

void GameScene::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	world->DrawDebugData();
}

void GameScene::showGameOverLayer()
{
	botBorder->removeFromParent();
	gameOverLayer = GameOverLayer::create();
	this->addChild(gameOverLayer, 1001);
}

void GameScene::stopAllLand()
{
	for (auto &iter : listOfLand)
	{
		auto land = (Land*)iter;
		land->getBody()->SetLinearVelocity(b2Vec2(0, 0));
		//land->changeToBlackLand();
	}

	for (auto playerBody : playerSkins)
	{
		playerBody->SetLinearVelocity(b2Vec2(0, 0));
	}

	this->unschedule(schedule_selector(GameScene::createLandTimer));
}

void GameScene::resumeAllLand(Ref *sender)
{
	/*gameState = GAME_START;
	for (auto &iter : listOfLand)
	{
		auto land = (Land*)iter;
		land->getBody()->SetLinearVelocity(b2Vec2(0, ySpeed / PTM_RATIO));
		land->changeToNormalLand();
	}
	float realTime = (LandVelocity * TimeLandSpawn) / ySpeed;
	this->schedule(schedule_selector(GameScene::createLandTimer), realTime);
	gameOverLayer->removeFromParentAndCleanup(true);
	this->deadBackground->setLocalZOrder(-1000);

	auto playerBody = player->getBody();
	playerBody->SetTransform(b2Vec2(visibleSize.width / (2 * PTM_RATIO), visibleSize.height / (2 * PTM_RATIO)), 0);
	playerBody->SetActive(true);*/
	gameOverLayer->removeFromParentAndCleanup(true);
	//this->deadBackground->setLocalZOrder(-1000);
	NotificationCenter::getInstance()->postNotification(CMD_GAME_OVER);
	this->deleteAllLand();
}

void GameScene::deleteAllLand()
{
	for (auto iter : listOfLand)
	{
		if (iter != nullptr)
		{
			listOfDestroy.push_back(iter);
		}
	}
	listOfLand.clear();
}

void GameScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void GameScene::update(float dt)
{
	if (gameState == GAME_EXIT)
	{
		Director::getInstance()->end();
	}

	float timeStep = 1.0f / 60.0f;
	int32 velocityInterations = 8;
	int32 positionInterations = 1;

	if (gameState != GAME_EXIT)
	{
		world->Step(timeStep, velocityInterations, positionInterations);
	}

	for (auto body = world->GetBodyList(); body; body = body->GetNext())
	{
		if (body->GetUserData() != nullptr)
		{
			auto node = (Sprite*)body->GetUserData();
			node->setPosition(body->GetPosition().x*PTM_RATIO, body->GetPosition().y*PTM_RATIO);
			node->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
		}
	}

	//==============//
	for (auto iter : listOfDestroy)
	{
		if (iter != nullptr)
		{
			((Land *)iter)->removeFromParent();//remove land create in create land timer

			world->DestroyBody(((Land *)iter)->getBody());
		}
	}
	listOfDestroy.clear();
	//=================//
	for (auto iter : playerSkinsDestroy)
	{
		if (iter != nullptr)
		{
			((Sprite*)iter->GetUserData())->removeFromParent();
			world->DestroyBody(iter);
		}
	}
	playerSkinsDestroy.clear();

	//=================// 

	if (gameState == GAME_OVER)
	{
		player->setMoveState(STOP);
		auto body = player->getBody();
		body->SetLinearVelocity(b2Vec2(0, 0));
		body->SetAngularVelocity(0);
		body->SetTransform(
			b2Vec2(visibleSize.width*0.5f / PTM_RATIO,
				-player->getContentSize().height / PTM_RATIO), 0);
		body->SetActive(false);
	}
}

bool GameScene::onTouchBegan(Touch * touch, Event * unused_event)
{
	if (gameState == GAME_PREPARED)
	{
		this->gameStartHandle();
		return true;
	}

	Vec2 touchPos = touch->getLocation();
	if (touchPos.x < visibleSize.width*0.5f)
	{
		player->setMoveState(LEFT);
	}
	else
	{
		player->setMoveState(RIGHT);
	}
	return true;
}

void GameScene::onTouchEnded(Touch * touch, Event * unused_event)
{
	player->setMoveState(STOP);
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode code, Event * event)
{
	if (gameState == GAME_PREPARED)
	{
		this->gameStartHandle();
		return;
	}
	if (code == EventKeyboard::KeyCode::KEY_A)
	{
		player->setMoveState(LEFT);
	}
	else if (code == EventKeyboard::KeyCode::KEY_D)
	{
		player->setMoveState(RIGHT);
	}
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode code, Event * event)
{
	player->setMoveState(STOP);
}

void GameScene::setStartVariablesValue()
{
	ySpeed = LandVelocity;
	curScore = 0;
	curLayerIndex = 0;
	preLayerIndex = 0;
	curLevel = 0;
}

void GameScene::gameStartHandle()
{
	gameState = GAME_START;

	this->setStartVariablesValue();

	float realTime = (LandVelocity * TimeLandSpawn) / ySpeed;
	this->levelIncrement();
	this->schedule(schedule_selector(GameScene::createLandTimer), realTime);
}

void GameScene::loadFirstBallAndLand()
{
	auto land = landsFile->popLand();
	land->setColliend(true);
	land->setLayerIndex(0);
	this->addChild(land, landZOder);
	listOfLand.push_back(land);
	land->moveToPoint(
		b2Vec2(
			(visibleSize.width*0.5f + origin.x) / PTM_RATIO,
			(-land->getContentSize().height + origin.x) / PTM_RATIO),
		b2Vec2((visibleSize.width*0.5f + origin.x) / PTM_RATIO,
			(visibleSize.height*0.08 + origin.x) / PTM_RATIO),
		0.5f);
	land->getBody()->SetActive(true);

	auto playerBody = player->getBody();
	playerBody->SetLinearVelocity(b2Vec2(0, 0));
	playerBody->SetAngularVelocity(0);
	playerBody->SetTransform(
		b2Vec2((visibleSize.width *0.5 + origin.x) / PTM_RATIO,
			(visibleSize.height*0.45f) / PTM_RATIO), 0);
	playerBody->SetActive(true);
}

void GameScene::gameLoadFinishHandle()
{
	gameState = GAME_PREPARED;

	this->loadFirstBallAndLand();

}

void GameScene::createLandTimer(float dt)
{
	curLayerIndex++;
	auto land = landsFile->popLand();
	land->setLayerIndex(curLayerIndex);
	this->addChild(land, landZOder);

	auto body = land->getBody();
	body->SetTransform(b2Vec2(
		((640 * RandomHelper::random_int(1, 7)) / 8 + origin.x + (visibleSize.width - 640) / 2) / PTM_RATIO,
		-land->getContentSize().height*0.5f / PTM_RATIO), 0);
	body->SetLinearVelocity(b2Vec2(0, ySpeed / PTM_RATIO));
	body->SetActive(true);
	listOfLand.push_back(land);

	//land->createPhysicWithHadthorn();


	if (RandomHelper::random_int(0, 20) == 0)
	{
		this->createPlayerSkin(land);
		/*auto playerSkin = PlayerSkin::create(world, themeNumber);
		this->addChild(playerSkin);

		auto body2 = playerSkin->getBody();
		body2->SetTransform(b2Vec2(
			((640 * RandomHelper::random_int(1, 7)) / 8 + origin.x + (visibleSize.width - 640) / 2) / PTM_RATIO,
			-land->getContentSize().height*2.5 / PTM_RATIO), 0);
		body2->SetLinearVelocity(b2Vec2(0, ySpeed / PTM_RATIO));
		body2->SetActive(true);
		playerSkins.push_back(body2);*/
	}
}

void GameScene::createPlayerSkin(Sprite *land)
{
	skinNumber = RandomHelper::random_int(1, TOTAL_SKIN - 1);

	auto playerSmall = Sprite::create(StringUtils::format("player_%i.png", skinNumber));
	playerSmall->setTag(skinNumber);
	//playerSmall->setOpacity(0);
	playerSmall->setScale(0.6);
	playerSmall->setPosition((640 * RandomHelper::random_int(1, 7)) / 8 + origin.x + (visibleSize.width - 640) / 2,
		-land->getContentSize().height*2.0);
	this->addChild(playerSmall);

	auto playerSkin = Sprite::create("player-border.png");
	playerSkin->setScale(1.8);
	playerSkin->setPosition(playerSmall->getContentSize()*0.5);
	playerSmall->addChild(playerSkin);

	auto scale = ScaleBy::create(0.3f, 1.2f);
	playerSkin->runAction(RepeatForever::create(Sequence::create(scale, EaseBackOut::create(RotateBy::create(0.3, 30)), scale->reverse(), nullptr)));

	b2CircleShape circleShape;
	circleShape.m_radius = playerSmall->getContentSize().width*0.5f / PTM_RATIO;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.friction = 1.0f;
	fixtureDef.filter.categoryBits = PLAYER_GIFT;
	fixtureDef.filter.maskBits = PLAYER;
	fixtureDef.isSensor = true;

	b2BodyDef bodyDef;
	bodyDef.type = b2_kinematicBody;

	auto body2 = world->CreateBody(&bodyDef);
	body2->CreateFixture(&fixtureDef);
	body2->SetUserData(playerSmall);
	body2->SetTransform(b2Vec2(
		((640 * RandomHelper::random_int(1, 7)) / 8 + origin.x + (visibleSize.width - 640) / 2) / PTM_RATIO,
		-land->getContentSize().height*2.5 / PTM_RATIO), 0);
	body2->SetLinearVelocity(b2Vec2(0, ySpeed / PTM_RATIO));
	//body2->
	playerSkins.push_back(body2);
}

void GameScene::levelIncrement()
{
	for (auto &iter : listOfLand)
	{
		auto land = (Land*)iter;
		land->getBody()->SetLinearVelocity(b2Vec2(0, ySpeed / PTM_RATIO));
	}

	for (auto playerSkin : playerSkins)
	{
		playerSkin->SetLinearVelocity(b2Vec2(0, ySpeed / PTM_RATIO));
	}
}


void GameScene::setActiveThemeList()
{
	for (auto i = listActive.begin(); i != listActive.end(); i++)
	{
		listTheme.at(*i) = '1';
	}

	UserDefault::getInstance()->setStringForKey(ACTIVE_THEME, listTheme);
	UserDefault::getInstance()->flush();
}

void GameScene::speedControl()
{
	if (curScore < 100)
	{
		speed = (curScore / 10) + 1;
	}
	if (curScore >= 100 && curScore < 120)
	{
		speed = 10;
	}
	if (curScore >= 120 && curScore < 210)
	{
		speed = ((curScore - 110) / 10) + 1;
	}
	if (curScore >= 210 && curScore < 230)
	{
		speed = 10;
	}
	if (curScore >= 230 && curScore < 310)
	{
		speed = ((curScore - 210) / 10) + 1;
	}
	if (curScore >= 310 && curScore < 330)
	{
		speed = 10;
	}
	if (curScore >= 330 && curScore < 400)
	{
		speed = ((curScore - 300) / 10) + 1;
	}
	if (curScore >= 400 && curScore < 420)
	{
		speed = 10;
	}
	if (curScore >= 420 && curScore < 480)
	{
		speed = ((curScore - 380) / 10) + 1;
	}
	if (curScore >= 480 && curScore < 500)
	{
		speed = 10;
	}
	if (curScore >= 500 && curScore < 550)
	{
		speed = ((curScore - 450) / 10) + 1;
	}
	if (curScore >= 550 && curScore < 570)
	{
		speed = 10;
	}
	if (curScore >= 570 && curScore < 610)
	{
		speed = ((curScore - 510) / 10) + 1;
	}
	if (curScore >= 610 && curScore < 630)
	{
		speed = 10;
	}
	if (curScore >= 630 && curScore < 660)
	{
		speed = ((curScore - 560) / 10) + 1;
	}
	if (curScore >= 660 && curScore < 680)
	{
		speed = 10;
	}
	if (curScore >= 680 && curScore < 700)
	{
		speed = ((curScore - 600) / 10) + 1;
	}
	if (curScore >= 700 && curScore < 720)
	{
		speed = 10;
	}
	if (curScore >= 720 && curScore < 730)
	{
		speed = ((curScore - 630) / 10) + 1;
	}
	if (curScore >= 730)
	{
		speed = 10;
	}
	ySpeed = (speed*speedIncrement) + LandVelocity;

	this->levelIncrement();

	float realTime = (LandVelocity * TimeLandSpawn) / ySpeed;


	this->schedule(schedule_selector(GameScene::createLandTimer), realTime);

}