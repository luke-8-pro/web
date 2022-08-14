#include <intrins.h>

#define uchar unsigned char	
typedef bit BOOL ;		

sbit rs	= P1^5;		
sbit rw = P1^6;		
sbit ep = P1^7;		
#define dataport_1602 P3

	
void delay_1602(uchar ms);

BOOL lcd_busy_1602(void);		
void lcd_wcmd_1602(uchar cmd);		
void lcd_pos_1602(uchar n,uchar pos);	
void lcd_wdat_1602(uchar dat);		
void lcd_init_1602(void);		


void delay_1602(uchar ms)
{			
	uchar i;
	while(ms--)
	{
		for(i = 0; i< 120; i++)
		{
			_nop_();
			_nop_();
			_nop_();
			_nop_();
		}
	}
}



BOOL lcd_busy_1602(void)
{				
	BOOL result;		
	rs = 0;
	rw = 1;
	ep = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	result = (BOOL)(dataport_1602 & 0x80);
	ep = 0;
	return result;	
}


void lcd_wcmd_1602(uchar cmd)
{				
	while(lcd_busy_1602());	
	rs = 0;
	rw = 0;
	ep = 0;
	_nop_();
	_nop_();	
	dataport_1602 = cmd;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	ep = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	ep = 0;		
}

void lcd_pos_1602(uchar n,uchar pos)
{				
	if(n==0)
	lcd_wcmd_1602(pos | 0x80);
	if(n==1)
	lcd_wcmd_1602(pos | 0xc0);
}

void lcd_wdat_1602(uchar dat)	
{				
	while(lcd_busy_1602());	
	rs = 1;
	rw = 0;
	ep = 0;
	dataport_1602 = dat;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	ep = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	ep = 0;	
}

void lcd_init_1602(void)
{				
	lcd_wcmd_1602(0x38);		
	delay_1602(1);
	lcd_wcmd_1602(0x0c);		
	delay_1602(1);
	lcd_wcmd_1602(0x06);		
	delay_1602(1);
	lcd_wcmd_1602(0x01);		
	delay_1602(1);
}


