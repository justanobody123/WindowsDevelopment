#include <Windows.h>
#include <cstdio>
#include "resource.h"

CONST CHAR* g_kszaInitListBox[] = { "This", "is", "my", "first", "List", "Box" };//���� �����-��, �� g_ - ������, k - ���������, sza - ������ ����-��������������� �����

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK GetInput(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, INT nCmdShow)
{
    DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc, 0);
    return 0;
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  //  static HINSTANCE hInst;
    static CHAR buffer[256]{};

    switch (uMsg)
    {
    case WM_INITDIALOG:
    {
        HWND hListBox = GetDlgItem(hwnd, IDC_LIST1);
        for (int i = 0; i < sizeof(g_kszaInitListBox) / sizeof(g_kszaInitListBox[0]); i++)
        {
            SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)g_kszaInitListBox[i]);
        }
        break;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK:
        {
            HWND hListBox = GetDlgItem(hwnd, IDC_LIST1);
            INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
            if (i == LB_ERR)
            {
                MessageBox(hwnd, "�������� �����-���� �����.", "Warning", MB_OK | MB_ICONWARNING);
            }
            else
            {
                CONST INT SIZE = 256;
                CHAR result[SIZE]{};
                CHAR listItem[SIZE]{};
                SendMessage(hListBox, LB_GETTEXT, i, (LPARAM)listItem);
                sprintf(result, "�� ������� ����� �%i �� ��������� %s", i, listItem);
                MessageBox(hwnd, result, "Your choice", MB_OK | MB_ICONINFORMATION);
            }
            break;
        }
        case IDC_BUTTON_ADD:
        {
            HINSTANCE hInst = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE); //���������� ������������� ���������� (?) ��� �������� ��������� ����
            if (DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hwnd, (DLGPROC)GetInput, (LPARAM)buffer) == IDOK1)//�������� ��������� ���� ��� ������ � �������� �������� ���� ��
            {
                HWND hListBox = GetDlgItem(hwnd, IDC_LIST1);
                SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)buffer);
            }
            break;
        }
        case IDC_BUTTON_DELETE:
        {
            HWND hListBox = GetDlgItem(hwnd, IDC_LIST1);
            INT i = SendMessage(hListBox, LB_GETCURSEL, 0, 0);
            if (i != LB_ERR)
            {
                SendMessage(hListBox, LB_DELETESTRING, i, 0);
            }
            else
            {
                MessageBox(hwnd, "�������� �����-���� ����� ��� ��������.", "Warning", MB_OK | MB_ICONWARNING);
            }
            break;
        }
        case IDC_CHECK_SORT:
            //�� ���� ����� �������� ������� � �����. �� ���� ����������?
            break;
        case IDCANCEL:
            EndDialog(hwnd, 0);
            break;
        }
        break;
    case WM_CLOSE:
        EndDialog(hwnd, 0);
        break;
    }
    return FALSE;
}

BOOL CALLBACK GetInput(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static CHAR* pBuffer;

    switch (uMsg)
    {
    case WM_INITDIALOG:
        pBuffer = (CHAR*)lParam; //�������� ��������� �� ����� �� lParam
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDOK1:
        {
            HWND hEdit = GetDlgItem(hwnd, IDC_EDIT1);
            SendMessage(hEdit, WM_GETTEXT, 256, (LPARAM)pBuffer);
            if (!strlen(pBuffer))
            {
                MessageBox(hwnd, "����� ������ �� ������ ���� ������", "Warning", MB_OK | MB_ICONWARNING);
            }
            else
            {
                EndDialog(hwnd, IDOK1); //���������� ��� ������ ��, ���������� ������������ ���� �� �������� ����� 
            }
            break;
        }
        case IDCANCEL:
            EndDialog(hwnd, 0);
            break;
        }
        break;
    case WM_CLOSE:
        EndDialog(hwnd, 0);
        break;
    }
    return FALSE;
}
