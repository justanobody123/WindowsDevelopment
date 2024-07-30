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
		SendMessage(GetDlgItem(hwnd, IDC_EDIT_LOGIN), WM_SETTEXT, 0, (LPARAM)"Введите логин");
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
		case IDC_BUTTON_COPY:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
			HWND hEditPassword = GetDlgItem(hwnd, IDC_EDIT_PASSWORD);
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE]{};
			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			SendMessage(hEditPassword, WM_SETTEXT, 0, (LPARAM)sz_buffer);
		}
			break;
		case IDC_EDIT_LOGIN:
		{
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT_LOGIN);
			CONST INT SIZE = 256;
			CHAR sz_buffer[SIZE]{};
			SendMessage(hEdit, WM_GETTEXT, SIZE, (LPARAM)sz_buffer);
			if (HIWORD(wParam) == EN_SETFOCUS)
			{
				if (!strcmp(sz_buffer, "Введите логин"))
				{
					SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"");
				}
			}
			else if (HIWORD(wParam) == EN_KILLFOCUS)
			{
				if (strlen(sz_buffer) == 0)
				{
					SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)"Введите логин");
				}
			}
			break;
		}
		}
		break;
	case WM_CLOSE://Отрабатывает при нажатии на крестик (закрыть)
		EndDialog(hwnd, 0);
	}
	return FALSE;
}
