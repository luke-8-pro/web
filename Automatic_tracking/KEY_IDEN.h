
/*包含相关头文件*/
#define u_char unsigned char

//是否需要跟踪判断

sbit inde_switch1=P0^6;	   //相位跟踪中用
sbit inde_switch2=P0^1;	   //手动跟踪加频率端口
sbit inde_switch3=P0^2;	   //手动跟踪减频率端口
sbit inde_switch6=P0^3;	   //步进选择端口
sbit inde_switch7=P1^1;	   //频率设定端口
/*函数声明*/
delay_inde_switch();
bit sw_iden(u_char n);

delay_inde_switch()
{
	u_char i,j;
	i=1; j=30;
	while(i!=0)
		{
			while(j!=0)
			{
				j--;
			}
			i--;
		}
}

bit sw_iden(u_char n)
{
	bit switch_down_flag;				
	switch_down_flag=0;	
	switch(n)
     {
/*	case 0 :					//自动跟踪加频率判定
	   {
		if(inde_switch6==1)			
		{
			delay_inde_switch();		
			if(inde_switch6==1)
			{	
                  
				switch_down_flag=1;
			}
		}
		return switch_down_flag;
	   }
	   break;					
	case 1:	      				//自动跟踪减频率判定	
	   {
		if(inde_switch5==0)			
		{
			delay_inde_switch();		
			if(inde_switch5==0)
			{
				switch_down_flag=1;
			}
		}
		return switch_down_flag;
	   }
		break;
				   */





	case 2 :					   //手动跟踪加频率判定
	   {
		if(inde_switch2==0)			
		{
			delay_inde_switch();		
			if(inde_switch2==0)
			{	
			    while(inde_switch2==0);
				switch_down_flag=1;
			}
		}
		return switch_down_flag;
	   }
	   break;					
	case 3:	      					//手动跟踪减频率判定
	   {
		if(inde_switch3==0)			
		{
			delay_inde_switch();		
			if(inde_switch3==0)
			{
			    while(inde_switch3==0);	
				switch_down_flag=1;
			}
		}
		return switch_down_flag;
	   }
		break;
    }										

}
