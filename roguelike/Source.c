#include "roguelike.h"

int main()
{
	HANDLE	hCSB	= GetStdHandle(STD_OUTPUT_HANDLE);
	PRL		pRL		= _makeRL();

	_setCodePage(CP_UTF8);
	_setCursorVisibility(hCSB, FALSE);

	while (pRL->pPC != NULL)
	{
		_showEnv(hCSB, pRL->curEnv);
		SetConsoleTextAttribute(hCSB, _FY);
		for (USHORT i = 0; i < 10; i++)
		{
			_setCursorPos(hCSB, 2 * i, 0);
			if (pRL->pPC->items[i] != NULL)
				_putwch(ICON_CHEST);
			else
				_putwch(ICON_HALLWAY);
		}
		_showEnt(hCSB, pRL->pPC);
		_handlePC(pRL);
	}
	_freeRL(pRL);

	return 0;
}
