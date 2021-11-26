#include "roguelike_item.h"

//--------------------------------------------------------------------------------
/*
	makes ITEM struct
	@param	pos		: position
	@param	type	: type
	@return	pointer to ITEM struct
*/
PITEM _makeItem(COORD pos, ITYPE type)
{
	PITEM result = NULL;

	if ((result = (PITEM)calloc(1, sizeof(ITEM))) != NULL)
	{
		result->pos = pos;
		result->type = type;
	}
	return result;
}
//--------------------------------------------------------------------------------
/*
	frees ITEM struct
	@param	pItem	: pointer to ITEM struct
*/
void _freeItem(PITEM pItem)
{
	free(pItem);
}
//--------------------------------------------------------------------------------
/*
	TODO: write description
*/
//--------------------------------------------------------------------------------
