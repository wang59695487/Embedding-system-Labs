/****************************************************************************
* Copyright (C), 2013 �ܶ�Ƕ��ʽ������ www.ourstm.net
*
* �������� �ܶ���STM32������V2,V2.1,V3,V5,MINI�ϵ���ͨ��           
* QQ: 9191274, ������sun68, Email: sun68@163.com 
* �Ա����̣�ourstm.taobao.com  
*
* �ļ���: main.c
* ���ݼ���:	
*       
*	��ʾͨ������1��ʾADC1��11ͨ���Ĳ������
*	���������PC������Ҫ���д��ڵ������������
    
	����MDK�汾��        3.8
	���ڹٷ������汾�� 3.5	
*
* �ļ���ʷ:
* �汾��  ����       ����    ˵��
* v0.2    2011-7-28 sun68  �������ļ�
*
*/

1���Ҫ��
����ADC�ĵ�11ͨ���Կ���������ĵ�ѹֵ��ADת������������ת��ģʽ��ת�����ͨ��DMAͨ
��1��ȡ��ADCת���Ľ����ÿ���1�����򴮿ڷ���һ�Ρ�

2 Ӳ����·���													
�ڿ�������ͨ��I/O��PC.01��XS8��14����������PC.01ӳ�䵽ADC��11ͨ��������ʵ������ADC_IN11
�������ѹ��ADת����

3����������
	�����������Ҫ�����������Ҫ������
	(1)	����GPIO�ڣ���PC.01����ΪADC�ĵ�11����ͨ����������GPIO��PA.09��PA.10������Ϊ�������������
	(2)	����ADC����ADC_IN11����Ϊ����ת��ģʽ��
	(3)	����DMAͨ��1����ADC_IN14����ת���Ľ����
	(4)	���ô��ڼ���ط��͹��ܣ�
	(5)	ÿ��1S�򴮿����ADת�������

4 ���й���
(1)	ʹ��Keil uVision3 ͨ��JLINK���������ӿ����壬ʹ�ô����ߣ�����ʵ���
�ϵ�UART1��XS5)��PC���Ĵ��ڣ���ʵ������Ŀ¼�µ�STM32-FD-ADC.Uv2���̣��������ӹ��̣�
(2)	��PC��������windows�Դ��ĳ����ն˴���ͨ�ų��򣨲�����115200��1λֹͣλ����У��λ����Ӳ����
���ƣ�������ʹ����������ͨ�ų���
(3)	���MDK ��Debug�˵������Start/Stop Debug Session��
(4)	���Կ������������ֵ���ϱ仯��������ʾ���������ʾ��

usart1 print AD_value --------------------------
The current AD value = 2096 
The current AD value = 2096 
The current AD value = 2048 
The current AD value = 2087 
The current AD value = 2112 
The current AD value = 2112 
The current AD value = 2003 
The current AD value = 1998 
The current AD value = 1999 
The current AD value = 2092 
The current AD value = 2048 
The current AD value = 2051 
The current AD value = 2056 
The current AD value = 2048 
The current AD value = 2048 
The current AD value = 2096 
The current AD value = 2001 
....
....

