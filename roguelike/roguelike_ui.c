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
				{
					pRL->curEnv = temp->pVal;
					pRL->pPC->pos.X = pRL->curEnv->posO.X;
					pRL->pPC->pos.Y = pRL->curEnv->posO.Y;
				}
			}
			else if (pVal == (PVOID)ICON_ENV_O && pRL->curIdx + 1 < pRL->lEnv.len)
			{
				// collision with exit detected
				PLNODE temp = _getListNodeI(&(pRL->lEnv), ++(pRL->curIdx));
				if (temp != NULL)
				{
					pRL->curEnv = temp->pVal;
					pRL->pPC->pos.X = pRL->curEnv->posI.X;
					pRL->pPC->pos.Y = pRL->curEnv->posI.Y;
				}
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
	make GUI struct
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
			free(result);
			return NULL;
		}
	}
	return result;
}
//--------------------------------------------------------------------------------
/*
	fills GUI with ENV and ENT data
	@param	pGUI	: pointer to GUI struct
	@param	pRL		: pointer to ROGUELIKE struct
*/
void _fillGUI(PGUI pGUI, PRL pRL)
{
	for (USHORT y = 0; y < GUI_ENV_W; y++)
	{
		wmemcpy(
			&(pGUI->gBuf[pGUI->dim.X * (y + 1) + 1]),
			&(pRL->curEnv->gBuf[pRL->curEnv->dim.X * (y) + 0]),
			GUI_ENV_W);
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

		SetConsoleTextAttribute(hCSB, _FC);
		WriteConsoleW(hCSB, pGUI->gBuf, pGUI->dim.X * pGUI->dim.Y, NULL, NULL);
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
