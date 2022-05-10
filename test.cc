#include <tchar.h>
#include <windows.h>

#include <iterator>
#include <stdexcept>

#include "sprite_library.h"

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
  explicit BaseWindowClass(HINSTANCE hInstance) : instance_(hInstance) {}

  HWND Create() {
    Register();
    const auto window = CreateWindow(
        GetClassName(), TEXT("The Test Project"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, nullptr,
        nullptr, instance_, this);
    if (!window) throw std::runtime_error("can't create window");
    return window;
  }

 protected:
  virtual void SetupWindowClass(WNDCLASS *wc) {
    wc->style = CS_HREDRAW | CS_VREDRAW;
    wc->lpfnWndProc = WindowClass::WindowProc;
    wc->hInstance = instance_;
    wc->lpszClassName = GetClassName();
    wc->hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc->hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc->hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
  }

  virtual PTCHAR GetClassName() const = 0;
  virtual LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam,
                                LPARAM lParam) = 0;

 private:
  void Register() {
    if (!window_class_) {
      WNDCLASS wc = {};
      SetupWindowClass(&wc);
      if (!(window_class_ = RegisterClass(&wc)))
        throw std::runtime_error("window class registration failed");
    }
  }

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

  HINSTANCE instance_ = nullptr;
  ATOM window_class_ = 0;
};

class MainWindowClass : public BaseWindowClass<MainWindowClass> {
 public:
  explicit MainWindowClass(HINSTANCE hInstance) : BaseWindowClass(hInstance) {
    for (auto i = 0; i < SpriteLibrary::kFrameCount; ++i)
      for (auto j = 0; j < SpriteLibrary::kCharacterCount; ++j) {
        sprite_grid_[i][j].x = i * (SpriteLibrary::kSpriteSize + 5);
        sprite_grid_[i][j].y = j * (SpriteLibrary::kSpriteSize + 5);
        sprite_grid_[i][j].bitmap =
            sprite_library_.GetBitmap(static_cast<SpriteLibrary::Frame>(i),
                                      static_cast<SpriteLibrary::Character>(j));
      }
  }

  LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam,
                        LPARAM lParam) override {
    static PAINTSTRUCT ps;
    static HDC hdc, hdc_mem;

    switch (uMsg) {
      case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        hdc_mem = CreateCompatibleDC(hdc);
        for (auto i = 0; i < SpriteLibrary::kFrameCount; ++i)
          for (auto j = 0; j < SpriteLibrary::kCharacterCount; ++j) {
            SelectObject(hdc_mem, sprite_grid_[i][j].bitmap);
            BitBlt(hdc, sprite_grid_[i][j].x, sprite_grid_[i][j].y,
                   SpriteLibrary::kSpriteSize, SpriteLibrary::kSpriteSize,
                   hdc_mem, 0, 0, SRCCOPY);
          }
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
  PTCHAR GetClassName() const override { return TEXT("TEST_PROJECT"); }

 private:
  SpriteLibrary sprite_library_;
  struct {
    int x, y;
    HBITMAP bitmap;
  } sprite_grid_[SpriteLibrary::kFrameCount][SpriteLibrary::kCharacterCount];
};

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int nCmdShow) try {
  MainWindowClass window_class(hInstance);
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
