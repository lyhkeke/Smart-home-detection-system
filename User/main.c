#include "stm32f10x.h"
#include "bsp_SysTick.h"
#include "bsp_dht11.h"
#include "bsp_usart.h"
#include "bsp_ili9341_lcd.h"
#include "bsp_beep.h"
#include "bsp_key.h"  
#include "bsp_exti.h"
#include "bsp_led.h"  
#include "bsp_breathing.h"
#include "./flash/bsp_spi_flash.h"


uint8_t warning_tem=30;

uint8_t dark;

void UpdateWarning();;

int main()
{

//   int temp;
  char dispBuff[100];
	char warning_str[100];
	DHT11_Data_TypeDef DHT11_Data;
	
	BEEP_GPIO_Config();
	
	
  UpdateWarning();
	
	EXTI_Key_Config();
		
	/* 初始化系统定时器 */
	SysTick_Init();
                              
  //LCD 初始化
	ILI9341_Init (); 
 //其中0、3、5、6 模式适合从左至右显示文字，
 //不推荐使用其它模式显示文字	其它模式显示文字会有镜像效果			
 //其中 6 模式为大部分液晶例程的默认显示方向  
	ILI9341_GramScan ( 6 );

  ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
  
	/*初始化DTT11的引脚*/
	DHT11_Init ();
	
	TIMx_Breathing_Init();

  LED_GPIO_Config();
	LED1_OFF;
	LED2_OFF;

	

	while(1)
	{	

		
			LCD_SetColors(RED,BLACK);
		
      ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);
		
		  dark= GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5);
		
//		ILI9341_DispStringLine_EN_CH(LINE(10),"温度监测系统");
  

      
		
		if(dark==0)
		{
			  sprintf(dispBuff,"NOW:day  ");
        LCD_ClearLine(LINE(0));	/* 清除单行文字 */
        ILI9341_DispStringLine_EN(LINE(0),dispBuff);

		}
		else
		{
			  sprintf(dispBuff,"NOW:night");
        LCD_ClearLine(LINE(0));	/* 清除单行文字 */
        ILI9341_DispStringLine_EN(LINE(0),dispBuff);

		}	
			/*调用DHT11_Read_TempAndHumidity读取温湿度，若成功则输出该信息*/

	
		if( DHT11_Read_TempAndHumidity ( & DHT11_Data ) == SUCCESS)
			{

        
        /* 显示温度 */
        sprintf(dispBuff,"Temperature : %d.%d ",DHT11_Data.temp_int, DHT11_Data.temp_deci);
        LCD_ClearLine(LINE(2));	/* 清除单行文字 */
        ILI9341_DispStringLine_EN(LINE(1),dispBuff);
        
				
				
//				sprintf(dispBuff,"当前温度:%d",temp);
//        LCD_ClearLine(LINE(2));	/* 清除单行文字 */
//            ILI9341_DispStringLine_EN_CH(LINE(1),dispBuff);
				
				
        /* 显示湿度 */
        sprintf(dispBuff,"Humidity : %d.%d%% ",DHT11_Data.humi_int, DHT11_Data.humi_deci);
        LCD_ClearLine(LINE(3));	/* 清除单行文字 */
        ILI9341_DispStringLine_EN(LINE(2),dispBuff);
				
        sprintf(warning_str,"Warning : %d",warning_tem);
        ILI9341_DispStringLine_EN(LINE(4),warning_str);
				LCD_ClearLine(LINE(7));	/* 清除单行文字 */

				
//				sprintf(warning_str,"报警温度: %d",warning_tem);
//        ILI9341_DispStringLine_EN_CH(LINE(4),warning_str);
				
				
				LCD_ClearLine(LINE(7));	/* 清除单行文字 */
				
				
				
	
//        ILI9341_DispStringLine_EN_CH(LINE(7),"");
	
				LCD_SetTextColor(RED);
				ILI9341_DrawCircle(80,200,20,0);
	
	
			  LCD_SetTextColor(GREEN);
				ILI9341_DrawCircle(140,200,20,0);
//				LCD_SetTextColor(GREEN);
//				ILI9341_DrawCircle(100,200,10,1);
	
				LCD_SetTextColor(BLUE);
				ILI9341_DrawCircle(110,200,20,0);
				
				
					LCD_SetTextColor(BLUE);
				ILI9341_DrawCircle(170,200,20,0);
//        LCD_SetTextColor(GREEN);
//				ILI9341_DrawCircle(140,200,10,1);
			}			
				
		if(DHT11_Data.temp_int>=warning_tem) 
				
			{
				BEEP(ON);
			}
			else 
			{
				BEEP(OFF);
			
		  }
				
			if(DHT11_Data.temp_int>=warning_tem) 

			{
				LED1_ON;
			}
			else
			{
			  LED1_OFF;
			}
//      LED1_OFF;
		 Delay_ms(1000);
		
	}

}
void UpdateWarning()
{
	char warning_str[100];
	sprintf(warning_str,"Warning : %d",warning_tem);
  ILI9341_DispStringLine_EN(LINE(4),warning_str);
	
	
	
//		sprintf(warning_str,"报警温度: %d",warning_tem);
//        ILI9341_DispStringLine_EN(LINE(4),warning_str);

}

//void EXTI0_IRQHandler()
//{

//	//确保是否产生了 EXTI Line 中断
//	   if (EXTI_GetITStatus(EXTI_Line0) != RESET) 
//			 {
//		     warning_tem++;
//			UpdateWarning();
//		 //清除中断标志位
//				 EXTI_ClearITPendingBit(EXTI_Line0);
//	     }

//}

//void EXTI15_10_IRQHandler()
//{
//  //确保是否产生了EXTI Line中断
//	if(EXTI_GetITStatus(EXTI_Line13) != RESET) 
//	{
//		 warning_tem--;
//	 UpdateWarning();
//    //清除中断标志位
//		 EXTI_ClearITPendingBit(EXTI_Line13);     
//	}  
//}



