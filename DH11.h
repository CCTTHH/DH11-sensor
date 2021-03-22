#ifndef __DHT11_H
#define __DHT11_H


void dht11_output_init(void);			//输出串口初始化
void dht11_input_init(void);			//输入串口初始化
void dht11_start(void);						//主机发送开始信号
int dht11_read_8bits(void);				//一次读8bit数据
void dht11_get_data(void);				//从传感器接收数据

#endif
