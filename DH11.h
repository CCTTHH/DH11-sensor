#ifndef __DHT11_H
#define __DHT11_H


void dht11_output_init(void);			//������ڳ�ʼ��
void dht11_input_init(void);			//���봮�ڳ�ʼ��
void dht11_start(void);						//�������Ϳ�ʼ�ź�
int dht11_read_8bits(void);				//һ�ζ�8bit����
void dht11_get_data(void);				//�Ӵ�������������

#endif
