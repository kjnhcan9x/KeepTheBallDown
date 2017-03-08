#ifndef __MENU_LAYER_H__
#define __MENU_LAYER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "Definitions.h"
#include "cocostudio/CocoStudio.h"


using namespace CocosDenshion;
using namespace ui;
using namespace cocostudio;
USING_NS_CC;

class MenuLayer : public cocos2d::Layer
{
public:
	MenuLayer();
	~MenuLayer();

	void onEnter();

    virtual bool init();

	void updateScoreText(int score);
    
	void onButtonClick(Ref *sender, ui::Widget::TouchEventType type);
	
	// a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MenuLayer);

private:
	bool isMusicOn;
	Button *startButton;
	Button *musicButton;
	Button *rankButton;
	Sprite *logo;
	Sprite *scoreBackground;
	Label  *scoreText;


	Vec2 startButtonOutPos;
	Vec2 musicButtonOutPos;
	Vec2 rankButtonOutPos;
	Vec2 logoOutPos;

	void ChangeMusicState();
	void MoveUIIntoScreen();
	void MoveUIOutScreen();
	void CheckIsFirstGame();
	void ShowRankLayer();
	void gameOverHandle(Ref * sender);
	void enableButton();
};

#endif // __MENU_LAYER_H__
