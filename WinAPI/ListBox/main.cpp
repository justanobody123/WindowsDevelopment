#include <Windows.h>
#include <cstdio>
#include "resource.h"

CONST CHAR* g_kszaInitListBox[] = { "This", "is", "my", "first", "List", "Box" };//Шиза какая-то, но g_ - глобал, k - константа, sza - массив нуль-терминированных строк

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
                MessageBox(hwnd, "Выберите какой-либо пункт.", "Warning", MB_OK | MB_ICONWARNING);
            }
            else
            {
                CONST INT SIZE = 256;
                CHAR result[SIZE]{};
                CHAR listItem[SIZE]{};
                SendMessage(hListBox, LB_GETTEXT, i, (LPARAM)listItem);
                sprintf(result, "Вы выбрали пункт №%i со значением %s", i, listItem);
                MessageBox(hwnd, result, "Your choice", MB_OK | MB_ICONINFORMATION);
            }
            break;
        }
        case IDC_BUTTON_ADD:
        {
            HINSTANCE hInst = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE); //Дескриптор родительского приложения (?) для создания дочернего окна
            if (DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hwnd, (DLGPROC)GetInput, (LPARAM)buffer) == IDOK1)//Создание дочернего окна для инпута и ожидание возврата кода ОК
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
                MessageBox(hwnd, "Выберите какой-либо пункт для удаления.", "Warning", MB_OK | MB_ICONWARNING);
            }
            break;
        }
        case IDC_CHECK_SORT:
            //Не могу найти валидный месседж в доках. Он хоть существует?
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
        pBuffer = (CHAR*)lParam; //Получаем указатель на буфер из lParam
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
                MessageBox(hwnd, "Пункт списка не должен быть пустым", "Warning", MB_OK | MB_ICONWARNING);
            }
            else
            {
                EndDialog(hwnd, IDOK1); //Возвращаем код кнопки ок, уведомляем родительское окно об успешном вводе 
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
