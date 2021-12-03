#include "roguelike_ui.h"

//--------------------------------------------------------------------------------
/*
	handles player character
	@param	pRoguelike	: pointer to ROGUELIKE struct
*/
void _handlePC(PRL pRL)
{
	COORD d		= { 0, 0 };
	PVOID pVal	= NULL;

	switch (_getwch())
	{
		case(INPUT_MOVE_U)	: { d.Y = -1; break; }
		case(INPUT_MOVE_L)	: { d.X = -1; break; }
		case(INPUT_MOVE_D)	: { d.Y = +1; break; }
		case(INPUT_MOVE_R)	: { d.X = +1; break; }
		case(27): { free(pRL->pPC); pRL->pPC = NULL; return; }
	}

	switch (_moveEnt(pRL->curEnv, pRL->pPC, d.X, d.Y, &pVal))
	{
		case(_NIL): { break; }	// ignored
		case(_ENV):
		{
			if (pVal == (PVOID)ICON_ENV_I && pRL->curIdx - 1 >= 0)
			{
				// collision with entrance detected
				PLNODE temp = _getListNodeI(&(pRL->lEnv), --(pRL->curIdx));
				if (temp != NULL)
					pRL->pPC->pos = (pRL->curEnv = temp->pVal)->posO;
			}
			else if (pVal == (PVOID)ICON_ENV_O && pRL->curIdx + 1 < pRL->lEnv.len)
			{
				// collision with exit detected
				PLNODE temp = _getListNodeI(&(pRL->lEnv), ++(pRL->curIdx));
				if (temp != NULL)
					pRL->pPC->pos = (pRL->curEnv = temp->pVal)->posI;
			}
			break;
		}
		case(_ENT): { break; }	// do something
		case(_ITM):
		{
			for (USHORT i = 0; i < 10; i++)
			{
				if (pRL->pPC->items[i] == NULL)
				{
					PENV	pE = pRL->curEnv;
					PITEM	pI = pVal;

					pRL->pPC->items[i] = pI;
					pE->cBuf[pE->dim.X * pI->pos.Y + pI->pos.X] = TRUE;
					_dropListNode(&(pRL->curEnv->lItem), pVal);
					break;
				}
			}
			break;
		}
	}
}
//--------------------------------------------------------------------------------
/*
	makes GUI struct
*/
PGUI _makeGUI()
{
	PGUI result = NULL;

	if ((result = (PGUI)calloc(1, sizeof(GUI))) != NULL)
	{
		result->dim.X	= GUI_W + 1;	// '+1' is for newline
		result->dim.Y	= GUI_H;

		result->rEnv.T	= 1;
		result->rEnv.L	= 1;
		result->rEnv.B	= result->rEnv.T + GUI_ENV_W;
		result->rEnv.R	= result->rEnv.L + GUI_ENV_W;

		result->rEnt.T	= 1;
		result->rEnt.L	= result->rEnv.R + 1;
		result->rEnt.B	= result->rEnt.T + GUI_ENV_W;
		result->rEnt.R	= result->rEnt.L + GUI_ENT_W;

		result->attr	= _FW;
		result->gBuf	= (PWCHAR)calloc(result->dim.X * result->dim.Y, sizeof(WCHAR));

		if (result->gBuf != NULL)
		{
			wmemset(result->gBuf, L'▩', result->dim.X * result->dim.Y);
			for (USHORT y = 0; y < result->dim.Y; y++)
				result->gBuf[result->dim.X * (y) + (result->dim.X - 1)] = L'\n';

			// clear area for ENV
			for (USHORT y = result->rEnv.T; y < result->rEnv.B; y++)
				for (USHORT x = result->rEnv.L; x < result->rEnv.R; x++)
					result->gBuf[result->dim.X * (y) + (x)] = ICON_VOID;
			// clear area for ENT
			for (USHORT y = result->rEnt.T; y < result->rEnt.B; y++)
				for (USHORT x = result->rEnt.L; x < result->rEnt.R; x++)
					result->gBuf[result->dim.X * (y) + (x)] = ICON_VOID;
		}
		else
		{
			_freeGUI(result);
			return NULL;
		}
	}
	return result;
}
//--------------------------------------------------------------------------------
/*
	fills GUI with ENV and ENT data
	@param	pRL		: pointer to ROGUELIKE struct
*/
void _fillGUI(PRL pRL)
{
	SRECT win = { 0, 0, 0, 0 };
	win.T = pRL->pPC->pos.Y - (GUI_ENV_W / 2);
	win.L = pRL->pPC->pos.X - (GUI_ENV_W / 2);
	win.B = win.T + GUI_ENV_W;
	win.R = win.L + GUI_ENV_W;
	
	COORD d = { 0, 0 };
	if (win.T < 0)
		d.Y = 0 - win.T;
	if (win.L < 0)
		d.X = 0 - win.L;
	if (win.B > pRL->curEnv->dim.Y)
		d.Y = pRL->curEnv->dim.Y - win.B;
	if (win.R > (pRL->curEnv->dim.X - 1))	// '-1' is for newline
		d.X = pRL->curEnv->dim.X - win.R - 1;
	
	win.T += d.Y;
	win.L += d.X;
	win.B += d.Y;
	win.R += d.X;

	for (USHORT y = 0; y < GUI_ENV_W; y++)
	{
		wmemcpy(
			&(pRL->pGUI->gBuf[pRL->pGUI->dim.X * (pRL->pGUI->rEnt.T + y) + (pRL->pGUI->rEnv.L)]),
			&(pRL->curEnv->gBuf[pRL->curEnv->dim.X * (win.T + y) + (win.L)]),
			GUI_ENV_W);
	}

	PITEM	pI	= NULL;
	PENT	pE	= NULL;
	COORD	pos	= { 0, 0 };

	// GUI color
	pRL->pGUI->attr = pRL->curEnv->attr;
	// items
	for (PLNODE tmp = pRL->curEnv->lItem.head; tmp != NULL; tmp = tmp->next)
	{
		if ((pI = tmp->pVal) != NULL && _coordInSrect(pI->pos, win))
		{
			pos.Y = (pRL->pGUI->rEnv.T + (pI->pos.Y - win.T));
			pos.X = (pRL->pGUI->rEnv.L + (pI->pos.X - win.L));
			pRL->pGUI->gBuf[pRL->pGUI->dim.X * (pos.Y) + (pos.X)] = ICON_CHEST;
		}
	}
	// entities
	for (PLNODE tmp = pRL->curEnv->lEnti.head; tmp != NULL; tmp = tmp->next)
	{
		if ((pE = tmp->pVal) != NULL && _coordInSrect(pE->pos, win))
		{
			pos.Y = (pRL->pGUI->rEnv.T + (pE->pos.Y - win.T));
			pos.X = (pRL->pGUI->rEnv.L + (pE->pos.X - win.L));
			pRL->pGUI->gBuf[pRL->pGUI->dim.X * (pos.Y) + (pos.X)] = ICON_ENEMY;
		}
	}
	// player character
	pos.Y = (pRL->pGUI->rEnv.T + (pRL->pPC->pos.Y - win.T));
	pos.X = (pRL->pGUI->rEnv.L + (pRL->pPC->pos.X - win.L));
	pRL->pGUI->gBuf[pRL->pGUI->dim.X * (pos.Y) + (pos.X)] = pRL->pPC->rep.icon;
	// player character inventory
	for (USHORT i = 0; i < 10; i++)
	{
		pos.Y = pRL->pGUI->rEnt.T + 1 + i;
		pos.X = pRL->pGUI->rEnt.L + 1;

		if (pRL->pPC->items[i] != NULL)
			pRL->pGUI->gBuf[pRL->pGUI->dim.X * (pos.Y) + (pos.X)] = ICON_CHEST;
		else
			pRL->pGUI->gBuf[pRL->pGUI->dim.X * (pos.Y) + (pos.X)] = ICON_HALLWAY;
	}
}
//--------------------------------------------------------------------------------
/*
	shows GUI struct
	@param	hCSB	: console screen buffer handle
	@param	pGUI	: pointer to GUI struct
*/
void _showGUI(HANDLE hCSB, PGUI pGUI)
{
	if (hCSB != INVALID_HANDLE_VALUE && pGUI != NULL)
	{
		// set console window & screen buffer size
		// winDim MUST BE SMALLER THAN bufDim, otherwise SetConsoleWindowInfo() function fails
		// https://docs.microsoft.com/en-us/windows/console/setconsolewindowinfo
		SMALL_RECT	winDim = { 0, 0, (2 * pGUI->dim.X), (pGUI->dim.Y) };
		COORD		bufDim = { (2 * pGUI->dim.X) + 1, (pGUI->dim.Y) + 1 };
		SetConsoleWindowInfo(hCSB, TRUE, &winDim);
		SetConsoleScreenBufferSize(hCSB, bufDim);

		SetConsoleTextAttribute(hCSB, pGUI->attr);
		WriteConsoleW(hCSB, pGUI->gBuf, pGUI->dim.X * pGUI->dim.Y, NULL, NULL);
		_setCursorPos(hCSB, 0, 0);
	}
}
//--------------------------------------------------------------------------------
/*
	frees GUI struct
	@param	pGUI	: pointer to GUI struct
*/
void _freeGUI(PGUI pGUI)
{
	if (pGUI != NULL)
	{
		free(pGUI->gBuf);
		free(pGUI);
	}
}
//--------------------------------------------------------------------------------
/*
	TODO: write description
*/
//--------------------------------------------------------------------------------
