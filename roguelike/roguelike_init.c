#include "roguelike_init.h"

//--------------------------------------------------------------------------------
/*
	makes ROGUELIKE struct
	@return	pointer to ROGUELIKE struct
*/
PRL _makeRL()
{
	PRL result = NULL;

	if ((result = (PRL)calloc(1, sizeof(RL))) != NULL)
	{
		srand((UINT)time(NULL));

		COORD dim = { 0, 0 };
		for (USHORT i = 0; i < 3; i++)
		{
			dim = _getValidParams(ENV_MIN_ITER + i);
			_pushListNode(&(result->lEnv), _makeEnv(dim, _FC, ENV_MIN_ITER + i));
			_pushListNode(&(result->lEnv), _makeEnv(dim, _FC, ENV_MIN_ITER + i));
		}

		result->curIdx	= 0;
		result->curEnv	= _getListNodeI(&(result->lEnv), result->curIdx)->pVal;

		result->pPC		= _makeEnt(result->curEnv->posI, ICON_PLAYER, _FG | _FI);
		result->pGUI	= _makeGUI();
	}
	return result;
}
//--------------------------------------------------------------------------------
/*
	frees ROGUELIKE struct
*/
void _freeRL(PRL pRL)
{
	for (PLNODE temp = pRL->lEnv.head; temp != NULL; temp = temp->next)
		_freeEnv(temp->pVal);
	_freeListNode(pRL->lEnv.head);
	_freeEnt(pRL->pPC);
	_freeGUI(pRL->pGUI);
}
//--------------------------------------------------------------------------------
/*
	TODO: write description
*/
//--------------------------------------------------------------------------------
