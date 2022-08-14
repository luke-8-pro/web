
#include <reg52.h>                  //���ڳ���
#define  uchar unsigned char  		
#define  uint unsigned int
long uint Frequency_Out_9850;		
double Con_Word_1_9850 = 0x00;		
double Con_Word_2_9850 = 0x00;
long uint ConTrol_Word_9850 = 0x00;  
sbit fqud_9850  = P1^2; 	  //AD9850�˿ڶ���
sbit reset_9850  = P1^3;	
sbit w_clk_9850 = P1^4; 		 		
#define dataport_9850 P2		
delay_9850(long int n)
 {
     while(n!=0)
	 {
	  n--; 
	 }
 }
Send_Control_Word_9850(long uint ConTrol_Word) //���������������
{ 
  long uint ConTrol_Word_Temporary ;	
  uchar data_word ;			
  w_clk_9850 = 0x00;			
  fqud_9850 = 0x00;  			
  data_word = 0x00;	 		
  dataport_9850 = data_word;		
  w_clk_9850=1;				
  w_clk_9850=0;				
  ConTrol_Word_Temporary = ConTrol_Word;
  ConTrol_Word = ConTrol_Word >> 24;		
  data_word = ConTrol_Word % 256;	
  dataport_9850 = data_word;
  w_clk_9850=1;
  w_clk_9850=0;
  ConTrol_Word = ConTrol_Word_Temporary;
  ConTrol_Word = ConTrol_Word >> 16;
  data_word = ConTrol_Word % 256;
  dataport_9850 = data_word;
  w_clk_9850=1;
  w_clk_9850=0;
  ConTrol_Word = ConTrol_Word_Temporary;  
  ConTrol_Word = ConTrol_Word >> 8;
  data_word = ConTrol_Word % 256;	
  dataport_9850 = data_word;
  w_clk_9850=1;
  w_clk_9850=0;
  ConTrol_Word = ConTrol_Word_Temporary;
  data_word = ConTrol_Word % 256;	 
  dataport_9850 = data_word;
  w_clk_9850=1;
  w_clk_9850=0;	   			
  fqud_9850=1;	  			

}

Calculate_Control_Word_9850(long uint Frequency_Out_9850)  //����Ƶ�ʣ����ݾ���Ĳ�ͬ��������ֵ��һ����һ������Ϊ125MHZ��������	
 {   	
Con_Word_1_9850 = Frequency_Out_9850 * 34;
   Con_Word_2_9850 = Frequency_Out_9850 * 0.3597; //����100Mhz��������4294967295/��100*1000000������42����34����0.9496����0.3597
  Con_Word_2_9850 = Con_Word_2_9850 + 0.5;		
  Con_Word_1_9850 = Con_Word_1_9850 + Con_Word_2_9850;                           				
  ConTrol_Word_9850 = Con_Word_1_9850 / 1;		
 }
 

init_9850()
{	reset_9850  =  1;		
	delay_9850(10);
	reset_9850  =  0; 		
}
Write_9850()
{
//	init_9850();//��ʼ��	
	Calculate_Control_Word_9850(Frequency_Out_9850);//��ֵת��	
	delay_9850(10);
	Send_Control_Word_9850(ConTrol_Word_9850);//����9850
}
