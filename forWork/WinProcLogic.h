#pragma once
#define TIMER_ID_1 1
#include <Windows.h>

namespace antonov {
	struct TimerHandler {
		HWND hwnd;
		const int timerTick = 10;
		
		bool isRunning = false;
		void operator()();
	};
}

//void ChangeTimerState();
