#define BLINKER_WIFI
#include<Arduino.h>
#include<SPI.h>
#include<TFT_eSPI.h>
#include<lvgl.h>
#include"ZA618.h"
#include<Blinker.h>
#include<TimeLib.h>
#include "esp_camera.h"
#include <Adafruit_NeoPixel.h>
char auth[] = "**********";
char ssid[] = "**********";
char pswd[] = "**********";
 #define PWDN_GPIO_NUM     -1
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      32
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       25
#define Y7_GPIO_NUM       0
#define Y6_GPIO_NUM       18
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       23
#define Y3_GPIO_NUM       22
#define Y2_GPIO_NUM        19
#define VSYNC_GPIO_NUM    39
#define HREF_GPIO_NUM     34
#define PCLK_GPIO_NUM     5
//图标
LV_IMG_DECLARE(wifioff)
LV_IMG_DECLARE(renlianoff)
LV_IMG_DECLARE(menguan)
LV_IMG_DECLARE(menkai)
LV_IMG_DECLARE(lanyaoff)
//在这里注册要显示的照片
//LV_IMG_DECLARE(zp1)
//LV_IMG_DECLARE(zp2)
//LV_IMG_DECLARE(zp3)
//LV_IMG_DECLARE(zp4)
//LV_IMG_DECLARE(zp5)
//LV_IMG_DECLARE(zp6)
//LV_IMG_DECLARE(zp7)
//LV_IMG_DECLARE(zp8)
//LV_IMG_DECLARE(zp9)
//LV_IMG_DECLARE(zp10)
//LV_IMG_DECLARE(zp11)
//LV_IMG_DECLARE(zp12)
//LV_IMG_DECLARE(zp13)
//LV_IMG_DECLARE(zp14)
//LV_IMG_DECLARE(zp15)
//LV_IMG_DECLARE(zp16)
//LV_IMG_DECLARE(zp17)
//LV_IMG_DECLARE(zp18)
//LV_IMG_DECLARE(zp19)
//LV_IMG_DECLARE(zp20)
//LV_IMG_DECLARE(zp21)
//LV_IMG_DECLARE(zp22)
//LV_IMG_DECLARE(zp23)
//LV_IMG_DECLARE(zp24)
//LV_IMG_DECLARE(zp25)
//LV_IMG_DECLARE(zp26)
//LV_IMG_DECLARE(zp27)
//LV_IMG_DECLARE(zp28)
//LV_IMG_DECLARE(zp29)
//LV_IMG_DECLARE(zp30)

LV_IMG_DECLARE(duigou)
LV_IMG_DECLARE(shouzhi)
//时钟数字图标
LV_IMG_DECLARE(n0)
LV_IMG_DECLARE(n1)
LV_IMG_DECLARE(n2)
LV_IMG_DECLARE(n3)
LV_IMG_DECLARE(n4)
LV_IMG_DECLARE(n5)
LV_IMG_DECLARE(n6)
LV_IMG_DECLARE(n7)
LV_IMG_DECLARE(n8)
LV_IMG_DECLARE(n9)
LV_IMG_DECLARE(welcome)
//blinker组件
BlinkerButton Button1("btn-door");
BlinkerButton Button2("btn-menu");
BlinkerButton Button3("btn-rgbmode");
BlinkerRGB RGB1("col-qsk");
Adafruit_NeoPixel pixels(8, 33, NEO_GRB + NEO_KHZ800);
//lvgl组件
lv_obj_t *scr;
lv_obj_t *cont1;
lv_obj_t *cont2;
lv_obj_t *cont3;
lv_obj_t *cont4;
lv_obj_t *cont5;
lv_obj_t *wifi;
lv_obj_t *bluetooth;
lv_obj_t *face;
lv_obj_t *door;
lv_obj_t *picture;
lv_obj_t *clk1;
lv_obj_t *clk2;
lv_obj_t *clk3;
lv_obj_t *clk4;
lv_obj_t *zcck;
lv_obj_t *fig;
lv_obj_t *youxia;
//lvgl组件样式
lv_style_t p_cont;
lv_style_t p_back;
lv_style_t p_status_wifi;
lv_style_t p_status_door;
lv_style_t p_status_face;
lv_style_t p_status_bluetooth;
lv_style_t p_clock;
int R=0,G=255,B=0,W=100,RS=0,GS=254,BS=1,WS=0;
uint8_t rgbmode=2;
uint8_t sleep_mode=1;
int freq = 50;      // 频率(20ms周期)
int channel = 8;    // 通道(高速通道（0 ~ 7）由80MHz时钟驱动，低速通道（8 ~ 15）由 1MHz 时钟驱动。)
int a_resolution = 8; // 分辨率
const int led = 15;

int calculatePWM(int degree)
{ //0-180度
 //20ms周期，高电平0.5-2.5ms，对应0-180度角度
  const float deadZone = 6.4;//对应0.5ms（0.5ms/(20ms/256）)
  const float max = 32;//对应2.5ms
  if (degree < 0)
    degree = 0;
  if (degree > 180)
    degree = 180;
  return (int)(((max - deadZone) / 180) * degree + deadZone);
}
TFT_eSPI tft = TFT_eSPI();
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];
lv_task_t *kaimen=NULL;
lv_task_t *Open_door=NULL;
lv_task_t *kmtb=NULL;
lv_task_t *gmtb=NULL;
lv_task_t *WIFIjiance=NULL;
lv_task_t *BLINKERTASK=NULL;
lv_task_t *Fclock=NULL;
lv_task_t *Ftime=NULL;
lv_task_t *zhuce=NULL;
lv_task_t *ZCENDa=NULL;
lv_task_t *waits=NULL;
lv_task_t *qiehuan=NULL;
lv_task_t *swk=NULL;
lv_task_t *RGBMODE=NULL;
//lvgl回调函数
void sleep(lv_task_t* task){
  sleep_mode=0;
  digitalWrite(2,0);
  ZA_closelight();
}
void wake(lv_task_t* task){
  sleep_mode=1;
  digitalWrite(2,1);
}
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p){
    uint16_t w = (area->x2 - area->x1 + 1);
    uint16_t h = (area->y2 - area->y1 + 1);
    tft.startWrite();
    tft.setAddrWindow(area->x1,area->y1,w,h);
    tft.pushColors(&color_p->full,w*h);
    tft.endWrite();
    lv_disp_flush_ready(disp);
}
void jiancezhiwen(lv_task_t* task);
void kaimentubiao(lv_task_t* task){
p_status_door.image.color=lv_color_hex(0xF2B28D);
lv_img_set_src(door,&menkai);
lv_obj_refresh_style(door);
ledcWrite(channel, calculatePWM(0));
lv_task_del(kaimen);
}
void guanmentubiao(lv_task_t* task){
p_status_door.image.color=lv_color_hex(0xD0D9D3);
lv_img_set_src(door,&menguan);
lv_obj_refresh_style(door);
ledcWrite(channel, calculatePWM(180));
kaimen =lv_task_create(jiancezhiwen,100,LV_TASK_PRIO_HIGH,NULL);
}
void opendoor(lv_task_t* task){
kmtb=lv_task_create(kaimentubiao,0,LV_TASK_PRIO_HIGHEST,NULL);
lv_task_once(kmtb);
gmtb=lv_task_create(guanmentubiao,3000,LV_TASK_PRIO_HIGH,NULL);
 lv_task_once(gmtb);
}
void jiancezhiwen(lv_task_t* task){
  int pl=ZA_Identify();
  if((pl!=-1)&&(pl!=2048)){

    swk=lv_task_create(wake,0,LV_TASK_PRIO_HIGH,NULL);
    lv_task_once(swk);
    Open_door=lv_task_create(opendoor,0,LV_TASK_PRIO_HIGH,NULL);
    lv_task_once(Open_door);
  }else if((pl==2048)&&(sleep_mode==0)){

    swk=lv_task_create(wake,0,LV_TASK_PRIO_HIGH,NULL);
    lv_task_once(swk);
  }
}
void  task_jc_start(){
kaimen =lv_task_create(jiancezhiwen,100,LV_TASK_PRIO_HIGH,NULL);
}
void wifijiance_cb(lv_task_t* task){
if(WiFi.status()==WL_CONNECTED){
  setTime(Blinker.time()+28800);
  p_status_wifi.image.color=lv_color_hex(0xF2B28D);
  lv_obj_refresh_style(wifi);
}else if(WiFi.status()!=WL_CONNECTED){
  Blinker.begin(auth,ssid,pswd);
  p_status_wifi.image.color=lv_color_hex(0xD0D9D3);
  lv_obj_refresh_style(wifi);
}
}
int LH=0;
int LM=0;
int NH=0;
int NM=0;
void Fclock_cd(lv_task_t* task){
  NH=hour();
  NM=minute();
  if((NH>=23||NH<7)&&(sleep_mode==1)&&NM!=0){
    sleep_mode=2; 
    swk=lv_task_create(sleep,20000,LV_TASK_PRIO_HIGH,NULL);
    lv_task_once(swk);
  }else if((NH<23&&NH>=7)&&(sleep_mode==0)){
    swk=lv_task_create(wake,0,LV_TASK_PRIO_HIGH,NULL);
    lv_task_once(swk);
  }
  if(LH!=NH){
  int HL=NH%10;
  int HH=NH/10;
switch(HH){
  case 0:lv_img_set_src(clk1,&n0);break;
  case 1:lv_img_set_src(clk1,&n1);break;
  case 2:lv_img_set_src(clk1,&n2);break;
  case 3:lv_img_set_src(clk1,&n3);break;
  case 4:lv_img_set_src(clk1,&n4);break;
  case 5:lv_img_set_src(clk1,&n5);break;
  case 6:lv_img_set_src(clk1,&n6);break;
  case 7:lv_img_set_src(clk1,&n7);break;
  case 8:lv_img_set_src(clk1,&n8);break;
  case 9:lv_img_set_src(clk1,&n9);break;
}
switch(HL){
  case 0:lv_img_set_src(clk2,&n0);break;
  case 1:lv_img_set_src(clk2,&n1);break;
  case 2:lv_img_set_src(clk2,&n2);break;
  case 3:lv_img_set_src(clk2,&n3);break;
  case 4:lv_img_set_src(clk2,&n4);break;
  case 5:lv_img_set_src(clk2,&n5);break;
  case 6:lv_img_set_src(clk2,&n6);break;
  case 7:lv_img_set_src(clk2,&n7);break;
  case 8:lv_img_set_src(clk2,&n8);break;
  case 9:lv_img_set_src(clk2,&n9);break;
}}
if(LM!=NM){
  int ML=NM%10;
  int MH=NM/10;
switch(MH){
  case 0:lv_img_set_src(clk3,&n0);break;
  case 1:lv_img_set_src(clk3,&n1);break;
  case 2:lv_img_set_src(clk3,&n2);break;
  case 3:lv_img_set_src(clk3,&n3);break;
  case 4:lv_img_set_src(clk3,&n4);break;
  case 5:lv_img_set_src(clk3,&n5);break;
  case 6:lv_img_set_src(clk3,&n6);break;
  case 7:lv_img_set_src(clk3,&n7);break;
  case 8:lv_img_set_src(clk3,&n8);break;
  case 9:lv_img_set_src(clk3,&n9);break;
}
switch(ML){
  case 0:lv_img_set_src(clk4,&n0);break;
  case 1:lv_img_set_src(clk4,&n1);break;
  case 2:lv_img_set_src(clk4,&n2);break;
  case 3:lv_img_set_src(clk4,&n3);break;
  case 4:lv_img_set_src(clk4,&n4);break;
  case 5:lv_img_set_src(clk4,&n5);break;
  case 6:lv_img_set_src(clk4,&n6);break;
  case 7:lv_img_set_src(clk4,&n7);break;
  case 8:lv_img_set_src(clk4,&n8);break;
  case 9:lv_img_set_src(clk4,&n9);break;
}}
LH=NH;
LM=NM;
}
void Waits(lv_task_t* task);
void zcend(lv_task_t* task){
switch (ZA_Enrollcl())
{
case 5:lv_task_once(ZCENDa);lv_img_set_src(fig,&duigou);waits=lv_task_create(Waits,1000,LV_TASK_PRIO_HIGHEST,NULL);break;
case 6:lv_obj_del(zcck);task_jc_start();lv_task_once(ZCENDa);break;
default:break;
}
}
void zczw(lv_task_t* task){
lv_task_del(kaimen);
zcck=lv_cont_create(scr,cont1);
lv_obj_set_size(zcck,180,180);
lv_obj_align_origo(zcck,NULL,LV_ALIGN_CENTER,0,0);
fig=lv_img_create(zcck,NULL);
lv_img_set_src(fig,&shouzhi);
lv_obj_align_origo(fig,NULL,LV_ALIGN_CENTER,0,0);
ZA_Enrollstart();
ZCENDa=lv_task_create(zcend,100,LV_TASK_PRIO_HIGHEST,NULL);

}
void Waits(lv_task_t* task){
  lv_obj_clean(zcck); lv_obj_del(zcck);task_jc_start();
}
uint8_t pn=0;
void qh(lv_task_t* task){
switch(pn){
  case 0 :lv_img_set_src(picture,&zp1); pn=1 ;break;
  case 1 :lv_img_set_src(picture,&zp2); pn=2 ;break;
  case 2 :lv_img_set_src(picture,&zp3); pn=3 ;break;
  case 3 :lv_img_set_src(picture,&zp4); pn=4 ;break;
  case 4 :lv_img_set_src(picture,&zp5); pn=5 ;break;
  case 5 :lv_img_set_src(picture,&zp6); pn=6 ;break;
  case 6 :lv_img_set_src(picture,&zp7); pn=7 ;break;
  case 7 :lv_img_set_src(picture,&zp8); pn=8 ;break;
  case 8 :lv_img_set_src(picture,&zp9); pn=9 ;break;
  case 9 :lv_img_set_src(picture,&zp10);pn=10;break;
  case 10:lv_img_set_src(picture,&zp11);pn=11;break;
  case 11:lv_img_set_src(picture,&zp12);pn=12;break;
  case 12:lv_img_set_src(picture,&zp13);pn=13;break;
  case 13:lv_img_set_src(picture,&zp14);pn=14;break;
  case 14:lv_img_set_src(picture,&zp15);pn=15;break;
  case 15:lv_img_set_src(picture,&zp16);pn=16;break;
  case 16:lv_img_set_src(picture,&zp17);pn=17;break;
  case 17:lv_img_set_src(picture,&zp18);pn=18;break;
  case 18:lv_img_set_src(picture,&zp19);pn=19;break;
  case 19:lv_img_set_src(picture,&zp20);pn=20;break;
  case 20:lv_img_set_src(picture,&zp21);pn=21;break;
  case 21:lv_img_set_src(picture,&zp22);pn=22;break;
  case 22:lv_img_set_src(picture,&zp23);pn=23;break;
  case 23:lv_img_set_src(picture,&zp24);pn=24;break;
  case 24:lv_img_set_src(picture,&zp25);pn=25;break;
  case 25:lv_img_set_src(picture,&zp26);pn=26;break;
  case 26:lv_img_set_src(picture,&zp27);pn=27;break;
  case 27:lv_img_set_src(picture,&zp28);pn=28;break;
  case 28:lv_img_set_src(picture,&zp29);pn=29;break;
  case 29:lv_img_set_src(picture,&zp30);pn=0 ;break;
}
}
int ab=10;
//目前有三种灯效
void rgb_mode_c(lv_task_t* task){
  if(sleep_mode!=0){
  if(rgbmode==0){
  pixels.fill(pixels.Color(R, G, B));
  pixels.setBrightness(W);
  pixels.show();
  }else if(rgbmode==1){
  WS+=ab;
  if(WS<=0){
    WS=0;
    ab=10;
  }
  if(WS>=255){
    WS=255;
    ab=-10;
  }
  pixels.fill(pixels.Color(R, G, B));
  pixels.setBrightness(WS);
  pixels.show();
  }else if(rgbmode==2){
    if(RS<=0){
      GS-=10;
      BS+=10;
    if(BS>=255){
      BS=255;
      GS=0;
      RS+=10;
    }
    if(GS<=0){
      GS=0;
    }
    }else if(GS<=0){
      BS-=10;
      RS+=10;
      if(RS>=255){
      RS=255;
      BS=0;
      GS+=10;
    }
        if(BS<=0){
      BS=0;
    }
    }else if(BS<=0){
      RS-=10;
      GS+=10;
    if(GS>=255){
      GS=255;
      RS=0;
      BS+=10;
    }
      if(RS<=0){
      RS=0;
    }
    }
  pixels.fill(pixels.Color(RS, GS, BS));
  pixels.setBrightness(W);
  pixels.show();
  }
  }else if(sleep_mode==0){
  pixels.clear();
  pixels.show();
  }
}
//blinker回调函数
void button1_callback(const String & state) {
Blinker.vibrate();
Blinker.print("IP:",WiFi.localIP().toString());
Open_door=lv_task_create(opendoor,0,LV_TASK_PRIO_HIGHEST,NULL);
lv_task_once(Open_door);
}
void blinkertask(lv_task_t* task){
    Blinker.run();
}
void zhuce_callback(const String & state){
  Blinker.vibrate();
  zhuce=lv_task_create(zczw,0,LV_TASK_PRIO_HIGHEST,NULL);
  lv_task_once(zhuce);
}
void rgb1_callback(uint8_t r_value, uint8_t g_value, uint8_t b_value, uint8_t bright_value){
R=r_value;G=g_value;B=b_value;W=bright_value;
}
void buttonrgb_callback(const String & state) {
switch (rgbmode){
  case 0:rgbmode=1;  Blinker.print("rgbmode:","呼吸灯");break;
  case 1:rgbmode=2;  Blinker.print("rgbmode:","彩色灯");break;
  case 2:rgbmode=0;  Blinker.print("rgbmode:","常亮");break;
}
}
void startCameraServer();

void setup() {
pinMode(2,OUTPUT);
digitalWrite(2,1);
pixels.begin();
pixels.clear();
Serial.begin(57600);
tft.begin();
   camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  //init with high specs to pre-allocate larger buffers
  if(psramFound()){
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
//    return;
  }else{
sensor_t * s = esp_camera_sensor_get();
s->set_brightness(s, -2);     // -2 to 2
s->set_contrast(s, 2);       // -2 to 2
s->set_saturation(s, 0);     // -2 to 2
s->set_special_effect(s, 0); // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
s->set_whitebal(s, 1);       // 0 = disable , 1 = enable
s->set_awb_gain(s, 1);       // 0 = disable , 1 = enable
s->set_wb_mode(s, 0);        // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
s->set_exposure_ctrl(s, 1);  // 0 = disable , 1 = enable
s->set_aec2(s, 0);           // 0 = disable , 1 = enable
s->set_ae_level(s, 0);       // -2 to 2
s->set_aec_value(s, 300);    // 0 to 1200
s->set_gain_ctrl(s, 1);      // 0 = disable , 1 = enable
s->set_agc_gain(s, 0);       // 0 to 30
s->set_gainceiling(s, (gainceiling_t)0);  // 0 to 6
s->set_bpc(s, 0);            // 0 = disable , 1 = enable
s->set_wpc(s, 1);            // 0 = disable , 1 = enable
s->set_raw_gma(s, 1);        // 0 = disable , 1 = enable
s->set_lenc(s, 1);           // 0 = disable , 1 = enable
s->set_hmirror(s, 1);        // 0 = disable , 1 = enable
s->set_vflip(s, 0);          // 0 = disable , 1 = enable
s->set_dcw(s, 1);            // 0 = disable , 1 = enable
s->set_colorbar(s, 0);       // 0 = disable , 1 = enable
s->set_framesize(s, FRAMESIZE_SVGA);
s->set_pixformat(s,PIXFORMAT_JPEG);

  }
ZA_Init();
Blinker.begin(auth,ssid,pswd);

ledcSetup(channel, freq, a_resolution); // 设置通道
ledcAttachPin(led, channel);          // 将通道与对应的引脚连接
ledcWrite(channel, calculatePWM(180));
lv_init();
lv_disp_buf_init(&disp_buf, buf,NULL, LV_HOR_RES_MAX * 10);
/*Initialize the display*/
lv_disp_drv_t disp_drv;
lv_disp_drv_init(&disp_drv);
disp_drv.hor_res = 240;
disp_drv.ver_res = 240;
disp_drv.flush_cb = my_disp_flush;
disp_drv.buffer = &disp_buf;
lv_disp_drv_register(&disp_drv);
task_jc_start();

//背景
scr=lv_scr_act();
lv_style_copy(&p_back,&lv_style_plain);
p_back.body.main_color=lv_color_hex(0x1C4C2D);
p_back.body.grad_color=lv_color_hex(0x1C4C2D);
lv_obj_set_style(scr,&p_back);
lv_style_copy(&p_cont,&lv_style_btn_ina);
p_cont.body.radius=15;
p_cont.body.opa=50;
lv_style_copy(&p_status_wifi,&lv_style_transp);
p_status_wifi.image.color=lv_color_hex(0xD0D9D3);
p_status_wifi.image.intense=200;
lv_style_copy(&p_status_door,&p_status_wifi);
lv_style_copy(&p_status_face,&p_status_wifi);
lv_style_copy(&p_status_bluetooth,&p_status_wifi);
lv_style_copy(&p_clock,&lv_style_transp);
p_clock.image.color=lv_color_hex(0x499E88);
cont1=lv_cont_create(scr,NULL);
cont2=lv_cont_create(scr,NULL);
cont3=lv_cont_create(scr,NULL);
cont4=lv_cont_create(scr,NULL);
cont5=lv_cont_create(scr,NULL);
wifi=lv_img_create(cont1,NULL);
bluetooth=lv_img_create(cont1,NULL);
face=lv_img_create(cont3,NULL);
door=lv_img_create(cont3,NULL);
clk1=lv_img_create(cont2,NULL);
clk2=lv_img_create(cont2,NULL);
clk3=lv_img_create(cont2,NULL);
clk4=lv_img_create(cont2,NULL);
picture=lv_img_create(cont4,NULL);
youxia=lv_img_create(cont5,NULL);
lv_img_set_src(wifi,&wifioff);
lv_img_set_src(bluetooth,&lanyaoff);
lv_img_set_src(door,&menguan);
lv_img_set_src(face,&renlianoff);
lv_img_set_src(picture,&zp1);
lv_img_set_src(clk1,&n0);
lv_img_set_src(clk2,&n0);
lv_img_set_src(clk3,&n0);
lv_img_set_src(clk4,&n0);
lv_img_set_src(youxia,&welcome);
lv_img_set_style(clk1,LV_IMG_STYLE_MAIN,&p_clock);
lv_img_set_style(clk2,LV_IMG_STYLE_MAIN,&p_clock);
lv_img_set_style(clk3,LV_IMG_STYLE_MAIN,&p_clock);
lv_img_set_style(clk4,LV_IMG_STYLE_MAIN,&p_clock);
lv_img_set_style(wifi,LV_IMG_STYLE_MAIN,&p_status_wifi);
lv_img_set_style(bluetooth,LV_IMG_STYLE_MAIN,&p_status_bluetooth);
lv_img_set_style(door,LV_IMG_STYLE_MAIN,&p_status_door);
lv_img_set_style(face,LV_IMG_STYLE_MAIN,&p_status_face);
lv_img_set_style(youxia,LV_IMG_STYLE_MAIN,&p_status_bluetooth);
lv_obj_set_pos(wifi,4,5);
lv_obj_set_pos(bluetooth,4,63);
lv_obj_set_pos(door,4,5);
lv_obj_set_pos(face,4,63);
lv_obj_set_pos(clk1,26,10);
lv_obj_set_pos(clk2,51,10);
lv_obj_set_pos(clk3,26,54);
lv_obj_set_pos(clk4,51,54);
lv_obj_set_pos(cont1,2,2);
lv_obj_set_size(cont1,56,116);
lv_obj_set_pos(cont2,62,2);
lv_obj_set_size(cont2,116,116);
lv_obj_set_pos(cont3,182,2);
lv_obj_set_size(cont3,56,116);
lv_obj_set_pos(cont4,2,122);
lv_obj_set_size(cont4,116,116);
lv_obj_set_pos(cont5,122,122);
lv_obj_set_size(cont5,116,116);
lv_obj_set_style(cont1,&p_cont);
lv_obj_set_style(cont2,&p_cont);
lv_obj_set_style(cont3,&p_cont);
lv_obj_set_style(cont4,&p_cont);
lv_obj_set_style(cont5,&p_cont);
lv_obj_align_origo(picture,cont4,LV_ALIGN_CENTER,0,0);
lv_obj_set_auto_realign(picture,true);
lv_obj_align_origo(youxia,cont5,LV_ALIGN_CENTER,0,0);
WIFIjiance=lv_task_create(wifijiance_cb,1024,LV_TASK_PRIO_HIGH,NULL);
Button1.attach(button1_callback);
Button2.attach(zhuce_callback);
Button3.attach(buttonrgb_callback);
RGB1.attach(rgb1_callback);
BLINKERTASK=lv_task_create(blinkertask,10,LV_TASK_PRIO_HIGHEST,NULL);
Fclock=lv_task_create(Fclock_cd,1000,LV_TASK_PRIO_HIGH,NULL);
qiehuan=lv_task_create(qh,3000,LV_TASK_PRIO_MID,NULL);
RGBMODE=lv_task_create(rgb_mode_c,100,LV_TASK_PRIO_HIGH,NULL);
  if (err == ESP_OK){
startCameraServer();
Serial.println("CameraServer start");
  }
}



void loop() {
    lv_task_handler();
}
