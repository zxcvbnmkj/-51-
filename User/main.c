/*************************************************************************************
ʵ�����󣺿��Էֱ�ʹ�ð������ֻ��������������ַ�ʽ������ledС�ƣ�����Ͳ��������
������һ��TFT��Ļ��ʾ����Ԫ����״̬��ÿ��һ�°�������һ�����ݣ�Ԫ����״̬�͸ı�һ�Ρ�

ʵ��˵����ʵ����С�Ƶ������ʾ�ҵ��С���ơ������𣬶����ת90�ȴ���Ҿߡ��š��Ŀ���
���������˳ʱ����ת����������������ʱ����ת���������ϡ�

����˵����
hex�ļ���output�ļ�����
k8,k7,k6�ֱ��P24,P23,P22�����������֣�
P07�뽻ͨ��ģ���D6���ӣ�LEDС�Ʋ��֣�
�����VCC�ӿ�������5V,GND��GND��PWM�߽�P03��������֣�
���������A+,A-,B+,B-�ֱ����J81��OA,OB,OC,OD��
IA,IB,IC,ID�ֱ��P00,P01,P02,P03(�����������)
����ģ���VCC,GND�ֱ��뿪�����ϵ�5V��GND����
TXD��P30,RXD��P31���������֣�

ע�����
1��һ��ֻ�ܰ���һ������������2����3������ͬʱ���¡�
2��ͨ���������ֻ����͡�01��ʱ���״̬�ı䣬���͡�02��ʱ�ŵ�״̬�ı�
���͡�03��ʱ������״̬�ı䡣������ͱ�ģ�����С����������������յ�
��Error!����Ϊ���͵������д�����Ҫ���·��͡�
*************************************************************************************/
#include "tftlcd.h"
#include "reg52.h"
#define SPEED 1900 //�޸Ĵ�ֵ�ɸı�����ת�ٶ�
sbit led1=P0^7;
sbit pwm=P0^6;
sbit MOTOA = P0^0;
sbit MOTOB = P0^1;
sbit MOTOC = P0^2;
sbit MOTOD = P0^3;
sbit key1=P2^4;//k8����ƿ���
sbit key2=P2^3;//k7���ſ���
sbit key3=P2^2;//k6����������
bit g1=1,g2=1,g3=1;//1�������״̬δ�ı�
bit flag1=1,flag2=1,flag3=1;/*����3�������洢������ǰ״̬��Ϊ1��ʾ����״̬Ϊ�أ�Ϊ0Ϊ��*/
bit f=1;/*Ĭ��Ϊ1����Ϊ1��ʾ���յ����ֻ��ı����״̬�����ݡ�ֻ����Ϊ1�����а�������ʱ
����Ż�����keyout�����е�ѭ��.�������ͣ������ȴ��������»�f��Ϊ1*/
char ascii[7]={"Error!"};//���ֻ����͵����ݴ��󣬾����ֻ���������������
unsigned char i=0;//�ñ�����ȡ�ֻ����͵�����
unsigned char jd=1;//�Ƕȱ�ʶ,Ĭ����Ϊ��
unsigned char count=0;//0.5ms������ʶ���������Ϊ0
void FontShow_Init();//��������
void Time0_Init();
void initUART();
void keyout();
void tftFontChange();
void doorChange();
void clockwise();
void antiClockwise();
void curtainChange();
void main()
{
	TFTLCD_Init();//��ʼ����Ļ
	FontShow_Init();//��ʼ����Ļ��ʾ
	Time0_Init();//�򿪶�ʱ��0
	initUART();//��ʼ������	
  //���¼���Ϊ����״̬��ʼ��
	led1=flag1;
	//jd=1;//��һ������Ҫ�ģ���Ϊ�ڶ������ʱ��ɣ��ʿ�ʡ��
	MOTOA=0;
	MOTOB=0;
	MOTOC=0;
	MOTOD=0;
	while(1)//���ϼ�⣬�Ӷ��ı����״̬
	{		
		keyout();	
		tftFontChange();
		if(g1==0){led1=flag1;g1=1;}		
		if(g2==0){doorChange();	g2=1;}
		if(g3==0){curtainChange();g3=1;}
	}
}
void FontShow_Init()
{
	FRONT_COLOR=RED;
	LCD_ShowFontHZ(10,10,"���״̬����");
	LCD_ShowFontHZ(10,50,"��״̬����");
	LCD_ShowFontHZ(10,90,"����״̬����");	
}
void keyout()
{
	unsigned int a;
	key1=1;
	key2=1;
	key3=1;
	while(key1==1&&key2==1&&key3==1&&f==1);//δ������ͣ����
	if(f==0)f=1;
	if(key1==0||key2==0||key3==0)//key=0˵��������������
	{
		for(a=0;a<1000;a++);//��ʱԼ10ms
		if(key1==0||key2==0||key3==0)//�ٴμ���Ƿ�����а�������
		{
			if(key1==0){flag1=~flag1;g1=0;}
			if(key2==0){flag2=~flag2;g2=0;}
			if(key3==0){flag3=~flag3;g3=0;}
		}
		while(!(key1==1&&key2==1&&key3==1));//ֻҪȫ��������ƽ��Ϊ1�ſ�ͨ����ѭ��
		for(a=0;a<1000;a++);//��ʱԼ10ms
		while(!(key1==1&&key2==1&&key3==1));//�ٴμ��
	}
}
void tftFontChange()
{
	if(g1==0)
	{
	  if(flag1==1)LCD_ShowFontHZ(170,10,"��");//������2���ֵĺ�������170
	  else LCD_ShowFontHZ(170,10,"��");//һ���ֿ�32��32*5+10=170
	}
	if(g2==0)
	{
	  if(flag2==1)LCD_ShowFontHZ(138,50,"��");//��״̬����138������ͬ��
	  else LCD_ShowFontHZ(138,50,"��");
	}
	if(g3==0)
	{
	  if(flag3==1)LCD_ShowFontHZ(170,90,"��");	
	  else LCD_ShowFontHZ(170,90,"��");		
	}
}
void Time0_Init()          //��ʱ����ʼ��
{
	TMOD=0X21;//��ʱ��0Ϊ0001����ʱ��1Ϊ0010��TMOD����λѰַ������������һ���Ը�ֵ��   
	EA=1;//�����ж�
	ET0=1;//�򿪶�ʱ��0���ж�
	TH0  = 0xfe;
	TL0  = 0x33; //11.0592MZ����0.5ms
	TR0=1; //��ʱ����ʼ
}

void initUART()//������9600
{
	SCON=0x50;
	TH1=0XFA;
	TL1=TH1;
	PCON=0X80;//ͨ���������SMODλΪ1
	ES=1;
	//EA=1;���ж���֮ǰ�ĺ���Time0_Init()���Ѵ򿪣���������ע�͵�
	ET1=0;
	TR1=1;
}
void doorChange()
{
	  if(flag2==1){jd=1;count=0;}//flag2ֻ��0��1����ֵ
	  else {jd=3;count=0;}
}
void clockwise()//�������˳ʱ����ת
{
	unsigned char j;
	for(j=0;j<20;j++)
	{
		MOTOA = 1;
		MOTOB = 1;
		MOTOC = 0;
		MOTOD = 1;//-B
		delay_10us(SPEED);
		
		MOTOA = 0;
		MOTOB = 1;//-A
		MOTOC = 1;
		MOTOD = 1;
		delay_10us(SPEED);
		
		MOTOA = 1;
		MOTOB = 1;
		MOTOC = 1;//B
		MOTOD = 0;
		delay_10us(SPEED);
		
		MOTOA = 1;//A
		MOTOB = 0;
		MOTOC = 1;
		MOTOD = 1;
		delay_10us(SPEED);
	}
}
void antiClockwise()
{
	unsigned char j;
	for(j=0;j<20;j++)//���������ʱ����ת
	{	
		MOTOA = 1;//A
		MOTOB = 0;
		MOTOC = 1;
		MOTOD = 1;
		delay_10us(SPEED);

		MOTOA = 1;
		MOTOB = 1;
		MOTOC = 1;//B
		MOTOD = 0;
		delay_10us(SPEED);

		MOTOA = 0;
		MOTOB = 1;//-A
		MOTOC = 1;
		MOTOD = 1;
		delay_10us(SPEED);

		MOTOA = 1;
		MOTOB = 1;
		MOTOC = 0;
		MOTOD = 1;//-B
		delay_10us(SPEED);												
	}
}
void curtainChange()
{
	  if(flag3==0)clockwise();
	  else antiClockwise();
}
void Time0_Int() interrupt 1     //��ʱ��0�ж�,TF0=1ʱ���������ȼ��ȴ����жϸ�
{
	TH0  = 0xfe;                 //���¸�ֵ
	TL0  = 0x33;
	if(count< jd)              	 //�ж�0.5ms�����Ƿ�С�ڽǶȱ�ʶ
	pwm=1;                       //ȷʵС�ڣ�PWM����ߵ�ƽ
	else
	pwm=0;                       //����������͵�ƽ
	count=(count+1);             //0.5ms������1	             
	if(count==40){count=0;}//����ʼ�ձ���Ϊ40 ����������Ϊ20ms
}
void uart() interrupt 4//RI=1ʱ����
{
	unsigned char j=0;
	if(RI==1)
	{
		RI=0;
		i=SBUF;
	}
	switch(i)
	{
		case 0x01:{flag1=~flag1;g1=0;break;}
		case 0x02:{flag2=~flag2;g2=0;break;}
		case 0x03:{flag3=~flag3;g3=0;break;}
		default://����ֻ����͵������д�Ļ�����ͨ���������ֻ�����"error!".
			{
				for(j=0;j<6;j++)
				{
					SBUF=ascii[j];
			    while(!TI);
				  TI=0;
				}
			}
	}
	f=0;	
}