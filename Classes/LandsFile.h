#ifndef __LANDS_FILE_H__
#define __LANDS_FILE_H__

#include "cocos2d.h"
#include "LandsFile.h"
#include "Box2D/Box2D.h"
#include "Land.h"

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

	void initLand(int numLand, b2World * _world);

	LandObject * createNewLand(TypeOfLand typeOfLand);

	Land * popLand();

	//void resetListOfLand(Land * land);

	void pushOneLand(Land * land);
	std::vector<LandObject*> listOfLand;

private:
	static LandsFile *singleton;
	b2World *world;
};

#endif // __LANDS_FILE_H__
