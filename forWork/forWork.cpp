// forWork.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "forWork.h"
#include "Timer.h"
#include "WinProcLogic.h"
#include <cwchar>
#include "GDIWrappers.h"
#include <Windowsx.h>



// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_FORWORK, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FORWORK));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FORWORK));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_FORWORK);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, 
	   WS_POPUPWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, 160, 200, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool started = false;
	static POINT startPos;
	static antonov::TimerHandler th = {hWnd};
	static antonov::Timer timer(th);
	static HWND hwndButton,hwndName;
	switch (message)
	{
	case WM_CREATE: {
		hwndName = CreateWindow(L"edit",NULL,WS_CHILD|WS_BORDER|WS_VISIBLE,30,10,100,25,hWnd,(HMENU)1,hInst,0);
		hwndButton = CreateWindow(L"button", L"Start",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			30, 100, 100, 25, hWnd, (HMENU)BTN_START, hInst, NULL);
	} break;
	case WM_SETFOCUS: {
		SetFocus(hwndButton);
	} return 0;
	case WM_COMMAND: {
		if (wParam == BTN_START) {
			timer.change();
			SetWindowText(hwndButton, (timer.isRun())? L"Stop" : L"Start");
			return 0;
		}
		int wmId = LOWORD(wParam);
		// Разобрать выбор в меню:
		switch (wmId)
		{

		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
					 break;
	case WM_MOUSEMOVE:
	{
		
		if (wParam&MK_LBUTTON) {
			
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			if (!started) { 
				SetCapture(hWnd);
				started = true; 
				startPos.x = xPos; 
				startPos.y = yPos; 
			}
			RECT r;
			GetWindowRect(hWnd, &r);
			MoveWindow(hWnd, r.left + xPos - startPos.x,
							 r.top+ yPos - startPos.y, 160, 200, FALSE);
		}
		else { 
			started = false;
			ReleaseCapture();
		}
	}return 0;
	
	case WM_TIMER: {
		switch (wParam)
		{
		case TIMER_ID_1:
		{
			antonov::GetDCWrapper hdc(hWnd);
			hdc.printString(TEXT_POSX, TEXT_POSY,timer.toWstring());
		}break;
		default:
			break;
		}
	} break;
    case WM_PAINT:
        {
		antonov::PaintHDC paint{ hWnd };
		paint.printString(TEXT_POSX, TEXT_POSY, timer.toWstring());
        }
        break;
    case WM_DESTROY:
		timer.~Timer();
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
