#include <reg52.h>

#define uchar unsigned char
#define uint  unsigned int

//这三个引脚参考资料
sbit E=P2^5;		//1602使能引脚
sbit RW=P2^6;		//1602读写引脚	
sbit RS=P2^7;		//1602数据/命令选择引脚

void delay();
uchar Convert(uchar );
void enable(uchar );
void write(uchar );
void L1602_init(void);
void L1602_char(uchar ,uchar ,char );
void L1602_string(uchar ,uchar ,uchar *);
void L1602_int(uchar, uchar, int);
