#ifndef __MAINMENU_SCENE_H__
#define __MAINMENU_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

class MainMenuScene : public cocos2d::Layer
{
public:
	MainMenuScene();

	static cocos2d::Scene* createScene();

	virtual bool init();

	void initButton();

	void onButtonClick(Ref * sender, Widget::TouchEventType type);

	void changeMusicState();

	void changeEffectState();

	void createPlayerStyle();

	void nextPlayerTheme(Ref * sender, Widget::TouchEventType type);

	void getActiveThemes();

	void prePlayerTheme(Ref * sender, Widget::TouchEventType type);


	std::string nameOfPlayer();

	void gotoGameScene();

	void onPlayButtonClick();

	void gotoAppPage();

	void onShareButtonClick();

	// implement the "static create()" method manually
	CREATE_FUNC(MainMenuScene);

private:
	Button *playButton;
	Button *musicButton;
	Button *rateButton;
	Button *rankButton;
	Button *shareButton;
	Button *effectButton;

	Sprite *player;

	int currentTheme;
	std::vector<int> activeThemes;
	std::string listTheme;

	bool isMusicOn;
	bool isEffectOn;
	bool isMusicChange;
	bool isEffectChange;
};

#endif // __MAINMENU_SCENE_H__
