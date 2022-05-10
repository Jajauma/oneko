#include <tchar.h>
#include <windows.h>

#include <iterator>
#include <stdexcept>

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

static void DisplayFatalError(HWND parent, const char *error_msg = nullptr) {
  static constexpr auto kMaxSymbolCount = 64;
  char buf[kMaxSymbolCount];
  std::snprintf(buf, kMaxSymbolCount, "Fatal: %s",
                error_msg ? error_msg : "unknown error");
#if defined(UNICODE)
  TCHAR buf_wide[kMaxSymbolCount];
  MultiByteToWideChar(CP_ACP, 0, buf, -1, buf_wide, kMaxSymbolCount);
#endif  // UNICODE
  MessageBox(parent,
#if defined(UNICODE)
             buf_wide,
#else   // UNICODE
             buf,
#endif  // UNICODE
             TEXT("TestProject"), MB_ICONERROR);
}

template <class WindowClass>
class BaseWindowClass {
 public:
  void Register() {
    if (!window_class_) {
      WNDCLASS wc = {};
      SetupWindowClass(&wc);
      if (!(window_class_ = RegisterClass(&wc)))
        throw std::runtime_error("window class registration failed");
    }
  }

  HWND Create() {
    Register();
    const auto window = CreateWindow(
        GetClassName(), TEXT("The Test Project"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr,
        nullptr, GetModuleHandle(nullptr), this);
    if (!window) throw std::runtime_error("can't create window");
    return window;
  }

 protected:
  virtual void SetupWindowClass(WNDCLASS *wc) {
    wc->style = CS_HREDRAW | CS_VREDRAW;
    wc->lpfnWndProc = WindowClass::WindowProc;
    wc->hInstance = GetModuleHandle(nullptr);
    wc->lpszClassName = GetClassName();
    wc->hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc->hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc->hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
  }

  virtual const PTCHAR GetClassName() const = 0;
  virtual LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam,
                                LPARAM lParam) = 0;

 private:
  static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam,
                                     LPARAM lParam) {
    WindowClass *self = nullptr;
    if (uMsg == WM_NCCREATE) {
      self = static_cast<WindowClass *>(
          reinterpret_cast<CREATESTRUCT *>(lParam)->lpCreateParams);
      SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
    } else {
      self = reinterpret_cast<WindowClass *>(
          GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }
    try {
      return self ? self->HandleMessage(hWnd, uMsg, wParam, lParam)
                  : DefWindowProc(hWnd, uMsg, wParam, lParam);
    } catch (std::exception &e) {
      DisplayFatalError(hWnd, e.what());
      PostQuitMessage(1);
      return 0;
    } catch (...) {
      DisplayFatalError(hWnd);
      PostQuitMessage(1);
      return 0;
    }
  }

  ATOM window_class_ = 0;
};

class MainWindowClass : public BaseWindowClass<MainWindowClass> {
 public:
  MainWindowClass()
      : tora_bitmap_(CreateBitmap(awake_tora_width, awake_tora_height, 1, 1,
                                  awake_tora_bits)) {}

  ~MainWindowClass() { DeleteObject(tora_bitmap_); }

  LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam,
                        LPARAM lParam) override {
    static PAINTSTRUCT ps;
    static HDC hdc, hdc_mem;

    switch (uMsg) {
      case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        hdc_mem = CreateCompatibleDC(hdc);
        SelectObject(hdc_mem, tora_bitmap_);
        BitBlt(hdc, 0, 0, awake_tora_width, awake_tora_height, hdc_mem, 0, 0,
               SRCCOPY);
        DeleteDC(hdc_mem);
        EndPaint(hWnd, &ps);
        return 0;

      case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

      default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
  }
  const PTCHAR GetClassName() const override { return TEXT("TEST_PROJECT"); }

 private:
  HBITMAP tora_bitmap_;
};

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int nCmdShow) try {
  FlipBytes(std::begin(awake_tora_bits), std::end(awake_tora_bits));

  MainWindowClass window_class;
  const auto window = window_class.Create();
  ShowWindow(window, nCmdShow);
  UpdateWindow(window);

  MSG msg = {};
  while (GetMessage(&msg, nullptr, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return static_cast<int>(msg.wParam);
} catch (std::exception &e) {
  DisplayFatalError(nullptr, e.what());
  return 1;
} catch (...) {
  DisplayFatalError(nullptr);
  return 1;
}
