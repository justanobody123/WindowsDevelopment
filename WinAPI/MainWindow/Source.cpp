#include <Windows.h>
#include "Resource.h"
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
	HWND hwnd = CreateWindowEx
	(
		NULL, //dwExStyle - расширенные стили 
		g_sz_WINDOW_CLASS, //Window class - класс окна
		g_sz_WINDOW_CLASS, //Window title - заголовок окна
		WS_OVERLAPPEDWINDOW, //WS_ - windows style, WS_OVERLAPPEDWINDOW - стиль главного окна, которое будет родителем для других окон
		CW_USEDEFAULT, CW_USEDEFAULT, //Позиция окна
		CW_USEDEFAULT, CW_USEDEFAULT, //Размер окна
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
		break;
	case WM_COMMAND:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}