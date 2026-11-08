#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>

struct DemoPlayer {
    float x, y;
    float vx, vy;
    int health;
    const wchar_t* name;
};

static const wchar_t CLASS_NAME[] = L"CS2ExternalESPDemoWindow";
static bool g_espEnabled = true;

static std::vector<DemoPlayer> g_players = {
    { 250, 220,  0.75f,  0.42f, 100, L"Demo Player 1" },
    { 520, 340, -0.48f,  0.58f,  72, L"Demo Player 2" },
    { 820, 190,  0.62f, -0.37f,  45, L"Demo Player 3" }
};

static void DrawTextLabel(HDC dc, int x, int y, const std::wstring& text,
                          COLORREF color, int height = 18) {
    SetBkMode(dc, TRANSPARENT);
    SetTextColor(dc, RGB(0, 0, 0));
    HFONT font = CreateFontW(height, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
    HFONT old = (HFONT)SelectObject(dc, font);
    TextOutW(dc, x + 1, y + 1, text.c_str(), (int)text.size());
    SetTextColor(dc, color);
    TextOutW(dc, x, y, text.c_str(), (int)text.size());
    SelectObject(dc, old);
    DeleteObject(font);
}

static void DrawPlayer(HDC dc, const DemoPlayer& player) {
    const int boxW = 54;
    const int boxH = 112;
    const int left = (int)player.x - boxW / 2;
    const int top = (int)player.y - boxH / 2;
    const int right = left + boxW;
    const int bottom = top + boxH;

    HPEN pen = CreatePen(PS_SOLID, 2, RGB(70, 255, 120));
    HPEN oldPen = (HPEN)SelectObject(dc, pen);
    HBRUSH oldBrush = (HBRUSH)SelectObject(dc, GetStockObject(HOLLOW_BRUSH));
    Rectangle(dc, left, top, right, bottom);
    SelectObject(dc, oldBrush);
    SelectObject(dc, oldPen);
    DeleteObject(pen);

    const int barX = left - 9;
    const int barY = top;
    const int barH = boxH;
    RECT bg{ barX, barY, barX + 5, barY + barH };
    HBRUSH bgBrush = CreateSolidBrush(RGB(35, 35, 35));
    FillRect(dc, &bg, bgBrush);
    DeleteObject(bgBrush);

    const int filled = (barH * (std::max)(0, (std::min)(100, player.health))) / 100;
    RECT hp{ barX, barY + barH - filled, barX + 5, barY + barH };
    COLORREF hpColor = player.health > 60 ? RGB(70, 230, 90) :
                       player.health > 30 ? RGB(255, 190, 40) : RGB(255, 65, 65);
    HBRUSH hpBrush = CreateSolidBrush(hpColor);
    FillRect(dc, &hp, hpBrush);
    DeleteObject(hpBrush);

    std::wostringstream label;
    label << player.name << L"  " << player.health << L" HP";
    DrawTextLabel(dc, left - 12, top - 23, label.str(), RGB(255, 255, 255), 16);

    HPEN centerPen = CreatePen(PS_SOLID, 1, RGB(255, 220, 80));
    oldPen = (HPEN)SelectObject(dc, centerPen);
    MoveToEx(dc, (left + right) / 2 - 5, (top + bottom) / 2, nullptr);
    LineTo(dc, (left + right) / 2 + 5, (top + bottom) / 2);
    MoveToEx(dc, (left + right) / 2, (top + bottom) / 2 - 5, nullptr);
    LineTo(dc, (left + right) / 2, (top + bottom) / 2 + 5);
    SelectObject(dc, oldPen);
    DeleteObject(centerPen);
}

static void UpdateDemoPlayers(int width, int height) {
    for (auto& player : g_players) {
        player.x += player.vx;
        player.y += player.vy;
        if (player.x < 90 || player.x > width - 90) player.vx = -player.vx;
        if (player.y < 90 || player.y > height - 90) player.vy = -player.vy;
    }
}

static void PaintOverlay(HWND hwnd) {
    PAINTSTRUCT ps{};
    HDC screen = BeginPaint(hwnd, &ps);

    RECT client{};
    GetClientRect(hwnd, &client);
    const int width = client.right;
    const int height = client.bottom;

    HDC mem = CreateCompatibleDC(screen);
    HBITMAP bitmap = CreateCompatibleBitmap(screen, width, height);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(mem, bitmap);

    HBRUSH black = CreateSolidBrush(RGB(0, 0, 0));
    FillRect(mem, &client, black);
    DeleteObject(black);

    if (g_espEnabled) {
        for (const auto& player : g_players) DrawPlayer(mem, player);
    }

    DrawTextLabel(mem, 18, 16, L"CS2 External ESP - DEMO MODE (synthetic targets only)",
                  RGB(100, 210, 255), 18);
    DrawTextLabel(mem, 18, 40, L"F1: toggle demo ESP    F2: quit",
                  RGB(230, 230, 230), 16);
    if (!g_espEnabled) {
        DrawTextLabel(mem, 18, 70, L"DEMO ESP OFF", RGB(255, 190, 70), 20);
    }

    BitBlt(screen, 0, 0, width, height, mem, 0, 0, SRCCOPY);
    SelectObject(mem, oldBitmap);
    DeleteObject(bitmap);
    DeleteDC(mem);
    EndPaint(hwnd, &ps);
}

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_PAINT:
        PaintOverlay(hwnd);
        return 0;
    case WM_TIMER: {
        RECT r{};
        GetClientRect(hwnd, &r);
        UpdateDemoPlayers(r.right, r.bottom);
        InvalidateRect(hwnd, nullptr, FALSE);
        return 0;
    }
    case WM_KEYDOWN:
        if (wParam == VK_F1) {
            g_espEnabled = !g_espEnabled;
            InvalidateRect(hwnd, nullptr, FALSE);
        } else if (wParam == VK_F2) {
            DestroyWindow(hwnd);
        }
        return 0;
    case WM_DESTROY:
        KillTimer(hwnd, 1);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int) {
    WNDCLASSW wc{};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = instance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    RegisterClassW(&wc);

    const int screenW = GetSystemMetrics(SM_CXSCREEN);
    const int screenH = GetSystemMetrics(SM_CYSCREEN);

    HWND hwnd = CreateWindowExW(
        WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,
        CLASS_NAME, L"CS2 ESP Demo Overlay", WS_POPUP,
        0, 0, screenW, screenH, nullptr, nullptr, instance, nullptr);

    if (!hwnd) return 1;

    SetLayeredWindowAttributes(hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
    ShowWindow(hwnd, SW_SHOW);
    SetTimer(hwnd, 1, 16, nullptr);

    MSG msg{};
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return 0;
}
