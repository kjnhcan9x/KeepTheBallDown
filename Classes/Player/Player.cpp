#include "Player.h"

Player::Player()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("player.plist", "player.png");
}

Player::~Player()
{
}

Player *Player::create(b2World *world, int themeNumber)
{
	auto bRet = new Player();
	if (bRet && bRet->init(world, themeNumber))
	{
		bRet->autorelease();
		return bRet;
	}
	CC_SAFE_DELETE(bRet);
	return nullptr;

}

bool Player::init(b2World *world, int themeNumber)
{
	//if (!Sprite::initWithSpriteFrameName("player_1.png"))
	if (!Sprite::initWithFile(this->nameOfPlayerType(themeNumber)))
	{
		return false;
	}

	//this->setOpacity(0);
	this->setTag(playerTag);
	this->CreatePhysicsBody(world);
	//this->createPlayerAnimation();
	this->schedule(schedule_selector(Player::moveTimer));



	return true;
}

std::string Player::nameOfPlayerType(int numOfType)
{
	auto fullName = StringUtils::format("player_%i.png", numOfType);
	return fullName;
}

void Player::CreatePhysicsBody(b2World *world)
{
	b2CircleShape circleShape;
	circleShape.m_radius = this->getContentSize().width*0.5f / PTM_RATIO;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.density = 0.06f;
	fixtureDef.restitution = 0.2f;
	fixtureDef.friction = 1.0f;
	fixtureDef.filter.categoryBits = PLAYER;
	fixtureDef.filter.maskBits = LAND | SCREEN_BORDER | DEAD_LINE | CEILLING | PLAYER_GIFT | DEAD_LAND;

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	//bodyDef.fixedRotation = true;
	bodyDef.position.Set(visibleSize.width*0.5f / PTM_RATIO, -this->getContentSize().height / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
	body->SetUserData(this);

	moveState = STOP;
}

void Player::createPlayerAnimation()
{
	Vector<SpriteFrame*> playerFrames;
	for (int frameNumber = 0; frameNumber < 2; frameNumber++)
	{
		//auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(StringUtils::format("player_%i.png", frameNumber + 1));		
		//playerFrames.pushBack(frame);
	}
	playerFrames.pushBack(SpriteFrame::create("ball1.png", Rect(0, 0, 55, 55)));
	playerFrames.pushBack(SpriteFrame::create("ball2.png", Rect(0, 0, 55, 55)));

	playerBlink = Animation::createWithSpriteFrames(playerFrames, 0.2f);
	playerBlink->setRestoreOriginalFrame(true);
	playerBlink->retain();

	this->schedule(schedule_selector(Player::blinkTimer), 5.0f);
}

void Player::blinkTimer(float dt)
{
	this->runAction(Animate::create(playerBlink));
}

void Player::breakAnimation()
{
	auto emitter = ParticleSystemQuad::create("particle_texture.plist");
	emitter->setPosition(this->getContentSize()*0.5);
	this->addChild(emitter, 10);
}

void Player::moveTimer(float dt)
{
	b2Vec2 velocity = body->GetLinearVelocity();
	float desiredVelocity = 0;
	switch (moveState)
	{
	case LEFT:
		desiredVelocity = b2Max(velocity.x - 1.5f, -20.0f);
		break;
	case RIGHT:
		desiredVelocity = b2Min(velocity.x + 1.5f, 20.0f);
		break;
	case STOP:
		desiredVelocity = velocity.x*0.98f;
		break;
	default:
		break;
	}
	float velocityChange = desiredVelocity - velocity.x;
	float impules = body->GetMass()*velocityChange;
	body->ApplyLinearImpulse(b2Vec2(impules, 0), body->GetWorldCenter(), true);

}

PlayerSkin * PlayerSkin::create(b2World * world, int themeNumber)
{
	auto bRet = new PlayerSkin();
	if (bRet && bRet->init(world, themeNumber))
	{
		bRet->autorelease();
		return bRet;
	}
	CC_SAFE_DELETE(bRet);
	return nullptr;
}

bool PlayerSkin::init(b2World * world, int themeNumber)
{
	if (!Sprite::initWithFile(this->nameOfPlayerType(themeNumber)))
	{
		return false;
	}

	//this->setOpacity(0);
	this->setTag(playerTag);
	this->CreatePhysicsBody(world);
	//this->createPlayerAnimation();
	//this->schedule(schedule_selector(Player::moveTimer));

	return true;
}

void PlayerSkin::CreatePhysicsBody(b2World *world)
{
	b2CircleShape circleShape;
	circleShape.m_radius = this->getContentSize().width*0.5f / PTM_RATIO;

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &circleShape;
	fixtureDef.friction = 1.0f;
	fixtureDef.filter.categoryBits = PLAYER_GIFT;
	fixtureDef.filter.maskBits = PLAYER;


	b2BodyDef bodyDef;
	bodyDef.type = b2_kinematicBody;

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
	body->SetUserData(this);
}