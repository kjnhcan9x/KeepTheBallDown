#ifndef __HUDLAYER_H__
#define __HUDLAYER_H__

#include "cocos2d.h"
#include "Definitions.h"

USING_NS_CC;

class HUDLayer : public cocos2d::Sprite
{
public:
	HUDLayer();
	~HUDLayer();

	static HUDLayer *create();

    virtual bool init();

	void createHUDLabel();

	void updateHUD(int score, int bestScore, int speed);

private:
	Label	*scoreLabel;
	Label	*bestScoreLabel;
	Label	*speedLabel;
	int		score;
	int		bestScore;
	int		speed;
};

#endif // __HUDLAYER_H__
