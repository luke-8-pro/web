#define  uchar unsigned char
/*-----------------------------------------------
  ���ƣ�������Դ���Ƴ���
  ��д������
  ��λ����������ѧ
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

#define AddWr 0x90   //д���ݵ�ַ 
#define AddRd 0x91   //�����ݵ�ַ

extern bit ack;
unsigned char ReadADC(unsigned char Chl);
bit WriteDAC(unsigned char dat);
//void Get_dianliu1_and_display();
//void Get_dianliu2_and_display();

sbit inde_switch0=P0^4;					   //��λ��������
sbit kx=P0^0;							   //�ֶ��Զ��л��˿�
sbit inde_switch4=P0^5;					   //��λ��������
sbit dy=P0^7;							   //�жϵ�Դ�ź��Ƿ��ͨ�˿�
sbit bj=P0^3;							   //����ѡ��
sbit sp=P1^1;							   //Ƶ���趨
sbit error_IF=P1^0;						   //����

unsigned char num1=0,num2=0,i;
float Voltage1=0;        //���帡�����	 //ʵ�ʵ���
float Voltage2=1620;                        //���õ���
float Voltage0=0; 						     //��������
float dianliu1=0;        //���帡�����	 //ʵ�ʵ���
float dianliu2=0;							 //���ٵ���
float dianliu0=0;							 //��������
float ljt=2.08;								 //���ٵ���ϵ��
float disp_track=1.808;					 //ʵ�ʵ���ϵ��
float setfreq=20800;						 //����Ƶ��ֵ
float trackfreq=20800;						 //����Ƶ��ֵ
float switch_track=0;						 //����Ƶ��ֵ
float sl=100;								 //��������С
float ml=400;								 //����������
float ll=800;								 //�������δ�
float sf=1;								 //����Ƶ�ʵ���ֵС
float mf=2;								 //����Ƶ�ʵ���ֵ��
float lf=3;								 //����Ƶ�ʵ���ֵ��
float sum_dianliu1=8888;
float sum_dianliu2=8888;
float low_Fq=18000;								 //��ֵ��Ƶ
float high_Fq=26000;								 //��ֵ��Ƶ
float i_dianliu;
float iiii;
uchar display_o;	  //��ʾλ�ñ�־λ		
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
              ������
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
	init_9850();//��ʼ��	
	if(Program_Choose==1)
	{
		display_logo();		//��ʾͼ��	
		display_guding();			
		delay2(100);          //�ȴ���ʾlogo
	}
 
    inde_switch0=1;	 //����˿�
	  inde_switch1=0;
    inde_switch2=1;
    inde_switch3=1;
    inde_switch6=1;
	inde_switch7=1;
	while(1)					   //����ʼ
	{
//		if(!kx)				//�ж��ֶ��Զ����ֶ���ʼ�����
//		{
//			delay2(10);
//			if(!kx)
//			{
//				dispmanual();								//3.5  ��ʾ���ֶ�ģʽ��
//				delay2(2);
//				Frequency_Out_9850 = setfreq;		 //�����ֶ���ʼƵ��
//				Write_9850();
//				display_blank();					 //��ʾ���
//				delay2(2);
//				hi_bit_xiabiao=data_separation(Frequency_Out_9850);
//				display_freq(hi_bit_xiabiao);			   //�ֶ������ʼ��END
//				delay2(2);
//				dispbl100();
//				while(1)
//				{
//					hi_bit_xiabiao=data_separation(Frequency_Out_9850);
//					display_freq(hi_bit_xiabiao);
//				
//				  	num1=ReadADC(0);      //��ȡ��1·��ѹֵ����Χ��0-255
//	                Voltage1=(float)num1*5*1000/256;   //���ݲο���ԴVREF���ʱ���ѹ��float��ǿ��ת�����ţ����ڽ����ת���ɸ�����
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
//					if(sw_iden(2))	               //�������ڼӼ�Ƶ��
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
//					if(kx==1)				//�ж��Ƿ��ֶ�������������
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
	//�Զ�����ʼ
		//if(kx==1)			   //�ж��ֶ��Զ����Զ���ʼ�����
		//{
			//delay2(10);
			//if(kx==1) 
			//{  
				while(1)					   //***********************************************************************�Զ������ѭ��
				{
					Frequency_Out_9850 = setfreq;		//����ɨƵ��ʼƵ��
					Write_9850();
					display_blank1();   //��ʾ�հ�
					iiii=5;
					while(iiii)
					{
						hi_bit_xiabiao=data_separation(Frequency_Out_9850);
						display_freq(hi_bit_xiabiao);	
						delay2(2);									   
						num1=ReadADC(0);      //��ȡ��1·��ѹֵ����Χ��0-255
						Voltage1=(float)num1*5*1000/256;   //���ݲο���ԴVREF���ʱ���ѹ��float��ǿ��ת�����ţ����ڽ����ת���ɸ�����
						dianliu1=(float)disp_track*Voltage1;
						hi_bit_xiabiao2=data_separation(dianliu1);
						display_freq2(hi_bit_xiabiao2);
						delay2(2);	
						
						num2=ReadADC(2);
						Voltage2=(float)num2*5*1000/256;  //���õ���
						dianliu2=(float)ljt*Voltage2; 						    
						hi_bit_xiabiao3=data_separation(dianliu2);                           
						display_freq3(hi_bit_xiabiao3);
						delay2(2);
						iiii-=1;
					}
					
					while(!switch_track)					   //**************************************************************ɨƵѭ��
					{
						if(dy==0)				  //�жϳ����Ƿ�򿪣��򿪺�ִ��
						{
							delay2(5);
							if(dy==0) 
							{ 
								dispscanfre();				  //��ʾɨƵ��
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
								
								num1=ReadADC(0);      //��ȡ��1·��ѹֵ����Χ��0-255
								Voltage1=(float)num1*5*1000/256;   //���ݲο���ԴVREF���ʱ���ѹ��float��ǿ��ת�����ţ����ڽ����ת���ɸ�����
								dianliu1=(float)disp_track*Voltage1;
								hi_bit_xiabiao2=data_separation(dianliu1);
								display_freq2(hi_bit_xiabiao2);
								delay2(2);	
								
								num2=ReadADC(2);
								Voltage2=(float)num2*5*1000/256;  //���õ���
								dianliu2=(float)ljt*Voltage2; 						    
								hi_bit_xiabiao3=data_separation(dianliu2);                           
								display_freq3(hi_bit_xiabiao3);
								delay2(2);
								
//								if(kx==0)			  //�ж��Ƿ��Զ�������������
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
						if(dy==1)				   //�жϵ�Դ�Ƿ�رգ��رպ�����
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
								
								num1=ReadADC(0);      //��ȡ��1·��ѹֵ����Χ��0-255
								Voltage1=(float)num1*5*1000/256;   //���ݲο���ԴVREF���ʱ���ѹ��float��ǿ��ת�����ţ����ڽ����ת���ɸ�����
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
//						if(kx==0)			  //�ж��Ƿ��Զ�������������
//						{
//							delay2(10);
//							if(kx==0) break;
//						}
							   
					}		 //***********************************************ɨƵѭ������
			   //***************************************************************************************************����ѭ����ʼ
                  	while(switch_track)						 //�������ٿ�ʼ
					{
						display_blank1();
						if(dy==0)			 //�жϵ�Դ�Ƿ�򿪣��򿪺�ִ��
         	  			{
							dispworking();
							while(1)
							{
								hi_bit_xiabiao=data_separation(Frequency_Out_9850);		  //��ʾƵ��
								delay2(2);
								display_freq(hi_bit_xiabiao);
								i_dianliu=5;
								while(i_dianliu)
								{
									num1=ReadADC(0);      //��ȡ��1·��ѹֵ����Χ��0-255
									Voltage1=(float)num1*5*1000/256;   //���ݲο���ԴVREF���ʱ���ѹ��float��ǿ��ת�����ţ����ڽ����ת���ɸ�����
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
//									Frequency_Out_9850+=sf;		  //��Ƶ�ʽ�����
//									Write_9850();
//									delay2(5);
//								}								
								
								if((sum_dianliu1>sum_dianliu2+sl)&&(sum_dianliu1<sum_dianliu2+ml))
								{
									Frequency_Out_9850+=sf;		  //��Ƶ�ʽ�����
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
//									num1=ReadADC(0);      //��ȡ��1·��ѹֵ����Χ��0-255
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
										Frequency_Out_9850 = trackfreq;		//����ɨƵ��ʼƵ��
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
										Frequency_Out_9850 = trackfreq;		//����ɨƵ��ʼƵ��
                           				Write_9850();
										switch_track=0;	
									//} 
//									else continue;
								}
								//if(kx==0) break;	 //�ж��Ƿ��Զ�������������
								if(dy==1)						     //�жϵ�Դ�Ƿ�رգ��رպ�����
								{
									dispoutwork();
									switch_track=0;
									while(dy);
									break;
								}
							}		
						}
						if(dy==1)						     //�жϵ�Դ�Ƿ�رգ��رպ�����
						{
							dispoutwork();
							switch_track=0;
							while(dy);
							break;
						}
						//if(kx==0) break;						     //�ж��Ƿ��Զ�������������
					} //************************************************************************************����ѭ������
					//if(kx==0) break;						     //�ж��Ƿ��Զ�������������
				}//**********************************************************************�Զ���ѭ������
			//}				  //�Զ�if
		//}				  //�Զ�if����
	}
}//************************************************ȫ��ѭ������



 
/*------------------------------------------------
             ��ADתֵ����
������� Chl ��ʾ��Ҫת����ͨ������Χ��0-3
����ֵ��Χ0-255
------------------------------------------------*/
unsigned char ReadADC(unsigned char Chl)
{
	unsigned char Val;
	Start_I2c();               //��������
	SendByte(AddWr);             //����������ַ
	if(ack==0)return(0);
	SendByte(0x40|Chl);            //���������ӵ�ַ
	if(ack==0)return(0);
	Start_I2c();
	SendByte(AddWr+1);
	if(ack==0)return(0);
	Val=RcvByte();
	NoAck_I2c();                 //���ͷ�Ӧλ
	Stop_I2c();                  //��������
	return(Val);
}
