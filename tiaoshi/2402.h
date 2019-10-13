
#ifndef _2402_H_
#define _2402_H_

#include <reg52.h>

//ģ��I2C ���ߵ����Ŷ���
sbit SCL = P1^0;
sbit SDA = P1^1;
//����2402 ��I2C ����Э���еĴӻ���ַ
//#define EEPROM_I2C_ADDR 0x00

void DELAY(unsigned int t);          //��ʱ����
void IICStart(void);         //I2C��ʼ����
void IICStop(void);          //I2C��������
void Ack(void);              //I2CӦ����
void NOAck(void);            //I2CӦ��Ǻ���
bit Check_Ack(void);         //I2CӦ���⺯��
void SEND0(void);            //I2C����0����
void SEND1(void);            //I2C����1���� 
void Write_byte(unsigned char dat);  //I2C�����ֽں���
unsigned char Read_byte(void);//I2C��ȡ�ֽں���
unsigned char rdeeprom(unsigned char addr);    //��AT2402����
void wrteeprom(unsigned char addr,unsigned char dat);  //дAT2402����

#endif //_2402_H_