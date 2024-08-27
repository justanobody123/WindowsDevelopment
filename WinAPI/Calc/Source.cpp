#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include "Resource.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits>

CONST INT g_i_SLEEP_TIME = 100;

CONST CHAR g_sz_WINDOW_CLASS[] = "Calc_PD_311";

CONST INT g_i_START_X = 10;
CONST INT g_i_START_Y = 10;

CONST INT g_i_BUTTON_SIZE = 50;
CONST INT g_i_INTERVAL = 5;

CONST INT g_i_BUTTON_DOUBLE_SIZE = g_i_BUTTON_SIZE * 2 + g_i_INTERVAL;

CONST INT g_i_DISPLAY_WIDTH = (g_i_BUTTON_SIZE + g_i_INTERVAL) * 5;
CONST INT g_i_DISPLAY_HEIGHT = 22;

CONST INT g_i_TITLE_HEIGHT = 39;

CONST INT g_i_WINDOW_WIDTH = g_i_DISPLAY_WIDTH + g_i_START_X * 2 + 16;
CONST INT g_i_WINDOW_HEIGHT = g_i_DISPLAY_HEIGHT + g_i_START_Y * 2 + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 4 + g_i_TITLE_HEIGHT + g_i_INTERVAL;

CONST INT g_i_START_X_BUTTON = g_i_START_X;
CONST INT g_i_START_Y_BUTTON = g_i_START_Y * 2 + g_i_DISPLAY_HEIGHT;
CONST INT g_i_START_X_OPERATIONS = g_i_START_X_BUTTON + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3;
CONST INT g_i_START_X_CONTROL_BUTTONS = g_i_START_X_BUTTON + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 4;

VOID PushButton(HWND parent, int id);
void TypeIn(HWND hWin, int numberID);
char* Parse(CHAR display_content[]);
bool IsParseable(CHAR display_content[]);
INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1) Регистрация класса окна


	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass)); // Зануляем экземпляр структуры
	wClass.style = 0;
	wClass.cbSize = sizeof(wClass); // cb - Count bytes
	wClass.cbWndExtra = 0; //Экстра байты для окна — почитать позже
	wClass.cbClsExtra = 0;//Экстра байты для класса — почитать позже
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
	//2) Создание окна
	HWND hwnd = CreateWindowEx(NULL, g_sz_WINDOW_CLASS, g_sz_WINDOW_CLASS, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT, g_i_WINDOW_WIDTH, g_i_WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);
	//3) Запуск цикла обработки сообщений
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
		RECT rectWindow;
		RECT rectClient;
		GetWindowRect(hwnd, &rectWindow);
		GetClientRect(hwnd, &rectClient);
		int window_height = rectWindow.bottom - rectWindow.top;
		int title_height = window_height - rectClient.bottom;
		HWND hDisplay = CreateWindowEx
		(
			NULL, "Edit", "0",
			WS_CHILD | WS_VISIBLE | ES_NUMBER | ES_RIGHT | ES_READONLY | WS_BORDER,
			g_i_START_X, g_i_START_Y, g_i_DISPLAY_WIDTH, g_i_DISPLAY_HEIGHT,
			hwnd, (HMENU)IDC_EDIT_DISPLAY, NULL, NULL
		);
		//DIGITS

		INT digit = 0;
		CHAR sz_digit[2]{};
		for (int i = 2; i >= 0; i--)
		{
			for (size_t j = 0; j < 3; j++, digit++)
			{
				sz_digit[0] = '1' + digit;
				CreateWindowEx(NULL, "Button", sz_digit, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, g_i_START_X_BUTTON + j * (g_i_BUTTON_SIZE + g_i_INTERVAL), g_i_START_Y_BUTTON + i * (g_i_BUTTON_SIZE + g_i_INTERVAL), g_i_BUTTON_SIZE, g_i_BUTTON_SIZE, hwnd, (HMENU)(IDC_BUTTON_1 + digit), NULL, NULL);
			}
		}
		CreateWindowEx(NULL, "Button", "0", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP, g_i_START_X_BUTTON, g_i_START_Y_BUTTON + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3, g_i_BUTTON_DOUBLE_SIZE, g_i_BUTTON_SIZE, hwnd, (HMENU)IDC_BUTTON_0, NULL, NULL);
		CreateWindowEx
		(
			NULL, "Button", ".", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			g_i_START_X_BUTTON + g_i_BUTTON_DOUBLE_SIZE + g_i_INTERVAL,
			g_i_START_Y_BUTTON + (g_i_BUTTON_SIZE + g_i_INTERVAL) * 3,
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd,
			(HMENU)IDC_BUTTON_POINT, NULL, NULL
		);
		//DIGITS
		//OPERATIONS
		CONST CHAR sz_operations[] = "+-*/";
		CHAR sz_operation[2] = "";

		for (size_t i = 0; i <= 3; i++)
		{
			sz_operation[0] = sz_operations[i];
			CreateWindowEx
			(
				NULL, "Button", sz_operation, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
				g_i_START_X_OPERATIONS, g_i_START_Y_BUTTON + (g_i_BUTTON_SIZE + g_i_INTERVAL) * (3 - i),
				g_i_BUTTON_SIZE, g_i_BUTTON_SIZE, hwnd,
				(HMENU)(IDC_BUTTON_PLUS + i), NULL, NULL
			);
		}
		CreateWindowEx
		(
			NULL, "Button", "C", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			g_i_START_X_CONTROL_BUTTONS, g_i_START_Y_BUTTON + g_i_BUTTON_SIZE + g_i_INTERVAL,
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd, (HMENU)IDC_BUTTON_CLEAR, NULL, NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "=", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			g_i_START_X_CONTROL_BUTTONS, g_i_START_Y_BUTTON + g_i_BUTTON_DOUBLE_SIZE + g_i_INTERVAL,
			g_i_BUTTON_SIZE, g_i_BUTTON_DOUBLE_SIZE,
			hwnd, (HMENU)IDC_BUTTON_EQUAL, NULL, NULL
		);
		CreateWindowEx
		(
			NULL, "Button", "<-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_BITMAP,
			g_i_START_X_CONTROL_BUTTONS, g_i_START_Y_BUTTON,
			g_i_BUTTON_SIZE, g_i_BUTTON_SIZE,
			hwnd, (HMENU)IDC_BUTTON_BSP, NULL, NULL
		);

		//Тема
		for (size_t t = 0; t <= 10; t++)
		{
			CHAR file_name[20]{};
			sprintf(file_name, "calc_%i%s", t, ".bmp");
			HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, file_name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			HWND hButton = GetDlgItem(hwnd, IDC_BUTTON_0 + t);
			SendMessage(hButton, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmap);
			if (t <= 6)
			{
				sprintf(file_name, "calc_op_%i%s", t, ".bmp");
				HBITMAP hOPBitmap = (HBITMAP)LoadImage(NULL, file_name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
				HWND hOPButton = GetDlgItem(hwnd, IDC_BUTTON_PLUS + t);
				SendMessage(hOPButton, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hOPBitmap);
			}
		}
		
		
	}
	break;
	case WM_COMMAND:
	{
		static double a = DBL_MIN, b = 0;
		static int operation = 0;
		static bool input = false;
		static bool operation_input = false;
		SetFocus(hwnd);
		CONST INT SIZE = 256;
		CHAR sz_display[SIZE]{};
		CHAR sz_digit[2]{};
		HWND hEditDisplay = GetDlgItem(hwnd, IDC_EDIT_DISPLAY);
		if (LOWORD(wParam) >= IDC_BUTTON_0 && LOWORD(wParam) <= IDC_BUTTON_POINT)
		{
			if (!input) SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)"");

			sz_digit[0] = LOWORD(wParam) - IDC_BUTTON_0 + '0';
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display);
			if (LOWORD(wParam) == IDC_BUTTON_POINT)
			{
				if (strchr(sz_display, '.'))break;
				strcat(sz_display, ".");
			}
			else
			{
				if (sz_display[0] == '0' && strlen(sz_display) == 1)sz_display[0] = sz_digit[0];
				else strcat(sz_display, sz_digit);
				input = true;
			}
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
		if (LOWORD(wParam) == IDC_BUTTON_BSP)
		{
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display);
			INT display_length = strlen(sz_display);
			if (display_length)
				sz_display[--display_length] = 0;
			if (display_length == 0)sz_display[0] = '0';
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
		}
		if (LOWORD(wParam) == IDC_BUTTON_CLEAR)
		{
			a = DBL_MIN, b = 0, operation = 0, input = false, operation_input = false;
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)"0");
		}
		if (LOWORD(wParam) >= IDC_BUTTON_PLUS && LOWORD(wParam) <= IDC_BUTTON_SLASH)
		{
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display);
			if (input && a == DBL_MIN)
			{
				a = atof(sz_display);
				input = false;
			}
			//else b = atof(sz_display);
			if (input) SendMessage(hwnd, WM_COMMAND, IDC_BUTTON_EQUAL, 0);
			//
			
			operation = LOWORD(wParam);
			operation_input = true;
			

				//if (IsParseable(sz_display))
				//{
				//	//Заменяем содержимое дисплея на результат мат. операции
				//	SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)Parse(sz_display));
				//	SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display);
				//}
				//if (LOWORD(wParam) == IDC_BUTTON_PLUS)
				//{
				//	strcat(sz_display, "+");
				//}
				//else if (LOWORD(wParam) == IDC_BUTTON_MINUS)
				//{
				//	strcat(sz_display, "-");
				//}
				//else if (LOWORD(wParam) == IDC_BUTTON_ASTER)
				//{
				//	strcat(sz_display, "*");
				//}
				//else if (LOWORD(wParam) == IDC_BUTTON_SLASH)
				//{
				//	strcat(sz_display, "/");
				//}
				//SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
	
			
		}
		if (LOWORD(wParam) == IDC_BUTTON_EQUAL)
		{
			SendMessage(hEditDisplay, WM_GETTEXT, SIZE, (LPARAM)sz_display);
			if (input)b = atof(sz_display);
			switch (operation)
			{
			case IDC_BUTTON_PLUS:
				a += b;
				break;
			case IDC_BUTTON_MINUS:
				a -= b;
					break;
			case IDC_BUTTON_ASTER:
				a *= b;
				break;
			case IDC_BUTTON_SLASH:
				a /= b;
				break;
			}
			sprintf(sz_display, "%f", a);
			SendMessage(hEditDisplay, WM_SETTEXT, 0, (LPARAM)sz_display);
			operation_input = false;
			input = false;
		}
	}
	/*switch (LOWORD(wParam))
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
	}*/
	break;
	case WM_KEYDOWN:
	{
		if (GetKeyState(VK_SHIFT) < 0 and LOWORD(wParam) == 0x38)
		{
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETSTATE, TRUE, 0);
		}

		else if (LOWORD(wParam) >= 0x30 && LOWORD(wParam) <= 0x39)//РАБОТАЕТ
		{
			SendMessage(GetDlgItem(hwnd, LOWORD(wParam - 0x30 + IDC_BUTTON_0)), BM_SETSTATE, TRUE, 0);
			//PushButton(hwnd, LOWORD(wParam - 0x30 + IDC_BUTTON_0));
			//SendMessage(hwnd, WM_COMMAND, LOWORD(wParam - 0x30 + IDC_BUTTON_0), 0);
		}
		if (LOWORD(wParam) >= 0x60 && LOWORD(wParam) <= 0x69)//РАБОТАЕТ
		{
			SendMessage(GetDlgItem(hwnd, LOWORD(wParam - 0x60 + IDC_BUTTON_0)), BM_SETSTATE, TRUE, 0);
			SendMessage(hwnd, WM_COMMAND, LOWORD(wParam - 0x60 + IDC_BUTTON_0), 0);
		}
		switch (LOWORD(wParam))
		{
		case VK_DECIMAL:
		case VK_OEM_PERIOD://РАБОТАЕТ
		{
			//PushButton(hwnd, IDC_BUTTON_POINT);
			SendMessage(GetDlgItem(hwnd, LOWORD(IDC_BUTTON_POINT)), BM_SETSTATE, TRUE, 0);
			//SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_POINT), 0);
			break;
		}
		case VK_OEM_PLUS:
		case VK_ADD://РАБОТАЕТ
		{
			SendMessage(GetDlgItem(hwnd, LOWORD(IDC_BUTTON_PLUS)), BM_SETSTATE, TRUE, 0);
			//SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_PLUS), 0);
			break;
		}

		case VK_SUBTRACT://РАБОТАЕТ
		case VK_OEM_MINUS:
		{
			SendMessage(GetDlgItem(hwnd, LOWORD(IDC_BUTTON_MINUS)), BM_SETSTATE, TRUE, 0);
			//SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_MINUS), 0);
			break;
		}
		case VK_MULTIPLY://РАБОТАЕТ
		{
			SendMessage(GetDlgItem(hwnd, LOWORD(IDC_BUTTON_ASTER)), BM_SETSTATE, TRUE, 0);
			//SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_ASTER), 0);
			break;
		}
		case VK_OEM_2:
		case VK_DIVIDE://РАБОТАЕТ
		{
			SendMessage(GetDlgItem(hwnd, LOWORD(IDC_BUTTON_SLASH)), BM_SETSTATE, TRUE, 0);
			//SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_SLASH), 0);
			break;
		}


		case VK_BACK://ПРИ НАЖАТИИ РЕАГИРУЕТ КЛАВИША МИНУС
		{
			SendMessage(GetDlgItem(hwnd, LOWORD(IDC_BUTTON_BSP)), BM_SETSTATE, TRUE, 0);
			//SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_BSP), 0);
			break;
		}

		case VK_ESCAPE://РАБОТАЕТ
		{
			SendMessage(GetDlgItem(hwnd, LOWORD(IDC_BUTTON_CLEAR)), BM_SETSTATE, TRUE, 0);
			//SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_CLEAR), 0);
			break;
		}
		//ЭТО РАВНО — РАБОТАЕТ
		case VK_RETURN:

			SendMessage(GetDlgItem(hwnd, LOWORD(IDC_BUTTON_EQUAL)), BM_SETSTATE, TRUE, 0);
			//SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_EQUAL), 0);
			break;
		}
	}
	break;
	case WM_KEYUP:
		if (GetKeyState(VK_SHIFT) < 0 and LOWORD(wParam) == 0x38)
		{
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_ASTER), 0);
			SendMessage(GetDlgItem(hwnd, IDC_BUTTON_ASTER), BM_SETSTATE, FALSE, 0);
		}
		else if (LOWORD(wParam) >= 0x30 && LOWORD(wParam) <= 0x39)//РАБОТАЕТ
		{
			SendMessage(GetDlgItem(hwnd, LOWORD(wParam - 0x30 + IDC_BUTTON_0)), BM_SETSTATE, FALSE, 0);
			//PushButton(hwnd, LOWORD(wParam - 0x30 + IDC_BUTTON_0));
			SendMessage(hwnd, WM_COMMAND, LOWORD(wParam - 0x30 + IDC_BUTTON_0), 0);
		}
		if (LOWORD(wParam) >= 0x60 && LOWORD(wParam) <= 0x69)//РАБОТАЕТ
		{
			SendMessage(GetDlgItem(hwnd, LOWORD(wParam - 0x60 + IDC_BUTTON_0)), BM_SETSTATE, FALSE, 0);
			SendMessage(hwnd, WM_COMMAND, LOWORD(wParam - 0x60 + IDC_BUTTON_0), 0);
		}
		switch (LOWORD(wParam))
		{
		case VK_DECIMAL:
		case VK_OEM_PERIOD://РАБОТАЕТ
		{
			SendMessage(GetDlgItem(hwnd, LOWORD(IDC_BUTTON_POINT)), BM_SETSTATE, FALSE, 0);
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_POINT), 0);
			break;
		}
		case VK_OEM_PLUS:
		case VK_ADD://РАБОТАЕТ
		{
			SendMessage(GetDlgItem(hwnd, LOWORD(IDC_BUTTON_PLUS)), BM_SETSTATE, FALSE, 0);
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_PLUS), 0);
			break;
		}
		case VK_OEM_MINUS:
		case VK_SUBTRACT://РАБОТАЕТ
		{
			SendMessage(GetDlgItem(hwnd, LOWORD(IDC_BUTTON_MINUS)), BM_SETSTATE, FALSE, 0);
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_MINUS), 0);
			break;
		}
		case VK_MULTIPLY://РАБОТАЕТ
		{
			SendMessage(GetDlgItem(hwnd, LOWORD(IDC_BUTTON_ASTER)), BM_SETSTATE, FALSE, 0);
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_ASTER), 0);
			break;
		}
		case VK_OEM_2:
		case VK_DIVIDE://НЕ РАБОТАЕТ
		{
			SendMessage(GetDlgItem(hwnd, LOWORD(IDC_BUTTON_SLASH)), BM_SETSTATE, FALSE, 0);
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_SLASH), 0);
			break;
		}


		case VK_BACK://ПРИ НАЖАТИИ РЕАГИРУЕТ КЛАВИША МИНУС
		{
			SendMessage(GetDlgItem(hwnd, LOWORD(IDC_BUTTON_BSP)), BM_SETSTATE, FALSE, 0);
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_BSP), 0);
			break;
		}

		case VK_ESCAPE://РАБОТАЕТ
		{
			SendMessage(GetDlgItem(hwnd, LOWORD(IDC_BUTTON_CLEAR)), BM_SETSTATE, FALSE, 0);
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_CLEAR), 0);
			break;
		}
		//ЭТО РАВНО — РАБОТАЕТ
		case VK_RETURN:

			PushButton(hwnd, IDC_BUTTON_EQUAL);
			SendMessage(hwnd, WM_COMMAND, LOWORD(IDC_BUTTON_EQUAL), 0);
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
//void TypeIn(HWND hWin, int buttonID)
//{
//	HWND hEdit = GetDlgItem(hWin, IDC_EDIT_DISPLAY);
//	CONST INT SIZE = 256;
//	CHAR sz_buffer[SIZE]{};
//	SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
//	if (!strcmp(sz_buffer, "0"))
//	{
//		sz_buffer[0] = ' ';
//	}
//	switch (buttonID)
//	{
//	case IDC_BUTTON_0:
//		strcat_s(sz_buffer, SIZE, "0");
//		break;
//	case IDC_BUTTON_1:
//		strcat_s(sz_buffer, SIZE, "1");
//		break;
//	case IDC_BUTTON_2:
//		strcat_s(sz_buffer, SIZE, "2");
//		break;
//	case IDC_BUTTON_3:
//		strcat_s(sz_buffer, SIZE, "3");
//		break;
//	case IDC_BUTTON_4:
//		strcat_s(sz_buffer, SIZE, "4");
//		break;
//	case IDC_BUTTON_5:
//		strcat_s(sz_buffer, SIZE, "5");
//		break;
//	case IDC_BUTTON_6:
//		strcat_s(sz_buffer, SIZE, "6");
//		break;
//	case IDC_BUTTON_7:
//		strcat_s(sz_buffer, SIZE, "7");
//		break;
//	case IDC_BUTTON_8:
//		strcat_s(sz_buffer, SIZE, "8");
//		break;
//	case IDC_BUTTON_9:
//		strcat_s(sz_buffer, SIZE, "9");
//		break;
//	default:
//		break;
//	}
//	SendMessage(hEdit, WM_SETTEXT, SIZE, (LPARAM)sz_buffer);
//}

/*
	------------------------СПИСОК БАГОВ К ИСПРАВЛЕНИЮ------------------------
	ПРИ НАЖАТИИ НА КЛАВИШУ BASKSPACE НА ЭКРАНЕ ВИЗУАЛЬНО РЕАГИРУЕТ МИНУС — DONE
	ПРИ НАЖАТИИ НА МИНУС МЫШЬЮ ПОСЛЕДНИЙ СИМВОЛ НА ДИСПЛЕЕ СТИРАЕТСЯ КАК ПРИ НАЖАТИИ НА BACKSPACE — DONE
	(САМ BACKSPACE СО ВСЕХ СТОРОН РАБОТАЕТ ОТЛИЧНО, А ПРИ НАЖАТИИ НА МИНУС С КЛАВИАТУРЫ ОН ВЕДЕТ СЕБЯ КАК И НАДО, НО БЕЗ ВИЗУАЛИЗАЦИИ НА ЭКРАНЕ)
	УМНОЖИТЬ И РАЗДЕЛИТЬ НЕ РАБОТАЮТ С МЫШИ — DONE
	РАВНО С КЛАВИАТУРЫ НЕ РАБОТАЕТ, ПОСКОЛЬКУ СОВПАДАЕТ С ПЛЮСОМ — DONE
	НЕ РАБОТАЕТ ЗАТИРКА ЛИШНИХ НУЛЕЙ И ТОЧКИ ДЛЯ ЦЕЛЫХ ЧИСЕЛ — ТУТ ТОЛЬКО ПЛАКАТЬ
	ОСТАЛЬНОЕ УЖЕ НЕ УСПЕЮ (ТОЧКА ОДНА НА ВЕСЬ ДИСПЛЕЙ, ДОБАВИТЬ ОШИБКУ ПРИ ДЕЛЕНИИ НА НОЛЬ)
	------------------------УЛУЧШЕНИЯ------------------------
	ВЫНЕСТИ В ФУНКЦИЮ ВИЗУАЛИЗАЦИЮ НАЖАТИЯ КЛАВИШИ — DONE
*/
bool IsParseable(CHAR display_content[])
{
	CHAR operations[] = "+-*/";
	for (size_t i = 0; operations[i] != 0; i++)
	{
		if (strchr(display_content, operations[i]))
		{
			return true;
		}
	}
	return false;
}
char* Parse(CHAR display_content[])
{
	//CHAR sz_error[] = "Error!";
	CHAR operations[] = "+-*/";
	double first_number = 0, second_number = 0;
	char* op = NULL;
	for (size_t i = 0; operations[i] != 0; i++)
	{
		op = strchr(display_content, operations[i]);
		if (op != NULL)
		{
			break;
		}
	}
	char first_part[256]{};
	strncpy(first_part, display_content, op - display_content);
	char second_part[256]{};
	strcpy(second_part, op + 1);
	first_number = strtod(first_part, NULL);
	second_number = strtod(second_part, NULL);
	double result = 0;
	switch (*op)
	{
	case '+':
		result = first_number + second_number;
		break;
	case '-':
		result = first_number - second_number;
		break;
	case '*':
		result = first_number * second_number;
		break;
	case '/':
		if (second_number != 0)
		{
			result = first_number / second_number;
		}
		else
		{
			//Что бы сюда написать? Пусть пока будет ноль.
			result = 0;
			//return sz_error;
		}
		break;
	}
	CONST INT RESULT_SIZE = 30;
	char result_str[RESULT_SIZE];
	snprintf(result_str, sizeof(result_str), "%f", result);
	//return result_str;
	//Удаляем нули
	//НЕ РАБОТАЕТ (НУ ПОЧЕМУ...)
	char* dot = strchr(result_str, '.');

	/*if (dot)
	{
		int dotIndex = dot - result_str;
		for (size_t i = RESULT_SIZE - 1; i >= dotIndex; i--)
		{
			if (i == dotIndex || result_str[i] == '0')
			{
				result_str[i] = '\0';
			}
			else
			{
				break;
			}
		}

	}*/
	return result_str;

}
VOID PushButton(HWND parent, int id)
{
	HWND hButton = GetDlgItem(parent, id);
	SendMessage(hButton, BM_SETSTATE, TRUE, 0);
	Sleep(100);
	SendMessage(hButton, BM_SETSTATE, FALSE, 0);
}