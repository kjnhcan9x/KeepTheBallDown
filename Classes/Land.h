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

	virtual bool init(b2World *world, TypeOfLand _type);
	static Land *create(b2World *world, TypeOfLand _type);

	b2Body *getBody() { return body; }
	void moveToPoint(const b2Vec2 & from, const b2Vec2 & to, float interval);

	void changeToBlackLand();

	void changeToNormalLand();

	CC_SYNTHESIZE(float, m_vSpeed, VSpeed);

	CC_SYNTHESIZE(int, m_layerIndex, LayerIndex);

	CC_SYNTHESIZE(bool, m_colliend, Colliend);
private:
	b2Body	*body;

	void CreatePhysicsBody();
	void moveTimer(float dt);



	b2World *world;
	b2Vec2	startPoint;
	b2Vec2	endPoint;
	float	xSpeedDelta;
	float	ySpeedDelta;
	TypeOfLand typeOfLand;

	int times;
};

#endif // __LAND_H__
