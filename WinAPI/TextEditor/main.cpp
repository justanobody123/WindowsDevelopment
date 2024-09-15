#undef UNICODE
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include "resource.h"
#include <iostream>
#include <Richedit.h>
#include <string.h>
#include <CommCtrl.h>

using std::cout;
using std::endl;

CONST CHAR g_sz_WINDOW_CLASS[] = "TextEditorPD_311";
CHAR* FormatLastError();
INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL LoadTextFileToEdit(HWND hEdit, LPSTR lpszFileName, CHAR sz_title[], BOOL beenChanged);
BOOL SaveTextFileFromEdit(HWND hEdit, LPSTR lpszFileName, CHAR sz_title[], BOOL beenChanged);
LPSTR FormatFileTime(FILETIME fileTime, CONST CHAR sz_message[], CHAR sz_buffer[]);
VOID SetFileDataToStatusBar(HWND hwnd, CONST CHAR szFileName[], CHAR sz_title[], BOOL beenChanged);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{

	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	system("chcp 1251");
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(WNDCLASSEX));
	wClass.style = 0;
	wClass.cbSize = sizeof(WNDCLASSEX);
	wClass.cbClsExtra = 0;
	wClass.cbWndExtra = 0;

	wClass.hIcon = (HICON)LoadImage(hInstance, "ICO\\text_icon.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wClass.hIconSm = (HICON)LoadImage(hInstance, "ICO\\text_icon.ico", IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_LOADFROMFILE);
	wClass.hCursor = (HCURSOR)LoadCursor(hInstance, IDC_ARROW);
	HBITMAP hBackground = (HBITMAP)LoadImage(hInstance, "IMG\\background.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	wClass.hbrBackground = CreatePatternBrush(hBackground);

	wClass.hInstance = hInstance;
	wClass.lpfnWndProc = (WNDPROC)WndProc;
	wClass.lpszMenuName = MAKEINTRESOURCE(IDR_MAIN_MENU);
	wClass.lpszClassName = g_sz_WINDOW_CLASS;

	if (!RegisterClassEx(&wClass))
	{
		MessageBox(NULL, "Class registration failed!", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	HWND hwnd = CreateWindowEx
	(
		WS_EX_ACCEPTFILES,
		g_sz_WINDOW_CLASS,
		g_sz_WINDOW_CLASS,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	std::cout << FormatLastError() << std::endl;
	if (!hwnd)
	{
		MessageBox(NULL, "Window creation failed!", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg;
	while (GetMessage(&msg, hwnd, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	FreeConsole();
	return 0;
}

INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HINSTANCE comCtrl = LoadLibrary("ComCtlr32.dll");
	static HINSTANCE hRichEdit = LoadLibrary("riched20.dll");
	static INITCOMMONCONTROLSEX icce;
	static CHAR szFileName[MAX_PATH] = "";
	static BOOL beenChanged = FALSE;
	static CHAR sz_title[MAX_PATH]{};
	switch (uMsg)
	{
	case WM_CREATE:
	{
		//DragAcceptFiles(hwnd, TRUE);
		RECT client;
		RECT window;
		GetWindowRect(hwnd, &window);
		GetClientRect(hwnd, &client);
		std::cout << "Window: " << window.left << "\t" << window.top << "\t" << window.right << "\t" << window.bottom << std::endl;
		std::cout << "Client: " << client.left << "\t" << client.top << "\t" << client.right << "\t" << client.bottom << std::endl;
		HWND hEdit = CreateWindowEx(NULL, RICHEDIT_CLASS, "Workspace",
			WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL,
			0, 0,
			window.right - window.left,
			window.bottom - window.top,
			hwnd,
			(HMENU)IDC_EDIT,
			NULL,
			NULL
		);
		SendMessage(hEdit, EM_SETEVENTMASK, 0, ENM_CHANGE);
		HWND hStatus = CreateWindowEx(NULL, STATUSCLASSNAME, "Status Bar", WS_CHILD | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hwnd, (HMENU)IDC_STATUS, NULL, NULL);
		//1) File path
		//2) Save status
		//3) Number of Words
		//4) Window Size
		//5) File size
		//6) Creation date
		//7) Date of last change
		INT dimensions[] = { 300, 400, 500, 580, 650, 900, -1 };
		SendMessage(hStatus, SB_SETPARTS, sizeof(dimensions) / sizeof(dimensions[0]), (LPARAM)dimensions);
	}
	break;
	case WM_DROPFILES:
	{
		cout << "Case WM_DROPFILES code: " << uMsg <<  endl;
		BOOL cancel = FALSE;
		if (beenChanged)
		{

			switch (MessageBox(hwnd, "Сохранить изменения в файле?", "Файл был изменен", MB_YESNOCANCEL | MB_ICONQUESTION))
			{
			case IDYES: SendMessage(hwnd, WM_COMMAND, ID_FILE_SAVE, 0);
			case IDNO:
				break;
			case IDCANCEL:
				cancel = TRUE;
				break;
			}
		}
		if (cancel) break;
		HDROP hDrop = (HDROP)wParam;
		if (DragQueryFile(hDrop, 0, szFileName, MAX_PATH))
		{
			
			LoadTextFileToEdit(GetDlgItem(hwnd, IDC_EDIT), szFileName, sz_title, FALSE);
			beenChanged = FALSE;
			SendMessage(GetDlgItem(hwnd, IDC_STATUS), SB_SETTEXT, 0, (LPARAM)szFileName);
			
			sprintf(sz_title, "%s - %s", g_sz_WINDOW_CLASS, strrchr(szFileName, '\\') + 1);
			SetWindowText(hwnd, sz_title);
		}
		DragFinish(hDrop);
		cout << "DragFinish" << endl;
	}
	break;
	case WM_SIZE:
	{
		HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
		RECT client;
		RECT window;
		RECT status;
		GetWindowRect(hwnd, &window);
		GetClientRect(hwnd, &client);
		GetWindowRect(GetDlgItem(hwnd, IDC_STATUS), &status);
		DWORD dwStatusHeight = status.bottom - status.top;
		MoveWindow(hEdit, 10, 10, client.right - 20, client.bottom - 20 - dwStatusHeight, TRUE);
		MoveWindow(GetDlgItem(hwnd, IDC_STATUS), 0, 0, 0, 0, TRUE);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILE_OPEN:
		{
			cout << "File open case: " << endl;
			BOOL cancel = FALSE;
			if (beenChanged)
			{
				
				switch (MessageBox(hwnd, "Сохранить изменения в файле?", "Файл был изменен", MB_YESNOCANCEL | MB_ICONQUESTION))
				{
				case IDYES: SendMessage(hwnd, WM_COMMAND, ID_FILE_SAVE, 0);
				case IDNO:
					break;
				case IDCANCEL:
					cancel = TRUE;
					break;
				}
			}
			if (cancel) break;
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "Text files: (*.txt)\0*.txt\0C Plus Plus files (*.cpp | *.h)\0*.ccp;*.h\0All files (*.*)\0*.*\0";
			ofn.lpstrDefExt = "txt";
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn.lpstrFile = szFileName;
			if (GetOpenFileName(&ofn))
			{
				HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
				LoadTextFileToEdit(hEdit, szFileName, sz_title, FALSE);//Фолз, потому что статическая переменная меняется после попадания в функцию на тру, хотя там она используется лишь в ифе. (Это правда, я не сумасшедшая)
				beenChanged = FALSE;
				SendMessage(GetDlgItem(hwnd, IDC_STATUS), SB_SETTEXT, 0, (LPARAM)sz_title);
			}
		}
		break;
		case ID_FORMAT_WORDWRAP:
		{
			HMENU hMenu = GetMenu(hwnd);
			HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
			LONG style = GetWindowLong(hEdit, GWL_STYLE);
			if (GetMenuState(hMenu, ID_FORMAT_WORDWRAP, MF_BYCOMMAND) == MF_CHECKED)
			{
				CheckMenuItem(hMenu, ID_FORMAT_WORDWRAP, MF_UNCHECKED);
				style = WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | WS_HSCROLL;
			}
			else
			{
				CheckMenuItem(hMenu, ID_FORMAT_WORDWRAP, MF_CHECKED);
				style = WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL;
			}
			SetWindowLongPtr(hEdit, GWL_STYLE, style);
			InvalidateRect(hwnd, NULL, TRUE);
			UpdateWindow(hwnd);
		}
		break;
		case ID_FILE_SAVE:
		{
			//ЗДЕСЬ НЕТ ТРАНСФОРМАЦИИ ИМЕНИ ФАЙЛА В ПОЛНЫЙ ПУТЬ
			if (strlen(szFileName) != 0 && !strlen(szFileName) != 1)
			{
				beenChanged = FALSE;
				SaveTextFileFromEdit(GetDlgItem(hwnd, IDC_EDIT), szFileName, sz_title, beenChanged);
				SetWindowText(hwnd, sz_title);
			}
			else
			{
				SendMessage(hwnd, WM_COMMAND, LOWORD(ID_FILE_SAVEAS), 0);
			}
		}
		break;
		case ID_FILE_SAVEAS:
		{
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "Text files: (*.txt)\0*.txt\0C Plus Plus files (*.cpp | *.h)\0*.cpp;*.h\0All files: (*.*)\0*.*\0";
			//https://stackoverflow.com/questions/39130256/mfc-openfilename-structure-m-ofn-lpstrcustomfilter
			/*To specify multiple filter patterns for a single display string,
			use a semicolon to separate the patterns (for example, "*.TXT;*.DOC;*.BAK").*/
			ofn.lpstrDefExt = "txt";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
			if (GetSaveFileName(&ofn))
			{
				beenChanged = FALSE;
				SaveTextFileFromEdit(GetDlgItem(hwnd, IDC_EDIT), szFileName, sz_title, beenChanged);
				SendMessage(GetDlgItem(hwnd, IDC_STATUS), SB_SETTEXT, 0, (LPARAM)ofn.lpstrFile);
				sprintf(sz_title, "%s - %s", g_sz_WINDOW_CLASS, strrchr(szFileName, '\\') + 1);
				std::cout << sz_title << std::endl;
				SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_title);
			}
		}
		break;
		case IDC_EDIT:
		{
			if (HIWORD(wParam) == EN_CHANGE)//If multiline was changed through WM_SETTEXT this case won't work;
			{
				if (!beenChanged)
				{
					beenChanged = TRUE;
					//Добавляем к имени файла звездочку
					strcat(sz_title, "*");
					SetWindowText(hwnd, sz_title);
					SendMessage(GetDlgItem(hwnd, IDC_STATUS), SB_SETTEXT, 1, (LPARAM)"Изменен");
				}
				//cout << "File was changed" << endl;
				HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
				DWORD dwTextLength = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
				LPSTR lpstrBuffer = (LPSTR)GlobalAlloc(GPTR, dwTextLength);
				SendMessage(hEdit, WM_GETTEXT, dwTextLength + 1, (LPARAM)lpstrBuffer);
				CHAR delimiters[] = " ,.!?;-()[]<>{}\"\':\\/\n";
				int i = 0;
				for (char* pch = strtok(lpstrBuffer, delimiters); pch; pch = strtok(NULL, delimiters))i++;
				GlobalFree(lpstrBuffer);
				CHAR sz_status[MAX_PATH]{};
				sprintf(sz_status, "%i %s", i, "слов");
				SendMessage(GetDlgItem(hwnd, IDC_STATUS), SB_SETTEXT, 2, (LPARAM)sz_status);
				sprintf(sz_status, "%s %i", "длина: ", dwTextLength);
				SendMessage(GetDlgItem(hwnd, IDC_STATUS), SB_SETTEXT, 3, (LPARAM)sz_status);

				
			}
		}
		break;
		}
		break;
	case WM_DESTROY:
		FreeLibrary(comCtrl);
		FreeLibrary(hRichEdit);
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return FALSE;
}
CHAR* FormatLastError()
{
	DWORD errorID = GetLastError();
	LPSTR lpszBuffer = NULL;
	DWORD dwSize = FormatMessage
	(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorID,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_RUSSIAN_RUSSIA),
		(LPSTR)&lpszBuffer,
		0,
		NULL
	);
	return lpszBuffer;
}

BOOL LoadTextFileToEdit(HWND hEdit, LPSTR lpszFileName, CHAR sz_title[], BOOL beenChanged)
{
	BOOL bSuccess = FALSE;
	HANDLE hFile = CreateFile
	(
		lpszFileName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		0,
		0
	);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize = GetFileSize(hFile, NULL);
		if (dwFileSize != UINT_MAX)
		{
			LPSTR lpszFileText = (LPSTR)GlobalAlloc(GPTR, dwFileSize + 1);
			if (lpszFileText)
			{
				DWORD dwRead = 0;
				if (ReadFile(hFile, lpszFileText, dwFileSize, &dwRead, NULL))
				{
					if (SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)lpszFileText))
					{
						bSuccess = TRUE;
					}
				}
				GlobalFree(lpszFileText);
			}
			CloseHandle(hFile);
			SetFileDataToStatusBar(GetParent(hEdit), lpszFileName, sz_title, beenChanged);
		}
	}
	return bSuccess;
}
BOOL SaveTextFileFromEdit(HWND hEdit, LPSTR lpszFileName, CHAR sz_title[], BOOL beenChanged)
{
	BOOL bSucces = FALSE;
	HANDLE hFile = CreateFile(lpszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwTextLength = SendMessage(hEdit, WM_GETTEXTLENGTH, 0, 0);
		if (dwTextLength)
		{
			LPSTR lpszText = (LPSTR)GlobalAlloc(GPTR, dwTextLength + 1);
			if (lpszText)
			{
				if (SendMessage(hEdit, WM_GETTEXT, dwTextLength + 1, (LPARAM)lpszText))
				{
					DWORD dwWritten = 0;
					if (WriteFile(hFile, lpszText, dwTextLength, &dwWritten, NULL))
					{
						bSucces = TRUE;
					}
				}
				GlobalFree(lpszText);
			}
		}
		CloseHandle(hFile);
		SetFileDataToStatusBar(GetParent(hEdit), lpszFileName, sz_title, beenChanged);
	}
	return bSucces;
}
LPSTR FormatFileTime(FILETIME fileTime,  CONST CHAR sz_message[], CHAR sz_buffer[])
{
	ZeroMemory(sz_buffer, MAX_PATH);
	FILETIME localTime;
	ZeroMemory(&localTime, sizeof(localTime));
	FileTimeToLocalFileTime(&fileTime, &localTime);
	SYSTEMTIME sysTime;
	ZeroMemory(&sysTime, sizeof(sysTime));
	FileTimeToSystemTime(&localTime, &sysTime);
	//ZeroMemory(sz_buffer, MAX_PATH);
	sprintf(sz_buffer, "%s%02d.%02d.%02d %02d:%02d:%02d", sz_message,
		sysTime.wYear, sysTime.wMonth, sysTime.wDay,
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	return sz_buffer;
}
VOID SetFileDataToStatusBar(HWND hwnd, CONST CHAR szFileName[], CHAR sz_title[], BOOL beenChanged)
{
	sprintf(sz_title, "%s - %s", g_sz_WINDOW_CLASS, strrchr(szFileName, '\\') + 1);
	cout << sz_title << endl;
	SendMessage(hwnd, WM_SETTEXT, 0, (LPARAM)sz_title);
	WIN32_FIND_DATA fileData;
	ZeroMemory(&fileData, sizeof(fileData));
	HANDLE hFile = FindFirstFile(szFileName, &fileData);
	cout << "\n==================================\n\n";
	cout << fileData.cFileName << "\t" << fileData.nFileSizeLow << "\t\n";
	cout << "\n==================================\n\n";
	CHAR sz_buffer[MAX_PATH]{};
	if (beenChanged)
	{
		SendMessage(GetDlgItem(hwnd, IDC_STATUS), SB_SETTEXT, 1, (LPARAM)"Изменен");
	}
	else
	{
		SendMessage(GetDlgItem(hwnd, IDC_STATUS), SB_SETTEXT, 1, (LPARAM)"Сохранен");
	}
	sprintf(sz_buffer, "%i B", fileData.nFileSizeLow);
	SendMessage(GetDlgItem(hwnd, IDC_STATUS), SB_SETTEXT, 4, (LPARAM)sz_buffer);
	SendMessage(GetDlgItem(hwnd, IDC_STATUS), SB_SETTEXT, 5,
		(LPARAM)FormatFileTime(fileData.ftCreationTime, "Дата создания: ", sz_buffer));
	SendMessage(GetDlgItem(hwnd, IDC_STATUS), SB_SETTEXT, 6,
		(LPARAM)FormatFileTime(fileData.ftLastWriteTime, "Дата изменения: ", sz_buffer));

}
