#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "hwjs.h"
#include "oled.h"
#include "show.h"
long a[]={0xffa25d,0xff629d,0xffe21d,0xff22dd,0xff02fd,0xffc23d,0xffe01f,0xffa857,0xff906f};
extern u8 flag1,flag2,flag3,flag4,flag5,flag6,flag7,flag8,flag9;


/***********************************************/
extern float Kp1;               //   加死区12      不加死区90
extern float Ki1;                //
extern float Kd1;            //      加死区19     不加死区160

extern float Kp2;               //   1.0
extern float Ki2;                //
extern float Kd2;            //

extern float Kp3;              // 0.5
extern float Ki3;             //
extern float Kd3;              //
/***********************************************/
extern float Set_Angle;


/***********************************************
电源     00ffa25d
模式     00ff629d
声音     00ffe21d
暂停     00ff22dd
左键     00ff02fd
右键     00ffc23d
EQ键     00ffe01f
VOL-     00ffa857
VOL+     00ff906f
 RPT     00ff9867
U/SD     00ffb04f
 0键     00ff6897
 1键     00ff30cf
 2键     00ff18e7
 3键     00ff7a85
 4键     00ff10ef
 5键     00ff38c7
 6键     00ff5aa5
 7键     00ff42bd
 8键     00ff4ab5
 9键     00ff52ad
 ****************************************************/
 
void HongWai_Scan()
{
       switch(hw_jsm) 
              { 
								   case 0xffa25d:flag1=1,hw_jsm=0; ;break;     //电源键   
								   case 0xff629d:flag2=1,hw_jsm=0; ;break;     //模式键
								   case 0xffe21d:flag3=1,hw_jsm=0; break;     //声音键    三种显示界面 
								   case 0xff22dd:flag4=1,hw_jsm=0; break;     //暂停键
								   case 0xff02fd:flag5=1,hw_jsm=0; break;     //左键
								   case 0xffc23d:flag6=1,hw_jsm=0; break;     //右键
								   case 0xffe01f:flag7=1,hw_jsm=0; break;     //EQ键
								   case 0xffa857:flag8=1,hw_jsm=0; break;     //VOL-
								   case 0xff906f:flag9=1,hw_jsm=0; break;     //VOL+       参数调节
								   default :     break;                     
							} 
}

void Key_Scan(void)
{
  while(flag1)
				{
					  oled_show1();
					  HongWai_Scan();
					  if(flag2==1||flag3==1) 
						    {
								        OLED_Clear();
						            flag1=0;
						    }
				}	
	while(flag2)
				{
					  oled_show2();
					  HongWai_Scan();
					    if(flag4==1) flag4=0,Kp1=Kp1+0.5;         //角度环参数调节
              if(flag7==1) flag7=0,Kp1=Kp1-0.5;
					    if(flag5==1) flag5=0,Set_Angle=Set_Angle+0.1;
              if(flag8==1) flag8=0,Set_Angle=Set_Angle-0.1;
              if(flag6==1) flag6=0,Kd1=Kd1+0.5;
              if(flag9==1) flag9=0,Kd1=Kd1-0.5;
					
					  if(flag1==1||flag3==1)
						   {
							   OLED_Clear();
						     flag2=0;
						   }
				}	
	while(flag3)
				{
					  oled_show3();
					  HongWai_Scan();
					    if(flag4==1) flag4=0,Kp2=Kp2+0.01;
              if(flag7==1) flag7=0,Kp2=Kp2-0.01;
					    
              if(flag6==1) flag6=0,Kd2=Kd2+1;
              if(flag9==1) flag9=0,Kd2=Kd2-1;
					
					  if(flag1==1||flag2==1)
						   {
							   OLED_Clear();
						     flag3=0;
						   }
				}	


}




