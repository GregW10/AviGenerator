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
const wchar_t *homePath;

#define MAIN_WINDOW_X (x_screen / 10.0)
#define MAIN_WINDOW_Y (y_screen / 10.0)

#define MAIN_WINDOW_WIDTH (x_screen - 2*MAIN_WINDOW_X)
#define MAIN_WINDOW_HEIGHT (y_screen - 2*MAIN_WINDOW_Y)

#define BUTTON_WIDTH (MAIN_WINDOW_WIDTH / 3.0)
#define BUTTON_HEIGHT (MAIN_WINDOW_HEIGHT / 10.0)

#define BUTTON_X (MAIN_WINDOW_WIDTH/2 - BUTTON_WIDTH/2.0)
#define BUTTON_Y (MAIN_WINDOW_HEIGHT/2 - BUTTON_HEIGHT/2.0 - MAIN_WINDOW_HEIGHT/60.0)

#define BTN_FONT_HEIGHT (y_screen / 15.0)
#define BTN_FONT_WIDTH (x_screen / 60.0)

#define MAIN_TEXT_WIDTH ((MAIN_WINDOW_WIDTH*8)/10.0)
#define MAIN_TEXT_HEIGHT (MAIN_WINDOW_HEIGHT / 4.0)

#define MAIN_TEXT_X (MAIN_WINDOW_WIDTH/2.0 - MAIN_TEXT_WIDTH/2.0)
#define MAIN_TEXT_Y (MAIN_WINDOW_HEIGHT/12.0)

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

HWND hCreators;
#define CREATORS_ID 127

HWND hOfdTxt;
#define OFD_TXT_ID 511

HWND hOfdBtn;
#define OFD_BTN_ID 255

wchar_t *filePath = NULL;
size_t timeOffset = 0;

wchar_t *folder;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            long double ratio = getBMPheightToWidth(BMP_PATH);
            HINSTANCE hInst = GetModuleHandleW(NULL);
            hTxt = CreateWindowW(L"STATIC", L"AVI File Generator", WS_CHILD | WS_VISIBLE | SS_NOTIFY | SS_CENTER,
                                MAIN_TEXT_X, MAIN_TEXT_Y, MAIN_TEXT_WIDTH, MAIN_TEXT_HEIGHT, hWnd, (HMENU) TEXT_ID,
                                hInst, NULL);
            hButton = CreateWindowW(L"Button", L"Generate Video", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, BUTTON_X,
                                   BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, hWnd, (HMENU) BTN_ID, hInst,
                                   NULL);
            hEdit = CreateWindowW(L"Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_LEFT |
            ES_AUTOVSCROLL, MAIN_WINDOW_WIDTH/7, (MAIN_WINDOW_HEIGHT*7)/10, MAIN_WINDOW_WIDTH/3.5, 30, hWnd,
            (HMENU) EDT_CTL_ID, hInst, NULL);
            hInitBMPWnd = CreateWindowW(L"STATIC", L"Bitmap initial", WS_CHILD | WS_VISIBLE | SS_BITMAP | WS_BORDER,
                                       (MAIN_WINDOW_WIDTH*7)/10.0, (MAIN_WINDOW_HEIGHT*3)/5.0, 0, 0, hWnd,
                                       (HMENU) IBMP_ID, hInst, NULL);
            hCreators = CreateWindowW(L"STATIC", L"From the creators of this app: ", WS_CHILD | WS_VISIBLE | SS_CENTER,
                                      (MAIN_WINDOW_WIDTH*7)/10.0, (MAIN_WINDOW_HEIGHT*3)/5.0 - MAIN_WINDOW_HEIGHT/20.0,
                                      MAIN_WINDOW_WIDTH/4, (int) (MAIN_WINDOW_HEIGHT/20.0), hWnd,
                                      (HMENU) CREATORS_ID, hInst, NULL);
            hOfdTxt = CreateWindowW(L"STATIC", L"Folder Path: ", WS_CHILD | WS_VISIBLE | SS_CENTER,
                                    MAIN_WINDOW_WIDTH/25, (MAIN_WINDOW_HEIGHT*7)/10, MAIN_WINDOW_WIDTH/10, 30, hWnd,
                                    (HMENU) OFD_TXT_ID, hInst, NULL);
            hOfdBtn = CreateWindowW(L"BUTTON", L"Choose Folder...", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON |
            WS_TABSTOP, MAIN_WINDOW_WIDTH/7, (MAIN_WINDOW_HEIGHT*7)/10 + 40, MAIN_WINDOW_WIDTH/7, 30, hWnd,
            (HMENU) OFD_BTN_ID, hInst, NULL);
            if (hTxt == NULL) {
                MessageBoxW(hWnd, L"Error creating big text static control.", L"Error!", MB_OK | MB_ICONEXCLAMATION);
                DestroyWindow(hWnd);
            }
            if (hButton == NULL) {
                MessageBoxW(hWnd, L"Error creating button.", L"Error!", MB_OK | MB_ICONEXCLAMATION);
                DestroyWindow(hWnd);
            }
            if (hEdit == NULL) {
                MessageBoxW(hWnd, L"Error creating edit control.", L"Error!", MB_OK | MB_ICONEXCLAMATION);
                DestroyWindow(hWnd);
            }
            if (hInitBMPWnd == NULL) {
                MessageBoxW(hWnd, L"Error creating BMP static control.", L"Error!", MB_OK | MB_ICONEXCLAMATION);
                DestroyWindow(hWnd);
            }
            if (hCreators == NULL) {
                MessageBoxW(hWnd, L"Error creating small text static control.", L"Error!", MB_OK | MB_ICONEXCLAMATION);
                DestroyWindow(hWnd);
            }
            if (hOfdTxt == NULL) {
                MessageBoxW(hWnd, L"Error creating file dialog text static control.", L"Error!", MB_OK |
                MB_ICONEXCLAMATION);
                DestroyWindow(hWnd);
            }
            if (hOfdBtn == NULL) {
                MessageBoxW(hWnd, L"Error creating file dialog button.", L"Error!", MB_OK | MB_ICONEXCLAMATION);
                DestroyWindow(hWnd);
            }
            hBMP = (HBITMAP) LoadImageW(NULL, BMP_PATH, IMAGE_BITMAP, MAIN_WINDOW_WIDTH/4,
                                        (int) ((MAIN_WINDOW_WIDTH/4.0)*ratio), LR_LOADFROMFILE);
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
            HFONT hTxtFont = CreateFontW(MAIN_TEXT_FONT_HEIGHT, MAIN_TEXT_FONT_WIDTH, 0, 0, FW_NORMAL, FALSE, TRUE,
                                        FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                                        DEFAULT_QUALITY, FF_DONTCARE, NULL);
            HFONT hBtnFont = CreateFontW(BTN_FONT_HEIGHT, BTN_FONT_WIDTH, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                                        FF_DONTCARE, NULL);
            HFONT hCrtFont = CreateFontW((int) (MAIN_WINDOW_HEIGHT/20.0), (int) (MAIN_WINDOW_HEIGHT/70.0), 0, 0,
                                         FW_NORMAL, ANSI_CHARSET, FALSE, FALSE, FALSE, OUT_DEFAULT_PRECIS,
                                         CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, NULL);
            HFONT hOfdFont = CreateFontW(30, MAIN_WINDOW_WIDTH/150.0, 0, 0, FW_NORMAL, FALSE, TRUE, FALSE, ANSI_CHARSET,
                                         OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, NULL);
            HFONT hEditFont = CreateFontW(30, MAIN_WINDOW_WIDTH/120.0, 0, 0, 200, FALSE, FALSE, FALSE, ANSI_CHARSET,
                                          OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, NULL);
            HFONT hOfdBtnFont = CreateFontW(25, MAIN_WINDOW_WIDTH/200.0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                                            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                                            FF_DONTCARE, NULL);
            if (hTxtFont == NULL) {
                MessageBoxW(hWnd, L"Error creating text font.", L"Error!", MB_ICONEXCLAMATION | MB_OK);
                DestroyWindow(hWnd);
            }
            if (hBtnFont == NULL) {
                MessageBoxW(hWnd, L"Error creating button font.", L"Error!", MB_ICONEXCLAMATION | MB_OK);
                DestroyWindow(hWnd);
            }
            if (hCrtFont == NULL) {
                MessageBoxW(hWnd, L"Error creating small text font.", L"Error!", MB_OK | MB_ICONEXCLAMATION);
                DestroyWindow(hWnd);
            }
            if (hOfdFont == NULL) {
                MessageBoxW(hWnd, L"Error creating left path font.", L"Error!", MB_OK | MB_ICONEXCLAMATION);
                DestroyWindow(hWnd);
            }
            if (hEditFont == NULL) {
                MessageBoxW(hWnd, L"Error creating edit control text font.", L"Error!", MB_OK | MB_ICONEXCLAMATION);
                DestroyWindow(hWnd);
            }
            if (hOfdBtnFont == NULL) {
                MessageBoxW(hWnd, L"Error creating file dialog button font.", L"Error!", MB_OK | MB_ICONEXCLAMATION);
                DestroyWindow(hWnd);
            }
            // if (hFinalBMPWnd == NULL) {
            //     MessageBox(hWnd, "Error creating final BMP static control.", "Error!", MB_OK | MB_ICONEXCLAMATION);
            //     DestroyWindow(hWnd);
            // }
            SendMessageW(hTxt, WM_SETFONT, (WPARAM) hTxtFont, TRUE);
            SendMessageW(hButton, WM_SETFONT, (WPARAM) hBtnFont, TRUE);
            SendMessageW(hCreators, WM_SETFONT, (WPARAM) hCrtFont, TRUE);
            SendMessageW(hOfdTxt, WM_SETFONT, (WPARAM) hOfdFont, TRUE);
            SendMessageW(hEdit, WM_SETFONT, (WPARAM) hEditFont, TRUE);
            SendMessageW(hOfdBtn, WM_SETFONT, (WPARAM) hOfdBtnFont, TRUE);
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
            if (lParam == (LPARAM) hCreators) {
                HBRUSH hBrush = CreateSolidBrush(RGB(150, 150, 150));
                HDC hdc = (HDC) wParam;
                SetBkColor(hdc, RGB(150, 150, 150));
                SetTextColor(hdc, RGB(0, 0, 0));
                return (LRESULT) hBrush;
            }
            if (lParam == (LPARAM) hOfdTxt) {
                HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
                HDC hdc = (HDC) wParam;
                SetBkColor(hdc, RGB(255, 255, 255));
                SetTextColor(hdc, RGB(0, 0, 0));
                return (LRESULT) hBrush;
            }
            break;
        }
        case WM_COMMAND: {
            switch (LOWORD(wParam)) {
                case BTN_ID: {
                    if (HIWORD(wParam) == BN_CLICKED) {
                        if (replaceWithTime(filePath, timeOffset)) {
                            MessageBoxW(hWnd, L"Error generating file path.", L"Error!", MB_OK | MB_ICONEXCLAMATION);
                            DestroyWindow(hWnd);
                        }
                        wchar_t *msg = malloc((wcslen_c(filePath) + 29) * sizeof(wchar_t));
                        swprintf(msg, (wcslen_c(filePath) + 29) * sizeof(wchar_t), L"The .avi file was saved as: %ls",
                                 filePath);
                        MessageBoxW(hWnd, msg, L"File Info", MB_OK | MB_ICONINFORMATION);
                        free(msg);
                    }
                    break;
                }
                case OFD_BTN_ID: {
                    if (HIWORD(wParam) == BN_CLICKED) {
                        folder = malloc(32768*sizeof(wchar_t));
                        ZeroMemory(folder, 32768*sizeof(wchar_t));
                        BROWSEINFOW br = {};
                        br.hwndOwner = hWnd;
                        br.pszDisplayName = folder;
                        br.lpszTitle = L"Choose BMP-Containing Folder";
                        br.ulFlags = BIF_NEWDIALOGSTYLE;
                        PIDLIST_ABSOLUTE pidl = SHBrowseForFolderW(&br);
                        /*
                        OPENFILENAMEW ofn = {};
                        ofn.lStructSize = sizeof(OPENFILENAMEW);
                        ofn.hwndOwner = hWnd;
                        ofn.lpstrFilter = L"All\0*\0All Files\0*.*\0Text Files\0*.txt\0C Files\0*.c\0Cpp Files\0*.cpp\0BMP Files\0*.bmp\0";
                        ofn.nFilterIndex = 1;
                        ofn.lpstrFile = folder;
                        ofn.nMaxFile = 32768;
                        ofn.lpstrInitialDir = homePath;
                        GetOpenFileNameW(&ofn); */
                        SetWindowTextW(hEdit, folder);
                        CoTaskMemFree(pidl);
                    }
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

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    x_screen = (long double) GetSystemMetrics(SM_CXSCREEN);
    y_screen = (long double) GetSystemMetrics(SM_CYSCREEN);

    homePath = getHomePath();
    filePath = malloc((MAX_PATH + 38)*sizeof(wchar_t));
    wcscpy_c(filePath, homePath);
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

    hWnd = CreateWindowExW(WS_EX_CLIENTEDGE, WindClsName, L"AVI Generator", WS_OVERLAPPEDWINDOW, MAIN_WINDOW_X,
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