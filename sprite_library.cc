#include "sprite_library.h"

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
#include "bitmasks/bsd/bsd.mask.include"
#include "bitmasks/dog/dog.mask.include"
#include "bitmasks/neko/neko.mask.include"
#include "bitmasks/sakura/sakura.mask.include"
#include "bitmasks/tomoyo/tomoyo.mask.include"

#if defined(_MSC_VER)
#pragma warning(pop)
#endif  // _MSC_VER

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif  // __GNUC__

template <typename It>
static void FlipBits(It start, It end) {
  while (start != end) {
    *start = (*start & 0xF0) >> 4 | (*start & 0x0F) << 4;
    *start = (*start & 0xCC) >> 2 | (*start & 0x33) << 2;
    *start = (*start & 0xAA) >> 1 | (*start & 0x55) << 1;
    ++start;
  }
}

HBITMAP SpriteLibrary::GetBitmap(Frame f, Character c) {
  if (!sprites_[f][c].bitmap) {
    LoadSprite(&sprites_[f][c]);
  }
  return sprites_[f][c].bitmap.get();
}

HBITMAP SpriteLibrary::GetMaskBitmap(Frame f, Character c) {
  if (!sprites_[f][c].mask_bitmap) {
    LoadSprite(&sprites_[f][c]);
  }
  return sprites_[f][c].mask_bitmap.get();
}

void SpriteLibrary::LoadSprite(SpriteData* sprite) {
  auto CreateBitmapFromData = [](void* data) -> HBITMAP {
    const auto u8_data = static_cast<char*>(data);
    FlipBits(u8_data, u8_data + (kSpriteSize * kSpriteSize) / 8);
    HBITMAP result = CreateBitmap(kSpriteSize, kSpriteSize, 1, 1, data);
    FlipBits(u8_data, u8_data + (kSpriteSize * kSpriteSize) / 8);
    return result;
  };
  sprite->bitmap.reset(CreateBitmapFromData(sprite->data));
  sprite->mask_bitmap.reset(CreateBitmapFromData(sprite->mask_data));
}

SpriteLibrary::SpriteData
    SpriteLibrary::sprites_[kFrameCount][kCharacterCount] = {
        {{mati2_bits, mati2_mask_bits, nullptr, nullptr},
         {mati2_tora_bits, mati2_mask_bits, nullptr, nullptr},
         {mati2_dog_bits, mati2_dog_mask_bits, nullptr, nullptr},
         {mati2_bsd_bits, mati2_bsd_mask_bits, nullptr, nullptr},
         {mati2_sakura_bits, mati2_sakura_mask_bits, nullptr, nullptr},
         {mati2_tomoyo_bits, mati2_tomoyo_mask_bits, nullptr, nullptr}},
        {{jare2_bits, jare2_mask_bits, nullptr, nullptr},
         {jare2_tora_bits, jare2_mask_bits, nullptr, nullptr},
         {jare2_dog_bits, jare2_dog_mask_bits, nullptr, nullptr},
         {jare2_bsd_bits, jare2_bsd_mask_bits, nullptr, nullptr},
         {jare2_sakura_bits, jare2_sakura_mask_bits, nullptr, nullptr},
         {jare2_tomoyo_bits, jare2_tomoyo_mask_bits, nullptr, nullptr}},
        {{kaki1_bits, kaki1_mask_bits, nullptr, nullptr},
         {kaki1_tora_bits, kaki1_mask_bits, nullptr, nullptr},
         {kaki1_dog_bits, kaki1_dog_mask_bits, nullptr, nullptr},
         {kaki1_bsd_bits, kaki1_bsd_mask_bits, nullptr, nullptr},
         {kaki1_sakura_bits, kaki1_sakura_mask_bits, nullptr, nullptr},
         {kaki1_tomoyo_bits, kaki1_tomoyo_mask_bits, nullptr, nullptr}},
        {{kaki2_bits, kaki2_mask_bits, nullptr, nullptr},
         {kaki2_tora_bits, kaki2_mask_bits, nullptr, nullptr},
         {kaki2_dog_bits, kaki2_dog_mask_bits, nullptr, nullptr},
         {kaki2_bsd_bits, kaki2_bsd_mask_bits, nullptr, nullptr},
         {kaki2_sakura_bits, kaki2_sakura_mask_bits, nullptr, nullptr},
         {kaki2_tomoyo_bits, kaki2_tomoyo_mask_bits, nullptr, nullptr}},
        {{mati3_bits, mati3_mask_bits, nullptr, nullptr},
         {mati3_tora_bits, mati3_mask_bits, nullptr, nullptr},
         {mati3_dog_bits, mati3_dog_mask_bits, nullptr, nullptr},
         {mati3_bsd_bits, mati3_bsd_mask_bits, nullptr, nullptr},
         {mati3_sakura_bits, mati3_sakura_mask_bits, nullptr, nullptr},
         {mati3_tomoyo_bits, mati3_tomoyo_mask_bits, nullptr, nullptr}},
        {{sleep1_bits, sleep1_mask_bits, nullptr, nullptr},
         {sleep1_tora_bits, sleep1_mask_bits, nullptr, nullptr},
         {sleep1_dog_bits, sleep1_dog_mask_bits, nullptr, nullptr},
         {sleep1_bsd_bits, sleep1_bsd_mask_bits, nullptr, nullptr},
         {sleep1_sakura_bits, sleep1_sakura_mask_bits, nullptr, nullptr},
         {sleep1_tomoyo_bits, sleep1_tomoyo_mask_bits, nullptr, nullptr}},
        {{sleep2_bits, sleep2_mask_bits, nullptr, nullptr},
         {sleep2_tora_bits, sleep2_mask_bits, nullptr, nullptr},
         {sleep2_dog_bits, sleep2_dog_mask_bits, nullptr, nullptr},
         {sleep2_bsd_bits, sleep2_bsd_mask_bits, nullptr, nullptr},
         {sleep2_sakura_bits, sleep2_sakura_mask_bits, nullptr, nullptr},
         {sleep2_tomoyo_bits, sleep2_tomoyo_mask_bits, nullptr, nullptr}},
        {{awake_bits, awake_mask_bits, nullptr, nullptr},
         {awake_tora_bits, awake_mask_bits, nullptr, nullptr},
         {awake_dog_bits, awake_dog_mask_bits, nullptr, nullptr},
         {awake_bsd_bits, awake_bsd_mask_bits, nullptr, nullptr},
         {awake_sakura_bits, awake_sakura_mask_bits, nullptr, nullptr},
         {awake_tomoyo_bits, awake_tomoyo_mask_bits, nullptr, nullptr}},
        {{up1_bits, up1_mask_bits, nullptr, nullptr},
         {up1_tora_bits, up1_mask_bits, nullptr, nullptr},
         {up1_dog_bits, up1_dog_mask_bits, nullptr, nullptr},
         {up1_bsd_bits, up1_bsd_mask_bits, nullptr, nullptr},
         {up1_sakura_bits, up1_sakura_mask_bits, nullptr, nullptr},
         {up1_tomoyo_bits, up1_tomoyo_mask_bits, nullptr, nullptr}},
        {{up2_bits, up2_mask_bits, nullptr, nullptr},
         {up2_tora_bits, up2_mask_bits, nullptr, nullptr},
         {up2_dog_bits, up2_dog_mask_bits, nullptr, nullptr},
         {up2_bsd_bits, up2_bsd_mask_bits, nullptr, nullptr},
         {up2_sakura_bits, up2_sakura_mask_bits, nullptr, nullptr},
         {up2_tomoyo_bits, up2_tomoyo_mask_bits, nullptr, nullptr}},
        {{down1_bits, down1_mask_bits, nullptr, nullptr},
         {down1_tora_bits, down1_mask_bits, nullptr, nullptr},
         {down1_dog_bits, down1_dog_mask_bits, nullptr, nullptr},
         {down1_bsd_bits, down1_bsd_mask_bits, nullptr, nullptr},
         {down1_sakura_bits, down1_sakura_mask_bits, nullptr, nullptr},
         {down1_tomoyo_bits, down1_tomoyo_mask_bits, nullptr, nullptr}},
        {{down2_bits, down2_mask_bits, nullptr, nullptr},
         {down2_tora_bits, down2_mask_bits, nullptr, nullptr},
         {down2_dog_bits, down2_dog_mask_bits, nullptr, nullptr},
         {down2_bsd_bits, down2_bsd_mask_bits, nullptr, nullptr},
         {down2_sakura_bits, down2_sakura_mask_bits, nullptr, nullptr},
         {down2_tomoyo_bits, down2_tomoyo_mask_bits, nullptr, nullptr}},
        {{left1_bits, left1_mask_bits, nullptr, nullptr},
         {left1_tora_bits, left1_mask_bits, nullptr, nullptr},
         {left1_dog_bits, left1_dog_mask_bits, nullptr, nullptr},
         {left1_bsd_bits, left1_bsd_mask_bits, nullptr, nullptr},
         {left1_sakura_bits, left1_sakura_mask_bits, nullptr, nullptr},
         {left1_tomoyo_bits, left1_tomoyo_mask_bits, nullptr, nullptr}},
        {{left2_bits, left2_mask_bits, nullptr, nullptr},
         {left2_tora_bits, left2_mask_bits, nullptr, nullptr},
         {left2_dog_bits, left2_dog_mask_bits, nullptr, nullptr},
         {left2_bsd_bits, left2_bsd_mask_bits, nullptr, nullptr},
         {left2_sakura_bits, left2_sakura_mask_bits, nullptr, nullptr},
         {left2_tomoyo_bits, left2_tomoyo_mask_bits, nullptr, nullptr}},
        {{right1_bits, right1_mask_bits, nullptr, nullptr},
         {right1_tora_bits, right1_mask_bits, nullptr, nullptr},
         {right1_dog_bits, right1_dog_mask_bits, nullptr, nullptr},
         {right1_bsd_bits, right1_bsd_mask_bits, nullptr, nullptr},
         {right1_sakura_bits, right1_sakura_mask_bits, nullptr, nullptr},
         {right1_tomoyo_bits, right1_tomoyo_mask_bits, nullptr, nullptr}},
        {{right2_bits, right2_mask_bits, nullptr, nullptr},
         {right2_tora_bits, right2_mask_bits, nullptr, nullptr},
         {right2_dog_bits, right2_dog_mask_bits, nullptr, nullptr},
         {right2_bsd_bits, right2_bsd_mask_bits, nullptr, nullptr},
         {right2_sakura_bits, right2_sakura_mask_bits, nullptr, nullptr},
         {right2_tomoyo_bits, right2_tomoyo_mask_bits, nullptr, nullptr}},
        {{upleft1_bits, upleft1_mask_bits, nullptr, nullptr},
         {upleft1_tora_bits, upleft1_mask_bits, nullptr, nullptr},
         {upleft1_dog_bits, upleft1_dog_mask_bits, nullptr, nullptr},
         {upleft1_bsd_bits, upleft1_bsd_mask_bits, nullptr, nullptr},
         {upleft1_sakura_bits, upleft1_sakura_mask_bits, nullptr, nullptr},
         {upleft1_tomoyo_bits, upleft1_tomoyo_mask_bits, nullptr, nullptr}},
        {{upleft2_bits, upleft2_mask_bits, nullptr, nullptr},
         {upleft2_tora_bits, upleft2_mask_bits, nullptr, nullptr},
         {upleft2_dog_bits, upleft2_dog_mask_bits, nullptr, nullptr},
         {upleft2_bsd_bits, upleft2_bsd_mask_bits, nullptr, nullptr},
         {upleft2_sakura_bits, upleft2_sakura_mask_bits, nullptr, nullptr},
         {upleft2_tomoyo_bits, upleft2_tomoyo_mask_bits, nullptr, nullptr}},
        {{upright1_bits, upright1_mask_bits, nullptr, nullptr},
         {upright1_tora_bits, upright1_mask_bits, nullptr, nullptr},
         {upright1_dog_bits, upright1_dog_mask_bits, nullptr, nullptr},
         {upright1_bsd_bits, upright1_bsd_mask_bits, nullptr, nullptr},
         {upright1_sakura_bits, upright1_sakura_mask_bits, nullptr, nullptr},
         {upright1_tomoyo_bits, upright1_tomoyo_mask_bits, nullptr, nullptr}},
        {{upright2_bits, upright2_mask_bits, nullptr, nullptr},
         {upright2_tora_bits, upright2_mask_bits, nullptr, nullptr},
         {upright2_dog_bits, upright2_dog_mask_bits, nullptr, nullptr},
         {upright2_bsd_bits, upright2_bsd_mask_bits, nullptr, nullptr},
         {upright2_sakura_bits, upright2_sakura_mask_bits, nullptr, nullptr},
         {upright2_tomoyo_bits, upright2_tomoyo_mask_bits, nullptr, nullptr}},
        {{dwleft1_bits, dwleft1_mask_bits, nullptr, nullptr},
         {dwleft1_tora_bits, dwleft1_mask_bits, nullptr, nullptr},
         {dwleft1_dog_bits, dwleft1_dog_mask_bits, nullptr, nullptr},
         {dwleft1_bsd_bits, dwleft1_bsd_mask_bits, nullptr, nullptr},
         {dwleft1_sakura_bits, dwleft1_sakura_mask_bits, nullptr, nullptr},
         {dwleft1_tomoyo_bits, dwleft1_tomoyo_mask_bits, nullptr, nullptr}},
        {{dwleft2_bits, dwleft2_mask_bits, nullptr, nullptr},
         {dwleft2_tora_bits, dwleft2_mask_bits, nullptr, nullptr},
         {dwleft2_dog_bits, dwleft2_dog_mask_bits, nullptr, nullptr},
         {dwleft2_bsd_bits, dwleft2_bsd_mask_bits, nullptr, nullptr},
         {dwleft2_sakura_bits, dwleft2_sakura_mask_bits, nullptr, nullptr},
         {dwleft2_tomoyo_bits, dwleft2_tomoyo_mask_bits, nullptr, nullptr}},
        {{dwright1_bits, dwright1_mask_bits, nullptr, nullptr},
         {dwright1_tora_bits, dwright1_mask_bits, nullptr, nullptr},
         {dwright1_dog_bits, dwright1_dog_mask_bits, nullptr, nullptr},
         {dwright1_bsd_bits, dwright1_bsd_mask_bits, nullptr, nullptr},
         {dwright1_sakura_bits, dwright1_sakura_mask_bits, nullptr, nullptr},
         {dwright1_tomoyo_bits, dwright1_tomoyo_mask_bits, nullptr, nullptr}},
        {{dwright2_bits, dwright2_mask_bits, nullptr, nullptr},
         {dwright2_tora_bits, dwright2_mask_bits, nullptr, nullptr},
         {dwright2_dog_bits, dwright2_dog_mask_bits, nullptr, nullptr},
         {dwright2_bsd_bits, dwright2_bsd_mask_bits, nullptr, nullptr},
         {dwright2_sakura_bits, dwright2_sakura_mask_bits, nullptr, nullptr},
         {dwright2_tomoyo_bits, dwright2_tomoyo_mask_bits, nullptr, nullptr}},
        {{utogi1_bits, utogi1_mask_bits, nullptr, nullptr},
         {utogi1_tora_bits, utogi1_mask_bits, nullptr, nullptr},
         {utogi1_dog_bits, utogi1_dog_mask_bits, nullptr, nullptr},
         {utogi1_bsd_bits, utogi1_bsd_mask_bits, nullptr, nullptr},
         {utogi1_sakura_bits, utogi1_sakura_mask_bits, nullptr, nullptr},
         {utogi1_tomoyo_bits, utogi1_tomoyo_mask_bits, nullptr, nullptr}},
        {{utogi2_bits, utogi2_mask_bits, nullptr, nullptr},
         {utogi2_tora_bits, utogi2_mask_bits, nullptr, nullptr},
         {utogi2_dog_bits, utogi2_dog_mask_bits, nullptr, nullptr},
         {utogi2_bsd_bits, utogi2_bsd_mask_bits, nullptr, nullptr},
         {utogi2_sakura_bits, utogi2_sakura_mask_bits, nullptr, nullptr},
         {utogi2_tomoyo_bits, utogi2_tomoyo_mask_bits, nullptr, nullptr}},
        {{dtogi1_bits, dtogi1_mask_bits, nullptr, nullptr},
         {dtogi1_tora_bits, dtogi1_mask_bits, nullptr, nullptr},
         {dtogi1_dog_bits, dtogi1_dog_mask_bits, nullptr, nullptr},
         {dtogi1_bsd_bits, dtogi1_bsd_mask_bits, nullptr, nullptr},
         {dtogi1_sakura_bits, dtogi1_sakura_mask_bits, nullptr, nullptr},
         {dtogi1_tomoyo_bits, dtogi1_tomoyo_mask_bits, nullptr, nullptr}},
        {{dtogi2_bits, dtogi2_mask_bits, nullptr, nullptr},
         {dtogi2_tora_bits, dtogi2_mask_bits, nullptr, nullptr},
         {dtogi2_dog_bits, dtogi2_dog_mask_bits, nullptr, nullptr},
         {dtogi2_bsd_bits, dtogi2_bsd_mask_bits, nullptr, nullptr},
         {dtogi2_sakura_bits, dtogi2_sakura_mask_bits, nullptr, nullptr},
         {dtogi2_tomoyo_bits, dtogi2_tomoyo_mask_bits, nullptr, nullptr}},
        {{ltogi1_bits, ltogi1_mask_bits, nullptr, nullptr},
         {ltogi1_tora_bits, ltogi1_mask_bits, nullptr, nullptr},
         {ltogi1_dog_bits, ltogi1_dog_mask_bits, nullptr, nullptr},
         {ltogi1_bsd_bits, ltogi1_bsd_mask_bits, nullptr, nullptr},
         {ltogi1_sakura_bits, ltogi1_sakura_mask_bits, nullptr, nullptr},
         {ltogi1_tomoyo_bits, ltogi1_tomoyo_mask_bits, nullptr, nullptr}},
        {{ltogi2_bits, ltogi2_mask_bits, nullptr, nullptr},
         {ltogi2_tora_bits, ltogi2_mask_bits, nullptr, nullptr},
         {ltogi2_dog_bits, ltogi2_dog_mask_bits, nullptr, nullptr},
         {ltogi2_bsd_bits, ltogi2_bsd_mask_bits, nullptr, nullptr},
         {ltogi2_sakura_bits, ltogi2_sakura_mask_bits, nullptr, nullptr},
         {ltogi2_tomoyo_bits, ltogi2_tomoyo_mask_bits, nullptr, nullptr}},
        {{rtogi1_bits, rtogi1_mask_bits, nullptr, nullptr},
         {rtogi1_tora_bits, rtogi1_mask_bits, nullptr, nullptr},
         {rtogi1_dog_bits, rtogi1_dog_mask_bits, nullptr, nullptr},
         {rtogi1_bsd_bits, rtogi1_bsd_mask_bits, nullptr, nullptr},
         {rtogi1_sakura_bits, rtogi1_sakura_mask_bits, nullptr, nullptr},
         {rtogi1_tomoyo_bits, rtogi1_tomoyo_mask_bits, nullptr, nullptr}},
        {{rtogi2_bits, rtogi2_mask_bits, nullptr, nullptr},
         {rtogi2_tora_bits, rtogi2_mask_bits, nullptr, nullptr},
         {rtogi2_dog_bits, rtogi2_dog_mask_bits, nullptr, nullptr},
         {rtogi2_bsd_bits, rtogi2_bsd_mask_bits, nullptr, nullptr},
         {rtogi2_sakura_bits, rtogi2_sakura_mask_bits, nullptr, nullptr},
         {rtogi2_tomoyo_bits, rtogi2_tomoyo_mask_bits, nullptr, nullptr}},
};
