#include "WinProcLogic.h"
#include <Windows.h>

void antonov::TimerHandler::operator()()
{
	{
		if (isRunning) {
			KillTimer(hwnd, TIMER_ID_1);
		}
		else {
			
			SetTimer(hwnd,TIMER_ID_1,timerTick,NULL);
		}
		isRunning = !isRunning;
	}
}
