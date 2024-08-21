#include <Windows.h>
#include "Resource.h"
#include <string.h>
CONST CHAR g_sz_WINDOW_CLASS[] = "Calc_PD_311";


CONST INT g_i_START_X = 10;
CONST INT g_i_START_Y = 10;

CONST INT g_i_BUTTON_SIZE = 50;
CONST INT g_i_INTERVAL = 5;

CONST INT g_i_DISPLAY_WIDTH = (g_i_BUTTON_SIZE + g_i_INTERVAL) * 5;
CONST INT g_i_DISPLAY_HEIGHT = 22;

CONST INT g_i_WINDOW_WIDTH = g_i_DISPLAY_WIDTH + g_i_START_X * 2 + 16;
CONST INT g_i_WINDOW_HEIGHT = g_i_DISPLAY_HEIGHT + g_i_START_Y * 2 + g_i_BUTTON_SIZE * 4 + g_i_INTERVAL * 4;

CONST INT g_i_START_X_BUTTON = g_i_START_X;
CONST INT g_i_START_Y_BUTTON = g_i_START_Y * 2 + g_i_DISPLAY_HEIGHT;

void TypeIn(HWND hWin, int numberID);
INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1) –егистраци€ класса окна


	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass)); // «анул€ем экземпл€р структуры
	wClass.style = 0;
	wClass.cbSize = sizeof(wClass); // cb - Count bytes
	wClass.cbWndExtra = 0; //Ёкстра байты дл€ окна Ч почитать позже
	wClass.cbClsExtra = 0;//Ёкстра байты дл€ класса Ч почитать позже
	wClass.hIcon = (HICON)LoadImage(hInstance, "ICO\\calcXP.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wClass.hIconSm = (HICON)LoadImage(hInstance, "ICO\\calcXP.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wClass.hCursor = LoadCursor(hInstance, IDC_ARROW);
	wClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wClass.hInstance = hInstance;
	wClass.lpszMenuName = NULL;
	wClass.lpszClassName = g_sz_WINDOW_CLASS;
	wClass.lpfnWndProc = (WNDPROC)WndProc;
	if (!RegisterClassEx(&wClass))
	{
		MessageBox(NULL, "Class registration failed!", "Error", MB_OK | MB_ICONERROR);
	}
	//2) —оздание окна
	HWND hwnd = CreateWindowEx(NULL, g_sz_WINDOW_CLASS, g_sz_WINDOW_CLASS, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, g_i_WINDOW_WIDTH, g_i_WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	//3) «апуск цикла обработки сообщений
	MSG msg;
	while (GetMessage(&msg, hwnd, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		HWND hDisplay = CreateWindowEx
		(
			NULL, "Edit", "0",
			WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_RIGHT | ES_READONLY | WS_BORDER,
			g_i_START_X, g_i_START_Y, g_i_DISPLAY_WIDTH, g_i_DISPLAY_HEIGHT,
			hwnd, (HMENU)IDC_EDIT_DISPLAY, NULL, NULL
		);
		INT digit = 0;
		CHAR sz_digit[2]{};
		for (int i = 2; i >= 0; i--)
		{
			for (size_t j = 0; j < 3; j++, digit++)
			{
				sz_digit[0] = '1' + digit;
				CreateWindowEx(NULL, "Button", sz_digit, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, g_i_START_X_BUTTON + j * (g_i_BUTTON_SIZE + g_i_INTERVAL), g_i_START_Y_BUTTON + i * (g_i_BUTTON_SIZE + g_i_INTERVAL), g_i_BUTTON_SIZE, g_i_BUTTON_SIZE, hwnd, (HMENU)(IDC_BUTTON_1 + digit), NULL, NULL);
			}
		}
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_0:
		case IDC_BUTTON_1:
		case IDC_BUTTON_2:
		case IDC_BUTTON_3:
		case IDC_BUTTON_4:
		case IDC_BUTTON_5:
		case IDC_BUTTON_6:
		case IDC_BUTTON_7:
		case IDC_BUTTON_8:
		case IDC_BUTTON_9:
			TypeIn(hwnd, LOWORD(wParam));
			break;
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}
void TypeIn(HWND hWin, int numberID)
{
	HWND hEdit = GetDlgItem(hWin, IDC_EDIT_DISPLAY);
	CONST INT SIZE = 256;
	CHAR sz_buffer[SIZE]{};
	SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
	if (!strcmp(sz_buffer, "0"))
	{
		sz_buffer[0] = ' ';
	}
	switch (numberID)
	{
	case IDC_BUTTON_0:
		strcat_s(sz_buffer, SIZE, "0");
		break;
	case IDC_BUTTON_1:
		strcat_s(sz_buffer, SIZE, "1");
		break;
	case IDC_BUTTON_2:
		strcat_s(sz_buffer, SIZE, "2");
		break;
	case IDC_BUTTON_3:
		strcat_s(sz_buffer, SIZE, "3");
		break;
	case IDC_BUTTON_4:
		strcat_s(sz_buffer, SIZE, "4");
		break;
	case IDC_BUTTON_5:
		strcat_s(sz_buffer, SIZE, "5");
		break;
	case IDC_BUTTON_6:
		strcat_s(sz_buffer, SIZE, "6");
		break;
	case IDC_BUTTON_7:
		strcat_s(sz_buffer, SIZE, "7");
		break;
	case IDC_BUTTON_8:
		strcat_s(sz_buffer, SIZE, "8");
		break;
	case IDC_BUTTON_9:
		strcat_s(sz_buffer, SIZE, "9");
		break;
	default:
		break;
	}
	SendMessage(hEdit, WM_SETTEXT, SIZE, (LPARAM)sz_buffer);
}