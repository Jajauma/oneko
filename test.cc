#include <tchar.h>
#include <windows.h>

#include <cstdint>

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable : 4309)
#pragma warning(disable : 4838)
#endif  // _MSC_VER

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnarrowing"
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif  // __GNUC__

#include "bitmaps/tora/tora.include"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif  // _MSC_VER

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif  // __GNUC__

template <typename It>
static void FlipBytes(It start, It end) {
  while (start != end) {
    *start = (*start & 0xF0) >> 4 | (*start & 0x0F) << 4;
    *start = (*start & 0xCC) >> 2 | (*start & 0x33) << 2;
    *start = (*start & 0xAA) >> 1 | (*start & 0x55) << 1;
    ++start;
  }
}

static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam,
                                   LPARAM lParam) {
  static HBITMAP tora_bitmap;
  static PAINTSTRUCT ps;
  static HDC hdc, hdc_mem;

  switch (uMsg) {
    case WM_CREATE:
      FlipBytes(std::begin(awake_tora_bits), std::end(awake_tora_bits));
      tora_bitmap = CreateBitmap(awake_tora_width, awake_tora_height, 1, 1,
                                 awake_tora_bits);
      return 0;

    case WM_PAINT:
      hdc = BeginPaint(hWnd, &ps);
      hdc_mem = CreateCompatibleDC(hdc);
      SelectObject(hdc_mem, tora_bitmap);
      BitBlt(hdc, 0, 0, awake_tora_width, awake_tora_height, hdc_mem, 0, 0,
             SRCCOPY);
      DeleteDC(hdc_mem);
      EndPaint(hWnd, &ps);
      return 0;

    case WM_DESTROY:
      DeleteObject(tora_bitmap);
      PostQuitMessage(0);
      return 0;

    default:
      return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int nCmdShow) {
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
