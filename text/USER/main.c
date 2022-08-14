#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "can.h"
#include <stdio.h>
#include "timer.h"
#include "math.h"

#define FILTER_BUF_LEN		5
#define ABS(x)		((x>0)? x: -x) 

/************************************************
				   PID�ṹ��
************************************************/
u8 CAN_MotorPosistionInit();

u8 Inertial_navigation_data[24];   /*�ӹߵ����յ�������*/

union{
 u8 initData[24];
 float ActVal[6];
}posture;                                    /*�������ȡ��λ����������*/
// 
typedef enum
{

	PID_Position,
	PID_Speed
	
}PID_ID;

typedef struct _PID_TypeDef
{
	PID_ID id;
	
	float target;							//Ŀ��ֵ
	float lastNoneZeroTarget;
	float kp;
	float ki;
	float kd;
	
	float   measure;					//����ֵ
	float   err;							//���
	float   last_err;      		//�ϴ����
	
	float pout;
	float iout;
	float dout;
	
	float output;						//�������
	float last_output;			//�ϴ����
	
	float MaxOutput;				//����޷�
	float IntegralLimit;		//�����޷�
	float DeadBand;			  //����������ֵ��
	float ControlPeriod;		//��������
	float  Max_Err;					//������
	
					  uint32_t thistime;
					uint32_t lasttime;
						uint8_t dtime;	
	
	void (*f_param_init)(struct _PID_TypeDef *pid,  //PID������ʼ��
				   PID_ID id,
				   uint16_t maxOutput,
				   uint16_t integralLimit,
				   float deadband,
				   uint16_t controlPeriod,
					int16_t max_err,     
					int16_t  target,
				   float kp,
				   float ki,
				   float kd);
				   
	void (*f_pid_reset)(struct _PID_TypeDef *pid, float kp,float ki, float kd);		//pid���������޸�
	float (*f_cal_pid)(struct _PID_TypeDef *pid, float measure);   //�����ٶ�Ŀʵ��ֵ��������Ƶ���ֵ
}PID_TypeDef;

PID_TypeDef drive_motor_pid[4];  //�ĸ����
PID_TypeDef lift_motor_pid[3];    


/************************************************
				   �ṹ����������
************************************************/
 struct message
{
	u8 index;
  uint16_t position;  /*ת�ӻ�е�Ƕ� */
  uint16_t ini_position;  /*ת�ӳ�ʼ��е�Ƕ� */
  int16_t velocity; /*ת��ת�� */
  uint16_t torque;     /*���ת��*/
  int  AccePosition;    /*λ���ۼ�*/
	int32_t		round_cnt;
	int lift_position_list[3];
	u8 lift_order;
	u8 lift_status;
} motorstatus[8];
/************************************************
				�ṹ�������任��
************************************************/



int position_control(struct message tempMotor);   //�������λ���ٶȿ���
void pid_init(PID_TypeDef* pid);                   
void Velocity_decomposition(int32_t Vx_position,int32_t Vy_position);          //���̵���ٶȽ���
void  sate_judgement(u8 N_4,u8 N_5,u8 N_6);     //״̬�ж�
void sendarry(USART_TypeDef*USARTx, u8* pBuffer,u16 len);   //ָ�����ڷ�������
void Instruction_Verification();
void send_lift_status(); 
void CAR_W(); 
int32_t set_spd = 3;
int flag_pid_cal_liftmotor=0,flag_pid_cal_drivemotor=0,flag_uart_send=0;
u8 target_current_drive_motor[8]={0};
u8 target_current_lift_motor[8]={0};
u8 flag_startgetorder;//��ʼ���ܱ�־λ
u8 flag_getpcmessage;//��λ�����ӵĴ��ڴ�����������һ֡���ݺ���λ������ѭ���н��������ݲ���ɷ��ͺ���գ�
u8 orderData[12];//��λ����������
u8 pcupsidedata[17];//����λ�����͵�����
int32_t Vx,Vy;
int pos_x=0,pos_y=0,zangle=0;
u8 car_w;//С����ǰ״̬
/************************************************
				     ������ʼ
************************************************/

 int main(void)
 {	 
  u16 ti=0;                     //forѭ��������
	int i,di;
	u8 t=0;                    //led����
	u8 cnt=0;                 //������������������	             
	                    
	u8 mode=CAN_Mode_LoopBack;//CAN����ģʽ;CAN_Mode_Normal(0)����ͨģʽ��CAN_Mode_LoopBack(1)������ģʽ �Է��������ڼ���
 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ���can���շ���Ҳ��Ҫ�ж�
	uart_init(9600);	 	//����1��ʼ��Ϊ9600
  usart2_init(115200);  //����2��ʼ��Ϊ9600
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	LCD_Init();			   	//��ʼ��LCD	 
  TIM3_Int_Init(9,719);    //������ʱ�жϽ���can����0.1ms   (9+1)*(7199+1)/72/1000

	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,2,CAN_Mode_Normal);//CAN��ʼ������ģʽ,������1MKbps   CAN_Mode_LoopBackΪ1���ǻػ�ģʽ ,CAN_Mode_Normal Ϊ����ģʽ   1MKbps=1000kbps(������ )36/(8+9+1)*2

	 
	//delay_ms(2000);
	 for(di=0; di<4; di++)
  {	

    pid_init(&drive_motor_pid[di]);
		
   drive_motor_pid[di].f_param_init(&drive_motor_pid[di],PID_Speed,16384,5000,10,0,8000,0,6,0.1,0.1);
    
  }
	for(di=0; di<3; di++)
  {	

    pid_init(&lift_motor_pid[di]);
   lift_motor_pid[di].f_param_init(&lift_motor_pid[di],PID_Speed,16384,5000,10,0,8000,0,0.6,0.1,0.1);
    
  }	
	CAN_MotorPosistionInit();


 	while(1)
	{

		if(flag_getpcmessage)
		 
		{
		
			Velocity_decomposition(Vx,Vy);	                                //1.�ֽ�VX,VYΪ�ĸ����������ʵʱת��ָ��,�ж�Wָ��
	  	CAR_W();
//			USART_SendData(USART1,'O');//�򴮿ڷ�������
//			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//������ɺ�TCλ��Ӳ����
//			USART_SendData(USART1,Vy>>24);//�򴮿ڷ�������
//			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//������ɺ�TCλ��Ӳ����
//			USART_SendData(USART1,Vy>>16);//�򴮿ڷ�������
//			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//������ɺ�TCλ��Ӳ����
//			USART_SendData(USART1,Vy>>8);//�򴮿ڷ�������
//			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//������ɺ�TCλ��Ӳ����
//			USART_SendData(USART1,Vy);//�򴮿ڷ�������
//			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//������ɺ�TCλ��Ӳ����
//			USART_SendData(USART1,'D');//�򴮿ڷ�������
//			while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//������ɺ�TCλ��Ӳ����
			pcupsidedata[0]='A';
			pcupsidedata[1]=pos_x>>24|0x00;
			pcupsidedata[2]=pos_x>>16|0x00;
			pcupsidedata[3]=pos_x>>8|0x00;
			pcupsidedata[4]=pos_x|0x00;
			
			pcupsidedata[5]=pos_y>>24|0x00;
			pcupsidedata[6]=pos_y>>16|0x00;
			pcupsidedata[7]=pos_y>>8|0x00;
			pcupsidedata[8]=pos_y|0x00;
			
			pcupsidedata[9]=zangle>>24|0x00;
			pcupsidedata[10]=zangle>>16|0x00;
			pcupsidedata[11]=zangle>>8|0x00;
			pcupsidedata[12]=zangle|0x00;
		
			pcupsidedata[13]=motorstatus[4].lift_status;
			pcupsidedata[14]=motorstatus[5].lift_status;
			pcupsidedata[15]=motorstatus[6].lift_status;
			pcupsidedata[16]='W';
			sendarry(USART1,pcupsidedata,17); 
			flag_getpcmessage=0;		
		}							
		
		if(flag_pid_cal_drivemotor>=10)
		{
			
 
		
			
			drive_motor_pid[0].f_cal_pid(&drive_motor_pid[0],motorstatus[0].velocity ); //1�ŵ���ٶ�PID����
			target_current_drive_motor[0]=(int)drive_motor_pid[0].output>>8;
			target_current_drive_motor[1]=(int)drive_motor_pid[0].output;
			drive_motor_pid[1].f_cal_pid(&drive_motor_pid[1],motorstatus[1].velocity ); //2�ŵ���ٶ�PID����
			target_current_drive_motor[2]=(int)drive_motor_pid[1].output>>8;
			target_current_drive_motor[3]=(int)drive_motor_pid[1].output;
			drive_motor_pid[2].f_cal_pid(&drive_motor_pid[2],motorstatus[2].velocity ); //3�ŵ���ٶ�PID����
			target_current_drive_motor[4]=(int)drive_motor_pid[2].output>>8;
			target_current_drive_motor[5]=(int)drive_motor_pid[2].output;
			drive_motor_pid[3].f_cal_pid(&drive_motor_pid[3],motorstatus[3].velocity ); //4�ŵ���ٶ�PID����
			target_current_drive_motor[6]=(int)drive_motor_pid[3].output>>8;
			target_current_drive_motor[7]=(int)drive_motor_pid[3].output; //�õ���̨���Ŀ�����	
		}
		if(flag_pid_cal_liftmotor>=100)
			{
				sate_judgement(motorstatus[4].lift_order,motorstatus[5].lift_order,motorstatus[6].lift_order);												          //2.�ж��������ָ���뵱ǰ״̬�Ƿ�һ�£���ǰ״̬���䣺��ǰ״̬���"3"��
				lift_motor_pid[0].target=position_control(motorstatus[4]);
				lift_motor_pid[1].target=position_control(motorstatus[5]);
				lift_motor_pid[2].target=position_control(motorstatus[6]);
	
			
				lift_motor_pid[0].f_cal_pid(&lift_motor_pid[0],motorstatus[4].velocity ); //��һ�������5���ٶ�PID����
				target_current_lift_motor[0]=(int)lift_motor_pid[0].output>>8;
				target_current_lift_motor[1]=(int)lift_motor_pid[0].output;
				lift_motor_pid[1].f_cal_pid(&lift_motor_pid[1],motorstatus[5].velocity ); //�ڶ��������6���ٶ�PID����
				target_current_lift_motor[2]=(int)lift_motor_pid[1].output>>8;
				target_current_lift_motor[3]=(int)lift_motor_pid[1].output;
				lift_motor_pid[2].f_cal_pid(&lift_motor_pid[2],motorstatus[6].velocity ); //�����������7���ٶ�PID����
				target_current_lift_motor[4]=(int)lift_motor_pid[2].output>>8;
				target_current_lift_motor[5]=(int)lift_motor_pid[2].output;
			
			}
	
		
		
		if((flag_pid_cal_drivemotor>=10)|(flag_pid_cal_liftmotor>=100))
		{
			
			//u8 tempVector[8]={0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00}; 
			flag_pid_cal_drivemotor=0;
			flag_pid_cal_liftmotor=0;
			                            
			Can_Send_Msg(target_current_drive_motor,8);	            //���ͱ�ʶ��Ϊ0x0200����֡		
      Can_Send_Msg2(target_current_lift_motor,8);	          //���ͱ�ʶ��Ϊ0x01ff����֡			
			delay_ms(10);
		}

		
		if(flag_uart_send>=50000)
		{   u16 tempVector[8]={0x0200,0x0201,0x0202,0x0203,0x0204,0x0205,0x0206,0x0207};
				flag_uart_send=0;
			
		}		
		
	}
}
 


/************************************************
						  ����1�жϷ������ 
************************************************/


void USART1_IRQHandler(void)                	
{   
   
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�
	{  
		static u8 count_receive=0;  
		u8 Res=0;
		static int count_data=0; 	
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);	
		Res =USART_ReceiveData(USART1);
		
		switch (count_receive)
		{
			case 0:
			if(Res=='O') count_receive++;
			else  count_receive=0;
			break;
			case 1: 		 
			orderData[count_data]=Res;
			count_data++;
			if(count_data>=12){count_data=0;count_receive++;}
			break;		
			case 2: 
			if(Res=='D')
			{
			flag_getpcmessage=1;
			Vx=orderData[0]<<24|orderData[1]<<16|orderData[2]<<8|orderData[3];
			Vy=orderData[4]<<24|orderData[5]<<16|orderData[6]<<8|orderData[7];
			car_w=orderData[8];
			motorstatus[4].lift_order=orderData[9]; 
			motorstatus[5].lift_order=orderData[10];
			motorstatus[6].lift_order=orderData[11];
			}
			count_receive=0;count_data=0;break;
	default:
	count_receive=0;break;



	}
//        	 if(n>=8){n=0;}				    //����������8�����ǵ�һλ		 
} 
}
/************************************************
						  ����2�жϷ������ 
************************************************/


void USART2_IRQHandler(void)                	
{   
   static  u8 ch,count=0,i=0;
					if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�
					{  USART_ClearITPendingBit(USART2,USART_IT_RXNE);	
 						ch=USART_ReceiveData(USART2);
						switch(count)
						{
							 case 0:{if(ch==0x0d) count++;else  count=0; break;}	 
      case 1:
			  {
						 if(ch==0x0a)        {i=0;count++;}
						 else if(ch==0x0d);
						 else                {count=0;}
						 break;		
			  }	 
      case 2:posture.initData[i]=ch;i++;
	           if(i>=24){i=0;count++;}
						 break;		
		  case 3:{if(ch==0x0a) count++;else count=0;  break;}	 	
			case 4:
							if(ch==0x0d){
								zangle=(int)(posture.ActVal[0]*100);
								pos_x=(int)(posture.ActVal[3]*100);
								pos_y=(int)(posture.ActVal[4]*100); 
							 } count=0;break;
			     default:count=0;break;				
						}
					}
} 
/************************************************
						 ָ�����ڷ��ͳ���
************************************************/

void sendarry(USART_TypeDef*USARTx, u8* pBuffer,u16 len)
{ 
   u16 i=0;
	 for(i=0;i<len;i++)
					{
						USART_SendData(USARTx,pBuffer[i]);//�򴮿ڷ�������
						while(USART_GetFlagStatus(USARTx,USART_FLAG_TC)!=SET);//������ɺ�TCλ��Ӳ����0
	
					}	
}





/************************************************
			  can���ݽ��ճ�ʼ��
************************************************/
u8 CAN_MotorPosistionInit()
{
	u8 init_count=0;
	u8 tempCount=0;
	CanRxMsg RxMessage;
	
	int i;
	for (i=0;i<7;i++){
		motorstatus[i].round_cnt=0;
		motorstatus[i].AccePosition=0;
	}
	while(init_count!=0X7F)
	{
		if( CAN_MessagePending(CAN1,CAN_FIFO0)==0) delay_ms(1);		//û�н��յ�����,ֱ���˳�   		
		else
		{
			CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//��ȡ����	
			switch(RxMessage.StdId)
				{				
				case 0x0201 :   motorstatus[0].index=1;motorstatus[0].ini_position=RxMessage.Data[0]<<8|RxMessage.Data[1];motorstatus[0].position=motorstatus[0].ini_position; init_count=init_count|0X01;break;            //�����ݴ浽1�ŵ�������� ��break;  
				case 0x0202 :   motorstatus[1].index=2;motorstatus[1].ini_position=RxMessage.Data[0]<<8|RxMessage.Data[1];motorstatus[1].position=motorstatus[1].ini_position; init_count=init_count|0X02;break;            //�����ݴ浽1�ŵ�������� ��break;  
				case 0x0203 :   motorstatus[2].index=3;motorstatus[2].ini_position=RxMessage.Data[0]<<8|RxMessage.Data[1]; motorstatus[2].position=motorstatus[2].ini_position;init_count=init_count|0X04;break;            //�����ݴ浽1�ŵ�������� ��break;  
				case 0x0204 :   motorstatus[3].index=4;motorstatus[3].ini_position=RxMessage.Data[0]<<8|RxMessage.Data[1]; motorstatus[3].position=motorstatus[3].ini_position;init_count=init_count|0X08;break;            //�����ݴ浽1�ŵ�������� ��break;  
				case 0x0205 :   motorstatus[4].index=5;motorstatus[4].ini_position=RxMessage.Data[0]<<8|RxMessage.Data[1]; motorstatus[4].position=motorstatus[4].ini_position;init_count=init_count|0X10;
					motorstatus[4].lift_position_list[0]=0;motorstatus[4].lift_position_list[1]=360*36*(-2); motorstatus[4].lift_position_list[2]=360*36*(-4)-5000;motorstatus[4].lift_status=0;motorstatus[4].lift_order=0;    break; //�����ݴ浽5�ŵ�������� ��break;  
				case 0x0206 :   motorstatus[5].index=6;motorstatus[5].ini_position=RxMessage.Data[0]<<8|RxMessage.Data[1]; motorstatus[5].position=motorstatus[5].ini_position;init_count=init_count|0X20;           //�����ݴ浽1�ŵ�������� ��break;  
					motorstatus[5].lift_position_list[0]=0;motorstatus[5].lift_position_list[1]=360*36*(-2);motorstatus[5].lift_position_list[2]=360*36*(-2);motorstatus[5].lift_status=0;motorstatus[5].lift_order=0; break; //�����ݴ浽6�ŵ�������� ��break;  
				case 0x0207 :   motorstatus[6].index=7;motorstatus[6].ini_position=RxMessage.Data[0]<<8|RxMessage.Data[1]; motorstatus[6].position=motorstatus[6].ini_position;init_count=init_count|0X40;            //�����ݴ浽1�ŵ�������� ��break;  
					motorstatus[6].lift_position_list[0]=0;motorstatus[6].lift_position_list[1]=360*36*(-2);motorstatus[6].lift_position_list[2]=360*36*(-4)-5000;motorstatus[5].lift_status=0;motorstatus[6].lift_order=0;break; //�����ݴ浽7�ŵ�������� ��break;  
				}
		}
		tempCount++;
	
	}
	return 0;
}

/************************************************
			  can���ݽ�����ȡ����
************************************************/
u8 CanRecieveMsg(){
	CanRxMsg RxMessage;
	if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//û�н��յ�����,ֱ���˳�   
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//��ȡ����	
	switch(RxMessage.StdId)
		{
		uint16_t old_position=0;			
		case 0x0201 :                //�����ݴ浽1�ŵ�������� ��break;  
			old_position= motorstatus[0].position;
			motorstatus[0].position=RxMessage.Data[0]<<8|RxMessage.Data[1];
		  if((motorstatus[0].position-old_position)>4096)
				{
				motorstatus[0].round_cnt--;	
				}	
				 if((motorstatus[0].position-old_position)<-4096)
				{
				motorstatus[0].round_cnt++;	
				}	
			motorstatus[0].AccePosition=motorstatus[0].round_cnt*360+(motorstatus[0].position-motorstatus[0].ini_position)*360/8192;
			motorstatus[0].velocity=RxMessage.Data[2]<<8|RxMessage.Data[3];
			if(motorstatus[0].velocity>30000) motorstatus[0].velocity-=65536;
			motorstatus[0].torque=RxMessage.Data[4]<<8|RxMessage.Data[5];
			break;				
		case 0x0202:
		  old_position= motorstatus[1].position;
			motorstatus[1].position=RxMessage.Data[0]<<8|RxMessage.Data[1];
		  if((motorstatus[1].position-old_position)>4096)
				{
				motorstatus[1].round_cnt--;	
				}	
				 if((motorstatus[1].position-old_position)<-4096)
				{
				motorstatus[1].round_cnt++;	
				}	
			 motorstatus[1].AccePosition=motorstatus[1].round_cnt*360+(motorstatus[1].position-motorstatus[1].ini_position)*360/8192;			
			 motorstatus[1].position=RxMessage.Data[0]<<8|RxMessage.Data[1];
		   motorstatus[1].velocity=RxMessage.Data[2]<<8|RxMessage.Data[3];
			if(motorstatus[1].velocity>30000) motorstatus[0].velocity-=65536;
		   motorstatus[1].torque=RxMessage.Data[4]<<8|RxMessage.Data[5];
	
		   break;
		case 0x0203:
			old_position= motorstatus[2].position;
			motorstatus[2].position=RxMessage.Data[0]<<8|RxMessage.Data[1];
		  if((motorstatus[2].position-old_position)>4096)
				{
				motorstatus[2].round_cnt--;	
				}	
				 if((motorstatus[2].position-old_position)<-4096)
				{
				motorstatus[2].round_cnt++;	
				}	
		 	 motorstatus[2].AccePosition=motorstatus[2].round_cnt*360+(motorstatus[2].position-motorstatus[2].ini_position)*360/8192;		
			 motorstatus[2].position=RxMessage.Data[0]<<8|RxMessage.Data[1];
		   motorstatus[2].velocity=RxMessage.Data[2]<<8|RxMessage.Data[3];
			if(motorstatus[2].velocity>30000) motorstatus[0].velocity-=65536;
		   motorstatus[2].torque=RxMessage.Data[4]<<8|RxMessage.Data[5];
		   break;
		case 0x0204:
			old_position= motorstatus[3].position;
				motorstatus[3].position=RxMessage.Data[0]<<8|RxMessage.Data[1];
				if((motorstatus[3].position-old_position)>4096)
					{
					motorstatus[3].round_cnt--;	
					}	
					 if((motorstatus[3].position-old_position)<-4096)
					{
					motorstatus[3].round_cnt++;	
					}	
			 motorstatus[3].AccePosition=motorstatus[3].round_cnt*360+(motorstatus[3].position-motorstatus[3].ini_position)*360/8192;			
			 motorstatus[3].position=RxMessage.Data[0]<<8|RxMessage.Data[1];
		   motorstatus[3].velocity=RxMessage.Data[2]<<8|RxMessage.Data[3];
			if(motorstatus[3].velocity>30000) motorstatus[0].velocity-=65536;
		   motorstatus[3].torque=RxMessage.Data[4]<<8|RxMessage.Data[5];
		   break;
		case 0x0205:
			old_position= motorstatus[4].position;
			motorstatus[4].position=RxMessage.Data[0]<<8|RxMessage.Data[1];
		  if((motorstatus[4].position-old_position)>4096)
				{
				motorstatus[4].round_cnt--;	
				}	
				 if((motorstatus[4].position-old_position)<-4096)
				{
				motorstatus[4].round_cnt++;	
				}	
			 motorstatus[4].AccePosition=(int)(motorstatus[4].round_cnt*360+(motorstatus[4].position-motorstatus[4].ini_position)*360/8192);
			 motorstatus[4].position=RxMessage.Data[0]<<8|RxMessage.Data[1];
		   motorstatus[4].velocity=RxMessage.Data[2]<<8|RxMessage.Data[3];
			if(motorstatus[4].velocity>30000) motorstatus[0].velocity-=65536;
		   motorstatus[4].torque=RxMessage.Data[4]<<8|RxMessage.Data[5];
		   break;
		case 0x0206:
		old_position= motorstatus[5].position;
			motorstatus[5].position=RxMessage.Data[0]<<8|RxMessage.Data[1];
		  if((motorstatus[5].position-old_position)>6000)
				{
				motorstatus[5].round_cnt--;	
				}	
				 if((motorstatus[5].position-old_position)<-6000)
				{
				motorstatus[5].round_cnt++;	
				}	
			motorstatus[5].AccePosition=(int)(motorstatus[5].round_cnt*360+(motorstatus[5].position-motorstatus[5].ini_position)*360/8192);	
			 motorstatus[5].position=RxMessage.Data[0]<<8|RxMessage.Data[1];
		   motorstatus[5].velocity=RxMessage.Data[2]<<8|RxMessage.Data[3];
			if(motorstatus[5].velocity>30000) motorstatus[0].velocity-=65536;
		   motorstatus[5].torque=RxMessage.Data[4]<<8|RxMessage.Data[5];
		   break;
		case 0x0207:
		old_position= motorstatus[6].position;
			motorstatus[6].position=RxMessage.Data[0]<<8|RxMessage.Data[1];
		  if((motorstatus[6].position-old_position)>4096)
				{
				motorstatus[6].round_cnt--;	
				}	
				 if((motorstatus[6].position-old_position)<-4096)
				{
				motorstatus[6].round_cnt++;	
				}	
			motorstatus[6].AccePosition=(int)(motorstatus[6].round_cnt*360+(motorstatus[6].position-motorstatus[6].ini_position)*360/8192);		
			 motorstatus[6].position=RxMessage.Data[0]<<8|RxMessage.Data[1];
		   motorstatus[6].velocity=RxMessage.Data[2]<<8|RxMessage.Data[3];
			if(motorstatus[6].velocity>30000) motorstatus[0].velocity-=65536;
		   motorstatus[6].torque=RxMessage.Data[4]<<8|RxMessage.Data[5];
		   break;
		default:
			return 0;
		}
		return RxMessage.DLC;	
}




void TIM3_IRQHandler(void)   //TIM3�ж�
{
	static int led_count=0;
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
	   CanRecieveMsg();      
			flag_pid_cal_drivemotor++;
			flag_pid_cal_liftmotor++;
			flag_uart_send++;
			led_count++;
			if(led_count>5000)
			{
			LED0=!LED0;         //��ʾϵͳ��������	
			led_count=0;	
			}
			
		}
}



/*������ʼ��--------------------------------------------------------------*/
static void pid_param_init(
	PID_TypeDef * pid, 
	PID_ID   id,
	uint16_t maxout,
	uint16_t intergral_limit,
	float deadband,
	uint16_t period,
	int16_t  max_err,
	int16_t  target,

	float 	kp, 
	float 	ki, 
	float 	kd)
{
	pid->id = id;		
	
	pid->ControlPeriod = period;             //û�õ�
	pid->DeadBand = deadband;
	pid->IntegralLimit = intergral_limit;
	pid->MaxOutput = maxout;
	pid->Max_Err = max_err;
	pid->target = target;
	
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	
	pid->output = 0;
}

/*��;���Ĳ����趨--------------------------------------------------------------*/
static void pid_reset(PID_TypeDef * pid, float kp, float ki, float kd)
{
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
}

/*pid����-----------------------------------------------------------------------*/

	
static float pid_calculate(PID_TypeDef* pid, float measure)
{

	
	
	pid->measure = measure;

		
	pid->last_err  = pid->err;
	pid->last_output = pid->output;
	
	pid->err = pid->target - pid->measure;  //���
	
	//�Ƿ��������
	if((ABS(pid->err) > pid->DeadBand))   //����������
	{
		pid->pout = pid->kp * pid->err;    //p���ΪKp*���
		pid->iout += (pid->ki * pid->err);   //i���Ϊi+ki*���
		 

		pid->dout =  pid->kd * (pid->err - pid->last_err);  //d���Ϊkd*�����-�ϴ���
		
		//�����Ƿ񳬳�����
		if(pid->iout > pid->IntegralLimit)
			pid->iout = pid->IntegralLimit;       
		if(pid->iout < - pid->IntegralLimit)
			pid->iout = - pid->IntegralLimit;
		
		//pid�����
		pid->output = pid->pout + pid->iout + pid->dout;   
		

		//pid->output = pid->output*0.7f + pid->last_output*0.3f;  //�˲���
		if(pid->output>pid->MaxOutput)         
		{
			pid->output = pid->MaxOutput;
		}
		if(pid->output < -(pid->MaxOutput))
		{
			pid->output = -(pid->MaxOutput);
		}
	
	}


	return pid->output;
}


void pid_init(PID_TypeDef* pid)
{
	pid->f_param_init = pid_param_init;
	pid->f_pid_reset = pid_reset;
	pid->f_cal_pid = pid_calculate;
}

/************************************************
             λ���ٶȱջ�����
************************************************/
int position_control(struct message tempMotor)
{
		int Lc,Lt;
		int16_t Vc;
		int ati,Vmax,Vmin,a0,Vt,dL,e;
		float dt;
		Vmax=5000;
		Vmin=100;	
		a0=2000;
		dt=0.01;
		e=1000;		
		Lt=tempMotor.lift_position_list[tempMotor.lift_order];
		Lc=tempMotor.AccePosition;
//		Lt=0;
//		Lc=-51840;	
		Vc=tempMotor.velocity;

		dL=ABS(Lt-Lc);
		ati=((Lt-Lc)>0?1:-1);
		//ati=(Lt-Lc)/dL;
		if(dL<=e)
		{
			Vt=0;
			
			return 0;
		}
		else{
			if((Vc*ati)<-50)
			{
				Vt=0;
				return 0;
			}
			else
			{
				if(dL<=((Vc*Vc)/(2*a0)))
				{
					Vt=(sqrt(2*a0*dL))*ati;	
				}
				else
				{
					if(ABS(Vc)<=Vmax)
					{
						Vt=Vc+a0*dt*ati;
						
					}
				}
				if(ABS(Vt)<=Vmin)
				{
					Vt=Vmin*ati;
				}
				else if(ABS(Vt)>Vmax)
				{
					Vt=Vmax*ati;
				}
						
			}
	}			
		//tempMotor.lift_status=3;		
			return Vt;
}
/************************************************
           �ֽ�����ٶ�
************************************************/

void Velocity_decomposition(int32_t Vx_position,int32_t Vy_position)
{

if(Vx_position||Vy_position)
{
										drive_motor_pid[0].target=(float)(Vx_position*0.069)-(float)(Vy_position*0.069);
										drive_motor_pid[1].target=-(float)(Vx_position*0.069)-(float)(Vy_position*0.069);
										drive_motor_pid[2].target=-(float)(Vx_position*0.069)+(float)(Vy_position*0.069);
										drive_motor_pid[3].target=(float)(Vx_position*0.069)+(float)(Vy_position*0.069);									 
//-(float)(Vy_position*0.069)
//-(float)(Vy_position*0.069)
//+(float)(Vy_position*0.069)
//(float)(Vy_position*0.069)
}
else

		{
				drive_motor_pid[0].target=0;
				drive_motor_pid[1].target=0;
				drive_motor_pid[2].target=0;
				drive_motor_pid[3].target=0;	
		}


}
		
	
/************************************************
              �жϵ����ǰ״̬
************************************************/
void  sate_judgement(u8 N_4,u8 N_5,u8 N_6){
	
	if(motorstatus[4].lift_status==N_4)
	{
		motorstatus[4].lift_status=N_4;
	}
	else
	{
	
		if(ABS(motorstatus[4].AccePosition-motorstatus[4].lift_position_list[N_4])>1000)
		{
		 motorstatus[4].lift_status=3;	
		}
		else
		{
		 motorstatus[4].lift_status=N_4;
		}
		
	}
	if(motorstatus[5].lift_status==N_5)
	{
		motorstatus[5].lift_status=N_5;
	}
	else
	{
		
		if(ABS(motorstatus[5].AccePosition-motorstatus[5].lift_position_list[N_5])>1000)
		{
		 motorstatus[5].lift_status=3;	
		}
		else
		{
		motorstatus[5].lift_status=N_5;	
		}
	}
	if(motorstatus[6].lift_status==N_6)
	{
		motorstatus[6].lift_status=N_6;
	}
	else
	{
		
		if(ABS(motorstatus[6].AccePosition-motorstatus[6].lift_position_list[N_6])>1000)
		{
		 motorstatus[6].lift_status=3;	
		}
		else
		{
		motorstatus[6].lift_status=N_6;	
		}
	}

}
/************************************************             
               �����������״̬
************************************************/
void send_lift_status()
{
 
		USART_SendData(USART1,motorstatus[4].lift_status);//�򴮿ڷ�������
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//������ɺ�TCλ��Ӳ����0
		USART_SendData(USART1,motorstatus[5].lift_status);//�򴮿ڷ�������
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//������ɺ�TCλ��Ӳ����0
		USART_SendData(USART1,motorstatus[6].lift_status);//�򴮿ڷ�������
		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//������ɺ�TCλ��Ӳ����0		
	
}
/************************************************             
          ��̬ �ж�
************************************************/
void CAR_W()
{
if(car_w==0x01)
{
       drive_motor_pid[0].target+=60;
			 drive_motor_pid[1].target+=60;
			 drive_motor_pid[2].target+=60;
			 drive_motor_pid[3].target+=60;	  	
	
}
else if(car_w==0x02)
{
			drive_motor_pid[0].target+=-60;
			drive_motor_pid[1].target+=-60;
			drive_motor_pid[2].target+=-60;
			drive_motor_pid[3].target+=-60;
	
}
//else if(car_w==0x00)
//{
//      drive_motor_pid[0].target+=0;
//			drive_motor_pid[1].target+=0;
//			drive_motor_pid[2].target+=0;
//			drive_motor_pid[3].target+=0;	

//}
}

