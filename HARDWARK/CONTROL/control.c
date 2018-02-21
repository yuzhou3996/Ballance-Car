#include "control.h"
#include "filter.h"
#include "encoder.h"
#include "dianji.h"
extern int Encoder_Left;             //左右编码器的脉冲计数
extern int Encoder_Right;             //左右编码器的脉冲计数
extern int Position;
u8 Flag_Target;
float Real_Angle;
extern float Set_Angle;

extern float Angle_PID_Out;
extern float Speed_PID_Out;
extern float Position_PID_Out;
extern float Zong_PID_Out;
/**************************/
float err1=0;
float err_last1=0;
extern float Kp1;               //
extern float Ki1;                //
extern float Kd1;            //
float PID_I1=0;              //角度环
/********************************/
/**************************/
float err2=0;
float err_last2=0;
extern float Kp2;              // 
extern float Ki2;              //
extern float Kd2;              //
float PID_I2=0;              //速度环
/********************************/
/**************************/
float err3=0;
float err_last3=0;
extern float Kp3;              // 
extern float Ki3;              //
extern float Kd3;              //
float PID_I3=0;                //  位置环
/********************************/
int TIM1_UP_IRQHandler(void)  
{    
	if(TIM1->SR&0X0001)//5ms定时中断
	{   
		  TIM1->SR&=~(1<<0);		//===清除定时器1中断标志位
      Flag_Target=!Flag_Target;
		  if(Flag_Target==1)                                       //5ms读取一次陀螺仪和加速度计的值，更高的采样频率可以改善卡尔曼滤波和互补滤波的效果
			{
			Get_Angle(Way_Angle);                                    //===更新姿态	
			return 0;	
			}   
      Encoder_Right=Read_Encoder(2);                            //===读取编码器的值
			Encoder_Left=Read_Encoder(4);                           //===读取编码器的值			
	  	Get_Angle(Way_Angle);                                    //===更新姿态			
		  Zong_PID_Out=Angle_PID()+Speed_PID(0)+Position_PID(0);
	//	printf("Angle_PID_Out=%f\r\r\r\rPosition_PID_Out=%f\n",Angle_PID_Out,Position_PID_Out);
			Set_Pwm();
	}       	
	 return 0;	  
} 

/**************************************************************************
函数功能：获取角度
入口参数：获取角度的算法 1：无  2：卡尔曼 3：互补滤波
返回  值：无
**************************************************************************/
void Get_Angle(u8 way)
{ 
	    float Accel_Y,Accel_X,Accel_Z,Gyro_Y;
	    if(way==1)                                      //DMP没有涉及到严格的时序问题，在主函数读取
			{	
			}			
      else
      {
			Gyro_Y=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_L);    //读取Y轴陀螺仪
			Accel_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_L);    //读取Z轴陀螺仪
		  Accel_X=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_L); //读取X轴加速度记
	 		if(Gyro_Y>32768)  Gyro_Y-=65536;     //数据类型转换  也可通过short强制类型转换
			if(Accel_Z>32768)  Accel_Z-=65536;     //数据类型转换
	  	if(Accel_X>32768) Accel_X-=65536;    //数据类型转换
	   	Accel_Y=atan2(Accel_X,Accel_Z)*180/PI;                 //计算与地面的夹角	
		  Gyro_Y=Gyro_Y/16.4;                                    //陀螺仪量程转换	
      if(Way_Angle==2)		  	Kalman_Filter(Accel_Y,-Gyro_Y);//卡尔曼滤波	
			else if(Way_Angle==3)   Yijielvbo(Accel_Y,-Gyro_Y);    //互补滤波
	    Angle_Balance=angle;                                   //更新平衡倾角
	  	}
}

float Angle_PID()
     {
			      
	           Real_Angle=Angle_Balance;
			           err1 = (Set_Angle-Real_Angle);
			              if(err1>=0.8||err1<=-0.8)
													 Angle_PID_Out = Kp1*err1 + Ki1* PID_I1 + Kd1*(err1-err_last1);
                   	else Angle_PID_Out=0;										
										err_last1= err1;
										        PID_I1 += err1;
								return 		Angle_PID_Out;
//			 printf("Angle_PID_Out=%f\n",Angle_PID_Out);
				       
			}

float Speed_PID(float Set_Speed)
    {
	       err2=Set_Speed-(Encoder_Right+Encoder_Left);  //===获取最新速度偏差==测量速度（左右编码器之和）-目标速度（此次为零） 
	   	   Speed_PID_Out+=Kp2*(err2-err_last2)+Ki2*err2;                        //===速度PI控制
			    err_last2=err2; 
	      return Speed_PID_Out;
    }
float Position_PID(float Set_Position)
{
        Position+=Encoder_Right+Encoder_Left;        //===对速度进行积分，得到位置信息  
        err3=Position;
           PID_I3=err3;
   //     if(PID_I3>1500000)   PID_I3=1500000;   //积分限幅
	 //     if(PID_I3<-1500000)  PID_I3=-1500000; //积分限幅 
	      Position_PID_Out=Kp3*err3+Ki3*PID_I3;
	        return Position_PID_Out;
	      //   printf("Position_PID_Out=%f\n",Position_PID_Out);
}
		
		
void Set_Pwm(void)
{
	      int duty;
      	if(Zong_PID_Out>=269)   Zong_PID_Out=269;
				if(Zong_PID_Out<=-269)	Zong_PID_Out=-269;	//819
						       if(Zong_PID_Out>=0) 
			                  {
				                            L1 = 0;
			                              L2 = 1;
			                              R1 = 0;
			                              R2 = 1;
													               duty= (int)Zong_PID_Out;
													          TIM_SetCompare3(TIM3,630+duty);        //左轮死区600       
                                    TIM_SetCompare4(TIM3,615+duty);	      //右轮死区615
			                  }
			             else if (Zong_PID_Out<0)
			                  {
				                          Zong_PID_Out= -Zong_PID_Out;
													          L1 = 1;
			                              L2 = 0;
			                              R1 = 1;
			                              R2 = 0;
													          duty= (int)Zong_PID_Out;	   
 													          TIM_SetCompare3(TIM3,630+duty);        //左轮死区600       
                                    TIM_SetCompare4(TIM3,615+duty);	      //右轮死区615
					              }
}









