#pragma once
#define TIMER_ID_1 1
#include <Windows.h>
#include "GDIWrappers.h"
#include "forWork.h"
#include <Windowsx.h>
#include "Timer.h"
#include <fstream>

struct TimerHandler {
	HWND hwnd;
	const int timerTick = 10;
	bool isRunning = false;
	void operator()() {
		if (isRunning)
			KillTimer(hwnd, TIMER_ID_1);
		else
			SetTimer(hwnd, TIMER_ID_1, timerTick, NULL);
		isRunning = !isRunning;
	}
};

class WorkTrackerWindow : public antonov::WNDWRAPPER {
	bool	started		= false;
	POINT	startPos	= {0,0};
	HWND	hwndButton	= 0;
	HWND	hwndName	= 0;

	         TimerHandler	th		{*this};
	antonov::Timer			timer	{th};

protected:
	virtual LRESULT create			(CREATESTRUCT* pcs)			   override;
	virtual LRESULT setFocus		(WPARAM wParam, LPARAM lParam) override {
		SetFocus(hwndButton);
		return 0;
	}
	virtual LRESULT timerMessage	(WPARAM wParam, LPARAM lParam) override;
	virtual LRESULT commandMessage	(WPARAM wParam, LPARAM lParam) override;
	virtual LRESULT mouseMove		(WPARAM wParam, LPARAM lParam) override;
	virtual LRESULT destroy			(WPARAM wParam, LPARAM lParam) override {
		
		if (timer.isRun())
			SendMessage(*this, WM_COMMAND, BTN_START, 0);
		timer.~Timer();
		PostQuitMessage(0);
		return 0;
	}
	virtual LRESULT paint			(WPARAM wParam, LPARAM lParam) {
		this->startPaint().printString(
			TEXT_POSX, TEXT_POSY, timer.toWstring());
		return 0;
	}
public:
	WorkTrackerWindow(HWND _hwnd): antonov::WNDWRAPPER(_hwnd){}
};

//void ChangeTimerState();
