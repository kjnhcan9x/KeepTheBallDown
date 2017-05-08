#include "Land.h"

Land::Land()
{
}

Land::~Land()
{
}

Land *Land::create(b2World *_world, int themeNumber, TypeOfLand _type)
{
	auto bRet = new Land();
	if (bRet && bRet->init(_world, themeNumber, _type))
	{
		bRet->autorelease();
		return bRet;
	}
	CC_SAFE_DELETE(bRet);
	return nullptr;

}

bool Land::init(b2World *_world, int themeNumber_, TypeOfLand type_)
{
	themeNumber = themeNumber_;
	typeOfLand = type_;

	if (!Sprite::initWithFile(this->nameOfLand()))
	{
		return false;
	}

	this->world = _world;

	this->setPosition(visibleSize.width*0.5f, -this->getContentSize().height);
	//this->setOpacity(100);
	this->setTag(landTag);
	//this->CreatePhysicsBody();
	this->createPhysicWithNormal();
	if (2 == typeOfLand)
	{
		trap = Sprite::create("trap.png");
		//trap->setScale(2.0f);
		trap->setPosition(0, this->getContentSize().height*1.3f);
		this->addChild(trap);

		this->createPhysicWithHadthorn();

		auto moveBy = MoveBy::create(1.0f, Vec2(this->getContentSize().width, 0));

		//auto rotateAni = RotateBy::create(1.0f, 180);

		trap->runAction(RepeatForever::create(Sequence::create(moveBy, moveBy->reverse(), nullptr)));

		//trap2->runAction(RepeatForever::create(rotateAni->reverse()));
	}

	return true;
}

std::string Land::nameOfLand()
{
	auto fullName = StringUtils::format("land_%i.png", themeNumber);
	return fullName;
}

void Land::CreatePhysicsBody()
{
	switch (typeOfLand)
	{
	case NORMAL:
		this->createPhysicWithNormal();
		break;
	case HADTHORN:
		this->createPhysicWithHadthorn();
		break;
	default:
		break;
	}
}

void Land::createPhysicWithNormal()
{
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

void Land::createPhysicWithHadthorn()
{
	moveState = true;
	times = 0;
	//if (2 == typeOfLand)
	{
		/*b2PolygonShape shape;
		shape.SetAsBox(
			(this->getContentSize().width - 14)*0.5f / PTM_RATIO,
			((this->getContentSize().height - 37)*0.5f / PTM_RATIO));*/
		b2CircleShape shape;
		shape.m_radius = trap->getContentSize().width*0.5f / PTM_RATIO;

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		fixtureDef.friction = 1.0f;
		fixtureDef.isSensor = true;
		fixtureDef.filter.categoryBits = DEAD_LAND;
		fixtureDef.filter.maskBits = PLAYER | DEAD_LINE;

		b2BodyDef bodyDef;
		bodyDef.type = b2_kinematicBody;
		bodyDef.position.Set(
			body->GetPosition().x,
			body->GetPosition().y);

		body2 = world->CreateBody(&bodyDef);
		body2->CreateFixture(&fixtureDef);
		//body2->SetUserData(trap);
		//this->moveTrap();
		this->schedule(schedule_selector(Land::moveTrapTimer));
	}
}

void Land::moveTrap()
{
	this->runAction(Sequence::create(
		CallFunc::create([&]() {this->moveToPoint2(
			b2Vec2((this->getPosition().x - this->getContentSize().width / 2) / PTM_RATIO, this->getContentSize().height*1.2f / PTM_RATIO),
			b2Vec2((this->getPosition().x + this->getContentSize().width / 2) / PTM_RATIO, this->getContentSize().height*1.2f / PTM_RATIO),
			0.5f); }),
		CallFunc::create([&]() {this->moveToPoint2(
			b2Vec2((this->getPosition().x + this->getContentSize().width / 2) / PTM_RATIO, this->getContentSize().height*1.2f / PTM_RATIO),
			b2Vec2((this->getPosition().x - this->getContentSize().width / 2) / PTM_RATIO, this->getContentSize().height*1.2f / PTM_RATIO),
			0.5f); }), nullptr));
	/*


		if (true == moveState)
		{
			this->moveToPoint2(
				b2Vec2((this->getPosition().x - this->getContentSize().width / 2) / PTM_RATIO, this->getContentSize().height*1.2f / PTM_RATIO),
				b2Vec2((this->getPosition().x + this->getContentSize().width / 2) / PTM_RATIO, this->getContentSize().height*1.2f / PTM_RATIO),
				0.5f);
		}
		else
		{
			this->moveToPoint2(
				b2Vec2((this->getPosition().x + this->getContentSize().width / 2) / PTM_RATIO, this->getContentSize().height*1.2f / PTM_RATIO),
				b2Vec2((this->getPosition().x - this->getContentSize().width / 2) / PTM_RATIO, this->getContentSize().height*1.2f / PTM_RATIO),
				0.5f);
		}
	*/

}

void Land::moveToPoint2(const b2Vec2 &from, const b2Vec2 &to, float interval)
{
	body2->SetTransform(from, body2->GetAngle());
	times = interval * 60;
	xSpeedDelta = (to.x - from.x) / times;
	ySpeedDelta = (to.y - from.y) / times;

	this->schedule(schedule_selector(Land::moveTrapTimer));
}

void Land::moveToPoint(const b2Vec2 &from, const b2Vec2 &to, float interval)
{
	body->SetTransform(from, body->GetAngle());
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

void Land::moveTrapTimer(float dt)
{
	Vec2 trapPosition = this->convertToWorldSpace(trap->getPosition());
	Vec2 trapBodyPosition = Vec2(trapPosition.x / PTM_RATIO, trapPosition.y / PTM_RATIO);
	body2->SetTransform(b2Vec2(trapBodyPosition.x, trapBodyPosition.y), body2->GetAngle());
}

void Land::changeToBlackLand()
{
	auto fullName = StringUtils::format("land_%i_%i_black.png", themeNumber, typeOfLand);

	this->setTexture(fullName);
}

void Land::changeToNormalLand()
{
	//auto fullName = StringUtils::format("land_%i_%i.png", themeNumber, typeOfLand);
	auto fullName = this->nameOfLand();

	this->setTexture(fullName);
}