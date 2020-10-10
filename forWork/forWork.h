#pragma once

#include "resource.h"

#define MAX_LOADSTRING 100
#define BTN_START 2
#define TEXT_POSX 30
#define TEXT_POSY 50

extern HINSTANCE hInst;

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
