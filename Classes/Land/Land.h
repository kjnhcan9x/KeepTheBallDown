#ifndef __LAND_H__
#define __LAND_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "Definitions.h"

USING_NS_CC;

class Land : public Sprite
{

public:
	Land();
	~Land();

	bool init(b2World * _world, int themeNumber, TypeOfLand _type);
	std::string nameOfLand();
	void CreatePhysicsBody();
	void createPhysicWithNormal();
	void createPhysicWithHadthorn();
	void moveTrap();
	void moveToPoint2(const b2Vec2 & from, const b2Vec2 & to, float interval);
	static Land *create(b2World *world, int themeNumber, TypeOfLand _type);

	b2Body *getBody() { return body; }
	void moveToPoint(const b2Vec2 & from, const b2Vec2 & to, float interval);

	void changeToBlackLand();

	void changeToNormalLand();

	CC_SYNTHESIZE(float, m_vSpeed, VSpeed);

	CC_SYNTHESIZE(int, m_layerIndex, LayerIndex);

	CC_SYNTHESIZE(bool, m_colliend, Colliend);
private:
	b2Body	*body;
	b2Body  *body2;

	void moveTimer(float dt);
	void moveTrapTimer(float dt);
	bool moveState;



	b2World *world;
	float	xSpeedDelta;
	float	ySpeedDelta;
	TypeOfLand typeOfLand;
	Sprite *trap;

	int themeNumber;
	int times;
};

#endif // __LAND_H__
