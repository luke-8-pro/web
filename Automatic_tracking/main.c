#define  uchar unsigned char
/*-----------------------------------------------
  名称：超声电源控制程序
  编写：韩坤
  单位：大连理工大学
------------------------------------------------*/  
#include <reg52.h>                
#include "i2c.h"
#include "delay.h"
#include "send.h"
#include <stdio.h>
#include <string.h>

#include <ad9850.h>			
#include <12864.h>		
#include <data_separation.h>	
#include <key_iden.h>		
#include <display_char.h>

#define AddWr 0x90   //写数据地址 
#define AddRd 0x91   //读数据地址

extern bit ack;
unsigned char ReadADC(unsigned char Chl);
bit WriteDAC(unsigned char dat);
//void Get_dianliu1_and_display();
//void Get_dianliu2_and_display();

sbit inde_switch0=P0^4;					   //相位跟踪中用
sbit kx=P0^0;							   //手动自动切换端口
sbit inde_switch4=P0^5;					   //相位跟踪中用
sbit dy=P0^7;							   //判断电源信号是否接通端口
sbit bj=P0^3;							   //步进选择
sbit sp=P1^1;							   //频率设定
sbit error_IF=P1^0;						   //报警

unsigned char num1=0,num2=0,i;
float Voltage1=0;        //定义浮点变量	 //实际电流
float Voltage2=1620;                        //设置电流
float Voltage0=0; 						     //跳出电流
float dianliu1=0;        //定义浮点变量	 //实际电流
float dianliu2=0;							 //跟踪电流
float dianliu0=0;							 //跳出电流
float ljt=2.08;								 //跟踪电流系数
float disp_track=1.808;					 //实际电流系数
float setfreq=20800;						 //设置频率值
float trackfreq=20800;						 //跳出频率值
float switch_track=0;						 //跳出频率值
float sl=100;								 //跟踪区段小
float ml=400;								 //跟踪区段中
float ll=800;								 //跟踪区段大
float sf=1;								 //跟踪频率调整值小
float mf=2;								 //跟踪频率调整值中
float lf=3;								 //跟踪频率调整值大
float sum_dianliu1=8888;
float sum_dianliu2=8888;
float low_Fq=18000;								 //阈值低频
float high_Fq=26000;								 //阈值高频
float i_dianliu;
float iiii;
uchar display_o;	  //显示位置标志位		
uchar hi_bit_xiabiao;  //display_o=1
uchar hi_bit_xiabiao2; //display_o=2
uchar hi_bit_xiabiao3; //display_o=3
//uchar sw_flag=0;
uchar flag;
uchar Program_Choose;
void delay2(unsigned int a)
{			
	unsigned int i;
	while( a-- != 0)
	{
		for(i = 0; i < 600; i++);
	}
}


/*------------------------------------------------
              主程序
------------------------------------------------*/
 void main()
{
	Voltage0=0.85*Voltage2;
	dianliu0=(float)ljt*Voltage0;
	Frequency_Out_9850 = setfreq;		 
    Write_9850();
	delay2(50);
    kx=1;
    dy=0;
	flag=0;
	Program_Choose=1;
	error_IF=1;
	init_9850();//初始化	
	if(Program_Choose==1)
	{
		display_logo();		//显示图标	
		display_guding();			
		delay2(100);          //等待显示logo
	}
 
    inde_switch0=1;	 //清零端口
	  inde_switch1=0;
    inde_switch2=1;
    inde_switch3=1;
    inde_switch6=1;
	inde_switch7=1;
	while(1)					   //程序开始
	{
//		if(!kx)				//判断手动自动，手动开始程序段
//		{
//			delay2(10);
//			if(!kx)
//			{
//				dispmanual();								//3.5  显示“手动模式”
//				delay2(2);
//				Frequency_Out_9850 = setfreq;		 //设置手动初始频率
//				Write_9850();
//				display_blank();					 //显示清除
//				delay2(2);
//				hi_bit_xiabiao=data_separation(Frequency_Out_9850);
//				display_freq(hi_bit_xiabiao);			   //手动程序初始化END
//				delay2(2);
//				dispbl100();
//				while(1)
//				{
//					hi_bit_xiabiao=data_separation(Frequency_Out_9850);
//					display_freq(hi_bit_xiabiao);
//				
//				  	num1=ReadADC(0);      //读取第1路电压值，范围是0-255
//	                Voltage1=(float)num1*5*1000/256;   //根据参考电源VREF算出时间电压，float是强制转换符号，用于将结果转换成浮点型
//	                dianliu1=(float)disp_track*Voltage1; 
//					sum_dianliu1=dianliu1;
//					delay2(2);	
//					  	
//	                num2=ReadADC(2);
//	                Voltage2=(float)num2*5*1000/256;
//					dianliu2=(float)ljt*Voltage2; 						    
//					sum_dianliu2=dianliu2;
//					delay2(2);
//					
//					sum_dianliu1=sum_dianliu1;
//					sum_dianliu2=sum_dianliu2;
//					hi_bit_xiabiao2=data_separation(sum_dianliu1);
//					display_freq2(hi_bit_xiabiao2);						                  
//					hi_bit_xiabiao3=data_separation(sum_dianliu2);                            
//					display_freq3(hi_bit_xiabiao3);	           
//					if(!bj)
//					{
//						delay2(10);
//						if(!bj)
//						{
//							switch(flag)
//							{
//								case 0: flag=1;dispbl10();break;
//								case 1: flag=2;dispbl1();break;
//								case 2: flag=0;dispbl100();break;
//							}
//							while(!bj);	
//						}	
//					}
//					if(sw_iden(2))	               //按键调节加减频率
//					{
//						if(flag==0) {Frequency_Out_9850+=100;delay2(2);   dispstep100_inc();dispbl100();}
//						if(flag==1) {Frequency_Out_9850+=10;delay2(2);	dispstep10_inc();dispbl10();}
//						if(flag==2) {Frequency_Out_9850+=1;delay2(2);	dispstep1_inc();dispbl1();}
//						Write_9850();
//						delay2(2);
//					}				
//					if(sw_iden(3))	
//					{
//						if(flag==0) {Frequency_Out_9850-=100;delay2(2);  	dispstep100_dec();dispbl100();}
//						if(flag==1) {Frequency_Out_9850-=10;delay2(2);	dispstep10_dec();dispbl10();}
//						if(flag==2) {Frequency_Out_9850-=1;delay2(2);	dispstep1_dec();dispbl1();}
//						Write_9850();
//						delay2(2);
//					}
//					hi_bit_xiabiao=data_separation(Frequency_Out_9850);	
//					display_freq(hi_bit_xiabiao);
//					if(inde_switch1==1) dispjx1();	
//					if(inde_switch1==0) dispjx0();	
//					if(sp==0)
//					{
//						delay2(10);
//						if(sp==0) {setfreq=Frequency_Out_9850;dispsetfreq();}
//					}			 
//					if(kx==1)				//判断是否手动，否跳出程序
//					{														//9
//						delay2(10);
//						if(kx==1)
//						kx=1;
//						switch_track=0; 
//						break;
//					}		
//				}		
//			}	
//		}   																 //1
	  
	//***************************************************************************************************************************************************************************************************
	//自动程序开始
		//if(kx==1)			   //判断手动自动，自动开始程序段
		//{
			//delay2(10);
			//if(kx==1) 
			//{  
				while(1)					   //***********************************************************************自动程序大循环
				{
					Frequency_Out_9850 = setfreq;		//设置扫频初始频率
					Write_9850();
					display_blank1();   //显示空白
					iiii=5;
					while(iiii)
					{
						hi_bit_xiabiao=data_separation(Frequency_Out_9850);
						display_freq(hi_bit_xiabiao);	
						delay2(2);									   
						num1=ReadADC(0);      //读取第1路电压值，范围是0-255
						Voltage1=(float)num1*5*1000/256;   //根据参考电源VREF算出时间电压，float是强制转换符号，用于将结果转换成浮点型
						dianliu1=(float)disp_track*Voltage1;
						hi_bit_xiabiao2=data_separation(dianliu1);
						display_freq2(hi_bit_xiabiao2);
						delay2(2);	
						
						num2=ReadADC(2);
						Voltage2=(float)num2*5*1000/256;  //设置电流
						dianliu2=(float)ljt*Voltage2; 						    
						hi_bit_xiabiao3=data_separation(dianliu2);                           
						display_freq3(hi_bit_xiabiao3);
						delay2(2);
						iiii-=1;
					}
					
					while(!switch_track)					   //**************************************************************扫频循环
					{
						if(dy==0)				  //判断超声是否打开，打开后执行
						{
							delay2(5);
							if(dy==0) 
							{ 
								dispscanfre();				  //显示扫频中
								delay2(2);
								Voltage0=0.85*Voltage2;
								dianliu0=(float)ljt*Voltage0;
								if(dianliu1<1000) Frequency_Out_9850-=30;
								else Frequency_Out_9850-=3;
								if(Frequency_Out_9850<low_Fq)
									Frequency_Out_9850=high_Fq;
								Write_9850();
								hi_bit_xiabiao=data_separation(Frequency_Out_9850);
								display_freq(hi_bit_xiabiao);	
								delay2(2);
								
								num1=ReadADC(0);      //读取第1路电压值，范围是0-255
								Voltage1=(float)num1*5*1000/256;   //根据参考电源VREF算出时间电压，float是强制转换符号，用于将结果转换成浮点型
								dianliu1=(float)disp_track*Voltage1;
								hi_bit_xiabiao2=data_separation(dianliu1);
								display_freq2(hi_bit_xiabiao2);
								delay2(2);	
								
								num2=ReadADC(2);
								Voltage2=(float)num2*5*1000/256;  //设置电流
								dianliu2=(float)ljt*Voltage2; 						    
								hi_bit_xiabiao3=data_separation(dianliu2);                           
								display_freq3(hi_bit_xiabiao3);
								delay2(2);
								
//								if(kx==0)			  //判断是否自动，否跳出程序
//								{
//									delay2(10);
//									if(kx==0) break;
//								}
				  
								if(dianliu1>dianliu0)
								{	
									trackfreq=Frequency_Out_9850;
									switch_track=1;
									break;
								}
								else continue;   	 
									  
							}
						}
						if(dy==1)				   //判断电源是否关闭，关闭后跳出
						{
							delay2(10);
							if(dy==1) 
							{ 
								hi_bit_xiabiao=data_separation(Frequency_Out_9850);
								display_freq(hi_bit_xiabiao);
								delay2(2);
								dispoutwork();
								switch_track=0;
								delay2(2);
								
								num1=ReadADC(0);      //读取第1路电压值，范围是0-255
								Voltage1=(float)num1*5*1000/256;   //根据参考电源VREF算出时间电压，float是强制转换符号，用于将结果转换成浮点型
								dianliu1=(float)disp_track*Voltage1; 
								delay2(2);	
									  	
								num2=ReadADC(2);
								Voltage2=(float)num2*5*1000/256;
								dianliu2=(float)ljt*Voltage2; 						    
								delay2(2);
								
								hi_bit_xiabiao2=data_separation(dianliu1);
                                display_freq2(hi_bit_xiabiao2);						                  
								hi_bit_xiabiao3=data_separation(dianliu2);                            
                                display_freq3(hi_bit_xiabiao3);
							}
						}
//						if(kx==0)			  //判断是否自动，否跳出程序
//						{
//							delay2(10);
//							if(kx==0) break;
//						}
							   
					}		 //***********************************************扫频循环结束
			   //***************************************************************************************************跟踪循环开始
                  	while(switch_track)						 //电流跟踪开始
					{
						display_blank1();
						if(dy==0)			 //判断电源是否打开，打开后执行
         	  			{
							dispworking();
							while(1)
							{
								hi_bit_xiabiao=data_separation(Frequency_Out_9850);		  //显示频率
								delay2(2);
								display_freq(hi_bit_xiabiao);
								i_dianliu=5;
								while(i_dianliu)
								{
									num1=ReadADC(0);      //读取第1路电压值，范围是0-255
									Voltage1=(float)num1*5*1000/256;   //根据参考电源VREF算出时间电压，float是强制转换符号，用于将结果转换成浮点型
									dianliu1=(float)disp_track*Voltage1; 
									sum_dianliu1+=dianliu1;
									delay2(1);	
									
									num2=ReadADC(2);
									Voltage2=(float)num2*5*1000/256;
									dianliu2=(float)ljt*Voltage2; 						    
									sum_dianliu2+=dianliu2;
									delay2(1);
									i_dianliu-=1;
								}
								i_dianliu=5;
								sum_dianliu1=sum_dianliu1/6;
								sum_dianliu2=sum_dianliu2/6;
								hi_bit_xiabiao2=data_separation(sum_dianliu1);
								display_freq2(hi_bit_xiabiao2);						                  
								hi_bit_xiabiao3=data_separation(sum_dianliu2);                            
								display_freq3(hi_bit_xiabiao3);
//								if((sum_dianliu1>sum_dianliu2+20)&&(sum_dianliu1<sum_dianliu2+sl))
//								{
//									Frequency_Out_9850+=sf;		  //加频率进给量
//									Write_9850();
//									delay2(5);
//								}								
								
								if((sum_dianliu1>sum_dianliu2+sl)&&(sum_dianliu1<sum_dianliu2+ml))
								{
									Frequency_Out_9850+=sf;		  //加频率进给量
									Write_9850();
									delay2(5);
								}
								
								if((sum_dianliu1>sum_dianliu2+ml)&&(sum_dianliu1<sum_dianliu2+ll))
								{
									Frequency_Out_9850+=mf;
									Write_9850();
									delay2(5);
								}
								
								if(sum_dianliu1>sum_dianliu2+ll)
								{
									Frequency_Out_9850+=lf;
									Write_9850();
									delay2(5);
								}
//								if((sum_dianliu1<sum_dianliu2-20)&&(sum_dianliu1>sum_dianliu2-sl))
//								{
//									Frequency_Out_9850-=sf;
//									Write_9850();
//									delay2(5);
//								}
								
								if((sum_dianliu1<sum_dianliu2-sl)&&(sum_dianliu1>sum_dianliu2-ml))
								{
									Frequency_Out_9850-=sf;
									Write_9850();
									delay2(5);
								}
												  
								if((sum_dianliu1<sum_dianliu2-ml)&&(sum_dianliu1>sum_dianliu2-ll))
								{
									Frequency_Out_9850-=mf;
									Write_9850();
									delay2(5);
								}
								
								if(sum_dianliu1<sum_dianliu2-ll)
								{
									Frequency_Out_9850-=lf;
									Write_9850();
									delay2(5);
								}
								
								delay2(5);
								if(inde_switch1==1) dispjx1();
								if(inde_switch1==0) dispjx0();
								delay2(5);
								if(sum_dianliu1<50)
								{
//									delay2(10);
//									num1=ReadADC(0);      //读取第1路电压值，范围是0-255
//									Voltage1=(float)num1*5*1000/256;
									//if(kx==1)
									//{
										disperror_I();
										error_IF=0;
										while(!error_IF);		
									//}
//									else continue; 
								}
								if(Frequency_Out_9850<low_Fq)
								{
//									delay2(10);
									//if(kx==1)
									//{
										Frequency_Out_9850 = trackfreq;		//设置扫频初始频率
                           				Write_9850();
 										switch_track=0;
									//}
//									else continue; 
								}
								if(Frequency_Out_9850>high_Fq)
								{
//									delay2(10);
									//if(kx==1)
									//{
										Frequency_Out_9850 = trackfreq;		//设置扫频初始频率
                           				Write_9850();
										switch_track=0;	
									//} 
//									else continue;
								}
								//if(kx==0) break;	 //判断是否自动，否跳出程序
								if(dy==1)						     //判断电源是否关闭，关闭后跳出
								{
									dispoutwork();
									switch_track=0;
									while(dy);
									break;
								}
							}		
						}
						if(dy==1)						     //判断电源是否关闭，关闭后跳出
						{
							dispoutwork();
							switch_track=0;
							while(dy);
							break;
						}
						//if(kx==0) break;						     //判断是否自动，否跳出程序
					} //************************************************************************************跟踪循环结束
					//if(kx==0) break;						     //判断是否自动，否跳出程序
				}//**********************************************************************自动大循环结束
			//}				  //自动if
		//}				  //自动if结束
	}
}//************************************************全部循环结束



 
/*------------------------------------------------
             读AD转值程序
输入参数 Chl 表示需要转换的通道，范围从0-3
返回值范围0-255
------------------------------------------------*/
unsigned char ReadADC(unsigned char Chl)
{
	unsigned char Val;
	Start_I2c();               //启动总线
	SendByte(AddWr);             //发送器件地址
	if(ack==0)return(0);
	SendByte(0x40|Chl);            //发送器件子地址
	if(ack==0)return(0);
	Start_I2c();
	SendByte(AddWr+1);
	if(ack==0)return(0);
	Val=RcvByte();
	NoAck_I2c();                 //发送非应位
	Stop_I2c();                  //结束总线
	return(Val);
}
