#include "WinProcLogic.h"
#include "forWork.h"
#include <Windows.h>
#include "Timer.h"
#include "GDIWrappers.h"

extern HINSTANCE hInst;

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
