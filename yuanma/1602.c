/********************************************************************
* 文件名  ： 液晶1602显示.c
* 描述    :  该程序实现了对液晶1602的控制。
***********************************************************************/
#include "1602.h"
#include "math.h"

/********************************************************************
* 名称 : delay()
* 功能 : 延时,延时时间大概为140US。
* 输入 : 无
* 输出 : 无
***********************************************************************/

void delay()
{
	int i,j;
	for(i=0; i<=10; i++)
	for(j=0; j<=2; j++)
;
}

/********************************************************************
* 名称 : Convert(uchar In_Date)
* 功能 : 因为电路设计时，P0.0--P0.7接法刚好了资料中的相反，所以设计该函数。
* 输入 : 1602资料上的值
* 输出 : 送到1602的值
***********************************************************************/
uchar Convert(uchar In_Date)
{
    /*
    uchar i, Out_Date = 0, temp = 0;
    for(i=0; i<8; i++)
    {
        temp = (In_Date >> i) & 0x01;
        Out_Date |= (temp << (7 - i));
    }
	
    return Out_Date;
	*/
    return In_Date;

}
/********************************************************************
* 名称 : enable(uchar del)
* 功能 : 1602命令函数
* 输入 : 输入的命令值
* 输出 : 无
***********************************************************************/
void enable(uchar del)
{	
	RS = 0;
	RW = 0;
	P0 = Convert(del);
	E = 1;
	delay();
	E = 0;
	delay();
}

/********************************************************************
* 名称 : write(uchar del)
* 功能 : 1602写数据函数
* 输入 : 需要写入1602的数据
* 输出 : 无
***********************************************************************/

void write(uchar del)
{  
	RS = 1;
	RW = 0;
	P0 = Convert(del);
	E = 1;
	delay();
	E = 0;
	delay();
}

/********************************************************************
* 名称 : L1602_init()
* 功能 : 1602初始化，请参考1602的资料
* 输入 : 无
* 输出 : 无
***********************************************************************/
void L1602_init(void)
{
	enable(0x38);
	enable(0x0c);
	enable(0x06);
	enable(0x01);
	enable(0xd0);
}

/********************************************************************
* 名称 : L1602_char(uchar hang,uchar lie,char sign)
* 功能 : 改变液晶中某位的值，如果要让第一行，第五个字符显示"b" ，调用该函数如下
		 L1602_char(1,5,'b')
* 输入 : 行，列，需要输入1602的数据
* 输出 : 无
***********************************************************************/
void L1602_char(uchar hang,uchar lie,char sign)
{
	uchar a;
	if(hang == 1) a = 0x80;
	if(hang == 2) a = 0xc0;
	a = a + lie - 1;
	enable(a);
	write(sign);
}

/********************************************************************
* 名称 : L1602_string(uchar hang,uchar lie,uchar *p)
* 功能 : 改变液晶中某位的值，如果要让第一行，第五个字符开始显示"ab cd ef" ，调用该函数如下
	 	 L1602_string(1,5,"ab cd ef;")
* 输入 : 行，列，需要输入1602的数据
* 输出 : 无
***********************************************************************/
void L1602_string(uchar hang,uchar lie,uchar *p)
{
	uchar a;
	if(hang == 1) a = 0x80;
	if(hang == 2) a = 0xc0;
	a = a + lie - 1;
	enable(a);
	while(1)
	{
		if(*p == '\0') break;
		write(*p);
		p++;
	}
}

//显示整型的温湿度数据用，共占用4位，其中一位符号位
void L1602_int(uchar hang, uchar lie, int num)
{
   uint temp;
   uint gewei,shiwei,baiwei,sign;
   if (num >= 0)
   {
      sign = 0;
   }
   else
   {
      sign = 1;
   }
   temp = abs(num);
   baiwei = temp / 100;
   temp = temp - baiwei*100;
   shiwei = temp / 10;
   gewei = temp - shiwei*10;
   num = abs(num);
   if (num>=100)
   {
      if (sign == 1) //负数
	  {
	     L1602_char(hang, lie, '-');
	  }
      L1602_char(hang, lie+1, baiwei+48);  
	  L1602_char(hang, lie+2, shiwei+48);  
	  L1602_char(hang, lie+3, gewei+48);  
   }
   else if (num>=10)
   {
      if (sign == 1)	   
	  {
	     L1602_char(hang, lie+1, '-');
	  }
	  L1602_char(hang, lie+2, shiwei+48);  
      L1602_char(hang, lie+3, gewei+48);
   }
   else
   {
      if (sign == 1)	   
	  {
	     L1602_char(hang, lie+2, '-');
	  }
      L1602_char(hang, lie+3, gewei+48);  
   }
}