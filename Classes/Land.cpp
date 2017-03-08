#include "Land.h"

Land::Land()
{
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("player.plist", "player.png");
}

Land::~Land()
{
}

Land *Land::create(b2World *_world, TypeOfLand _type)
{
	auto bRet = new Land;
	if (bRet && bRet->init(_world, _type))
	{
		bRet->autorelease();
		return bRet;
	}
	CC_SAFE_DELETE(bRet);
	return nullptr;

}

bool Land::init(b2World *_world, TypeOfLand _type)
{
	typeOfLand = _type;
	if (typeOfLand == NORMAL)
	{
		//if (!Sprite::initWithFile("land1.png"))
		if (!Sprite::initWithFile("normal-land.png"))
		{
			return false;
		}
	}
	else if (typeOfLand == HADTHORN)
	{
		if (!Sprite::initWithFile("dead-land.png"))
		//if (!Sprite::initWithFile("land2.png"))
		{
			return false;
		}
	}
	this->world = _world;

	//this->setOpacity(0);
	this->setTag(landTag);
	this->CreatePhysicsBody();

	return true;
}

void Land::CreatePhysicsBody()
{
	//Size visibleSize = Director::getInstance()->getVisibleSize();

	b2PolygonShape shape;
	shape.SetAsBox(
		this->getContentSize().width*0.5f / PTM_RATIO,
		(this->getContentSize().height - 14) / (PTM_RATIO * 2));

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shape;
	fixtureDef.friction = 1.0f;
	fixtureDef.filter.categoryBits = LAND;
	fixtureDef.filter.maskBits = PLAYER | DEAD_LINE;
	//fixtureDef.isSensor = true;

	b2BodyDef bodyDef;
	bodyDef.type = b2_kinematicBody;
	bodyDef.position.Set(
		visibleSize.width*0.5f / PTM_RATIO, 
		-this->getContentSize().height *0.5f / PTM_RATIO);

	body = world->CreateBody(&bodyDef);
	body->CreateFixture(&fixtureDef);
	body->SetUserData(this);
}

void Land::moveToPoint(const b2Vec2 &from, const b2Vec2 &to, float interval)
{
	body->SetTransform(from, body->GetAngle());
	startPoint = from;
	endPoint = to;
	times = interval * 60;
	xSpeedDelta = (to.x - from.x) / times;
	ySpeedDelta = (to.y - from.y) / times;

	this->schedule(schedule_selector(Land::moveTimer));
	//this->schedule(SEL_SCHEDULE(&Land::moveTimer));
}

void Land::moveTimer(float dt)
{
	if (0 != times)
	{
		times--;
		b2Vec2 velocity = body->GetPosition();
		velocity.Set(velocity.x + xSpeedDelta, velocity.y + ySpeedDelta);
		body->SetTransform(velocity, body->GetAngle());
	}
	else
	{
		this->unschedule(SEL_SCHEDULE(&Land::moveTimer));
	}
}

void Land::changeToBlackLand()
{
	switch (typeOfLand)
	{
	case NORMAL:
		this->setTexture("normal-land-black.png");
		break;
	case HADTHORN:
		this->setTexture("dead-land-black.png");
		break;
	default:
		break;
	}
}

void Land::changeToNormalLand()
{
	switch (typeOfLand)
	{
	case NORMAL:
		this->setTexture("normal-land.png");
		break;
	case HADTHORN:
		this->setTexture("dead-land.png");
		break;
	default:
		break;
	}
}