#include "roguelike.h"

int main()
{
	HANDLE	hCSB	= GetStdHandle(STD_OUTPUT_HANDLE);
	PRL		pRL		= _makeRL();

	_setCodePage(CP_UTF8);
	_setCursorVisibility(hCSB, FALSE);

	while (pRL->pPC != NULL)
	{
		_fillGUI(pRL);
		_showGUI(hCSB, pRL->pGUI);
		_handlePC(pRL);
	}
	_freeRL(pRL);

	return 0;
}
