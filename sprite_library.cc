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
    const auto data_begin = static_cast<char *>(sprites_[f][c].data);
    const auto data_end = data_begin + (kSpriteSize * kSpriteSize) / 8;
    FlipBits(data_begin, data_end);
    sprites_[f][c].bitmap.reset(
        CreateBitmap(kSpriteSize, kSpriteSize, 1, 1, data_begin));
  }
  return sprites_[f][c].bitmap.get();
}

SpriteLibrary::SpriteData
    SpriteLibrary::sprites_[kFrameCount][kCharacterCount] = {
        {{mati2_bits, nullptr},
         {mati2_tora_bits, nullptr},
         {mati2_dog_bits, nullptr},
         {mati2_bsd_bits, nullptr},
         {mati2_sakura_bits, nullptr},
         {mati2_tomoyo_bits, nullptr}},
        {{jare2_bits, nullptr},
         {jare2_tora_bits, nullptr},
         {jare2_dog_bits, nullptr},
         {jare2_bsd_bits, nullptr},
         {jare2_sakura_bits, nullptr},
         {jare2_tomoyo_bits, nullptr}},
        {{kaki1_bits, nullptr},
         {kaki1_tora_bits, nullptr},
         {kaki1_dog_bits, nullptr},
         {kaki1_bsd_bits, nullptr},
         {kaki1_sakura_bits, nullptr},
         {kaki1_tomoyo_bits, nullptr}},
        {{kaki2_bits, nullptr},
         {kaki2_tora_bits, nullptr},
         {kaki2_dog_bits, nullptr},
         {kaki2_bsd_bits, nullptr},
         {kaki2_sakura_bits, nullptr},
         {kaki2_tomoyo_bits, nullptr}},
        {{mati3_bits, nullptr},
         {mati3_tora_bits, nullptr},
         {mati3_dog_bits, nullptr},
         {mati3_bsd_bits, nullptr},
         {mati3_sakura_bits, nullptr},
         {mati3_tomoyo_bits, nullptr}},
        {{sleep1_bits, nullptr},
         {sleep1_tora_bits, nullptr},
         {sleep1_dog_bits, nullptr},
         {sleep1_bsd_bits, nullptr},
         {sleep1_sakura_bits, nullptr},
         {sleep1_tomoyo_bits, nullptr}},
        {{sleep2_bits, nullptr},
         {sleep2_tora_bits, nullptr},
         {sleep2_dog_bits, nullptr},
         {sleep2_bsd_bits, nullptr},
         {sleep2_sakura_bits, nullptr},
         {sleep2_tomoyo_bits, nullptr}},
        {{awake_bits, nullptr},
         {awake_tora_bits, nullptr},
         {awake_dog_bits, nullptr},
         {awake_bsd_bits, nullptr},
         {awake_sakura_bits, nullptr},
         {awake_tomoyo_bits, nullptr}},
        {{up1_bits, nullptr},
         {up1_tora_bits, nullptr},
         {up1_dog_bits, nullptr},
         {up1_bsd_bits, nullptr},
         {up1_sakura_bits, nullptr},
         {up1_tomoyo_bits, nullptr}},
        {{up2_bits, nullptr},
         {up2_tora_bits, nullptr},
         {up2_dog_bits, nullptr},
         {up2_bsd_bits, nullptr},
         {up2_sakura_bits, nullptr},
         {up2_tomoyo_bits, nullptr}},
        {{down1_bits, nullptr},
         {down1_tora_bits, nullptr},
         {down1_dog_bits, nullptr},
         {down1_bsd_bits, nullptr},
         {down1_sakura_bits, nullptr},
         {down1_tomoyo_bits, nullptr}},
        {{down2_bits, nullptr},
         {down2_tora_bits, nullptr},
         {down2_dog_bits, nullptr},
         {down2_bsd_bits, nullptr},
         {down2_sakura_bits, nullptr},
         {down2_tomoyo_bits, nullptr}},
        {{left1_bits, nullptr},
         {left1_tora_bits, nullptr},
         {left1_dog_bits, nullptr},
         {left1_bsd_bits, nullptr},
         {left1_sakura_bits, nullptr},
         {left1_tomoyo_bits, nullptr}},
        {{left2_bits, nullptr},
         {left2_tora_bits, nullptr},
         {left2_dog_bits, nullptr},
         {left2_bsd_bits, nullptr},
         {left2_sakura_bits, nullptr},
         {left2_tomoyo_bits, nullptr}},
        {{right1_bits, nullptr},
         {right1_tora_bits, nullptr},
         {right1_dog_bits, nullptr},
         {right1_bsd_bits, nullptr},
         {right1_sakura_bits, nullptr},
         {right1_tomoyo_bits, nullptr}},
        {{right2_bits, nullptr},
         {right2_tora_bits, nullptr},
         {right2_dog_bits, nullptr},
         {right2_bsd_bits, nullptr},
         {right2_sakura_bits, nullptr},
         {right2_tomoyo_bits, nullptr}},
        {{upleft1_bits, nullptr},
         {upleft1_tora_bits, nullptr},
         {upleft1_dog_bits, nullptr},
         {upleft1_bsd_bits, nullptr},
         {upleft1_sakura_bits, nullptr},
         {upleft1_tomoyo_bits, nullptr}},
        {{upleft2_bits, nullptr},
         {upleft2_tora_bits, nullptr},
         {upleft2_dog_bits, nullptr},
         {upleft2_bsd_bits, nullptr},
         {upleft2_sakura_bits, nullptr},
         {upleft2_tomoyo_bits, nullptr}},
        {{upright1_bits, nullptr},
         {upright1_tora_bits, nullptr},
         {upright1_dog_bits, nullptr},
         {upright1_bsd_bits, nullptr},
         {upright1_sakura_bits, nullptr},
         {upright1_tomoyo_bits, nullptr}},
        {{upright2_bits, nullptr},
         {upright2_tora_bits, nullptr},
         {upright2_dog_bits, nullptr},
         {upright2_bsd_bits, nullptr},
         {upright2_sakura_bits, nullptr},
         {upright2_tomoyo_bits, nullptr}},
        {{dwleft1_bits, nullptr},
         {dwleft1_tora_bits, nullptr},
         {dwleft1_dog_bits, nullptr},
         {dwleft1_bsd_bits, nullptr},
         {dwleft1_sakura_bits, nullptr},
         {dwleft1_tomoyo_bits, nullptr}},
        {{dwleft2_bits, nullptr},
         {dwleft2_tora_bits, nullptr},
         {dwleft2_dog_bits, nullptr},
         {dwleft2_bsd_bits, nullptr},
         {dwleft2_sakura_bits, nullptr},
         {dwleft2_tomoyo_bits, nullptr}},
        {{dwright1_bits, nullptr},
         {dwright1_tora_bits, nullptr},
         {dwright1_dog_bits, nullptr},
         {dwright1_bsd_bits, nullptr},
         {dwright1_sakura_bits, nullptr},
         {dwright1_tomoyo_bits, nullptr}},
        {{dwright2_bits, nullptr},
         {dwright2_tora_bits, nullptr},
         {dwright2_dog_bits, nullptr},
         {dwright2_bsd_bits, nullptr},
         {dwright2_sakura_bits, nullptr},
         {dwright2_tomoyo_bits, nullptr}},
        {{utogi1_bits, nullptr},
         {utogi1_tora_bits, nullptr},
         {utogi1_dog_bits, nullptr},
         {utogi1_bsd_bits, nullptr},
         {utogi1_sakura_bits, nullptr},
         {utogi1_tomoyo_bits, nullptr}},
        {{utogi2_bits, nullptr},
         {utogi2_tora_bits, nullptr},
         {utogi2_dog_bits, nullptr},
         {utogi2_bsd_bits, nullptr},
         {utogi2_sakura_bits, nullptr},
         {utogi2_tomoyo_bits, nullptr}},
        {{dtogi1_bits, nullptr},
         {dtogi1_tora_bits, nullptr},
         {dtogi1_dog_bits, nullptr},
         {dtogi1_bsd_bits, nullptr},
         {dtogi1_sakura_bits, nullptr},
         {dtogi1_tomoyo_bits, nullptr}},
        {{dtogi2_bits, nullptr},
         {dtogi2_tora_bits, nullptr},
         {dtogi2_dog_bits, nullptr},
         {dtogi2_bsd_bits, nullptr},
         {dtogi2_sakura_bits, nullptr},
         {dtogi2_tomoyo_bits, nullptr}},
        {{ltogi1_bits, nullptr},
         {ltogi1_tora_bits, nullptr},
         {ltogi1_dog_bits, nullptr},
         {ltogi1_bsd_bits, nullptr},
         {ltogi1_sakura_bits, nullptr},
         {ltogi1_tomoyo_bits, nullptr}},
        {{ltogi2_bits, nullptr},
         {ltogi2_tora_bits, nullptr},
         {ltogi2_dog_bits, nullptr},
         {ltogi2_bsd_bits, nullptr},
         {ltogi2_sakura_bits, nullptr},
         {ltogi2_tomoyo_bits, nullptr}},
        {{rtogi1_bits, nullptr},
         {rtogi1_tora_bits, nullptr},
         {rtogi1_dog_bits, nullptr},
         {rtogi1_bsd_bits, nullptr},
         {rtogi1_sakura_bits, nullptr},
         {rtogi1_tomoyo_bits, nullptr}},
        {{rtogi2_bits, nullptr},
         {rtogi2_tora_bits, nullptr},
         {rtogi2_dog_bits, nullptr},
         {rtogi2_bsd_bits, nullptr},
         {rtogi2_sakura_bits, nullptr},
         {rtogi2_tomoyo_bits, nullptr}},
};
