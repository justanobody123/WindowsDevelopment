#include <Windows.h>
#include "resource.h"
#include <string.h>

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
	}
	break;
	case WM_COMMAND:

		switch (LOWORD(wParam))
		{
		case IDOK:
		{
			CONST INT SIZE = 256;
			CHAR output[SIZE]{ "Вы не выбрали какой-либо пункт." };
			HWND hCB = GetDlgItem(hwnd, IDC_COMBO1);
			INT index = SendMessage(hCB, CB_GETCURSEL, 0, 0);

			if (index != CB_ERR)
			{
				CHAR indexStr[20];
				CHAR selectedItem[SIZE];
				SendMessage(hCB, CB_GETLBTEXT, index, (LPARAM)selectedItem); //GET LIST BOX TEXT
				strcpy_s(output, SIZE, "Вы выбрали пункт №");
				_itoa_s(index, indexStr, 10);
				strcat_s(output, SIZE, indexStr);
				strcat_s(output, SIZE, " с содержимым ");
				strcat_s(output, SIZE, selectedItem);
			}
			
			MessageBox(hwnd, output, "Output", MB_OK);
			break;
		}

		}
		break;
	case WM_CLOSE:
		EndDialog(hwnd, 0);
	}
	return FALSE;
}