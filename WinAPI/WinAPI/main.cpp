#include <Windows.h>
#include "resource.h"
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//MessageBox(NULL, "HelloWorld!\nЭто окно сообщения, детка", "Hello WinAPI", MB_YESNOCANCEL | MB_ICONQUESTION | MB_DEFBUTTON3 | MB_HELP | MB_TOPMOST);
	//std::cout << typeid(char).name() << std::endl;
	//std::cout << typeid(wchar_t).name() << std::endl;
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
	return 0;
}
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//hwnd - Handler to Window (дескриптор окна)
	switch (uMsg)
	{
		//WM - Windows Message
	case WM_INITDIALOG://Отрабатывает толкьо один раз при создании окна
	{
		//H — Handler (обработчик, дескриптор)
		HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_BTC));
		SendMessage(hwnd, WM_SETICON, 0, (LPARAM)hIcon);
		break;
	}
	case WM_COMMAND://Здесь обрабатываются нажатия на кнопки, ввод текста и любые изменения состояния окна
		switch (LOWORD(wParam))
		{
		case IDOK: 
			MessageBox(hwnd, "Была нажата кнопка ОК", "Info", MB_OK | MB_ICONINFORMATION);
			break;
		case IDCANCEL:
			EndDialog(hwnd, 0);
			break;
		}
		break;
	case WM_CLOSE://Отрабатывает при нажатии на крестик (закрыть)
		EndDialog(hwnd, 0);
	}
	return false;
}
