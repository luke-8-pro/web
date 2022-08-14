#ifndef __display_char_H__
#define __display_char_H__

#define uchar unsigned char
void display_logo(void);	
void display_freq(uchar hi_bit_xiabiao);
void display_freq2(uchar hi_bit_xiabiao2);
void display_freq3(uchar hi_bit_xiabiao3);	
void dispstep1_inc(void);		
void dispstep1_dec(void);
void dispstep10_inc(void);		
void dispstep10_dec(void);		
void dispstep100_inc(void);		
void dispstep100_dec(void);		
void display_guding(void);	
void display_blank(void);
void display_blank0(void);
void display_blank1(void);
void display_blank2(void);
void display_blank3(void);	
void ClrScreen(void);
void dispworking(void);
void dispscanfre(void);
void dispscanfre1(void);
void dispsetfreq(void);
void dispmanual(void);
void dispoutwork(void);
void dispbl1(void);
void dispbl10(void);
void dispbl100(void);
void dispjx0(void);
void dispjx1(void);
void disperror_I(void);
void disperror_F(void);

#endif