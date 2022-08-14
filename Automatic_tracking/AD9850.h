
#include <reg52.h>                  //并口程序
#define  uchar unsigned char  		
#define  uint unsigned int
long uint Frequency_Out_9850;		
double Con_Word_1_9850 = 0x00;		
double Con_Word_2_9850 = 0x00;
long uint ConTrol_Word_9850 = 0x00;  
sbit fqud_9850  = P1^2; 	  //AD9850端口定义
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
Send_Control_Word_9850(long uint ConTrol_Word) //并口数据送入程序
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

Calculate_Control_Word_9850(long uint Frequency_Out_9850)  //计算频率，根据晶振的不同，计算数值不一样，一下数据为125MHZ晶振数据	
 {   	
Con_Word_1_9850 = Frequency_Out_9850 * 34;
   Con_Word_2_9850 = Frequency_Out_9850 * 0.3597; //对于100Mhz晶振，先用4294967295/（100*1000000），用42代替34，用0.9496代替0.3597
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
//	init_9850();//初始化	
	Calculate_Control_Word_9850(Frequency_Out_9850);//数值转换	
	delay_9850(10);
	Send_Control_Word_9850(ConTrol_Word_9850);//送入9850
}
