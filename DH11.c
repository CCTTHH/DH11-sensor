#include "stm32f10x.h"
#include "delay.h"
#include <stdio.h>
#include "usart1.h"
#include"DH11.h"
#define DH11ioPin GPIO_Pin_7




void dht11_output_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;	//初始化GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = DH11ioPin;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}//把连接dht11的引脚设置成输出模式

void dht11_input_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = DH11ioPin;
	GPIO_InitStructure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void dht11_start(void)
{
	dht11_output_init();
	GPIO_ResetBits(GPIOA, DH11ioPin);
	delay_ms(19);
	GPIO_SetBits(GPIOA, DH11ioPin);
	delay_us(25);
}

int dht11_read_8bits(void)
{
	int data = 0;
	int time = 0;
	int i = 0;
	for(i = 0; i<8; i++)
	{
		time = 0;
		data <<=1;
			while(0==GPIO_ReadInputDataBit(GPIOA, DH11ioPin))
		{
		
		}
		while(1==GPIO_ReadInputDataBit(GPIOA, DH11ioPin))
		{
			delay_us(1);
			time++;
		}
		if(time>20)
		{
			data |=1;
		}
	
	}
	return data;
	
}


void dht11_get_data(void)
{
	unsigned char rh,rl,th,tl,check;
	dht11_start();
	dht11_input_init();
	if(0==GPIO_ReadInputDataBit(GPIOA, DH11ioPin))
	{
		while(0==GPIO_ReadInputDataBit(GPIOA, DH11ioPin))
		{
		
		}
		while(1==GPIO_ReadInputDataBit(GPIOA, DH11ioPin))
		{
		
		}
		rh = dht11_read_8bits();		//湿度整数部分
		rl = dht11_read_8bits();		//湿度小数部分
		th = dht11_read_8bits();		//温度的整数部分
		tl = dht11_read_8bits();		//温度的小数部分
		check = dht11_read_8bits();	//校验
		
		dht11_output_init();
		GPIO_SetBits(GPIOA,GPIO_Pin_7);	//把引脚设置成高电平，使模块处于空闲状态，方便下一次数据采集
		
		if(check == rh + rl + th + tl)
		{
			char rh_s[10];  
			char rl_s[10]; 
			char th_s[10]; 
			char tl_s[10]; 
			sprintf(rh_s,"%d",rh); 
			sprintf(rl_s,"%d",rl);
			sprintf(th_s,"%d",th);
			sprintf(tl_s,"%d",tl);
			printf("%d, %d, %d, %d",rh, rl, th, tl);
			usart2_printf("AT+CIPSEND=0,65\r\n");
			delay_ms(100);
			
			usart2_printf("check successed ..... right data \ntemp ");

			usart2_printf(rh_s);
			USART_SendData(USART2, '.');
			while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
			usart2_printf(rl_s);

			usart2_printf("\nhumi ");
			usart2_printf(th_s);
			USART_SendData(USART2, '.');
			while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
			usart2_printf(tl_s);
			

			usart2_printf("\n-------------------------");
			delay_ms(1000);
		}
		else
		{
			usart2_printf("AT+CIPSEND=0,15\r\n");
			usart2_printf("check failed.......\n");
		}
		
			
	}
}

