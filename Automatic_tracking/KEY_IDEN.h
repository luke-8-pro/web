
/*�������ͷ�ļ�*/
#define u_char unsigned char

//�Ƿ���Ҫ�����ж�

sbit inde_switch1=P0^6;	   //��λ��������
sbit inde_switch2=P0^1;	   //�ֶ����ټ�Ƶ�ʶ˿�
sbit inde_switch3=P0^2;	   //�ֶ����ټ�Ƶ�ʶ˿�
sbit inde_switch6=P0^3;	   //����ѡ��˿�
sbit inde_switch7=P1^1;	   //Ƶ���趨�˿�
/*��������*/
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
/*	case 0 :					//�Զ����ټ�Ƶ���ж�
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
	case 1:	      				//�Զ����ټ�Ƶ���ж�	
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





	case 2 :					   //�ֶ����ټ�Ƶ���ж�
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
	case 3:	      					//�ֶ����ټ�Ƶ���ж�
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
