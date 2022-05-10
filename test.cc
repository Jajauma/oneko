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
    kCharacterBeastie,
    kCharacterSakura,
    kCharacterTomoyo,
    kCharacterCount
  };
  enum Frame {
    kFrameMati2 = 0,
    kFrameJare2,
    kFrameKaki1,
    kFrameKaki2,
    kFrameMati3,
    kFrameSleep1,
    kFrameSleep2,
    kFrameAwake,
    kFrameUp1,
    kFrameUp2,
    kFrameDown1,
    kFrameDown2,
    kFrameLeft1,
    kFrameLeft2,
    kFrameRight1,
    kFrameRight2,
    kFrameUpLeft1,
    kFrameUpLeft2,
    kFrameUpRight1,
    kFrameUpRight2,
    kFrameDownLeft1,
    kFrameDownLeft2,
    kFrameDownRight1,
    kFrameDownRight2,
    kFrameUpTogi1,
    kFrameUpTogi2,
    kFrameDownTogi1,
    kFrameDownTogi2,
    kFrameLeftTogi1,
    kFrameLeftTogi2,
    kFrameRightTogi1,
    kFrameRightTogi2,
    kFrameCount
  };

  HBITMAP GetBitmap(Frame f, Character c) {
    if (!sprites_[f][c].bitmap) {
      const auto data_begin = static_cast<char *>(sprites_[f][c].data);
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
    void *data;
    std::unique_ptr<HBITMAP, BitmapDeleter> bitmap;
  };

  SpriteData sprites_[kFrameCount][kCharacterCount] = {
      {{mati2_bits},
       {mati2_tora_bits},
       {mati2_dog_bits},
       {mati2_bsd_bits},
       {mati2_sakura_bits},
       {mati2_tomoyo_bits}},
      {{jare2_bits},
       {jare2_tora_bits},
       {jare2_dog_bits},
       {jare2_bsd_bits},
       {jare2_sakura_bits},
       {jare2_tomoyo_bits}},
      {{kaki1_bits},
       {kaki1_tora_bits},
       {kaki1_dog_bits},
       {kaki1_bsd_bits},
       {kaki1_sakura_bits},
       {kaki1_tomoyo_bits}},
      {{kaki2_bits},
       {kaki2_tora_bits},
       {kaki2_dog_bits},
       {kaki2_bsd_bits},
       {kaki2_sakura_bits},
       {kaki2_tomoyo_bits}},
      {{mati3_bits},
       {mati3_tora_bits},
       {mati3_dog_bits},
       {mati3_bsd_bits},
       {mati3_sakura_bits},
       {mati3_tomoyo_bits}},
      {{sleep1_bits},
       {sleep1_tora_bits},
       {sleep1_dog_bits},
       {sleep1_bsd_bits},
       {sleep1_sakura_bits},
       {sleep1_tomoyo_bits}},
      {{sleep2_bits},
       {sleep2_tora_bits},
       {sleep2_dog_bits},
       {sleep2_bsd_bits},
       {sleep2_sakura_bits},
       {sleep2_tomoyo_bits}},
      {{awake_bits},
       {awake_tora_bits},
       {awake_dog_bits},
       {awake_bsd_bits},
       {awake_sakura_bits},
       {awake_tomoyo_bits}},
      {{up1_bits},
       {up1_tora_bits},
       {up1_dog_bits},
       {up1_bsd_bits},
       {up1_sakura_bits},
       {up1_tomoyo_bits}},
      {{up2_bits},
       {up2_tora_bits},
       {up2_dog_bits},
       {up2_bsd_bits},
       {up2_sakura_bits},
       {up2_tomoyo_bits}},
      {{down1_bits},
       {down1_tora_bits},
       {down1_dog_bits},
       {down1_bsd_bits},
       {down1_sakura_bits},
       {down1_tomoyo_bits}},
      {{down2_bits},
       {down2_tora_bits},
       {down2_dog_bits},
       {down2_bsd_bits},
       {down2_sakura_bits},
       {down2_tomoyo_bits}},
      {{left1_bits},
       {left1_tora_bits},
       {left1_dog_bits},
       {left1_bsd_bits},
       {left1_sakura_bits},
       {left1_tomoyo_bits}},
      {{left2_bits},
       {left2_tora_bits},
       {left2_dog_bits},
       {left2_bsd_bits},
       {left2_sakura_bits},
       {left2_tomoyo_bits}},
      {{right1_bits},
       {right1_tora_bits},
       {right1_dog_bits},
       {right1_bsd_bits},
       {right1_sakura_bits},
       {right1_tomoyo_bits}},
      {{right2_bits},
       {right2_tora_bits},
       {right2_dog_bits},
       {right2_bsd_bits},
       {right2_sakura_bits},
       {right2_tomoyo_bits}},
      {{upleft1_bits},
       {upleft1_tora_bits},
       {upleft1_dog_bits},
       {upleft1_bsd_bits},
       {upleft1_sakura_bits},
       {upleft1_tomoyo_bits}},
      {{upleft2_bits},
       {upleft2_tora_bits},
       {upleft2_dog_bits},
       {upleft2_bsd_bits},
       {upleft2_sakura_bits},
       {upleft2_tomoyo_bits}},
      {{upright1_bits},
       {upright1_tora_bits},
       {upright1_dog_bits},
       {upright1_bsd_bits},
       {upright1_sakura_bits},
       {upright1_tomoyo_bits}},
      {{upright2_bits},
       {upright2_tora_bits},
       {upright2_dog_bits},
       {upright2_bsd_bits},
       {upright2_sakura_bits},
       {upright2_tomoyo_bits}},
      {{dwleft1_bits},
       {dwleft1_tora_bits},
       {dwleft1_dog_bits},
       {dwleft1_bsd_bits},
       {dwleft1_sakura_bits},
       {dwleft1_tomoyo_bits}},
      {{dwleft2_bits},
       {dwleft2_tora_bits},
       {dwleft2_dog_bits},
       {dwleft2_bsd_bits},
       {dwleft2_sakura_bits},
       {dwleft2_tomoyo_bits}},
      {{dwright1_bits},
       {dwright1_tora_bits},
       {dwright1_dog_bits},
       {dwright1_bsd_bits},
       {dwright1_sakura_bits},
       {dwright1_tomoyo_bits}},
      {{dwright2_bits},
       {dwright2_tora_bits},
       {dwright2_dog_bits},
       {dwright2_bsd_bits},
       {dwright2_sakura_bits},
       {dwright2_tomoyo_bits}},
      {{utogi1_bits},
       {utogi1_tora_bits},
       {utogi1_dog_bits},
       {utogi1_bsd_bits},
       {utogi1_sakura_bits},
       {utogi1_tomoyo_bits}},
      {{utogi2_bits},
       {utogi2_tora_bits},
       {utogi2_dog_bits},
       {utogi2_bsd_bits},
       {utogi2_sakura_bits},
       {utogi2_tomoyo_bits}},
      {{dtogi1_bits},
       {dtogi1_tora_bits},
       {dtogi1_dog_bits},
       {dtogi1_bsd_bits},
       {dtogi1_sakura_bits},
       {dtogi1_tomoyo_bits}},
      {{dtogi2_bits},
       {dtogi2_tora_bits},
       {dtogi2_dog_bits},
       {dtogi2_bsd_bits},
       {dtogi2_sakura_bits},
       {dtogi2_tomoyo_bits}},
      {{ltogi1_bits},
       {ltogi1_tora_bits},
       {ltogi1_dog_bits},
       {ltogi1_bsd_bits},
       {ltogi1_sakura_bits},
       {ltogi1_tomoyo_bits}},
      {{ltogi2_bits},
       {ltogi2_tora_bits},
       {ltogi2_dog_bits},
       {ltogi2_bsd_bits},
       {ltogi2_sakura_bits},
       {ltogi2_tomoyo_bits}},
      {{rtogi1_bits},
       {rtogi1_tora_bits},
       {rtogi1_dog_bits},
       {rtogi1_bsd_bits},
       {rtogi1_sakura_bits},
       {rtogi1_tomoyo_bits}},
      {{rtogi2_bits},
       {rtogi2_tora_bits},
       {rtogi2_dog_bits},
       {rtogi2_bsd_bits},
       {rtogi2_sakura_bits},
       {rtogi2_tomoyo_bits}},
  };
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
        SelectObject(hdc_mem, sprite_library_.GetBitmap(
                                  SpriteLibrary::kFrameRightTogi1,
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
