//
// created by Gregor Hartl Watters on 17/07/2022
//

#ifndef _WIN32
#error Cannot compile on operating system other than Windows.
#else

#include <windows.h>
#include "funcs.h"

#define FILE_STR_LEN 37

unsigned short x_screen;
unsigned short y_screen;

#define MAIN_WINDOW_X (x_screen / 10)
#define MAIN_WINDOW_Y (y_screen / 10)

#define MAIN_WINDOW_WIDTH (x_screen - 2*MAIN_WINDOW_X)
#define MAIN_WINDOW_HEIGHT (y_screen - 2*MAIN_WINDOW_Y)

#define BUTTON_WIDTH (MAIN_WINDOW_WIDTH / 3)
#define BUTTON_HEIGHT (MAIN_WINDOW_HEIGHT / 10)

#define BUTTON_X (MAIN_WINDOW_WIDTH/2 - BUTTON_WIDTH/2)
#define BUTTON_Y (MAIN_WINDOW_HEIGHT/2 - BUTTON_HEIGHT/2)

#define BTN_FONT_HEIGHT (y_screen / 15)
#define BTN_FONT_WIDTH (x_screen / 60)

#define MAIN_TEXT_WIDTH ((MAIN_WINDOW_WIDTH*8)/10)
#define MAIN_TEXT_HEIGHT (MAIN_WINDOW_HEIGHT / 4)

#define MAIN_TEXT_X (MAIN_WINDOW_WIDTH/2 - MAIN_TEXT_WIDTH/2)
#define MAIN_TEXT_Y (MAIN_WINDOW_HEIGHT/10)

#define MAIN_TEXT_FONT_HEIGHT (x_screen / 10)
#define MAIN_TEXT_FONT_WIDTH (y_screen / 20)

HWND hTxt;
#define TEXT_ID 7

HWND hButton;
#define BTN_ID 3

char *filePath;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            hTxt = CreateWindow("STATIC", "AVI File Generator", WS_CHILD | WS_VISIBLE | SS_NOTIFY | SS_CENTER,
                                MAIN_TEXT_X, MAIN_TEXT_Y, MAIN_TEXT_WIDTH, MAIN_TEXT_HEIGHT, hWnd, (HMENU) TEXT_ID,
                                GetModuleHandle(NULL), NULL);
            hButton = CreateWindow("Button", "Generate Video", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, BUTTON_X,
                                   BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, hWnd, (HMENU) BTN_ID, GetModuleHandle(NULL),
                                   NULL);
            HFONT hTxtFont = CreateFont(MAIN_TEXT_FONT_HEIGHT, MAIN_TEXT_FONT_WIDTH, 0, 0, FW_NORMAL, FALSE, TRUE,
                                        FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                        DEFAULT_QUALITY, FF_DONTCARE, NULL);
            HFONT hBtnFont = CreateFont(BTN_FONT_HEIGHT, BTN_FONT_WIDTH, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                                        FF_DONTCARE, NULL);
            if (hTxt == NULL) {
                MessageBox(hWnd, "Error creating text window.", "Error!", MB_ICONEXCLAMATION | MB_OK);
                DestroyWindow(hWnd);
            }
            if (hTxtFont == NULL) {
                MessageBox(hWnd, "Error creating text font.", "Error!", MB_ICONEXCLAMATION | MB_OK);
                DestroyWindow(hWnd);
            }
            if (hButton == NULL) {
                MessageBox(hWnd, "Error creating button.", "Error!", MB_OK | MB_ICONEXCLAMATION);
                DestroyWindow(hWnd);
            }
            if (hBtnFont == NULL) {
                MessageBox(hWnd, "Error creating button font.", "Error!", MB_ICONEXCLAMATION | MB_OK);
                DestroyWindow(hWnd);
            }
            SendMessage(hTxt, WM_SETFONT, (WPARAM) hTxtFont, TRUE);
            SendMessage(hButton, WM_SETFONT, (WPARAM) hBtnFont, TRUE);
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
            char buffer[MAX_PATH];
            HMODULE hModule = GetModuleHandle(NULL);
            GetModuleFileName(hModule, buffer, MAX_PATH);
            MessageBox(hWnd, buffer, "Program File Path", MB_OK | MB_ICONINFORMATION);
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
                    char *msg = malloc(strlen_c(filePath) + 29);
                    sprintf(msg, "The .avi file was saved as: %s", filePath);
                    MessageBox(hWnd, msg, "File Info", MB_OK | MB_ICONINFORMATION);
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
    x_screen = GetSystemMetrics(SM_CXSCREEN);
    y_screen = GetSystemMetrics(SM_CYSCREEN);

    filePath = getHomePath(False);
    if (!filePath) {
        MessageBox(NULL, "Error retrieving home path.", "Error!", MB_OK | MB_ICONEXCLAMATION);
        return -1;
    }

    if (PATH_MAX - strlen_c(filePath) - 1 < FILE_STR_LEN) {
        MessageBox(NULL, "Home directory path is too long.", "Error!", MB_OK | MB_ICONEXCLAMATION);
        return -1;
    }

    strcat_c(filePath, "\\GeneratedAvi"); strcat_c(filePath, getFormattedTime()); strcat_c(filePath, ".avi");

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

    hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, WindClsName, "AVI Attempt", WS_OVERLAPPEDWINDOW, MAIN_WINDOW_X,
                          MAIN_WINDOW_Y, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);
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
    free(filePath);
    return (int) msg.lParam;
}
#endif