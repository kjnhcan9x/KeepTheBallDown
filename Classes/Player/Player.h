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

	static Player * create(b2World * world, int themeNumber);

	bool init(b2World * world, int themeNumber);
	std::string nameOfPlayerType(int numOfType);

	b2Body *getBody() { return body; }
	void setMoveState(MoveState state) { moveState = state; }
	void breakAnimation();
protected:
	b2Body	*body;

	MoveState moveState;
	Animation *playerBlink;

	virtual void CreatePhysicsBody(b2World *world);
	void createPlayerAnimation();
	void blinkTimer(float dt);
	void moveTimer(float dt);

	b2World *world;
};

class PlayerSkin :public Player
{
public :
	static PlayerSkin * create(b2World * world, int themeNumber);

	bool init(b2World * world, int themeNumber);
	void CreatePhysicsBody(b2World * world);
};

#endif // __PLAYER_H__
