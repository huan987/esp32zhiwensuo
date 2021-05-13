#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include <lvgl.h>
#endif


#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_N1
#define LV_ATTRIBUTE_IMG_N1
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_N1 uint8_t n1_map[] = {
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xbe, 0xba, 0x00, 0x00, 0x00, 
};

const lv_img_dsc_t n1 = {
  .header.always_zero = 0,
  .header.w = 32,
  .header.h = 32,
  .data_size = 256,
  .header.cf = LV_IMG_CF_ALPHA_2BIT,
  .data = n1_map,
};
