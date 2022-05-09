#include <tchar.h>
#include <windows.h>

int APIENTRY _tWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, LPTSTR cmdline,
                       int cmdshow) {
  return MessageBox(NULL, TEXT("hello, world"), TEXT("caption"), 0);
}
