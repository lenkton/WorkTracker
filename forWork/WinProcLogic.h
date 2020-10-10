#pragma once
#define TIMER_ID_1 1
#include <Windows.h>
#include "GDIWrappers.h"
#include "forWork.h"
#include <Windowsx.h>
#include "Timer.h"

namespace antonov {
	struct TimerHandler {
		HWND hwnd;
		const int timerTick = 10;
		
		bool isRunning = false;
		void operator()();
	};
}

class WorkTrackerWindow : public antonov::WNDWRAPPER {
	bool started = false;
	POINT startPos = { 0,0 };
	antonov::TimerHandler th = { *this };
	antonov::Timer timer{ th };
	HWND hwndButton=0, hwndName=0;
protected:
	virtual LRESULT create(CREATESTRUCT* pcs) override {
		hwndName = CreateWindow(L"edit", NULL, WS_CHILD | WS_BORDER | WS_VISIBLE,
			30, 10, 100, 25, *this, (HMENU)1, hInst, 0);
		hwndButton = CreateWindow(L"button", L"Start",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			30, 100, 100, 25, *this, (HMENU)BTN_START, hInst, NULL);
		return 0;
	}
	virtual LRESULT setFocus(WPARAM wParam, LPARAM lParam) override {
		SetFocus(hwndButton);
		return 0;
	}
	virtual LRESULT timerMessage(WPARAM wParam, LPARAM lParam) override {
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
	virtual LRESULT commandMessage(WPARAM wParam, LPARAM lParam) override {

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

	virtual LRESULT mouseMove(WPARAM wParam, LPARAM lParam) override {
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


	virtual LRESULT destroy(WPARAM wParam, LPARAM lParam) override {
			timer.~Timer();
			PostQuitMessage(0);
			return 0;
	}
	virtual LRESULT paint(WPARAM wParam, LPARAM lParam) {
		antonov::PaintHDC paint{ *this };
		paint.printString(TEXT_POSX, TEXT_POSY, timer.toWstring());
		return 0;
	}
public:

	WorkTrackerWindow(HWND _hwnd): antonov::WNDWRAPPER(_hwnd){}

};

//void ChangeTimerState();
