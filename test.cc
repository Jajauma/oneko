#include <tchar.h>
#include <windows.h>

#include <iterator>
#include <memory>
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

#include "bitmaps/bsd/bsd.include"
#include "bitmaps/dog/dog.include"
#include "bitmaps/neko/neko.include"
#include "bitmaps/sakura/sakura.include"
#include "bitmaps/tomoyo/tomoyo.include"
#include "bitmaps/tora/tora.include"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif  // _MSC_VER

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif  // __GNUC__

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

template <typename It>
static void FlipBits(It start, It end) {
  while (start != end) {
    *start = (*start & 0xF0) >> 4 | (*start & 0x0F) << 4;
    *start = (*start & 0xCC) >> 2 | (*start & 0x33) << 2;
    *start = (*start & 0xAA) >> 1 | (*start & 0x55) << 1;
    ++start;
  }
}

class SpriteLibrary {
 public:
  static constexpr auto kSpriteSize = 32;
  enum Character {
    kCharacterNeko = 0,
    kCharacterTora,
    kCharacterDog,
    // kCharacterBeastie,
    // kCharacterSakura,
    // kCharacterTomoyo
    kCharacterCount
  };
  enum Frame {
    kFrameMati2 = 0,
    kFrameJare2,
    kFrameKaki1,
    // kFrameKaki2,
    // kFrameMati3,
    // kFrameSleep1,
    // kFrameSleep2,
    // kFrameAwake,
    kFrameCount
  };

  HBITMAP GetBitmap(Frame f, Character c) {
    if (!sprites_[f][c].bitmap) {
      const auto data_begin = sprites_[f][c].data;
      const auto data_end = data_begin + (kSpriteSize * kSpriteSize) / 8;
      FlipBits(data_begin, data_end);
      sprites_[f][c].bitmap.reset(
          CreateBitmap(kSpriteSize, kSpriteSize, 1, 1, data_begin));
    }
    return sprites_[f][c].bitmap.get();
  }

 private:
  struct BitmapDeleter {
    using pointer = HBITMAP;
    void operator()(HBITMAP h) const { DeleteObject(h); }
  };

  struct SpriteData {
    char *data;
    std::unique_ptr<HBITMAP, BitmapDeleter> bitmap;
  };

  SpriteData sprites_[kFrameCount][kCharacterCount] = {
      {{mati2_bits, nullptr},
       {mati2_tora_bits, nullptr},
       {mati2_dog_bits, nullptr}},
      {{jare2_bits, nullptr},
       {jare2_tora_bits, nullptr},
       {jare2_dog_bits, nullptr}},
      {{kaki1_bits, nullptr},
       {kaki1_tora_bits, nullptr},
       {kaki1_dog_bits, nullptr}}};
};

class MainWindowClass : public BaseWindowClass<MainWindowClass> {
 public:
  explicit MainWindowClass(HINSTANCE hInstance) : BaseWindowClass(hInstance) {}

  LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam,
                        LPARAM lParam) override {
    static PAINTSTRUCT ps;
    static HDC hdc, hdc_mem;

    switch (uMsg) {
      case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        hdc_mem = CreateCompatibleDC(hdc);
        SelectObject(hdc_mem,
                     sprite_library_.GetBitmap(SpriteLibrary::kFrameJare2,
                                               SpriteLibrary::kCharacterNeko));
        BitBlt(hdc, 0, 0, SpriteLibrary::kSpriteSize,
               SpriteLibrary::kSpriteSize, hdc_mem, 0, 0, SRCCOPY);
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
