#ifndef __send_h__
#define __send_h__
void delay_1ms(unsigned int t);
void Buff_init(unsigned char *Dat);
void usart_init();
void send_data(unsigned char *Buff,unsigned char D1);
void send(unsigned char *Dat,unsigned char D2);

#endif