#include <reg52.h>
#include "1602.h"
#include "dht.h"
#include "2402.h"

//��������LED��
sbit Led_qushi=P1^6;		//ȥʪ��
sbit Led_jiangwen=P1^5;		//���µ�
sbit fmq=P1^3;	//������
sbit Key_TH1 = P3^2;
sbit Key_TH2 = P3^3;
sbit Key_HH1 = P3^4;
sbit Key_HH2 = P3^5;

//�����ʶ
volatile bit FlagStartRH = 0;  //��ʼ��ʪ��ת����־
volatile bit FlagKeyPress = 0; //�м�����
volatile bit fmqt = 0;
volatile bit fmqh = 0;
//������ʪ�ȴ��������ⲿ����
extern U8  U8FLAG,k;
extern U8  U8count,U8temp;
extern U8  U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
extern U8  U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;
extern U8  U8comdata;
extern U8  count, count_r;

U16 temp;
S16 temperature, humidity;
S16 idata TH, HH;  //�¶����޺�ʪ������
char * pSave;
U8 keyvalue, keyTH1, keyTH2, keyHH1, keyHH2;

//�������
U16 RHCounter;


//���ݳ�ʼ��
void Data_Init()
{
   RHCounter = 0;
   Led_qushi = 1;
   Led_jiangwen = 1;
   TH = 40;
   HH = 85;
   keyvalue = 0;
   keyTH1 = 1;
   keyTH2 = 1;
   keyHH1 = 1;
   keyHH2 = 1;

}

//��ʱ��0��ʼ��
void Timer0_Init()
{
	ET0 = 1;        //����ʱ��0�ж�
	TMOD = 1;       //��ʱ��������ʽѡ��
	TL0 = 0x06;     
	TH0 = 0xf8;     //��ʱ�������ֵ
	TR0 = 1;        //������ʱ��
}

//��ʱ��0�ж�
void Timer0_ISR (void) interrupt 1 using 0
{
	TL0 = 0x06;
	TH0 = 0xf8;     //��ʱ�������ֵ

	//ÿ2��������һ����ʪ��ת��
    RHCounter ++;
    if (RHCounter >= 1000)
    {
       FlagStartRH = 1;
	   RHCounter = 0;
    }
}

//�����趨ֵ��
void Save_Setting()
{
   pSave =  (char *)&TH;	  //��ַ��λ��Ӧ��8λ����λ��Ӧ��8λ
   wrteeprom(0, *pSave);	  //���¶�����ֵTH��8λ
   DELAY(500);
   pSave ++;
   wrteeprom(1, *pSave);	  //���¶�����ֵTH��8λ
   DELAY(500);
   pSave =  (char *)&HH;
   wrteeprom(2, *pSave);	  //��ʪ������ֵRH��8λ
   DELAY(500);
   pSave ++;
   wrteeprom(3, *pSave);	  //��ʪ������ֵRH��8λ
   DELAY(500);

}

//�����趨ֵ��
void Load_Setting()
{
   pSave =  (char *)&TH;
   *pSave++ = rdeeprom(0);
   *pSave = rdeeprom(1);
   pSave = (char *)&HH;
   *pSave++ = rdeeprom(2);
   *pSave = rdeeprom(3);
   if ((TH>99)||(TH<0)) TH = 40;
   if ((HH>99)||(HH<0)) HH = 85;
}

void KeyProcess(uint num)
{
   switch (num)
   {
      case 1:
	     if (TH<99) TH++;
	     L1602_char(1, 15, TH/10+48);
	     L1602_char(1, 16, TH%10+48);
	     break;
	  case 2:
	     if (TH>1) TH--;
	     L1602_char(1, 15, TH/10+48);
	     L1602_char(1, 16, TH%10+48);
	     break;
	  case 3:
	     if (HH<99) HH++;
	     L1602_char(2, 15, HH/10+48);
	     L1602_char(2, 16, HH%10+48);
	     break;
	  case 4:
	     if (HH>1) HH--;
	     L1602_char(2, 15, HH/10+48);
	     L1602_char(2, 16, HH%10+48);
	     break;
	  default:
	     break;
   }
   Save_Setting();

}

/********************************************************************
* ���� : Main()
* ���� : ������
***********************************************************************/
void main()
{
    U16 i, j, testnum;

    EA = 0;

	Timer0_Init();  //��ʱ��0��ʼ��

    Data_Init();
	EA = 1;

	L1602_init();
	L1602_string(1,1,"Hua Hua`s Design");
	L1602_string(2,1,"HaHaHaHa");
	//��ʱ
	for (i=0;i<700;i++)
	   for (j=0;j<700;j++)
	   {;}
    //����
	L1602_string(1,1,"                ");
	L1602_string(2,1,"                ");
    L1602_string(1,1,"Tem:   C  TH:");
	L1602_string(2,1,"Hum:   %  HH:");
    
    //�����¶����޺�ʪ�������趨ֵ
	Load_Setting();
	L1602_char(1, 15, TH/10+48);
	L1602_char(1, 16, TH%10+48);
	L1602_char(2, 15, HH/10+48);
	L1602_char(2, 16, HH%10+48);


	while(1)
	{
       //��ʪ��ת����־���
		 if (FlagStartRH == 1)
		 {
		     TR0 = 0;
             testnum = RH();
 			 FlagStartRH = 0;
			 TR0 = 1;
             //������ʪ�ȣ�ֻȡ��������
			 humidity = U8RH_data_H;
			 temperature = U8T_data_H;
 			 //��ʾ��ʪ��
			 L1602_int(1,5,temperature);
		     L1602_int(2,5,humidity);	
		}
		//��ʪ�ȿ���
		if (temperature > TH) 
		{
		Led_jiangwen = 0;
		fmqt=1;
		}
		else 
		{
		Led_jiangwen = 1;
		fmqt=0;
		}			        //����
		if (humidity > HH)
		{ 
		Led_qushi = 0;
		fmqh=1;
		}
		else 
		{
		Led_qushi = 1;
		fmqh=0;
		}						//ȥʪ
		if((fmqt==1)||(fmqh==1))
		fmq=0;
		else
		fmq=1;
		 
		//���̲�ѯ���ڵ���ʱ��Ӧ
		if ((Key_TH1)&&(keyTH1==0)) {FlagKeyPress = 1; keyvalue = 1;}
        else if ((Key_TH2)&&(keyTH2==0)) {FlagKeyPress = 1; keyvalue = 2;}
        else if ((Key_HH1)&&(keyHH1==0)) {FlagKeyPress = 1; keyvalue = 3;}
        else if ((Key_HH2)&&(keyHH2==0)) {FlagKeyPress = 1; keyvalue = 4;}
		if (FlagKeyPress == 1)
		{
           KeyProcess(keyvalue);
           FlagKeyPress = 0;           
		}
        if (!Key_TH1) keyTH1 = 0;
		else keyTH1 = 1;
        if (!Key_TH2) keyTH2 = 0;
		else keyTH2 = 1;
        if (!Key_HH1) keyHH1 = 0;
		else keyHH1 = 1;
        if (!Key_HH2) keyHH2 = 0;
		else keyHH2 = 1;
		 
	}	
}