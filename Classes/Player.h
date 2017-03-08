#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "Definitions.h"

USING_NS_CC;

class Player : public Sprite
{

public:
	Player();
	~Player();

	virtual bool init(b2World *world);
	static Player *create(b2World *world);

	b2Body *getBody() { return body; }
	void setMoveState(MoveState state) { moveState = state; }
private:
	b2Body	*body;

	MoveState moveState;
	Animation *playerBlink;
	
	void CreatePhysicsBody(b2World *world);
	void createPlayerAnimation();
	void blinkTimer(float dt);
	void moveTimer(float dt);

	b2World *world;
};

#endif // __PLAYER_H__
