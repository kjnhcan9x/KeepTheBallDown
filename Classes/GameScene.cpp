#include "GameScene.h"
#include "GameOverLayer.h"

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
	preLayerIndex(0)


{
	isMusicOn = UserDefault::getInstance()->getBoolForKey(IS_MUSICON, false);
}

GameScene::~GameScene()
{
	CC_SAFE_DELETE(world);
}

void GameScene::onEnter()
{
	Layer::onEnter();

	landsFile = LandsFile::getInstance();
	landsFile->initLand(20, world);
	NotificationCenter::getInstance()->addObserver(this, SEL_CallFuncO(&GameScene::gameStartHandle), CMD_GAME_START, NULL);
	NotificationCenter::getInstance()->addObserver(this, SEL_CallFuncO(&GameScene::gameLoadFinishHandle), CMD_GAME_LOAD_FINISH, NULL);
	NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameScene::resumeAllLand), CMD_RESUME_ALL_LAND, NULL);
	//NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(GameScene::deleteAllLand), CMD_DELETE_ALL_LAND, NULL);
}

Scene* GameScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	
	// 'layer' is an autorelease object
	auto layer = GameScene::create();

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
	this->addChild(deadBackground, -1000);

	this->initPhysics();

	auto background = Sprite::create("bg.png");
	background->setPosition(visibleSize.width *0.5f, visibleSize.height *0.5f);
	background->setZOrder(backgroundZOder);
	this->addChild(background);
	auto backgroundLayer = LayerColor::create(Color4B::WHITE);
	//this->addChild(backgroundLayer, -1);




	menuLayer = MenuLayer::create();
	menuLayer->setZOrder(uiZOder);
	this->addChild(menuLayer);

	player = Player::create(world);
	//player->setPosition(200, 800);
	this->addChild(player, playerZOder);
	auto playerBody = player->getBody();
	playerBody->SetActive(false);

	/*tail = MotionStreak::create(0.2f, 5.0f, 55.0f, Color3B::BLACK, "player2.png");
	tail->setFastMode(true);
	this->addChild(tail);*/

	touchEventListener = EventListenerTouchOneByOne::create();
	touchEventListener->retain();
	touchEventListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	touchEventListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);

	keyBoardListener = EventListenerKeyboard::create();
	keyBoardListener->retain();
	keyBoardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	keyBoardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoardListener, this);


	/*land = Land::create(world);
	this->addChild(land);*/

	this->scheduleUpdate();
	return true;
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
		visibleSize.width *0.5f / PTM_RATIO, g_borderSize *0.5f / PTM_RATIO,
		b2Vec2(visibleSize.width *0.5f / PTM_RATIO, (-200) / (2 * PTM_RATIO)),
		0);
	fixtureDef.shape = &polygonShape;
	fixtureDef.isSensor = true;
	fixtureDef.filter.categoryBits = DEAD_LINE;
	fixtureDef.filter.maskBits = PLAYER | LAND;
	deadLineBody->CreateFixture(&fixtureDef);
	fixtureDef.isSensor = false;

	//deadline collision land out of screen and delete it
	polygonShape.SetAsBox(
		visibleSize.width / (2 * PTM_RATIO),
		g_borderSize / (2 * PTM_RATIO),
		b2Vec2(
			visibleSize.width / (2 * PTM_RATIO),
			(visibleSize.height + 200) / PTM_RATIO), 0);
	fixtureDef.shape = &polygonShape;
	fixtureDef.isSensor = true;
	fixtureDef.filter.categoryBits = DEAD_LINE;
	fixtureDef.filter.maskBits = LAND;
	deadLineBody->CreateFixture(&fixtureDef);
	fixtureDef.isSensor = false;

	bodyDef.type = b2_staticBody;
	auto screenBorderBody = world->CreateBody(&bodyDef);
	/*auto worldBodyNode = Sprite::create();
	worldBodyNode->retain();
	worldBodyNode->setTag(screenBorderTag);
*/

//border left
	edgeShape.Set(
		b2Vec2(0, -200 / PTM_RATIO),
		b2Vec2(0, (visibleSize.height + 200) / PTM_RATIO));
	fixtureDef.shape = &edgeShape;
	fixtureDef.friction = 0;
	fixtureDef.filter.categoryBits = SCREEN_BORDER;
	fixtureDef.filter.maskBits = PLAYER;
	screenBorderBody->CreateFixture(&fixtureDef);

	//border right
	edgeShape.Set(
		b2Vec2(visibleSize.width / PTM_RATIO, -200 / PTM_RATIO),
		b2Vec2(visibleSize.width / PTM_RATIO, (visibleSize.height + 200) / PTM_RATIO));
	fixtureDef.shape = &edgeShape;
	fixtureDef.friction = 0;
	fixtureDef.filter.categoryBits = SCREEN_BORDER;
	fixtureDef.filter.maskBits = PLAYER;
	screenBorderBody->CreateFixture(&fixtureDef);
	//////////////////////////////////////////////////////////////////////////
	//ceiling
	ceiling = Sprite::create("ceiling.png");
	ceiling->setTag(ceilingTag);
	this->addChild(ceiling, ceilingZOder);

	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(
		visibleSize.width / (2 * PTM_RATIO),
		(visibleSize.height - ceiling->getContentSize().height / 2) / PTM_RATIO);

	polygonShape.SetAsBox(
		ceiling->getContentSize().width / (PTM_RATIO * 2),
		ceiling->getContentSize().height / (PTM_RATIO * 2));
	fixtureDef.shape = &polygonShape;
	fixtureDef.filter.categoryBits = CEILLING;
	fixtureDef.filter.maskBits = PLAYER;
	auto ceilingBody = world->CreateBody(&bodyDef);
	ceilingBody->CreateFixture(&fixtureDef);
	ceilingBody->SetUserData(ceiling);

	/*ceiling->setPosition(
		visibleSize.width / 2,
		visibleSize.height - ceiling->getContentSize().height / 2);*/
}

void GameScene::BeginContact(b2Contact *contact)
{
	auto fixtureA = contact->GetFixtureA();
	auto fixtureB = contact->GetFixtureB();

	int addMask = fixtureA->GetFilterData().categoryBits + fixtureB->GetFilterData().categoryBits;
	if (addMask == (PLAYER + DEAD_LINE) || addMask == (PLAYER + CEILLING) || addMask == (PLAYER + DEAD_LAND))
	{
		if (isMusicOn)
		{
			SimpleAudioEngine::getInstance()->playEffect("music/pop.wav");
		}
		gameState = GAME_OVER;
		//CCLOG("Game Over!");

		this->showGameOverLayer();
		this->stopAllLand();
		this->deadBackground->setZOrder(0);

		_eventDispatcher->removeEventListener(touchEventListener);


		//NotificationCenter::getInstance()->postNotification(CMD_GAME_OVER);


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
				land->setColliend(true);

				curScore += (land->getLayerIndex() - preLayerIndex)*scorePerLayer;
				menuLayer->updateScoreText(curScore);

				preLayerIndex = land->getLayerIndex();
				int level = (curScore / 10);

				if (level > curLevel)
				{
					curLevel = level;
					if (ySpeed < MaxVelocity)
					{

						ySpeed += (speedIncrement);

						//ySpeed = MIN(ySpeed, MaxVelocity);
						this->levelIncrement();

						float realTime = (LandVelocity * TimeLandSpawn) / ySpeed;


						this->schedule(schedule_selector(GameScene::createLandTimer), realTime);
					}
					else
					{
						ySpeed = MaxVelocity;
					}

					CCLOG("Speed: %.2f", ySpeed);
					CCLOG("Level: %i", curLevel);
				}
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
			for (auto iter = listOfLand.begin(); iter != listOfLand.end(); iter++)
			{
				if (*iter != nullptr && *iter == land)
				{
					listOfLand.erase(iter);
					//CCLOG("%i", listOfLand.size());
					break;
				}
			}
		}
	}
}

void GameScene::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
	world->DrawDebugData();
}

void GameScene::showGameOverLayer()
{
	gameOverLayer = GameOverLayer::create();
	this->addChild(gameOverLayer, 100);
}

void GameScene::stopAllLand()
{
	for (auto &iter : listOfLand)
	{
		auto land = (Land*)iter;
		land->getBody()->SetLinearVelocity(b2Vec2(0, 0));
		land->changeToBlackLand();
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
	this->deadBackground->setZOrder(-1000);

	auto playerBody = player->getBody();
	playerBody->SetTransform(b2Vec2(visibleSize.width / (2 * PTM_RATIO), visibleSize.height / (2 * PTM_RATIO)), 0);
	playerBody->SetActive(true);*/
	gameOverLayer->removeFromParentAndCleanup(true);
	this->deadBackground->setZOrder(-1000);
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

	//tail->setPosition(player->getPosition());

	float timeStep = 1 / 60.0;
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

			landsFile->pushOneLand((Land *)iter);
			//CCLOG("Land file: %i", landsFile->listOfLand.size());
		}
	}
	listOfDestroy.clear();
	//==============//
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

void GameScene::gameStartHandle(Ref *sender)
{
	gameState = GAME_START;

	this->setStartVariablesValue();

	//enable touch 
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchEventListener, this);

	float realTime = (LandVelocity * TimeLandSpawn) / ySpeed;
	this->levelIncrement();
	this->schedule(schedule_selector(GameScene::createLandTimer), realTime);

	menuLayer->updateScoreText(0);
}

void GameScene::loadFirstBallAndLand()
{
	landsFile->initLand(20, world);
	auto land = landsFile->popLand();
	land->setLayerIndex(0);
	this->addChild(land, landZOder);
	listOfLand.push_back(land);
	land->moveToPoint(
		b2Vec2(
			(visibleSize.width*1.1) / PTM_RATIO,
			(visibleSize.height*0.08) / PTM_RATIO),
		b2Vec2(visibleSize.width / (2 * PTM_RATIO),
			(visibleSize.height*0.08) / PTM_RATIO),
		0.5f);
	land->getBody()->SetActive(true);

	auto playerBody = player->getBody();
	playerBody->SetLinearVelocity(b2Vec2(0, 0));
	playerBody->SetAngularVelocity(0);
	playerBody->SetTransform(
		b2Vec2(visibleSize.width / (PTM_RATIO * 2),
			(visibleSize.height*0.45f) / PTM_RATIO), 0);
	playerBody->SetActive(true);
}

void GameScene::gameLoadFinishHandle(Ref *pSender)
{
	gameState = GAME_PREPARED;

	this->loadFirstBallAndLand();

}

void GameScene::createLandTimer(float dt)
{
	curLayerIndex++;
	int i = RandomHelper::random_int(1, 4);
	auto land = landsFile->popLand();
	land->setLayerIndex(curLayerIndex);
	this->addChild(land, landZOder);

	auto body = land->getBody();
	body->SetTransform(b2Vec2(
		(visibleSize.width * (RandomHelper::random_int(1, 7))) / (8 * PTM_RATIO),
		-land->getContentSize().height*0.5f / PTM_RATIO), 0);
	body->SetLinearVelocity(b2Vec2(0, ySpeed / PTM_RATIO));
	body->SetActive(true);
	listOfLand.push_back(land);
}

void GameScene::levelIncrement()
{
	for (auto &iter : listOfLand)
	{
		auto land = (Land*)iter;
		land->getBody()->SetLinearVelocity(b2Vec2(0, ySpeed / PTM_RATIO));
	}
}