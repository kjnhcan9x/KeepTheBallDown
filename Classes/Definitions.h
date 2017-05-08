#ifndef _DEFINITIONS_H_
#define _DEFINITIONS_H_

#include "cocos2d.h"
USING_NS_CC;

#define gEventDispatcher Director::getInstance()->getEventDispatcher()

#define BACKGROUND_MUSIC_LINK		"Music/bgmusic.mp3"
#define EFFECT_PLAYER_LAND			"Music/jump.wav"
#define EFFECT_PLAYER_DIE			"Music/pop.wav"

#define CMD_GAME_START				"game_start"		
#define CMD_GAME_OVER				"game_over"
#define CMD_GAME_LOAD_FINISH		"game_load_finish"
#define CMD_STOP_ALL_LAND			"stop_all_land"
#define CMD_RESUME_ALL_LAND			"resume_all_land"
#define CMD_DELETE_ALL_LAND			"delete_all_land"
#define gameSceneTag 1011

#define visibleSize			Director::getInstance()->getVisibleSize()
#define origin				Director::getInstance()->getVisibleOrigin()
#define topMid		Vec2(visibleSize.width * 0.5+origin.x, visibleSize.height+origin.y)
#define topLeft		Vec2(origin.x,visibleSize.height+origin.y)


#define startButtonPos		Vec2(332, 517)
#define musicButtonPos		Vec2(77, 95)
#define rankButtonPos		Vec2(77, 265)
#define logoPos				Vec2(640*0.5f, 1136*0.75f)
#define scoreTextPos		Vec2(561, 1078)

#define IS_MUSICON			"is-music-on"
#define IS_EFFECTON			"is-effect-on"
#define	IS_FIRSTGAME		"is-first-game"
#define PLAYER_TYPE			"player-type"
#define CURRENT_THEME		"current-theme"
#define ACTIVE_THEME		"active-theme"
#define SCORE				"score"
#define BEST_SCORE			"best-score"
#define SPEED				"speed"
#define COUNT_TO_SHOW_ADS	"count-to-show-ads"

#define TOTAL_SKIN		31
#define TRANSITION_TIME 0.5
#define PTM_RATIO		32
#define ANIM_TIME		1.0
#define g_borderSize	50.0
#define scorePerLayer	2
#define speedIncrement	20.0
#define TimeLandSpawn	1.5
#define LandVelocity	150
#define MaxVelocity		350

static const TTFConfig superbigFont("fonts/Quicksand_Bold.otf", 128);
static const TTFConfig bigFont("fonts/Quicksand_Bold.otf", 64);
static const TTFConfig normalFont("fonts/Quicksand_Bold.otf", 40);
static const TTFConfig smallFont("fonts/Quicksand_Bold.otf", 24);

#define BACKGROUND_COLOR	Color4B(16,57,109,255)


enum EntityTag
{
	landTag = 10,
	playerTag = 11,
	ceilingTag = 12,
	screenBorderTag = 13,
	deadlineTag = 14,
};

enum CategoryMask
{
	SCREEN_BORDER = 0x0001,
	PLAYER = 0x0002,
	DEAD_LINE = 0x0004,
	CEILLING = 0x0008,
	LAND = 0x0010,
	DEAD_LAND = 0x0020,
	PLAYER_GIFT = 0x0040
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
	NORMAL = 1,
	HADTHORN = 2
};

struct ThemeOfGame
{
	int themeNumber;
	bool isActive;
};

static const Color4B backgroundColor[] = {
Color4B(249,249,249,255),	//binh thuong
Color4B(0,0,0,255),			//neon
Color4B(147,101,184,255),	//bat quai
Color4B(239,153,144,255),	//humberger
Color4B(236,240,241,255),	//cuc ao
Color4B(159,231,216,255),	//dua hau
Color4B(149,165,165,255),	//su tu
Color4B(239,184,119,255),	//con ga
Color4B(152,230,150,255),	//tenis
Color4B(200,113,55,255),	//bowling
Color4B(144,200,224,255),		//thuoc
Color4B(233,175,198,255),	//keo
Color4B(45,204,112,255),	//bong da
Color4B(40,49,78,255),		//bong den
Color4B(225,171,122,255),		//bong chay
Color4B(147,101,184,255),	//bien bao
Color4B(149,165,165,255),	//oc vit
Color4B(255,213,157,255),	//bong ro
Color4B(155,162,183,255),		//banh xe
Color4B(189,227,220,255),	//loading
Color4B(0,168,133,255),		//con cu
Color4B(200,183,196,255),	//kiwi
Color4B(84,172,210,255),	//minion
Color4B(200,190,183,255),	//pig
Color4B(233,185,198,255),	//pizza
Color4B(45,204,112,255),	//cam
Color4B(233,175,221,255),	//mat
Color4B(71,85,119,255),		//mat trang
Color4B(158,213,237,255),	//hoa
Color4B(26,188,156,255),	//bida
Color4B(144,200,224,255),	//khien 
};

enum ButtonTag
{
	playButtonTag,
	rateButtonTag,
	musicButtonTag,
	effectButtonTag,
	rankButtonTag,
	shareButtonTag,
	mainmenuButtonTag,
};
#endif //_DEFINITIONS_H_