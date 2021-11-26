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
		USHORT	iter	= ENV_MIN_ITER;
		COORD	dim		= { 0, 0 };

		srand((UINT)time(NULL));
		for (USHORT i = 0; i < 3; i++, iter++)
		{
			dim = _getValidParams(iter);
			_pushListNode(&(result->lEnv), _makeEnv(dim, _FC, iter));
		}

		result->curIdx	= 0;
		result->curEnv	= _getListNodeI(&(result->lEnv), result->curIdx)->pVal;
		result->pPC		= _makeEnt(result->curEnv->posI, ICON_PLAYER, _FG | _FI);
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
}
//--------------------------------------------------------------------------------
/*
	TODO: write description
*/
//--------------------------------------------------------------------------------
