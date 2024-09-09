#undef UNICODE
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include "resource.h"
#include <iostream>
#include <Richedit.h>

using std::cout;
using std::endl;

CONST CHAR g_sz_WINDOW_CLASS[] = "TextEditorPD_311";
CHAR* FormatLastError();
INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL LoadTextFileToEdit(HWND hEdit, LPSTR lpszFileName);
BOOL SaveTextFileFromEdit(HWND hEdit, LPSTR lpszFileName);


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nCmdShow)
{

	AllocConsole();
	freopen("CONOUT$", "w", stdout);
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
		NULL,
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
	static HINSTANCE hRichEdit = LoadLibrary("riched20.dll");
	switch (uMsg)
	{
	case WM_CREATE:
	{

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
	}
	break;
	case WM_SIZE:
	{
		HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
		RECT client;
		GetClientRect(hwnd, &client);
		MoveWindow(hEdit, 10, 10, client.right - 20, client.bottom - 20, TRUE);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_FILE_OPEN:
		{
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			CHAR szFileName[MAX_PATH]{};

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "Text files: (*.txt)\0*.txt\0All files (*.*)\0*.*\0";
			ofn.lpstrDefExt = "txt";
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn.lpstrFile = szFileName;
			if (GetOpenFileName(&ofn))
			{
				HWND hEdit = GetDlgItem(hwnd, IDC_EDIT);
				LoadTextFileToEdit(hEdit, szFileName);
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
		}
		break;
	case WM_DESTROY:
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

BOOL LoadTextFileToEdit(HWND hEdit, LPSTR lpszFileName)
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
		}
	}
	return bSuccess;
}
BOOL SaveTextFileFromEdit(HWND hEdit, LPSTR lpszFileName)
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
	}
	return bSucces;
}