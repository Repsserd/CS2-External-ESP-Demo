#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>

static HWND g_status, g_pluginList;
static const COLORREF BG = RGB(18, 20, 27);
static const COLORREF PANEL = RGB(27, 30, 40);
static const COLORREF ACCENT = RGB(105, 91, 255);

void AddText(HWND parent, const wchar_t* text, int x, int y, int w, int h, int size=18, COLORREF color=RGB(235,238,245)) {
    HWND label = CreateWindowW(L"STATIC", text, WS_CHILD|WS_VISIBLE, x,y,w,h,parent,nullptr,nullptr,nullptr);
    SendMessageW(label, WM_SETFONT, (WPARAM)GetStockObject(DEFAULT_GUI_FONT), TRUE);
    (void)size; (void)color;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch(msg) {
    case WM_CREATE: {
        AddText(hwnd, L"NEXUS  /  GAME HUB", 28, 22, 400, 34, 22);
        AddText(hwnd, L"Library", 30, 100, 150, 28);
        AddText(hwnd, L"CS2", 34, 150, 120, 25);
        AddText(hwnd, L"Counter-Strike 2", 34, 180, 250, 25);
        CreateWindowW(L"BUTTON", L"Launch game", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 34, 225, 145, 38, hwnd, (HMENU)101, nullptr, nullptr);
        AddText(hwnd, L"PLUGIN MANAGER", 340, 100, 250, 28);
        g_pluginList = CreateWindowW(L"LISTBOX", L"", WS_CHILD|WS_VISIBLE|WS_BORDER|LBS_NOTIFY, 340, 140, 350, 130, hwnd, (HMENU)102, nullptr, nullptr);
        SendMessageW(g_pluginList, LB_ADDSTRING, 0, (LPARAM)L"Example overlay theme (demo)");
        SendMessageW(g_pluginList, LB_ADDSTRING, 0, (LPARAM)L"Performance monitor (placeholder)");
        SendMessageW(g_pluginList, LB_ADDSTRING, 0, (LPARAM)L"Local configuration profile");
        CreateWindowW(L"BUTTON", L"Add local plugin", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 340, 285, 145, 36, hwnd, (HMENU)103, nullptr, nullptr);
        CreateWindowW(L"BUTTON", L"Remove selected", WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON, 495, 285, 145, 36, hwnd, (HMENU)104, nullptr, nullptr);
        AddText(hwnd, L"ACTIVITY", 30, 350, 180, 25);
        g_status = CreateWindowW(L"STATIC", L"Ready. This launcher manages benign local plugins only.", WS_CHILD|WS_VISIBLE, 30, 385, 650, 28, hwnd, nullptr, nullptr, nullptr);
        AddText(hwnd, L"Settings  •  Updates  •  About", 30, 455, 500, 25);
        break;
    }
    case WM_COMMAND:
        switch(LOWORD(wp)) {
        case 101:
            SetWindowTextW(g_status, L"Demo action: launch your installed game from Steam or its official launcher.");
            break;
        case 103:
            MessageBoxW(hwnd, L"Add only trusted, documented plugins. DLL injection and anti-cheat bypass are not supported.", L"Plugin safety", MB_OK|MB_ICONINFORMATION);
            break;
        case 104: {
            LRESULT selected = SendMessageW(g_pluginList, LB_GETCURSEL, 0, 0);
            if(selected != LB_ERR) {
                SendMessageW(g_pluginList, LB_DELETESTRING, (WPARAM)selected, 0);
                SetWindowTextW(g_status, L"Removed selected demo entry.");
            }
            break;
        }
        }
        break;
    case WM_DESTROY: PostQuitMessage(0); return 0;
    }
    return DefWindowProcW(hwnd,msg,wp,lp);
}
int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int show) {
    const wchar_t CLASS_NAME[] = L"NexusLauncherWindow";
    WNDCLASSW wc{}; wc.lpfnWndProc=WndProc; wc.hInstance=instance; wc.lpszClassName=CLASS_NAME;
    wc.hCursor=LoadCursor(nullptr, IDC_ARROW); wc.hbrBackground=CreateSolidBrush(BG);
    RegisterClassW(&wc);
    HWND hwnd=CreateWindowExW(0,CLASS_NAME,L"Nexus Game Hub",WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX,
        CW_USEDEFAULT,CW_USEDEFAULT,760,550,nullptr,nullptr,instance,nullptr);
    if(!hwnd) return 0;
    ShowWindow(hwnd,show);
    MSG msg{}; while(GetMessageW(&msg,nullptr,0,0)>0){ TranslateMessage(&msg); DispatchMessageW(&msg); }
    return 0;
}