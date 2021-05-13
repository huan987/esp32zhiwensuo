//指纹模块
#include<Arduino.h>
#define ZA_write(command) Serial.write(command,sizeof(command))
static uint16_t ValidTempleteNum=0;

const uint8_t AutoEnroll[]={0x00,0x08,0x31};
const uint8_t AutoIdentify[]={0x00,0x08,0x32,0x03,0xFF,0xFF,0x00,0x06,0x00,0x00};
const uint8_t DeletChar[]={0x00,0x07,0x0C};
const uint8_t Empty[]={0x00,0x03,0x0d,0x00,0x11};
const uint8_t ReadVTNum[]={0x00,0x03,0x1D,0x00,0x21};
const uint8_t GetChipEcho[]={0x00,0x03,0x35,0x00,0x39};
const uint8_t GetDummyTempleteNo[]={0x00,0x03,0x68,0x00,0x6C};
const uint8_t Cancel[]={0x00,0x03,0x30,0x00,0x33};
const uint8_t closelight[]={0x00,0x06,0x3c,0x04,0x00,0x01,0x00,0x47};

void ZA_Init();
void ZA_start_write();
void ZA_write_CMD();
uint16_t ZA_getnum();
//自动注册
void ZA_Enrollstart();
//注册结果
uint8_t ZA_Enrollcl();
//自动检测
int ZA_Identify();
//删除模板
bool ZA_Delect(uint8_t ID);
//清空指纹库
uint8_t ZA_Empty();
//取消指令
void ZA_Cancel();
uint8_t ZA_GetID();
void Getback(uint8_t* buffer);
void ZA_closelight();
