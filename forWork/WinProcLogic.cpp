#include "WinProcLogic.h"
#include "forWork.h"
#include <Windows.h>
#include "Timer.h"
#include "GDIWrappers.h"
#include <chrono>
#include <ctime> 
#include <sstream>

extern HINSTANCE hInst;

void save_log(HWND hwnd, bool isStart) {
	using std::chrono::system_clock;
	std::time_t current_datetime = system_clock::to_time_t(
		system_clock::now()
	);
	std::tm date_time;
	localtime_s(&date_time, &current_datetime);

	date_time.tm_mon += 1;
	date_time.tm_year += 1900;
	date_time.tm_mday;

	std::wstringstream wss;
	wss << L"����� �� "
		<< date_time.tm_mday << L'.'
		<< date_time.tm_mon  << L'.'
		<< date_time.tm_year << L".csv";

	std::wofstream results(wss.str(), std::ios::app);

	auto len = GetWindowTextLength(hwnd) + 1;
	wchar_t* buf = new wchar_t[len];
	GetWindowText(hwnd, buf, len);
	std::wstring app_name{ buf };

	results << app_name << L";" << (isStart ? L"start;" : L"end;")
		<< date_time.tm_hour << L';'
		<< date_time.tm_min  << L';'
		<< date_time.tm_sec  << L";\n";

	results.close();
	delete[]buf;
}


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
	case TIMER_ID_1:{
			antonov::GetDCWrapper hdc(*this);
			hdc.printString(TEXT_POSX, TEXT_POSY, timer.toWstring());
		}
		break;
	}
	return 0;
}

LRESULT WorkTrackerWindow::commandMessage(WPARAM wParam, LPARAM lParam)
{

	int wmId = LOWORD(wParam);

	switch (wmId)
	{
	case 1:
		if (HIWORD(wParam) == EN_CHANGE) {
			size_t len = 1 + SendMessage(hwndName, EM_LINELENGTH, 0, 0);
			wchar_t* buf = new wchar_t[len];
			buf[0] = (WORD)len;
			SendMessage(hwndName, EM_GETLINE, 0,(LPARAM) buf);
			buf[len-1] = L'\0';
			SetWindowText(*this, buf);
			delete[] buf;
		}
		break;
	case BTN_START:
		timer.change();
		SetWindowText(hwndButton, (timer.isRun()) ? L"Stop" : L"Start");
		save_log(*this,timer.isRun());
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
