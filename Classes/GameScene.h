#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "MenuLayer.h"
#include "Player.h"
#include "Box2D/Box2D.h"
#include "GLES-Render.h"
#include "Land.h"
#include "LandsFile.h"
#include "GameOverLayer.h"

USING_NS_CC;

class GameScene : public cocos2d::Layer,b2ContactListener
{
public:
	GameScene();
	~GameScene();

	void onEnter();

	static cocos2d::Scene* createScene();

	virtual bool init();

	void initPhysics();

	void BeginContact(b2Contact * contact);

	void draw(Renderer * renderer, const Mat4 & transform, uint32_t flags);

	void showGameOverLayer();

	void stopAllLand();

	void resumeAllLand(Ref * sender);

	void deleteAllLand();


	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	void update(float dt);

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);
private:
	bool onTouchBegan(Touch *touch, Event *unused_event);
	void onTouchEnded(Touch *touch, Event *unused_event);

	void onKeyPressed(EventKeyboard::KeyCode code, Event *event);
	void onKeyReleased(EventKeyboard::KeyCode code, Event *event);
	void setStartVariablesValue();
	void gameStartHandle(Ref * sender);
	void loadFirstBallAndLand();
	void gameLoadFinishHandle(Ref * pSender);
	void createLandTimer(float dt);
	void levelIncrement();
private:
	//Size visibleSize;

	GLESDebugDraw* _debugDraw;
	MenuLayer	*menuLayer;
	Player		*player;
	b2World		*world;
	Sprite		*ceiling;
	//Land		*land;
	LandsFile	*landsFile;

	std::vector<Land *> listOfLand;
	std::vector<Node *>	listOfDestroy;

	EventListenerKeyboard		*keyBoardListener;
	EventListenerTouchOneByOne	*touchEventListener;

	GameState gameState;
	int curLayerIndex;
	int preLayerIndex;
	int curScore;
	int curLevel;
	float ySpeed;
	float timerScale;
	bool isMusicOn;
	MotionStreak *tail;
	GameOverLayer *gameOverLayer;
	LayerColor *deadBackground;
};

#endif // __GAME_SCENE_H__
