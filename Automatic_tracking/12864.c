#include <intrins.h>
#include <reg52.h>
#include "12864.h"
	
#define LCD_data  P3             //���ݿ�
sbit LCD_RS  =  P1^5;            //�Ĵ���ѡ������ 
sbit LCD_RW  =  P1^6;            //Һ����/д����
sbit LCD_EN  =  P1^7;            //Һ��ʹ�ܿ���
//sbit LCD_PSB =  P0^4;            //��/����ʽ����
void delay(int ms)
{
    while(ms--)
	{
      uchar i;
	  for(i=0;i<250;i++)  
	   {
	    _nop_();			   
		_nop_();
		_nop_();
		_nop_();
	   }
	}
}



bit lcd_busy()
 {                          
    bit result;
    LCD_RS = 0;
    LCD_RW = 1;
    LCD_EN = 1;
    _nop_();_nop_();_nop_();_nop_();
    result = (bit)(LCD_data&0x80);
    LCD_EN = 0;
    return(result); 
 }


void lcd_wcmd(uchar cmd)
{                          
   //while(lcd_busy());
    lcd_busy();
    LCD_RS = 0;
    LCD_RW = 0;
    LCD_EN = 0;
    _nop_();
    _nop_(); 
    LCD_data = cmd;
    _nop_();_nop_();_nop_();_nop_();
    LCD_EN = 1;
    _nop_();_nop_();_nop_();_nop_();
    LCD_EN = 0;  
}

/*********************************************************/
/*                                                       */
/* �趨��ʾλ��                                          */
/*                                                       */
/*********************************************************/
void lcd_pos(uchar X,uchar Y)
{                          
   uchar  pos;
   if (X==0)
     {X=0x80;}
   else if (X==1)
     {X=0x90;}
   else if (X==2)
     {X=0x88;}
   else if (X==3)
     {X=0x98;}
   pos = X+Y ;  
   lcd_wcmd(pos);     //��ʾ��ַ
}
/*******************************************************************/
/*                                                                 */
/*д��ʾ���ݵ�LCD                                                  */
/*RS=H��RW=L��E=�����壬D0-D7=���ݡ�                               */
/*                                                                 */
/*******************************************************************/
void lcd_wdat(uchar dat)
{                          
   //while(lcd_busy());
    lcd_busy();
    LCD_RS = 1;
    LCD_RW = 0;
    LCD_EN = 0;
    LCD_data = dat;
    _nop_();_nop_();_nop_();_nop_();
    LCD_EN = 1;
    _nop_();_nop_();_nop_();_nop_();
    LCD_EN = 0; 
}
void lcd_init()
{ 

    //LCD_PSB = 1;         //���ڷ�ʽ
    
    lcd_wcmd(0x34);      //����ָ�����
    delay(5);
    lcd_wcmd(0x30);      //����ָ�����
    delay(5);
    lcd_wcmd(0x0C);      //��ʾ�����ع��
    delay(5);
    lcd_wcmd(0x01);      //���LCD����ʾ����
    delay(5);
}


