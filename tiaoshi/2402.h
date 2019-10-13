
#ifndef _2402_H_
#define _2402_H_

#include <reg52.h>

//模拟I2C 总线的引脚定义
sbit SCL = P1^0;
sbit SDA = P1^1;
//定义2402 在I2C 总线协议中的从机地址
//#define EEPROM_I2C_ADDR 0x00

void DELAY(unsigned int t);          //延时函数
void IICStart(void);         //I2C开始函数
void IICStop(void);          //I2C结束函数
void Ack(void);              //I2C应答函数
void NOAck(void);            //I2C应答非函数
bit Check_Ack(void);         //I2C应答检测函数
void SEND0(void);            //I2C发送0函数
void SEND1(void);            //I2C发送1函数 
void Write_byte(unsigned char dat);  //I2C发送字节函数
unsigned char Read_byte(void);//I2C读取字节函数
unsigned char rdeeprom(unsigned char addr);    //读AT2402函数
void wrteeprom(unsigned char addr,unsigned char dat);  //写AT2402函数

#endif //_2402_H_