#include <Windows.h>
#include <string.h>
#include <cstdio>
#include "resource.h"
CONST CHAR* values[] = { "This", "is", "my", "first", "Combo", "Box" };
BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
	return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		HWND hCombo = GetDlgItem(hwnd, IDC_COMBO1);
		for (int i = 0; i < sizeof(values) / sizeof(values[0]); i++)
		{
			SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)values[i]);
		}
		//SendMessage(hCombo, CB_SETCURSEL, 0, 0);
	}
	break;
	case WM_COMMAND:

		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			//МОЕ
			//CONST INT SIZE = 256;
			//CHAR output[SIZE]{ "Вы не выбрали какой-либо пункт." };
			//HWND hCB = GetDlgItem(hwnd, IDC_COMBO1);
			//INT index = SendMessage(hCB, CB_GETCURSEL, 0, 0);

			//if (index != CB_ERR)
			//{
			//	CHAR indexStr[20];
			//	CHAR selectedItem[SIZE];
			//	SendMessage(hCB, CB_GETLBTEXT, index, (LPARAM)selectedItem); //GET LIST BOX TEXT
			//	strcpy_s(output, SIZE, "Вы выбрали пункт №");
			//	_itoa_s(index, indexStr, 10);
			//	strcat_s(output, SIZE, indexStr);
			//	strcat_s(output, SIZE, " с содержимым ");
			//	strcat_s(output, SIZE, selectedItem);
			//}
			//
			//MessageBox(hwnd, output, "Output", MB_OK);
			//break;
			//НЕ МОЕ
			HWND hCombo = GetDlgItem(hwnd, IDC_COMBO);
			INT i = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
			if (i != CB_ERR)
			{
				CONST INT SIZE = 256;
				CHAR sz_buffer[SIZE]{};
				CHAR sz_message[SIZE]{};
				SendMessage(hCombo, CB_GETLBTEXT, i, (LPARAM)sz_buffer);
				sprintf(sz_message, "Вы выбрали пункт №%i со значением %s", i, sz_buffer);
				MessageBox(hwnd, sz_message, "Your choice", MB_OK | MB_ICONINFORMATION);
			}
			else
			{
				MessageBox(hwnd, "Для начала сделайте ваш выбор", "Warning", MB_OK | MB_ICONWARNING);
			}
			
			break;
		}
		case IDCANCEL:
			EndDialog(hwnd, 0);
		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}