#include "sys.h"
#include "pwm.h"
#include "dianji.h"
#include "encoder.h"
#include "oled.h"
#include "hwjs.h"
#include "show.h"
#include "key.h"
/**************************************************************************
死区问题   位置环问题
**************************************************************************/
u8 Way_Angle=2 ; 
float Angle_Balance; 
u16 CCR3_Val=0;
u16 CCR4_Val=0;
int Encoder_Left;             //左编码器的脉冲计数
int Encoder_Right;             //右编码器的脉冲计数
int Position;
float Angle_PID_Out;
float Speed_PID_Out;
float Position_PID_Out;
float Zong_PID_Out;
u8 flag1=1,flag2,flag3,flag4,flag5,flag6,flag7,flag8,flag9;

float Set_Angle=1.7;    //设定的中值角度2.0

/***********************************************/
float Kp1=11;               //   加死区12      不加死区90    28
float Ki1=0;                //
float Kd1=33;            //      加死区19     不加死区160     60

float Kp2=0.35;               //   3.03`
float Ki2=0;                //
float Kd2=0;            //

float Kp3=0;              // 0.5
float Ki3=0;             //
float Kd3=0;              //
/***********************************************/

int main(void)
{ 
	
	Stm32_Clock_Init(9);            //系统时钟设置
	delay_init(72);                 //延时初始化
	OLED_Init();			//初始化OLED  
	OLED_Clear();
	uart_init(72,115200);           //初始化串口1
	TIM3_PWM_Init(&CCR3_Val,&CCR4_Val);
	Encoder_Init_TIM2();            //初始化编码器1 
	Encoder_Init_TIM4();            //初始化编码器2 
	IIC_Init();                     //模拟IIC初始化
  MPU6050_initialize();           //=====MPU6050初始化
  DMP_Init();	
	DIANJI_Init();
  hwjs_init();   //红外接口初始化
  Timer1_Init(49,7199);           //=====5MS进一次中断服务函数	
	while(1)
		{
			  
			   HongWai_Scan();
	       Key_Scan();	
         printf("红外接收码 %d\r\n",1234);	//打印
//         TIM_SetCompare3(TIM3,700);        //左轮       
//         TIM_SetCompare4(TIM3,600);	      //右轮
//	     printf("互 补 滤 波 输 出 Pitch:  %f\r\n  ",Angle_Balance);  //y      3
//       printf("卡 尔 曼 滤 波 输 出 Pitch:  %f\r\n  ",Angle_Balance);  //y   2  
//			if(Way_Angle==1)                     //DMP没有涉及到严格的时序问题，在主函数读取
//				{
//					Read_DMP();                      //===读取角速度和倾角
//					Angle_Balance=Pitch;             //===更新平衡倾角
//     	                         printf("Roll:%f  ",Roll);  //X                                      1
//			                         printf("Pitch:%f  ",Pitch); //Y
// 			                         printf("Yaw:%f\r\n",Yaw);   //Z	
//				}
//                               printf("Encoder_Left=%d,Encoder_Right%d\n",Encoder_Left,Encoder_Right);
//			if(hw_jsbz==1)	//如果红外接收到
//		{
//			hw_jsbz=0;	   //清零
//			                         printf("红外接收码 %0.8X\r\n",hw_jsm);	//打印
//			                        
//			hw_jsm=0;					//接收码清零
//		}
		} 
}
