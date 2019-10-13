#include "2402.h"

/*********************************
      BASE  DRIVE
**********************************/
void DELAY(unsigned int t)
{
	while(t!=0)
	t--;
}

/******************************
      IIC  DRIVE
******************************/
void IICStart(void)
{
     SCL=0;  
 	 DELAY(1);
     SDA=1; SCL=1; DELAY(1);
     SDA=0; DELAY(1);
     SCL=0; 
}

void IICStop(void)
{
	 SDA=0;SCL=1; DELAY(1);
     SDA=1; DELAY(1);
     SCL=0; 	
}
void SEND0(void)
{
	SDA=0;
	SCL=1;
	DELAY(1);
	SCL=0;
}

void SEND1(void)
{
	SDA=1;
	DELAY(1);
	SCL=1;
	DELAY(1);
	SCL=0;
}

bit Check_Ack(void)
{
   unsigned char errtime=250;
   DELAY(1);
   SCL=1; 
   DELAY(1);
   CY=SDA;
   while(CY)
   {
    errtime--; 
    CY=SDA;
    if (!errtime)
     {
     IICStop();
    
     return 1;
     }
}
 DELAY(1);
 SCL=0; 
 return 0;
}


void Write_byte(unsigned char dat) 
{
    unsigned char i;
	for(i=0;i<8;i++)
	{	
		if((dat<<i)&0x80)
		SEND1();
		else
		SEND0();
	}
}


unsigned char Read_byte(void)
{
	unsigned char i,temp=0;     
	for(i=0;i<8;i++)
	{
        SDA=1;
	    SCL=1;
	    DELAY(1);  
		if(SDA==1)
		{
			temp=temp<<1;
			temp=temp|0x01;
		}
		else
			temp=temp<<1;
		 SCL=0;	
	  }
	  return temp;
}

/************************************
      EEPROM  DRIVE
	  Addr:from 0x00-->
*************************************/
unsigned char rdeeprom(unsigned char addr)
{ 
        unsigned char temp=0;
		bit flag=0;
		IICStart();
		Write_byte(0xa0);
        Check_Ack();
		Write_byte(addr);
		Check_Ack();
		IICStart();
		Write_byte(0xa1);
		Check_Ack();
		temp=Read_byte();
		SEND1();
		IICStop();
 	    return temp;   
}  
    
   
void wrteeprom(unsigned char addr,unsigned char dat)
{
       	IICStart();
		Write_byte(0xa0);
		Check_Ack();
		Write_byte(addr);
		Check_Ack();
		Write_byte(dat);
		Check_Ack();
		IICStop();
}  

