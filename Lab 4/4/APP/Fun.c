
#include "..\GUIinc\GUI.h" 
#include "..\GUIinc\LCD_ConfDefaults.h"  /* valid LCD configuration */
#include "..\GUIinc\WM.h"
#include "..\GUIinc\DIALOG.h" 
#include "..\GUIinc\EDIT.h"
#include "..\GUIinc\SLIDER.h"
#include "..\GUIinc\FRAMEWIN.h"
#include "..\GUIinc\MULTIEDIT.h"
#include "demo.h"
static void _OnValueChanged(WM_HWIN hDlg, int Id);

/* ucgui���Ͷ���*/ 
WM_HWIN hWin,hWin1;
MULTIEDIT_HANDLE hmultiedit;
WM_HWIN text0,text1,text2,text3,text4,edit0,edit1,edit2,slider0,slider1,slider2,edit3,text5,text6;
const GUI_FONT* pFont = &GUI_FontComic24B_1;	
#pragma diag_suppress 870 
/* �����˶Ի�����Դ�б� */
static const GUI_WIDGET_CREATE_INFO aDialogCreate[] = {
  //�������壬 ��С��240X320  ԭ����0��0
  { FRAMEWIN_CreateIndirect, "LED Brightness Config", 0, 0,   0, 240, 200, FRAMEWIN_CF_ACTIVE }, //FRAMEWIN_CF_MOVEABLE},
	
  //����TEXT�ؼ�������Ǵ����10��20����С180X30  ���������
  { TEXT_CreateIndirect,     "Led Brightness",  GUI_ID_TEXT3,   10,   20,  180,  30, TEXT_CF_LEFT  },
  
  //����TEXT�ؼ�������Ǵ����200��20����С39X30  ���������
  { TEXT_CreateIndirect,     "%",  GUI_ID_TEXT4,   200,   20,  39,  30, TEXT_CF_LEFT  },
  
  //����EDIT�ؼ�������Ǵ����191��60����С47X25  �����Ҷ��� 4���ַ����
  { EDIT_CreateIndirect,     "",       GUI_ID_EDIT0,    191,   60, 39,  25, EDIT_CF_RIGHT, 3 },


  //����TEXT�ؼ�������Ǵ����5��60����С50X55  �����Ҷ���
  { TEXT_CreateIndirect,     "Led1",  GUI_ID_TEXT3,     5,   60,  50,  55, TEXT_CF_RIGHT },
 
  
  //�����������ؼ�������Ǵ����60��60����С130X25  
  { SLIDER_CreateIndirect,   NULL,     GUI_ID_SLIDER0,  60,  60, 130, 25, 0, 0 },
};

static const GUI_WIDGET_CREATE_INFO Adcdata[] = {
  //�������壬 ��С��240X320  ԭ����0��0
{ FRAMEWIN_CreateIndirect, "ADC1 Channel 10 Demo", 0,               0,   200, 240, 200, FRAMEWIN_CF_ACTIVE },
	
    //��������1�� ��С��240X160  ԭ����0��0
  //����TEXT�ؼ�������Ǵ����138��50����С95X35  �����Ҷ���
  { TEXT_CreateIndirect,     "",  GUI_ID_TEXT5,    138,   70,  95,  35, TEXT_CF_RIGHT },
  //����TEXT�ؼ�������Ǵ����2��60����С130X55  �����Ҷ���
  { TEXT_CreateIndirect,     "Channel-10:",  GUI_ID_TEXT6,     2,   80,  130,  55, TEXT_CF_RIGHT },
};
/****************************************************************************
* ��    �ƣ�static void _cbCallback(WM_MESSAGE * pMsg)  
* ��    �ܣ�����ص����� 
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
static void _cbCallback(WM_MESSAGE * pMsg) {    
  int NCode, Id;
  WM_HWIN hDlg;
  hDlg = pMsg->hWin;	  
  switch (pMsg->MsgId) {     
    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);      /*��ô��岿����ID*/
      NCode = pMsg->Data.v;                 /*�������� */
      switch (NCode) {	          
        case WM_NOTIFICATION_VALUE_CHANGED: /*���岿����ֵ���ı� */	   
          _OnValueChanged(hDlg, Id);		 
          break;
        default:
          break;
      }
      break;  
    default:
      WM_DefaultProc(pMsg);
  }
}

/****************************************************************************
* ��    �ƣ�static void _OnValueChanged(WM_HWIN hDlg, int Id) 
* ��    �ܣ�ֵ���ı�Ķ���
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/	   
static void _OnValueChanged(WM_HWIN hDlg, int Id) {

  if ((Id == GUI_ID_SLIDER0)) {							 //slider0 ��ֵ���ı�
    milsec1=SLIDER_GetValue(slider0);					 //���slider0��ֵ
	EDIT_SetValue(edit0,milsec1);						 //EDIT0 ��ֵ���ı�
  }		  
}

/****************************************************************************
* ��    �ƣ�void Fun(void)
* ��    �ܣ���ʾ���������
* ��ڲ�������
* ���ڲ�������
* ˵    ����
* ���÷������� 
****************************************************************************/
void Fun(void) {	   
  unsigned char edit_cur;
  GUI_CURSOR_Show(); 
  WM_SetCreateFlags(WM_CF_MEMDEV);       /* Automatically use memory devices on all windows */   
  /* �������壬��������Դ�б���Դ��Ŀ�� ��ָ���ص����� */  
  hWin = GUI_CreateDialogBox(aDialogCreate, GUI_COUNTOF(aDialogCreate), _cbCallback, 0, 0, 0);
  
  /* ���ô������� */
  FRAMEWIN_SetFont(hWin, &GUI_FontComic24B_1);
	
	hWin1 = GUI_CreateDialogBox(Adcdata, GUI_COUNTOF(Adcdata), 0, 0, 0, 0);
	FRAMEWIN_SetFont(hWin1, &GUI_FontComic24B_1);
	
  /* ���TEXT �����ľ�� */
  text0 = WM_GetDialogItem(hWin, GUI_ID_TEXT0);
  text1 = WM_GetDialogItem(hWin, GUI_ID_TEXT1);
  text2 = WM_GetDialogItem(hWin, GUI_ID_TEXT2);
  text3 = WM_GetDialogItem(hWin, GUI_ID_TEXT3);
  text4 = WM_GetDialogItem(hWin, GUI_ID_TEXT4);
	text5 = WM_GetDialogItem(hWin1, GUI_ID_TEXT5);
	text6 = WM_GetDialogItem(hWin1, GUI_ID_TEXT6);
	
  /* ���slider�����ľ�� */			
  slider0 = WM_GetDialogItem(hWin, GUI_ID_SLIDER0);

  /* ���edit �����ľ�� */
  edit0 = WM_GetDialogItem(hWin, GUI_ID_EDIT0);  
	
  /* ����TEXT���������� */			
  EDIT_SetFont(edit0,&GUI_FontComic18B_1);

  /* ����EDIT��������10���� ��Χ50-20000*/ 
	//EDIT_SetDecMode(edit0,milsec1,0,100,0,0);
		EDIT_SetFloatMode(edit0,milsec1,0,100,0,0);
 // EDIT_SetDecMode(edit1,milsec2,50,2000,0,0);
 // EDIT_SetDecMode(edit2,milsec3,50,2000,0,0);
						
  /* ����TEXT���������� */
  TEXT_SetFont(text0,pFont);
  TEXT_SetFont(text1,pFont);
  TEXT_SetFont(text2,pFont);
  TEXT_SetFont(text3,pFont);
  TEXT_SetFont(text4,pFont);	
	TEXT_SetFont(text5,&GUI_FontD32);
	TEXT_SetFont(text6,pFont);	
  
  /* ����TEXT������������ɫ */        	
  TEXT_SetTextColor(text0,GUI_WHITE);
  TEXT_SetTextColor(text1,GUI_WHITE);
  TEXT_SetTextColor(text2,GUI_WHITE);
  TEXT_SetTextColor(text3,GUI_WHITE);
  TEXT_SetTextColor(text4,GUI_WHITE);
	TEXT_SetTextColor(text5,GUI_WHITE);
	TEXT_SetTextColor(text6,GUI_WHITE);
  /* ����slider������ȡֵ��Χ50-2000 */		
  SLIDER_SetRange(slider0,0,100);

  /* ����slider������ֵ*/					
  SLIDER_SetValue(slider0,milsec1);	

	while (1)
  {
  	if(ADC_R==1){                         //1��������
		ADC_R=0;
		//�ı�����ʾ
		TEXT_SetText(text5,ADC_STR1);	  
	}		  
	WM_Exec(); 							  //ˢ����Ļ
  }											 //��Ļˢ��
  }

/******************* (C) COPYRIGHT 2013 �ܶ�STM32 *****END OF FILE****/
