#include <Windows.h>
#include "Buttons.h"
CONST CHAR g_sz_WINDOW_CLASS[] = "Calc_PD_311";
INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
	//1) ����������� ������ ����
	//2) �������� ����
	//3) ������ ����� ��������� ���������
	WNDCLASSEX wClass;
	ZeroMemory(&wClass, sizeof(wClass)); // �������� ��������� ���������
	wClass.style = 0; 
	wClass.cbSize = sizeof(wClass); // cb - Count bytes
	wClass.cbWndExtra = 0; //������ ����� ��� ���� � �������� �����
	wClass.cbClsExtra = 0;//������ ����� ��� ������ � �������� �����
	wClass.hIcon = 
	return 0;
}
INT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		break;
	case WM_COMMAND:
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