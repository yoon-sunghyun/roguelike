#include "roguelike_ent.h"

//--------------------------------------------------------------------------------
/*
	makes ENTITY struct - must be freed by _freeEnt() function
	@param	pos			: position
	@param	icon		: icon
	@param	attr		: attributes(icon colors)
	@return	pointer to ENT struct
*/
PENT _makeEnt(COORD pos, WCHAR icon, WORD attr)
{
	PENT result = NULL;

	if ((result = (PENT)calloc(1, sizeof(ENT))) != NULL)
	{
		result->pos			= pos;
		result->rep.icon	= icon;
		result->rep.attr	= attr;

		result->hp = 10;
	}
	return result;
}
//--------------------------------------------------------------------------------
/*
	moves ENTITY struct
	@param	pEnv	: pointer to an ENV struct
	@param	pEnt	: pointer to an ENT struct
	@param	dX		: delta x
	@param	dY		: delta y
	@param	ppVal	: reserved, should be NULL
	@return	TRUE if moved, FALSE if otherwise
*/
CTYPE _moveEnt(PENV pEnv, PENT pEnt, SHORT dX, SHORT dY, PVOID* ppVal)
{
	if (pEnv != NULL && pEnt != NULL)
	{
		COORD newPos = pEnt->pos;
		newPos.X += dX;
		newPos.Y += dY;

		if (pEnv->cBuf[pEnv->dim.X * (newPos.Y) + (newPos.X)])
		{
			// no collision detected, position updated
			pEnt->pos = newPos;
			return _NIL;
		}
		else
		{
			// check collision with entrance/exit
			WCHAR icon = pEnv->gBuf[pEnv->dim.X * (newPos.Y) + (newPos.X)];
			if (icon == ICON_ENV_I) { (*ppVal) = (PVOID)ICON_ENV_I; return _ENV; }
			if (icon == ICON_ENV_O) { (*ppVal) = (PVOID)ICON_ENV_O; return _ENV; }

			// check collision with items
			PITEM pValI = NULL;
			for (PLNODE temp = pEnv->lItem.head; temp != NULL; temp = temp->next)
				if ((pValI = temp->pVal) != NULL)
					if (pValI->pos.X == newPos.X && pValI->pos.Y == newPos.Y)
					{
						// collision with an item detected, position updated
						if (ppVal != NULL) { (*ppVal) = pValI; }
						pEnt->pos = newPos;
						return _ITM;
					}
			// catch-all, collision with environment detected
			return _ENV;
		}
	}
	return _NIL;
}
//--------------------------------------------------------------------------------
/*
	shows ENTITY struct
	@param	hCSB	: handle of a console screen buffer
	@param	pEnt	: pointer to ENT struct
*/
void _showEnt(HANDLE hCSB, PENT pEnt)
{
	/*
	USHORT r = 5;
	for (SHORT y = pEnt->pos.Y - r; y < pEnt->pos.Y + r; y++)
	{
		for (SHORT x = pEnt->pos.X - r; x < pEnt->pos.X + r; x++)
		{
			if (((SHORT)pow(y - pEnt->pos.Y, 2)) + ((SHORT)pow(x - pEnt->pos.X, 2)) < ((SHORT)pow(r, 2))-1)
			{
				_setCursorPos(hCSB, 2 * x, y);
				_putwch(L'▲');
			}
		}
	}
	*/

	SetConsoleTextAttribute(hCSB, pEnt->rep.attr);
	_setCursorPos(hCSB, 2 * pEnt->pos.X, pEnt->pos.Y);
	_putwch(pEnt->rep.icon);
	_setCursorPos(hCSB, 0, 0);
}
//--------------------------------------------------------------------------------
/*
	frees ENTITY struct
	@param	pEnt	: pointer to an ENTITY struct
*/
void _freeEnt(PENT pEnt)
{
	if (pEnt != NULL)
	{
		for (USHORT i = 0; i < 10; i++)
			_freeItem(pEnt->items[i]);
		free(pEnt);
	}
}
//--------------------------------------------------------------------------------
/*
	TODO: write description
*/
//--------------------------------------------------------------------------------
