#ifndef __DELAY_H__
#define __DELAY_H__
#define uchar unsigned char
typedef bit BOOL ;
void delay(int ms);
bit lcd_busy(void);
void lcd_wcmd(uchar cmd);				
void lcd_pos(uchar X,uchar Y);	
void lcd_wdat(uchar dat);		
void lcd_init(void);

#endif