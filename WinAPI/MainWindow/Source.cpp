#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include "Resource.h"
#include <cstdio>
//#define TEXT_ELEMENTS
#ifdef TEXT_ELEMENTS
#define IDC_STATIC_TEXT 1000
#define IDC_EDIT 1001
#define IDC_BUTTON 1002  
#endif // TEXT_ELEMENTS

#define IDC_COMBO 1001
#define IDC_STATIC 1000
#define IDC_BUTTON 1002


CONST CHAR g_sz_WINDOW_CLASS[] = "My first Window"; //Имя нашего класса окна
INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//Регистрация класса окна:
	WNDCLASSEX wc; //Создаем экземпляр структуры WNDCLASSEX, описывающей класс окна
	ZeroMemory(&wc, sizeof(wc)); //Обнуляем экземпляр 
	//Инициализируем поля объекта:
	//Инициализируем размеры
	wc.style = 0;
	wc.cbSize = sizeof(wc);
	wc.cbWndExtra = 0;
	wc.cbClsExtra = 0;
	//Внешний вид
	//wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	//wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIcon = (HICON)LoadImage(hInstance, "palm_beach_travel_vacation_leisure_icon-icons.com_53586.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wc.hIconSm = (HICON)LoadImage(hInstance, "shell_icon-icons.com_51219.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	//wc.hCursor = LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1));
	wc.hCursor = (HCURSOR)LoadImage(hInstance, "Working In Background.ani", IMAGE_CURSOR, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	//Основные параметры класса
	wc.lpszClassName = g_sz_WINDOW_CLASS;
	wc.lpszMenuName = NULL;
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.hInstance = hInstance; //Дескриптор приложения (запущеный экземпляр *.exe-файла нашего приложения)
	//Класс окна создан, теперь нужно зарегистрировать его в операционной системе
	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Class registration failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	//Создание окна:
	int screen_width = GetSystemMetrics(SM_CXSCREEN), screen_height = GetSystemMetrics(SM_CYSCREEN); //Получаем размеры экрана
	int window_width = screen_width / 100.0 * 75, window_height = screen_height / 100.0 * 75; //Вычисляем размеры окна
	CHAR window_title[MAX_PATH]{};
	sprintf(window_title, "Ширина: %i; Высота: %i; х: %i; y: %i;", window_width, window_height, (screen_width - window_width) / 2, (screen_height - window_height) / 2);
	HWND hwnd = CreateWindowEx
	(
		NULL, //dwExStyle - расширенные стили 
		g_sz_WINDOW_CLASS, //Window class - класс окна
		window_title, //Window title - заголовок окна
		WS_OVERLAPPEDWINDOW, //WS_ - windows style, WS_OVERLAPPEDWINDOW - стиль главного окна, которое будет родителем для других окон
		(screen_width - window_width) / 2, (screen_height - window_height) / 2, //Позиция окна
		window_width, window_height, //Размер окна
		NULL, //Родительское окно
		NULL, //hMenu (Строка меню — ряд кнопок под строкой заголовка: Файл, Правка, Вид, Вставка и прочее) — это для главного окна
		//Для дочернего окна это его айди, по которому его можно найти при помощи функции GetDlgItem()
		hInstance,
		NULL
	);
	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window creation failed", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow); //Задает режим отображения окна: Свернуто в окно, развернуто во весь экран, свернуто в панель задач
	UpdateWindow(hwnd);
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	//Запуск цикла сообщений

	return 0;
}






INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE://INITDIALOG
	{
#ifdef TEXT_ELEMENTS
		HWND HStatic = CreateWindowEx
		(
			NULL, //Расширенные стили
			"Static", //Имя класса
			"This is static text", //Заголовок
			WS_CHILDWINDOW | WS_VISIBLE, //
			100, 100, //Позиция х и у
			200, 200, //Размеры
			hwnd, //Родитель
			(HMENU)IDC_STATIC_TEXT, //ID
			NULL,//Если оставить NULL, то возьмет hInstance родительского окна, а так функция для получения GetModuleHandle(NULL)
			NULL
		);
		HWND hEdit = CreateWindowEx
		(
			NULL, "Edit", "Введите текст", WS_CHILD | WS_BORDER | WS_VISIBLE | ES_CENTER, 100, 20, 200, 20, hwnd, (HMENU)IDC_EDIT, NULL, NULL
		);
		/*HWND hEdit1 = CreateWindowEx
		(
			NULL, "Edit", "Введите текст", WS_CHILD | WS_VISIBLE | ES_CENTER | ES_NOHIDESEL, 100, 60, 200, 20, hwnd, (HMENU)1002, NULL, NULL
		);*/
		HWND hButton = CreateWindowEx
		(
			NULL, "Button", "Применить", WS_CHILD | WS_VISIBLE, 200, 55, 100, 32, hwnd, (HMENU)IDC_BUTTON, NULL, NULL
		);
#endif // TEXT_ELEMENTS
		HWND hStatic = CreateWindowEx(NULL, "Static", "Выберите указатель мыши:", WS_CHILD | WS_VISIBLE, 10, 10, 200, 20, hwnd, (HMENU)IDC_STATIC, NULL, NULL);
		HWND hCombo = CreateWindowEx(NULL, "ComboBox", "", WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 10, 25, 200, 200, hwnd, (HMENU)IDC_COMBO, NULL, NULL);
		HWND hButton = CreateWindowEx(NULL, "Button", "Применить", WS_CHILD | WS_VISIBLE, 110, 50, 100, 28, hwnd, (HMENU)IDC_BUTTON, NULL, NULL);
		CHAR sz_current_dir[MAX_PATH]{};
		SetCurrentDirectory("starcraft-original");
		GetCurrentDirectory(MAX_PATH, sz_current_dir);
		//SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_current_dir);
		WIN32_FIND_DATA data;
		//ZeroMemory(&data, sizeof(data));
		HANDLE hFile = FindFirstFile("*", &data);
		for (; FindNextFile(hFile, &data);)
		{
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)data.cFileName);
		}
		FindClose(hFile);
		break;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
#ifdef TEXT_ELEMENTS
		case IDC_BUTTON:

		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
			HWND hStatic = GetDlgItem(hwnd, IDC_STATIC_TEXT);
			const int SIZE = 256;
			CHAR sz_buffer[SIZE]{};
			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			SendMessage(hStatic, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_buffer);
			break;
		}
#endif // TEXT_ELEMENTS
		case IDC_BUTTON:
		{
			HWND hCombo = GetDlgItem(hwnd, IDC_COMBO);
			int i = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
			CHAR sz_selected_cursor[_MAX_FNAME]{};
			SendMessage(hCombo, CB_GETLBTEXT, i, (LPARAM)sz_selected_cursor);
			HCURSOR hCursor = (HCURSOR)LoadImage(NULL, sz_selected_cursor, IMAGE_CURSOR, SM_CXCURSOR, SM_CYCURSOR, LR_LOADFROMFILE);
			SetCursorPos(300, 300);

			SetCursor(hCursor);
			return FALSE;
		}
			break;
		case IDC_COMBO:
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		/*switch(MessageBox(hwnd, "Вы дейтвительно хотите закрыть окно?", "Че, серьезно?", MB_YESNO | MB_ICONQUESTION))
		{
		case IDYES:
			MessageBox(hwnd, "Лучше двери закройте", "Info", MB_OK | MB_ICONINFORMATION);
			DestroyWindow(hwnd); 
			break;
		case IDNO: 
			break;
		}		*/
		DestroyWindow(hwnd);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}