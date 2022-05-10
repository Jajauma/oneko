#pragma once

#include <windows.h>

#include <memory>

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
  HBITMAP GetBitmap(Frame f, Character c);
  HBITMAP GetMaskBitmap(Frame f, Character c);

 private:
  struct BitmapDeleter {
    using pointer = HBITMAP;
    void operator()(HBITMAP h) const { DeleteObject(h); }
  };
  struct SpriteData {
    void *data, *mask_data;
    std::unique_ptr<HBITMAP, BitmapDeleter> bitmap, mask_bitmap;
  };
  static SpriteData sprites_[kFrameCount][kCharacterCount];
  void LoadSprite(SpriteData* sprite);
};
