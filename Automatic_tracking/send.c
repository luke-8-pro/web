#include<reg52.h>
#include<string.h>

#define _READY_  0X06//主机握手信号
#define _BUSY_   0x15//从机忙应答
#define _OK_     0x00//从机准备好
#define _SUCC_   0X0F//数据传输成功
#define _ERR_    0XF0//数据传输失败
#define uchar  unsigned char


//unsigned char Dat[9]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f,0xff};
//unsigned char Buff[20];//数据缓冲区
uchar temp=_BUSY_; 
uchar  lenth;
extern uchar  display_o;

/*void delay_1ms(unsigned int t)
{
 unsigned int x,y;
 for(x=t;x>0;x--)
  for(y=110;y>0;y--);
}

void Buff_init(unsigned char *Dat)  //缓冲区初始化
{
unsigned char i;
	for(i=0;i<9;i++)
	{
	Buff[i]=Dat[i];
	delay_1ms(1);
	}
}*/

void usart_init() //串口初始化
{
 TMOD=0x20;   //定时器1工作于方式2
 TH1=0xfd;  
 TL1=0xfd;   //波特率为9600
 PCON=0;
 SCON=0x50;  //串口工作于方式1
 TR1=1;      //开启定时器
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
lenth=strlen(DA);//计算数据长度
check=lenth;
TI=0;
SBUF=lenth;        //发送数据长度
while(!TI);
TI=0;

	for(i=0;i<lenth;i++) //发送数据
	{
	 check=check^(DA[i]-0X30);//校验位，^为按位异或，对数据的八位同时按位异或，相同为0，不同为1
	 SBUF=DA[i]-0X30;
	 while(!TI);
	 TI=0;
	}

   SBUF=D1;  //发送显示标志位
   while(!TI);
   TI=0;

   SBUF=check;	   //发送校验字节
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
			
			while(temp!=_OK_)//主机等待从机应答信号
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
