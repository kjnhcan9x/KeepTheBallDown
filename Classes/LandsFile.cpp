#include "LandsFile.h"

LandsFile *LandsFile::singleton = nullptr;

LandsFile::LandsFile()
	:world(nullptr),
	landNumber(0)
{
}

LandsFile::~LandsFile()
{
}

LandsFile * LandsFile::getInstance()
{
	if (singleton == nullptr)
	{
		singleton = new LandsFile();
	}
	return singleton;
}

void LandsFile::initLand(b2World *_world,int themeNumber_)
{
	landNumber = 1;
	themeNumber = themeNumber_;
	if (_world == nullptr)
	{
		return;
	}
	world = _world;
}

LandObject *LandsFile::createNewLand(TypeOfLand typeOfLand)
{
	b2Body *body = nullptr;
	LandObject *landObject = new LandObject;
	landObject->land = Land::create(world, themeNumber, typeOfLand);
	landObject->land->setLayerIndex(0);
	landObject->land->setColliend(false);
	landObject->land->retain();
	landObject->isActive = false;
	body = landObject->land->getBody();
	body->SetActive(false);

	return landObject;
}

Land *LandsFile::popLand()
{
	landNumber++;

	Land *land;
	int i = RandomHelper::random_int(1, 10);
	if (i != 1 || landNumber <= 10)
		land = this->createNewLand(NORMAL)->land;
	else
		land = this->createNewLand(HADTHORN)->land;

	return land;
}