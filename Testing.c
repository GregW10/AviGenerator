//
// Created by mario on 18/07/2022.
//

#include <windows.h>

HWND hBMPWnd, hButton;
HBITMAP hBMP;

#define BMP_ID 3
#define BTN_ID 7

const char BMP_PATH[] = "GregoriaLuchoRojo.bmp";

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            HINSTANCE hInst = GetModuleHandle(NULL);
            hBMPWnd = CreateWindow("STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, 100, 100, 250, 200, hWnd, (HMENU) BMP_ID, hInst, NULL);
            hButton = CreateWindow("Button", "Resize", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 0, 0, 50, 20, hWnd, (HMENU) BTN_ID, hInst, NULL);
            hBMP = (HBITMAP) LoadImage(NULL, BMP_PATH, IMAGE_BITMAP, 250, 200, LR_LOADFROMFILE);
            SendMessage(hBMPWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM) hBMP);
            break;
        }
        case WM_PAINT: {
            HBRUSH hbrush = CreateSolidBrush(RGB(255, 255, 255));
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            FillRect(hdc, &ps.rcPaint, hbrush);
            EndPaint(hWnd, &ps);
            ReleaseDC(hWnd, hdc);
            DeleteObject(hbrush);
            break;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) == BTN_ID) {
                if (HIWORD(wParam) == BN_CLICKED) {
                    HDC hdc = GetDC(hWnd);
                }
            }
            break;
        }
        case WM_RBUTTONDOWN: {
            char buffer[MAX_PATH];
            HMODULE hModule = GetModuleHandle(NULL);
            GetModuleFileName(hModule, buffer, MAX_PATH);
            MessageBox(hWnd, buffer, "Program File Path", MB_OK | MB_ICONINFORMATION);
            break;
        }
        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    const char WindClsName[] = "Video Window Class";

    WNDCLASS wc = {};
    HWND hWnd;
    MSG msg;

    wc.lpszClassName = WindClsName;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;

    ATOM retval = RegisterClass(&wc);
    if (retval == 0) {
        MessageBox(NULL, "Error registering window class.", "Error!", MB_OK | MB_ICONEXCLAMATION);
        return -1;
    }

    hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, WindClsName, "Test", WS_OVERLAPPEDWINDOW, 100, 100, 1000, 600, NULL,
                          NULL, hInstance, NULL);
    if (hWnd == NULL) {
        MessageBox(NULL, "Error creating window.", "Error!", MB_OK | MB_ICONEXCLAMATION);
        return -1;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}