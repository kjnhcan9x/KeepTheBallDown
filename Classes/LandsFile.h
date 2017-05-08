#ifndef __LANDS_FILE_H__
#define __LANDS_FILE_H__

#include "cocos2d.h"
#include "LandsFile.h"
#include "Box2D/Box2D.h"
#include "Land/Land.h"

USING_NS_CC;

struct LandObject
{
	Land *land;
	bool isActive;
};

class LandsFile 
{
public:

	LandsFile();
	~LandsFile();
	static LandsFile *getInstance();

	void initLand(b2World * _world, int themNumber_);


	LandObject * createNewLand(TypeOfLand typeOfLand);

	Land * popLand();
private:
	static LandsFile *singleton;
	b2World *world;
	int landNumber;
	int themeNumber;
};

#endif // __LANDS_FILE_H__
