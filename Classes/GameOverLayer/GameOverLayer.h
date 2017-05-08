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

	std::string nameOfPlayer();

	void createPlayerStyle();

	void nextPlayerTheme(Ref * sender, Widget::TouchEventType type);

	void prePlayerTheme(Ref * sender, Widget::TouchEventType type);

	void getActiveThemeList();

	void reviveTheBall(Ref * sender, Widget::TouchEventType type);

	void gotoGameScene();

	void onButtonClick(Ref * sender, Widget::TouchEventType type);

	void gotoMainMenuScene();

	void showScreenShot(bool succeed, const std::string & outputFile);

    // implement the "static create()" method manually
    CREATE_FUNC(GameOverLayer);

private:
	Sprite		*scoreBoard;
	Button		*playButton;
	std::string listTheme;
	std::vector<int> listActive;
	int currentTheme;
	Sprite		*player;
	bool isEffectOn;
};

#endif // __GAMEOVER_LAYER_H__
