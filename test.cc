#include <tchar.h>
#include <windows.h>

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam,
                                   LPARAM lParam) {
  switch (uMsg) {
    case WM_PAINT: {
      PAINTSTRUCT ps;
      RECT rect;
      const auto hdc = BeginPaint(hWnd, &ps);
      GetClientRect(hWnd, &rect);
      DrawText(hdc, TEXT("Hello Windows again ..."), -1, &rect,
               DT_SINGLELINE | DT_CENTER | DT_VCENTER);
      EndPaint(hWnd, &ps);
      return 0;
    }
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
    default:
      return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hInstPrev, LPTSTR cmdline,
                     int nCmdShow) {
  static constexpr TCHAR kClassName[] = TEXT("Test Project");
  WNDCLASS wc = {};
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = kClassName;
  wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
  wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
  if (!RegisterClass(&wc)) {
    MessageBox(nullptr, TEXT("Class registration failed"), kClassName,
               MB_ICONERROR);
    return 1;
  }

  const auto window =
      CreateWindow(kClassName, TEXT("The Test Project"), WS_OVERLAPPEDWINDOW,
                   CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                   nullptr, nullptr, hInstance, nullptr);
  ShowWindow(window, nCmdShow);
  UpdateWindow(window);

  MSG msg = {};
  while (GetMessage(&msg, nullptr, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return static_cast<int>(msg.wParam);
}
