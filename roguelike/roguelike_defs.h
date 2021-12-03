#ifndef _ROGUELIKE_DEFS_
#define _ROGUELIKE_DEFS_

#include "roguelike.h"

//------------------------------------------------------ roguelike_util -----

#define _FI	FOREGROUND_INTENSITY
#define _FR	FOREGROUND_RED
#define _FG	FOREGROUND_GREEN
#define _FB	FOREGROUND_BLUE
#define _FY	( _FR | _FG )		// Foreground Yellow
#define _FM	( _FR | _FB )		// Foreground Magenta
#define _FC	( _FG | _FB )		// Foreground Cyan
#define _FW	( _FR | _FG | _FB )	// Foreground White

#define _BI	BACKGROUND_INTENSITY
#define _BR	BACKGROUND_RED
#define _BG	BACKGROUND_GREEN
#define _BB	BACKGROUND_BLUE
#define _BY	( _BR | _BG )		// Background Yellow
#define _BM	( _BR | _BB )		// Background Magenta
#define _BC	( _BG | _BB )		// Background Cyan
#define _BW	( _BR | _BG | _BB )	// Background White

// same as SMALL_RECT struct
typedef struct _SRECT {
	SHORT		L;				// Left
	SHORT		T;				// Top
	SHORT		R;				// Right
	SHORT		B;				// Bottom
} SRECT, * PSRECT, ** PPSRECT;

// same as CHAR_INFO struct
typedef struct _TILE {
	WCHAR		icon;			// icon
	WORD		attr;			// icon's color
} TILE, * PTILe, ** PPTILE;

typedef struct _TREE_NODE {
	PVOID pVal;					// pointer to value
	struct _TREE_NODE* pP;		// pointer to parent node
	struct _TREE_NODE* pL;		// pointer to left node
	struct _TREE_NODE* pR;		// pointer to right node
} TNODE, * PTNODE, ** PPTNODE;

typedef struct _LIST_NODE {
	PVOID pVal;					// pointer to value
	struct _LIST_NODE* next;	// pointer to next node
	struct _LIST_NODE* prev;	// pointer to previous node
} LNODE, * PLNODE, ** PPLNODE;

typedef struct _LIST {
	USHORT len;
	PLNODE head;
	PLNODE tail;
} LIST, * PLIST, ** PPLIST;

//------------------------------------------------------ roguelike_item -----

#define	ITEM_GEN_CHANCE	3		// 1 in 'n' chance of item generation

typedef enum _ITEM_TYPE {
	_WEAPON,
	_ARMOUR,
	_POTION,
	_SCROLL,
} ITYPE, * PITYPE, ** PPITYPE;
#define	NUM_ITYPE		4		// number of item type(s)

typedef struct _ITEM {
	COORD	pos;				// position
	ITYPE	type;				// type
	CHAR	name[16];			// name
} ITEM, * PITEM, ** PPITEM;

//------------------------------------------------------- roguelike_ent -----

#define ICON_PLAYER		L'Ⓟ'	// U+24C5
#define ICON_ENEMY		L'Ⓔ'	// U+24BA

typedef enum _COLLISION_TYPE {
	_NIL,						// no collision
	_ITM,						// collision with item
	_ENT,						// collision with entity
	_ENV,						// collision with environment
} CTYPE, * PCTYPE, ** PPCTYPE;
#define	NUM_CTYPE		4		// number of collision type(s)

typedef enum _STATS {
	_STR,						// STR-ength
	_DEX,						// DEX-terity
	_CON,						// CON-stitution
	_INT,						// INT-elligence
	_WIS,						// WIS-dom
	_CHA,						// CHA-risma
} STATS, * PSTATS, ** PPSTATS;
#define	NUM_STATS		6		// number of stat(s)

typedef struct _ENTITY {
	COORD	pos;				// position
	TILE	rep;				// graphic representation

	USHORT	hp;					// hitpoint

	USHORT	stats[NUM_STATS];	// stats/ability scores
	PITEM	items[10];			// array of items
} ENT, * PENT, ** PPENT;

//------------------------------------------------------- roguelike_env -----

#define ENV_MIN_ITER	4		// MUST NOT BE LOWER THAN 4, affects _designateEnvIO() function
#define	ENV_MIN_LEN		5 + 1	// MUST NOT BE LOWER THAN 6, affects _createRoomData() function

#define ICON_ENV_I		L'▲'	// U+25B2
#define ICON_ENV_O		L'▼'	// U+25BC

#define ICON_VOID		L'　'	// U+3000
#define ICON_WALL		L'■'	// U+25A0
#define ICON_FLOOR		L'·'	// U+00B7
#define ICON_HALLWAY	L'□'	// U+25A1
#define ICON_CHEST		L'▣'	// U+25A3

typedef struct _ENVIRONMENT {
	UINT	seed;				// seed for proc-gen
	COORD	dim;				// dimension of environment
	WORD	attr;				// color attributes

	PTNODE	tRoom;				// binary tree for storing room data
	LIST	lRoom;				// linked list for storing room data
	LIST	lItem;				// linked list for storing item data
	LIST	lEnti;				// linked list for storing entity data

	COORD	posI;				// coordinate of entrance
	COORD	posO;				// coordinate of exit

	PWCHAR	gBuf;				// buffer for graphics
	PUCHAR	cBuf;				// buffer for collision detection
	PUCHAR	fBuf;				// buffer for fog-of-war
} ENV, * PENV, ** PPENV;

//-------------------------------------------------------- roguelike_ui -----

#define	GUI_ENV_W		27
#define	GUI_ENT_W		9
#define GUI_W			( 1 + GUI_ENV_W + 1 + GUI_ENT_W + 1)
#define GUI_H			( 1 + GUI_ENV_W + 1 )

#define INPUT_MOVE_U	L'w'
#define INPUT_MOVE_L	L'a'
#define INPUT_MOVE_D	L's'
#define INPUT_MOVE_R	L'd'

typedef struct _GUI {
	COORD	dim;				// dimension of GUI
	SRECT	rEnv;				// pos/dim of ENV part of GUI
	SRECT	rEnt;				// pos/dim of ENT part of GUI

	WORD	attr;
	PWCHAR	gBuf;				// buffer for graphics
} GUI, * PGUI, ** PPGUI;

//---------------------------------------------------------------------------

typedef struct _ROGUELIKE {
	LIST	lEnv;				// list of ENV struct
	SHORT	curIdx;				// current ENV struct's index in list
	PENV	curEnv;				// current ENV struct

	PENT	pPC;
	PGUI	pGUI;
} RL, * PRL, ** PPRL;

//---------------------------------------------------------------------------

#endif
