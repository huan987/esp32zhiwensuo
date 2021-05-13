#include"ZA618.h"

void ZA_Init(){
ValidTempleteNum=ZA_getnum();
}

void ZA_start_write(){
    delay(10);
    uint8_t head[6]={0XEF,0X01,0XFF,0XFF,0XFF,0XFF};
    Serial.write(head,6);
}
void ZA_write_CMD(){
    Serial.write(0x01);
}

uint16_t ZA_getnum(){
    while(Serial.read() >= 0){}
    ZA_start_write();
    ZA_write_CMD();
    ZA_write(ReadVTNum);
    uint8_t buffer[20];
    Getback(buffer);
    return (uint16_t)buffer[11];
}
//自动注册
void ZA_Enrollstart(){
    uint8_t ID=ZA_GetID();
    while(Serial.read() >= 0){}
    ZA_start_write();
    ZA_write_CMD();
    ZA_write(AutoEnroll);
    Serial.write(0x00);
    Serial.write(ID);
    Serial.write(0x04);
    Serial.write(0x00);
    Serial.write(0x06);
    Serial.write(0x00);
    Serial.write(ID+4+6+1+8+0x31);
}
//注册结果
uint8_t ZA_Enrollcl(){
if(Serial.available()>=14){
uint8_t buffer[20];
Serial.readBytes(buffer,14);
if((buffer[9]==0x00)&&(buffer[10]==0x03)){
    return buffer[11];
}else if((buffer[9]==0x00)&&(buffer[10]==0x06)&&(buffer[11]==0xF2)){
    return 5;
}else if((buffer[9]==0x00)&&(buffer[10]==0x06)&&(buffer[11]==0xF2)){
    return 6;
}else if((buffer[9]==0x00)&&(buffer[10]==0x00)){
    return 0;
}
}
return 10;
}
//自动检测
int ZA_Identify(){
    while(Serial.read() >= 0){}
    ZA_start_write();
    ZA_write_CMD();
    ZA_write(AutoIdentify);
    uint8_t buffer[20];
    Getback(buffer);
    if((buffer[9]==0x00)&&(buffer[10]==0x05)){
        return buffer[12];
    }else if((buffer[9]==0x09)&&(buffer[10]==0x05)){
        return 2048;
    }else{
        // ZA_Cancel();
        return -1;
    }
}
//删除模板
bool ZA_Delect(uint8_t ID){
    while(Serial.read() >= 0){}
    ZA_start_write();
    ZA_write_CMD();
    ZA_write(DeletChar);
    Serial.write(0x00);
    Serial.write(ID);
    Serial.write(0x00);
    Serial.write(0x01);
    Serial.write(0x00);
    Serial.write(0x12+ID+1);
    uint8_t buffer[20];
    Getback(buffer);
    return !buffer[9];
}
//清空指纹库
uint8_t ZA_Empty(){
    while(Serial.read() >= 0){}
    ZA_start_write();
    ZA_write_CMD();
    ZA_write(Empty);
    uint8_t buffer[20];
    Getback(buffer);
    return buffer[9];
}
//取消指令
void ZA_Cancel(){
    ZA_start_write();
    ZA_write_CMD();
    ZA_write(Cancel);
    delay(1);
    while(Serial.read() >= 0){}
}
uint8_t ZA_GetID(){
    while(Serial.read() >= 0){}
    ZA_start_write();
    ZA_write_CMD();
    ZA_write(GetDummyTempleteNo);
    uint8_t buffer[20]={0};
    Getback(buffer);
    if(buffer[9]==0x1F){
        return 0;
    }
    return buffer[11];
}
void Getback(uint8_t* buffer){
    int waittime=1000;
    while((!Serial.available())&&waittime){
        waittime--;
        delay(1);
    }
    uint8_t i=0;
    delay(5);
    while (Serial.available())
    {
        buffer[i]=Serial.read();
        i++;
    }
}
void ZA_closelight(){
    ZA_start_write();
    ZA_write_CMD();
    ZA_write(closelight);
}
