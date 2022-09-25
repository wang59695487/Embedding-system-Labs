
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

/* ucgui类型定义*/ 
WM_HWIN hWin,hWin1;
MULTIEDIT_HANDLE hmultiedit;
WM_HWIN text0,text1,text2,text3,text4,edit0,edit1,edit2,slider0,slider1,slider2,edit3,text5,text6;
const GUI_FONT* pFont = &GUI_FontComic24B_1;	
#pragma diag_suppress 870 
/* 定义了对话框资源列表 */
static const GUI_WIDGET_CREATE_INFO aDialogCreate[] = {
  //建立窗体， 大小是240X320  原点在0，0
  { FRAMEWIN_CreateIndirect, "LED Brightness Config", 0, 0,   0, 240, 200, FRAMEWIN_CF_ACTIVE }, //FRAMEWIN_CF_MOVEABLE},
	
  //建立TEXT控件，起点是窗体的10，20，大小180X30  文字左对齐
  { TEXT_CreateIndirect,     "Led Brightness",  GUI_ID_TEXT3,   10,   20,  180,  30, TEXT_CF_LEFT  },
  
  //建立TEXT控件，起点是窗体的200，20，大小39X30  文字左对齐
  { TEXT_CreateIndirect,     "%",  GUI_ID_TEXT4,   200,   20,  39,  30, TEXT_CF_LEFT  },
  
  //建立EDIT控件，起点是窗体的191，60，大小47X25  文字右对齐 4个字符宽度
  { EDIT_CreateIndirect,     "",       GUI_ID_EDIT0,    191,   60, 39,  25, EDIT_CF_RIGHT, 3 },


  //建立TEXT控件，起点是窗体的5，60，大小50X55  文字右对齐
  { TEXT_CreateIndirect,     "Led1",  GUI_ID_TEXT3,     5,   60,  50,  55, TEXT_CF_RIGHT },
 
  
  //建立滑动条控件，起点是窗体的60，60，大小130X25  
  { SLIDER_CreateIndirect,   NULL,     GUI_ID_SLIDER0,  60,  60, 130, 25, 0, 0 },
};

static const GUI_WIDGET_CREATE_INFO Adcdata[] = {
  //建立窗体， 大小是240X320  原点在0，0
{ FRAMEWIN_CreateIndirect, "ADC1 Channel 10 Demo", 0,               0,   200, 240, 200, FRAMEWIN_CF_ACTIVE },
	
    //建立窗体1， 大小是240X160  原点在0，0
  //建立TEXT控件，起点是窗体的138，50，大小95X35  文字右对齐
  { TEXT_CreateIndirect,     "",  GUI_ID_TEXT5,    138,   70,  95,  35, TEXT_CF_RIGHT },
  //建立TEXT控件，起点是窗体的2，60，大小130X55  文字右对齐
  { TEXT_CreateIndirect,     "Channel-10:",  GUI_ID_TEXT6,     2,   80,  130,  55, TEXT_CF_RIGHT },
};
/****************************************************************************
* 名    称：static void _cbCallback(WM_MESSAGE * pMsg)  
* 功    能：窗体回调函数 
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
static void _cbCallback(WM_MESSAGE * pMsg) {    
  int NCode, Id;
  WM_HWIN hDlg;
  hDlg = pMsg->hWin;	  
  switch (pMsg->MsgId) {     
    case WM_NOTIFY_PARENT:
      Id    = WM_GetId(pMsg->hWinSrc);      /*获得窗体部件的ID*/
      NCode = pMsg->Data.v;                 /*动作代码 */
      switch (NCode) {	          
        case WM_NOTIFICATION_VALUE_CHANGED: /*窗体部件的值被改变 */	   
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
* 名    称：static void _OnValueChanged(WM_HWIN hDlg, int Id) 
* 功    能：值被改变的动作
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/	   
static void _OnValueChanged(WM_HWIN hDlg, int Id) {

  if ((Id == GUI_ID_SLIDER0)) {							 //slider0 的值被改变
    milsec1=SLIDER_GetValue(slider0);					 //获得slider0的值
	EDIT_SetValue(edit0,milsec1);						 //EDIT0 的值被改变
  }		  
}

/****************************************************************************
* 名    称：void Fun(void)
* 功    能：显示及处理界面
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无 
****************************************************************************/
void Fun(void) {	   
  unsigned char edit_cur;
  GUI_CURSOR_Show(); 
  WM_SetCreateFlags(WM_CF_MEMDEV);       /* Automatically use memory devices on all windows */   
  /* 建立窗体，包含了资源列表，资源数目， 并指定回调函数 */  
  hWin = GUI_CreateDialogBox(aDialogCreate, GUI_COUNTOF(aDialogCreate), _cbCallback, 0, 0, 0);
  
  /* 设置窗体字体 */
  FRAMEWIN_SetFont(hWin, &GUI_FontComic24B_1);
	
	hWin1 = GUI_CreateDialogBox(Adcdata, GUI_COUNTOF(Adcdata), 0, 0, 0, 0);
	FRAMEWIN_SetFont(hWin1, &GUI_FontComic24B_1);
	
  /* 获得TEXT 部件的句柄 */
  text0 = WM_GetDialogItem(hWin, GUI_ID_TEXT0);
  text1 = WM_GetDialogItem(hWin, GUI_ID_TEXT1);
  text2 = WM_GetDialogItem(hWin, GUI_ID_TEXT2);
  text3 = WM_GetDialogItem(hWin, GUI_ID_TEXT3);
  text4 = WM_GetDialogItem(hWin, GUI_ID_TEXT4);
	text5 = WM_GetDialogItem(hWin1, GUI_ID_TEXT5);
	text6 = WM_GetDialogItem(hWin1, GUI_ID_TEXT6);
	
  /* 获得slider部件的句柄 */			
  slider0 = WM_GetDialogItem(hWin, GUI_ID_SLIDER0);

  /* 获得edit 部件的句柄 */
  edit0 = WM_GetDialogItem(hWin, GUI_ID_EDIT0);  
	
  /* 设置TEXT部件的字体 */			
  EDIT_SetFont(edit0,&GUI_FontComic18B_1);

  /* 设置EDIT部件采用10进制 范围50-20000*/ 
	//EDIT_SetDecMode(edit0,milsec1,0,100,0,0);
		EDIT_SetFloatMode(edit0,milsec1,0,100,0,0);
 // EDIT_SetDecMode(edit1,milsec2,50,2000,0,0);
 // EDIT_SetDecMode(edit2,milsec3,50,2000,0,0);
						
  /* 设置TEXT部件的字体 */
  TEXT_SetFont(text0,pFont);
  TEXT_SetFont(text1,pFont);
  TEXT_SetFont(text2,pFont);
  TEXT_SetFont(text3,pFont);
  TEXT_SetFont(text4,pFont);	
	TEXT_SetFont(text5,&GUI_FontD32);
	TEXT_SetFont(text6,pFont);	
  
  /* 设置TEXT部件的字体颜色 */        	
  TEXT_SetTextColor(text0,GUI_WHITE);
  TEXT_SetTextColor(text1,GUI_WHITE);
  TEXT_SetTextColor(text2,GUI_WHITE);
  TEXT_SetTextColor(text3,GUI_WHITE);
  TEXT_SetTextColor(text4,GUI_WHITE);
	TEXT_SetTextColor(text5,GUI_WHITE);
	TEXT_SetTextColor(text6,GUI_WHITE);
  /* 设置slider部件的取值范围50-2000 */		
  SLIDER_SetRange(slider0,0,100);

  /* 设置slider部件的值*/					
  SLIDER_SetValue(slider0,milsec1);	

	while (1)
  {
  	if(ADC_R==1){                         //1秒间隔采样
		ADC_R=0;
		//文本框显示
		TEXT_SetText(text5,ADC_STR1);	  
	}		  
	WM_Exec(); 							  //刷新屏幕
  }											 //屏幕刷新
  }

/******************* (C) COPYRIGHT 2013 奋斗STM32 *****END OF FILE****/
