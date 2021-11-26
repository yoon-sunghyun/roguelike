#ifndef _ROGUELIKE_ENT_
#define _ROGUELIKE_ENT_

#include "roguelike.h"

PENT	_makeEnt(COORD, WCHAR, WORD);
CTYPE	_moveEnt(PENV, PENT, SHORT, SHORT, PVOID*);
void	_showEnt(HANDLE, PENT);
void	_freeEnt(PENT);

#endif
