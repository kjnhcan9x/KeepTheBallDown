#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#include "cocos2d.h"
USING_NS_CC;

#define CMD_GAME_START				"game_start"		
#define CMD_GAME_OVER				"game_over"
#define CMD_GAME_LOAD_FINISH		"game_load_finish"
#define CMD_STOP_ALL_LAND			"stop_all_land"
#define CMD_RESUME_ALL_LAND			"resume_all_land"
#define CMD_DELETE_ALL_LAND			"delete_all_land"

#define visibleSize			Director::getInstance()->getVisibleSize()
#define startButtonPos		Vec2(332, 517)
#define musicButtonPos		Vec2(77, 95)
#define rankButtonPos		Vec2(553, 98)
#define logoPos				Vec2(640*0.5f, 1136*0.75f)
#define scoreTextPos		Vec2(561, 1078)

#define IS_MUSICON			"is-music-on"
#define	IS_FIRSTGAME		"is-first-game"

#define PTM_RATIO		32
#define ANIM_TIME		1.0
const float g_borderSize = 50.0;
#define scorePerLayer	2
#define speedIncrement	20.0
#define TimeLandSpawn	1.5
#define LandVelocity	150
#define MaxVelocity		350

static const TTFConfig textScoreFont("fonts/Marker Felt.ttf", 30);

enum EntityTag
{
	landTag			= 10,	
	playerTag		= 11,	
	ceilingTag		= 12,	
	screenBorderTag = 13,
	deadlineTag		= 14,
};

enum CategoryMask
{
	SCREEN_BORDER	= 0x0001,
	PLAYER			= 0x0002,
	DEAD_LINE		= 0x0004,
	CEILLING		= 0x0008,
	LAND			= 0x0010,
	DEAD_LAND		= 0x0011
};

enum MoveState {
	STOP,
	LEFT,
	RIGHT,
};

typedef enum _GameState
{
	GAME_OVER = 1,
	GAME_PREPARED,
	GAME_START,
	GAME_EXIT,
}GameState;

enum TypeOfLand
{
	NORMAL,
	HADTHORN
};


#endif //_DEFINITIONS_H_