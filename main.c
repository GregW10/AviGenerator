//
// created by Gregor Hartl Watters on 17/07/2022
//

#ifndef _WIN32
#error Cannot compile on operating system other than Windows.
#else

#include <windows.h>
#include "funcs.h"

#define FILE_STR_LEN 37

long double x_screen;
long double y_screen;

const wchar_t BMP_PATH[] = L"GregoriaLuchoRojo.bmp";

#define MAIN_WINDOW_X (x_screen / 10.0)
#define MAIN_WINDOW_Y (y_screen / 10.0)

#define MAIN_WINDOW_WIDTH (x_screen - 2*MAIN_WINDOW_X)
#define MAIN_WINDOW_HEIGHT (y_screen - 2*MAIN_WINDOW_Y)

#define BUTTON_WIDTH (MAIN_WINDOW_WIDTH / 3.0)
#define BUTTON_HEIGHT (MAIN_WINDOW_HEIGHT / 10.0)

#define BUTTON_X (MAIN_WINDOW_WIDTH/2 - BUTTON_WIDTH/2.0)
#define BUTTON_Y (MAIN_WINDOW_HEIGHT/2 - BUTTON_HEIGHT/2.0)

#define BTN_FONT_HEIGHT (y_screen / 15.0)
#define BTN_FONT_WIDTH (x_screen / 60.0)

#define MAIN_TEXT_WIDTH ((MAIN_WINDOW_WIDTH*8)/10.0)
#define MAIN_TEXT_HEIGHT (MAIN_WINDOW_HEIGHT / 4.0)

#define MAIN_TEXT_X (MAIN_WINDOW_WIDTH/2.0 - MAIN_TEXT_WIDTH/2.0)
#define MAIN_TEXT_Y (MAIN_WINDOW_HEIGHT/10.0)

#define MAIN_TEXT_FONT_HEIGHT (x_screen / 10.0)
#define MAIN_TEXT_FONT_WIDTH (y_screen / 20.0)

HWND hTxt;
#define TEXT_ID 7

HWND hButton;
#define BTN_ID 3

HWND hEdit;
#define EDT_CTL_ID 15

HBITMAP hBMP;

HWND hInitBMPWnd;
#define IBMP_ID 63

HWND hFinalBMPWnd;
#define FBMP_ID 31

wchar_t *filePath = NULL;
size_t timeOffset = 0;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            HINSTANCE hInst = GetModuleHandleW(NULL);
            hTxt = CreateWindow("STATIC", "AVI File Generator", WS_CHILD | WS_VISIBLE | SS_NOTIFY | SS_CENTER,
                                MAIN_TEXT_X, MAIN_TEXT_Y, MAIN_TEXT_WIDTH, MAIN_TEXT_HEIGHT, hWnd, (HMENU) TEXT_ID,
                                hInst, NULL);
            hButton = CreateWindow("Button", "Generate Video", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, BUTTON_X,
                                   BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, hWnd, (HMENU) BTN_ID, hInst,
                                   NULL);
            hEdit = CreateWindow("Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOVSCROLL,
                                 MAIN_WINDOW_WIDTH/10, (MAIN_WINDOW_HEIGHT*8)/10, 200,
                                 20, hWnd, (HMENU) EDT_CTL_ID, hInst, NULL);
            hInitBMPWnd = CreateWindow("STATIC", "Bitmap initial", WS_CHILD | WS_VISIBLE | SS_BITMAP | WS_BORDER,
                                       (MAIN_WINDOW_WIDTH*7)/10.0, (MAIN_WINDOW_HEIGHT*3)/5.0, 0, 0, hWnd,
                                       (HMENU) IBMP_ID, hInst, NULL);
            if (hInitBMPWnd == NULL) {
                MessageBoxW(hWnd, L"Error creating initial BMP static control.", L"Error!", MB_OK | MB_ICONEXCLAMATION);
                DestroyWindow(hWnd);
            }
            long double ratio = getBMPheightToWidth(BMP_PATH);
            hBMP = (HBITMAP) LoadImageW(NULL, BMP_PATH, IMAGE_BITMAP, MAIN_WINDOW_WIDTH/4, (int) ((((double) MAIN_WINDOW_WIDTH)/4.0)*ratio), LR_LOADFROMFILE);
            if (hBMP == NULL) {
                MessageBoxW(hWnd, L"Error loading BMP image.", L"Error!", MB_OK | MB_ICONEXCLAMATION);
                DestroyWindow(hWnd);
            }
            SendMessageW(hInitBMPWnd, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM) hBMP);
            /*
            RECT iBMPRect; GetWindowRect(hInitBMPWnd, &iBMPRect);
            double ratio = ((double) iBMPRect.right - (double) iBMPRect.left) /
                           ((double) iBMPRect.bottom - (double) iBMPRect.top);
            hFinalBMPWnd = CreateWindow("STATIC", "Bitmap final", WS_CHILD | WS_VISIBLE | WS_BORDER | SS_BITMAP |
            SS_NOTIFY, (MAIN_WINDOW_WIDTH*6)/10, (MAIN_WINDOW_HEIGHT*6)/10, MAIN_WINDOW_WIDTH / 4,
            ((double) (MAIN_WINDOW_WIDTH / 4))/ratio, hWnd, (HMENU) FBMP_ID, hInst, NULL);
            RECT fBMPRect; GetWindowRect(hFinalBMPWnd, &fBMPRect);
            HDC initHDC = GetDC(hInitBMPWnd);
            HDC finalHDC = GetDC(hFinalBMPWnd);
            HDC compDC = CreateCompatibleDC(GetDC(hWnd));
            HBITMAP dest = CreateCompatibleBitmap(compDC, MAIN_WINDOW_WIDTH / 4, ((double) (MAIN_WINDOW_WIDTH / 4))/ratio);
            SelectObject(compDC, dest);
            // char what[100] = {0}; sprintf(what, "GetDeviceCaps(): %d", GetDeviceCaps(finalHDC, RC_STRETCHBLT));
            // MessageBoxA(hWnd, what, "Error!", MB_OK);
            if (!StretchBlt(compDC, (MAIN_WINDOW_WIDTH*6)/10, (MAIN_WINDOW_HEIGHT*6)/10, MAIN_WINDOW_WIDTH / 4,
                       ((double) (MAIN_WINDOW_WIDTH / 4))/ratio, initHDC, (MAIN_WINDOW_WIDTH*8)/10,
                       (MAIN_WINDOW_HEIGHT*3)/5, iBMPRect.right - iBMPRect.left, iBMPRect.bottom - iBMPRect.top,
                       SRCCOPY)) {
                MessageBoxW(hWnd, L"Error displacing BMP image.", L"Error!", MB_OK | MB_ICONEXCLAMATION);
                DestroyWindow(hWnd);
            }
            if (initHDC == NULL) {
                MessageBoxW(hWnd, L"Error retrieving HDC of the initial BMP static control.", L"Error!", MB_OK |
                MB_ICONEXCLAMATION);
                DestroyWindow(hWnd);
            }
             */
            HFONT hTxtFont = CreateFont(MAIN_TEXT_FONT_HEIGHT, MAIN_TEXT_FONT_WIDTH, 0, 0, FW_NORMAL, FALSE, TRUE,
                                        FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                        DEFAULT_QUALITY, FF_DONTCARE, NULL);
            HFONT hBtnFont = CreateFont(BTN_FONT_HEIGHT, BTN_FONT_WIDTH, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                                        FF_DONTCARE, NULL);
            if (hTxt == NULL) {
                MessageBoxW(hWnd, L"Error creating text window.", L"Error!", MB_ICONEXCLAMATION | MB_OK);
                DestroyWindow(hWnd);
            }
            if (hTxtFont == NULL) {
                MessageBoxW(hWnd, L"Error creating text font.", L"Error!", MB_ICONEXCLAMATION | MB_OK);
                DestroyWindow(hWnd);
            }
            if (hButton == NULL) {
                MessageBoxW(hWnd, L"Error creating button.", L"Error!", MB_OK | MB_ICONEXCLAMATION);
                DestroyWindow(hWnd);
            }
            if (hBtnFont == NULL) {
                MessageBoxW(hWnd, L"Error creating button font.", L"Error!", MB_ICONEXCLAMATION | MB_OK);
                DestroyWindow(hWnd);
            }
            if (hEdit == NULL) {
                MessageBoxW(hWnd, L"Error creating text edit control.", L"Error!", MB_OK | MB_ICONEXCLAMATION);
                DestroyWindow(hWnd);
            }
            // if (hFinalBMPWnd == NULL) {
            //     MessageBox(hWnd, "Error creating final BMP static control.", "Error!", MB_OK | MB_ICONEXCLAMATION);
            //     DestroyWindow(hWnd);
            // }
            SendMessageW(hTxt, WM_SETFONT, (WPARAM) hTxtFont, TRUE);
            SendMessageW(hButton, WM_SETFONT, (WPARAM) hBtnFont, TRUE);
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
        case WM_RBUTTONDOWN: {
            wchar_t buffer[MAX_PATH];
            HMODULE hModule = GetModuleHandle(NULL);
            GetModuleFileNameW(hModule, buffer, MAX_PATH);
            MessageBoxW(hWnd, buffer, L"Program File Path", MB_OK | MB_ICONINFORMATION);
            break;
        }
        case WM_CTLCOLORSTATIC: {
            if (lParam == (LPARAM) hTxt) {
                HBRUSH hbrush = CreateSolidBrush(RGB(255, 255, 255));
                HDC hdc = (HDC) wParam;
                SetBkColor(hdc, RGB(255, 255, 255));
                SetTextColor(hdc, RGB(0, 0, 0));
                return (LPARAM) hbrush;
            }
            break;
        }
        case WM_COMMAND: {
            if (LOWORD(wParam) == BTN_ID) {
                if (HIWORD(wParam) == BN_CLICKED) {
                    if (replaceWithTime(filePath, timeOffset)) {
                        MessageBoxW(hWnd, L"Error generating file path.", L"Error!", MB_OK | MB_ICONEXCLAMATION);
                        DestroyWindow(hWnd);
                    }
                    wchar_t *msg = malloc((wcslen_c(filePath) + 29)*sizeof(wchar_t));
                    swprintf(msg, (wcslen_c(filePath) + 29)*sizeof(wchar_t), L"The .avi file was saved as: %ls", filePath);
                    MessageBoxW(hWnd, msg, L"File Info", MB_OK | MB_ICONINFORMATION);
                    free(msg);
                }
            }
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
    x_screen = (long double) GetSystemMetrics(SM_CXSCREEN);
    y_screen = (long double) GetSystemMetrics(SM_CYSCREEN);

    filePath = getHomePath(False);
    if (!filePath) {
        MessageBoxW(NULL, L"Error retrieving home path.", L"Error!", MB_OK | MB_ICONEXCLAMATION);
        return -1;
    }

    if (PATH_MAX - wcslen_c(filePath) - 1 < FILE_STR_LEN) {
        MessageBoxW(NULL, L"Home directory path is too long.", L"Error!", MB_OK | MB_ICONEXCLAMATION);
        return -1;
    }
    timeOffset = wcslen_c(filePath) + 13;
    wcscat_c(filePath, L"\\GeneratedAvi"); wcscat_c(filePath, L"                   "); wcscat_c(filePath, L".avi");

    const wchar_t WindClsName[] = L"Video Window Class";

    WNDCLASSW wc = {};
    HWND hWnd;
    MSG msg;

    wc.lpszClassName = WindClsName;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;

    ATOM retval = RegisterClassW(&wc);
    if (retval == 0) {
        MessageBoxW(NULL, L"Error registering window class.", L"Error!", MB_OK | MB_ICONEXCLAMATION);
        return -1;
    }

    hWnd = CreateWindowExW(WS_EX_CLIENTEDGE, WindClsName, L"AVI Attempt", WS_OVERLAPPEDWINDOW, MAIN_WINDOW_X,
                          MAIN_WINDOW_Y, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);
    if (hWnd == NULL) {
        MessageBoxW(NULL, L"Error creating window.", L"Error!", MB_OK | MB_ICONEXCLAMATION);
        return -1;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    while (GetMessageW(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    free(filePath);
    return (int) msg.lParam;
}
#endif