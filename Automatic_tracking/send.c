#include<reg52.h>
#include<string.h>

#define _READY_  0X06//���������ź�
#define _BUSY_   0x15//�ӻ�æӦ��
#define _OK_     0x00//�ӻ�׼����
#define _SUCC_   0X0F//���ݴ���ɹ�
#define _ERR_    0XF0//���ݴ���ʧ��
#define uchar  unsigned char


//unsigned char Dat[9]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f,0xff};
//unsigned char Buff[20];//���ݻ�����
uchar temp=_BUSY_; 
uchar  lenth;
extern uchar  display_o;

/*void delay_1ms(unsigned int t)
{
 unsigned int x,y;
 for(x=t;x>0;x--)
  for(y=110;y>0;y--);
}

void Buff_init(unsigned char *Dat)  //��������ʼ��
{
unsigned char i;
	for(i=0;i<9;i++)
	{
	Buff[i]=Dat[i];
	delay_1ms(1);
	}
}*/

void usart_init() //���ڳ�ʼ��
{
 TMOD=0x20;   //��ʱ��1�����ڷ�ʽ2
 TH1=0xfd;  
 TL1=0xfd;   //������Ϊ9600
 PCON=0;
 SCON=0x50;  //���ڹ����ڷ�ʽ1
 TR1=1;      //������ʱ��
 TI=0;
 RI=0;
}

void send_data(uchar DA[8],uchar D1)
{
uchar i;
//uchar k;
uchar check;
/*uchar long_;
uchar transfer[8];

long_=strlen(DA);
for(k=0;k<long_;k++)
{transfer[k]=DA[k]-0x30;}
transfer[k]='\0';*/
D1=display_o;
lenth=strlen(DA);//�������ݳ���
check=lenth;
TI=0;
SBUF=lenth;        //�������ݳ���
while(!TI);
TI=0;

	for(i=0;i<lenth;i++) //��������
	{
	 check=check^(DA[i]-0X30);//У��λ��^Ϊ��λ��򣬶����ݵİ�λͬʱ��λ�����ͬΪ0����ͬΪ1
	 SBUF=DA[i]-0X30;
	 while(!TI);
	 TI=0;
	}

   SBUF=D1;  //������ʾ��־λ
   while(!TI);
   TI=0;

   SBUF=check;	   //����У���ֽ�
   while(!TI);
   TI=0;
}

void send(uchar DAT[8],uchar D2)
{
			//Buff_init(DAT);
			usart_init();
		    TI=0;    
            SBUF=_READY_;
            while(!TI);
            TI=0;
			
			while(temp!=_OK_)//�����ȴ��ӻ�Ӧ���ź�
			{
			 RI=0;
			 while(!RI);
			 temp=SBUF;
			 RI=0;
			}

			temp=_ERR_ ;
			while(temp!=_SUCC_) //
			{
    		 send_data(DAT,D2);
     	  	 RI=0;
    		 while(!RI);
    		 temp=SBUF;
    		 RI=0;
			}
}
