#ifndef __GAMEOVER_LAYER_H__
#define __GAMEOVER_LAYER_H__

#include "cocos2d.h"
#include "Definitions.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

class GameOverLayer : public cocos2d::Layer
{
public:
	GameOverLayer();
	~GameOverLayer();

	void onEnter();
	virtual bool init();

	void reviveTheBall(Ref * sender, Widget::TouchEventType type);

	void gotoGameScene(Ref * sender, Widget::TouchEventType type);

   
    // implement the "static create()" method manually
    CREATE_FUNC(GameOverLayer);

private:
	Sprite *scoreBoard;
};

#endif // __GAMEOVER_LAYER_H__
