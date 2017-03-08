#include "LandsFile.h"

LandsFile *LandsFile::singleton = nullptr;

LandsFile::LandsFile()
	:world(nullptr)
{
	listOfLand.clear();
}

LandsFile::~LandsFile()
{
}

LandsFile * LandsFile::getInstance()
{
	if (singleton==nullptr)
	{
		singleton = new LandsFile;
	}
	return singleton;
}

void LandsFile::initLand(int numLand, b2World *_world)
{
	listOfLand.clear();
	if (_world == nullptr)
	{
		return;
	}
	world = _world;
	for (int i = 0; i < numLand; i++)
	{
		LandObject *landObject = new LandObject;
		
		landObject->land = Land::create(world, NORMAL);
		landObject->land->getBody()->SetActive(false);
		landObject->land->setColliend(false);
		landObject->land->retain();
		landObject->isActive = false;

		listOfLand.push_back(landObject);
	}
}

LandObject *LandsFile::createNewLand(TypeOfLand typeOfLand)
{
	b2Body *body = nullptr;
	LandObject *landObject = new LandObject;
	landObject->land = Land::create(world, typeOfLand);
	landObject->land->setLayerIndex(0);
	landObject->land->setColliend(false);
	landObject->land->retain();
	landObject->isActive = false;
	body = landObject->land->getBody();
	body->SetTransform(b2Vec2(
		visibleSize.width*0.5f / PTM_RATIO,
		-landObject->land->getContentSize().height*0.5f / PTM_RATIO), 0);
	body->SetActive(false);
	if (typeOfLand== HADTHORN)
	{
		b2PolygonShape shape;
		shape.SetAsBox(
			(landObject->land->getContentSize().width - 14)*0.5f / PTM_RATIO,
			((landObject->land->getContentSize().height - 37)*0.5f / PTM_RATIO));
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &shape;
		fixtureDef.friction = 1.0f;
		fixtureDef.filter.categoryBits = DEAD_LAND;
		fixtureDef.filter.maskBits = PLAYER | DEAD_LINE;
		b2Body* body = landObject->land->getBody();
		body->DestroyFixture(body->GetFixtureList());
		body->CreateFixture(&fixtureDef);
		body->SetUserData(landObject->land);
	}

	//listOfLand.push_back(landObject);
	return landObject;
}

Land *LandsFile::popLand()
{
	for (auto &iter:listOfLand)
	{
		if (!iter->isActive)
		{
			iter->isActive = true;
			return iter->land;
		}
	}

	auto landObject = new	LandObject;
	landObject->isActive = false;
	landObject->land = Land::create(world,NORMAL);
	landObject->land->getBody()->SetActive(false);
	landObject->land->retain();

	listOfLand.push_back(landObject);
	return landObject->land;
}

void LandsFile::pushOneLand(Land *land)
{
	b2Body *body = nullptr;
	for (auto & iter : listOfLand)
	{
		if (iter->land == land)
		{
			body = iter->land->getBody();
			world->DestroyBody(body);
			int i = RandomHelper::random_int(1, 10);
			//CCLOG("RANDON: %i", i);
			if (i != 1)
				iter = this->createNewLand(NORMAL);
			else
				iter = this->createNewLand(HADTHORN);
			return;
		}
	}
}