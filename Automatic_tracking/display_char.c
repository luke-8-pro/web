#include<reg52.h>
#include<string.h>
#include <intrins.h>
#include "12864.h"
#include "display_char.h"

extern unsigned char data_sepa[8];

uchar code logo[]={"大连理工大学"};	
uchar code welcome[]={"现代制造技术"};
uchar code welcome2[]={"研究所      "};	
uchar code outfre[]={"频率:"};	
uchar code shuzi[]={"0123456789 "};
uchar code out2[]={"设置电流:"};
uchar code out3[]={"实际电流:"};
uchar code out4[]={"扫频中请稍后"};
uchar code out5[]={"工作中        "};
uchar code out6[]={"未工作        "};
uchar code out7[]={"频率设定成功"};
uchar code out8[]={"      "};
uchar code out9[]={"手动模式    "};		
//uchar code freq_unit[]={"Unit:Hz"};	
uchar code step1_inc[]={"步进: + 1 Hz"};	
uchar code step1_dec[]={"步进: - 1 Hz"};
uchar code step10_inc[]={"步进: +10 Hz"};	
uchar code step10_dec[]={"步进: -10 Hz"};	
uchar code step100_inc[]={"步进: +100Hz"};	
uchar code step100_dec[]={"步进: -100Hz"};
uchar code bl1[]={"01"};
uchar code bl10[]={"02"};
uchar code bl100[]={"03"};
uchar code jx0[]={" 0"};
uchar code jx1[]={" 1"};
uchar code step_error_I[]={"电流低"};
uchar code step_error_F[]={"超频率"};
uchar code step_empty[]={"Step:empty      "};
uchar code wait[]={"Scan Freq       "};	


uchar code space8[]={"        "};	
uchar code space9[]={"         "};	
uchar code space16[]={"                "};
uchar code step[]={"Step:"};
uchar code space1[]={" "};		
uchar code Hz[]={"Hz"};	
uchar code poit[]={"."};
uchar code poit1[]={"0."};
uchar code zero[]={"0"};
uchar code zero6[]={"000000"};
uchar code K[]={"K"};
uchar code M[]={"M"};
uchar code A[]={"A "};
uchar code mA[]={"mA "};
uchar code maohao[]={":"};		


void display_logo(void)
{
	int  i=0,j=0,k=0;
	lcd_init();				
	delay(10);
	
	lcd_pos(0,1);	

	while(i!=12)		
	{
		lcd_wdat(logo[i]);
		i++;
		delay(5);	
	}
	delay(10);	
	lcd_pos(1,1);	
	while(j!=12)
	{
		lcd_wdat(welcome[j]);
		j++;
		delay(5);
	}
	delay(10);
	
	lcd_pos(2,2);	
	while(k!=6)
	{
		lcd_wdat(welcome2[k]);
		k++;
		delay(5);
	}
	ClrScreen();
	delay(10);	
}

void display_guding(void)
{
  {
	int  i=0;
	lcd_pos(2,0);	
	while(i!=6)
	{
		lcd_wdat(outfre[i]);
		i++;
	 }
  }
  {
	int  j=0;
	lcd_pos(0,0);	
	while(j!=10)
	{
		lcd_wdat(out2[j]);
		j++;
	 }
  }{
	int  k=0;

	lcd_pos(1,0);	
	while(k!=10)
	{
		lcd_wdat(out3[k]);
		k++;
	 }
  }
}


void display_blank(void)   //先设置为第四行清零
{
	int  i=0;
	
	lcd_pos(3,0);	
	i=0;
	while(i!=16)
	{
		lcd_wdat(space16[i]);
		i++;
	}
}

void display_blank1(void)	 //先设置为第四行清零
{
	int  i=0;
	
	lcd_pos(3,0);	
	i=0;
	while(i!=16)
	{
		lcd_wdat(space16[i]);
		i++;
	}
}

void ClrScreen()
{ 
   lcd_wcmd(0x01);
   delay(4);
}

void dispstep1_inc(void)
{

	uchar i;
	lcd_pos(3,0);	
	i=0;
	while(i!=12)
	{
		lcd_wdat(step1_inc[i]);
		i++;
		delay(1);
		
	}
}

void dispstep1_dec(void)
{	
	uchar i;
	lcd_pos(3,0);	
	i=0;
	while(i!=12)
	{
		lcd_wdat(step1_dec[i]);
		i++;
		delay(1);
		
	}
}
void dispstep10_inc(void)
{

	uchar i;
	lcd_pos(3,0);	
	i=0;
	while(i!=12)
	{
		lcd_wdat(step10_inc[i]);
		i++;
		delay(1);
		
	}
}

void dispstep10_dec(void)
{	
	uchar i;
	lcd_pos(3,0);	
	i=0;
	while(i!=12)
	{
		lcd_wdat(step10_dec[i]);
		i++;
		delay(1);
		
	}
}
void dispstep100_inc(void)
{

	uchar i;
	lcd_pos(3,0);	
	i=0;
	while(i!=12)
	{
		lcd_wdat(step100_inc[i]);
		i++;
		delay(1);
		
	}
}

void dispstep100_dec(void)
{	
	uchar i;
	lcd_pos(3,0);	
	i=0;
	while(i!=12)
	{
		lcd_wdat(step100_dec[i]);
		i++;
		delay(1);
		
	}
}
void dispbl1(void)
{
	uchar i;
	lcd_pos(3,6);
	i=0;
	while(i!=2)
	{
		lcd_wdat(bl1[i]);
		i++;
		delay(1);
	}
}
void dispbl10(void)
{
	uchar i;
	lcd_pos(3,6);
	i=0;
	while(i!=2)
	{
		lcd_wdat(bl10[i]);
		i++;
		delay(1);
	}
}
void dispbl100(void)
{
	uchar i;
	lcd_pos(3,6);
	i=0;
	while(i!=2)
	{
		lcd_wdat(bl100[i]);
		i++;
		delay(1);
	}
}
void dispjx0(void)
{
	uchar i;
	lcd_pos(3,7);
	i=0;
	while(i!=2)
	{
		lcd_wdat(jx0[i]);
		i++;
		delay(1);
	}
}
void dispjx1(void)
{
	uchar i;
	lcd_pos(3,7);
	i=0;
	while(i!=2)
	{
		lcd_wdat(jx1[i]);
		i++;
		delay(1);
	}
} 
void disperror_I(void)
{
	uchar i;
	lcd_pos(3,0);
	i=0;
	while(i!=6)
	{
		lcd_wdat(step_error_I[i]);
		i++;
		delay(1);
	}
}
void disperror_F(void)
{
	uchar i;
	lcd_pos(3,4);
	i=0;
	while(i!=6)
	{
		lcd_wdat(step_error_F[i]);
		i++;
		delay(1);
	}
} 
void dispworking(void)
{	
	uchar i;
	lcd_pos(3,0);	
	i=0;
	while(i!=12)
	{
		lcd_wdat(out5[i]);
		i++;
	}
}

void dispscanfre(void)
{	
	uchar i;
	lcd_pos(3,0);	
	i=0;
	while(i!=12)
	{
		lcd_wdat(out4[i]);
		i++;
	}
}
void dispsetfreq(void)
{	
	uchar i;
	lcd_pos(3,0);	
	i=0;
	while(i!=12)
	{
		lcd_wdat(out7[i]);
		i++;		
	}
}
void dispoutwork(void)
{	
	uchar i;
	lcd_pos(3,0);	
	i=0;
	while(i!=12)
	{
		lcd_wdat(out6[i]);
		i++;
	}
}
 void dispmanual(void)
{	
	uchar i;
	lcd_pos(3,0);	
	i=0;
	while(i!=12)
	{
		lcd_wdat(out9[i]);
		i++;
	}
}


void display_freq2(uchar hi_bit_xiabiao2) //实际电流
{	 	
	display_guding();	
	if(hi_bit_xiabiao2<=2)	 //2位
	{
		lcd_pos(1,5);
		lcd_wdat(space1[0]);
		lcd_wdat(space1[0]);	
		while(hi_bit_xiabiao2!=0)
		{

		    lcd_wdat(shuzi[data_sepa[hi_bit_xiabiao2-1]]);	
			hi_bit_xiabiao2--;	
		}
		lcd_pos(1,7);
		lcd_wdat(mA[0]);lcd_wdat(mA[1]);	
	}
	if(hi_bit_xiabiao2==3)	    //3位
	{
		lcd_pos(1,5);
		lcd_wdat(space1[0]);
		while(hi_bit_xiabiao2!=0)
		{
			lcd_wdat(shuzi[data_sepa[hi_bit_xiabiao2-1]]);	
			hi_bit_xiabiao2--;
		}
		lcd_pos(1,7);
		lcd_wdat(mA[0]);lcd_wdat(mA[1]);
	}
	if(hi_bit_xiabiao2==4)	    //4位
	{
		lcd_pos(1,5);
		while(hi_bit_xiabiao2!=0)
		{
			lcd_wdat(shuzi[data_sepa[hi_bit_xiabiao2-1]]);	
			hi_bit_xiabiao2--;
		}
		lcd_pos(1,7);
		lcd_wdat(mA[0]);lcd_wdat(mA[1]);
	}
}

void display_freq3(uchar hi_bit_xiabiao3)//设置电流
{		
	display_guding();	
	if(hi_bit_xiabiao3<=2)	 //2位
	{
		lcd_pos(0,5);
		lcd_wdat(space1[0]);
		lcd_wdat(space1[0]);	
		while(hi_bit_xiabiao3!=0)
		{

		    lcd_wdat(shuzi[data_sepa[hi_bit_xiabiao3-1]]);	
			hi_bit_xiabiao3--;	
		}
		lcd_pos(0,7);
		lcd_wdat(mA[0]);lcd_wdat(mA[1]);	
	}
	if(hi_bit_xiabiao3==3)	    //3位
	{
		lcd_pos(0,5);
		lcd_wdat(space1[0]);
		while(hi_bit_xiabiao3!=0)
		{
			lcd_wdat(shuzi[data_sepa[hi_bit_xiabiao3-1]]);	
			hi_bit_xiabiao3--;
		}
		lcd_pos(0,7);
		lcd_wdat(mA[0]);lcd_wdat(mA[1]);
	}
	if(hi_bit_xiabiao3==4)	    //4位
	{
		lcd_pos(0,5);
		while(hi_bit_xiabiao3!=0)
		{
			lcd_wdat(shuzi[data_sepa[hi_bit_xiabiao3-1]]);	
			hi_bit_xiabiao3--;
		}
		lcd_pos(0,7);
		lcd_wdat(mA[0]);lcd_wdat(mA[1]);
	}
}
void display_freq(uchar hi_bit_xiabiao)//频率
{		
	display_guding();	
	if(hi_bit_xiabiao<=2)	 //2位
	{
		lcd_pos(2,5);
		lcd_wdat(space1[0]);
		lcd_wdat(space1[0]);	
		while(hi_bit_xiabiao!=0)
		{

		    lcd_wdat(shuzi[data_sepa[hi_bit_xiabiao-1]]);	
			hi_bit_xiabiao--;	
		}
		lcd_pos(2,7);
		lcd_wdat(Hz[0]);lcd_wdat(Hz[1]);	
	}
	if(hi_bit_xiabiao==3)	    //3位
	{
		lcd_pos(2,5);
		lcd_wdat(space1[0]);
		while(hi_bit_xiabiao!=0)
		{
			lcd_wdat(shuzi[data_sepa[hi_bit_xiabiao-1]]);	
			hi_bit_xiabiao--;
		}
		lcd_pos(2,7);
		lcd_wdat(Hz[0]);lcd_wdat(Hz[1]);
	}
	if(hi_bit_xiabiao==4)	    //4位
	{
		lcd_pos(2,5);
		while(hi_bit_xiabiao!=0)
		{
			lcd_wdat(shuzi[data_sepa[hi_bit_xiabiao-1]]);	
			hi_bit_xiabiao--;
		}
		lcd_pos(2,7);
		lcd_wdat(Hz[0]);lcd_wdat(Hz[1]);
	}
	if(hi_bit_xiabiao==5)	    //5位
	{
		lcd_pos(2,4);
		lcd_wdat(space1[0]);
		while(hi_bit_xiabiao!=0)
		{
			lcd_wdat(shuzi[data_sepa[hi_bit_xiabiao-1]]);	
			hi_bit_xiabiao--;
		}
		lcd_pos(2,7);
		lcd_wdat(Hz[0]);lcd_wdat(Hz[1]);
	}
}

