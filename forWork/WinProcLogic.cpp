#include "WinProcLogic.h"
#include "forWork.h"
#include <Windows.h>
#include "Timer.h"
#include "GDIWrappers.h"

extern HINSTANCE hInst;

LRESULT WorkTrackerWindow::create(CREATESTRUCT* pcs)
{
	hwndName = CreateWindow(L"edit", NULL, WS_CHILD | WS_BORDER | WS_VISIBLE,
		30, 10, 100, 25, *this, (HMENU)1, hInst, 0);
	hwndButton = CreateWindow(L"button", L"Start",
		WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
		30, 100, 100, 25, *this, (HMENU)BTN_START, hInst, NULL);
	return 0;
}

LRESULT WorkTrackerWindow::timerMessage(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case TIMER_ID_1:
	{
		antonov::GetDCWrapper hdc(*this);
		hdc.printString(TEXT_POSX, TEXT_POSY, timer.toWstring());
	}break;
	default:
		break;
	}
	return 0;
}

LRESULT WorkTrackerWindow::commandMessage(WPARAM wParam, LPARAM lParam)
{

	int wmId = LOWORD(wParam);

	switch (wmId)
	{
	case BTN_START:
		timer.change();
		SetWindowText(hwndButton, (timer.isRun()) ? L"Stop" : L"Start");
		return 0;
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), *this, About);
		break;
	case IDM_EXIT:
		DestroyWindow(*this);
		break;
	default:
		return DefWindowProc(*this, WM_COMMAND, wParam, lParam);
	}
	return 0;
}

LRESULT WorkTrackerWindow::mouseMove(WPARAM wParam, LPARAM lParam)
{
	if (wParam & MK_LBUTTON) {

		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		if (!started) {
			SetCapture(*this);
			started = true;
			startPos.x = xPos;
			startPos.y = yPos;
		}
		RECT r;
		GetWindowRect(*this, &r);
		MoveWindow(*this, r.left + xPos - startPos.x,
			r.top + yPos - startPos.y, 160, 200, FALSE);
	}
	else {
		started = false;
		ReleaseCapture();
	}
	return 0;
}
