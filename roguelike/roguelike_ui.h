#ifndef _ROGUELIKE_UI_
#define _ROGUELIKE_UI_

#include "roguelike.h"

void	_handlePC(PRL);

PGUI	_makeGUI();
void	_fillGUI(PRL);
void	_showGUI(HANDLE, PGUI);
void	_freeGUI(PGUI);

#endif
