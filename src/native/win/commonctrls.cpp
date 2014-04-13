/*
 * Scriptographer
 *
 * This file is part of Scriptographer, a Scripting Plugin for Adobe Illustrator
 * http://scriptographer.org/
 *
 * Copyright (c) 2002-2010, Juerg Lehni
 * http://scratchdisk.com/
 *
 * All rights reserved. See LICENSE file for details.
 */

#include "stdHeaders.h"
#include "ScriptographerEngine.h"
#include <hash_map>
#include "commonctrls.h"

#include <CommCtrl.h>
#pragma comment(lib,"comctl32.lib")
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

int CommonControlManager::nextid = 33000;

//delete on plugin end
CommonControlManager* commonCtrlManager;

CommonControlManager::CommonControlManager()
{
   INITCOMMONCONTROLSEX icex;           // Structure for control initialization.
    icex.dwICC = ICC_STANDARD_CLASSES ;
    bool res =  InitCommonControlsEx(&icex);
    if (!res)
      ; //throw?

    //ctrlMap = new ControlMap();
}

   bool CommonControlManager::OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
   {
      int nNotifyCode =HIWORD(wParam); 
     // BN_CLICKED
      //== BN_CLICKED )
     int id = LOWORD(wParam);
     HWND child = (HWND)lParam;
     
     GetControl(id)->OnCommand(nNotifyCode, wParam, lParam);

     return false;
   }
  
 
   bool CommonControlManager::OnParentNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
   {
     int msg = LOWORD(wParam);

      int id = HIWORD(wParam);


     if (msg == WM_CREATE)
     {
       //call create for id;
       GetControl(id)->OnCreate();
     }
     else
     if (msg == WM_DESTROY)
     {
       GetControl(id)->OnDestroy();
     }

    //todo - mouse events - track
     HWND child = (HWND)lParam;
     return false;
   }
  
   CommonControl *  CreateControl(char * itemType, int controlID);

   CommonControl * CommonControlManager::CreateItem(HWND hDlg, char * itemType,  RECT* rect, CommonControlInitProc Item_onInit, void * userData)
  {
      int ctrlId = CommonControlManager::nextid++;
      CommonControl * ctrl = CreateControl(itemType, ctrlId);
      ctrl->SetInitProc(Item_onInit);
      ctrl->SetUserData(userData);
      ctrlMap[ctrlId] = ctrl;

      ctrl->Create(hDlg, rect);
      //todo: map : id -> control;
      return ctrl;
  }


  void CommonControl::SetInitProc(CommonControlInitProc initProc)
  {
      this->initProc = initProc;
  }
  void CommonControl::SetNotifyProc(CommonControlNotifyProc notifyProc)
  {
      this->notifyProc = notifyProc;
  }
  void CommonControl::SetDestroyProc(CommonControlDestroyProc destroyProc)
  {
      this->destroyProc = destroyProc;
  }

  void * CommonControl::GetUserData()
  {
    return this->userData;
  }

  void   CommonControl::SetUserData(void * data)
  {
    this->userData = data;
  }

  bool CommonControl::Create(HWND hDlg, RECT* rect)
  {
    return false;
  }

  bool CommonControl::OnCommand(int notifyCode, WPARAM wParam, LPARAM lParam)
  {
      return false;
  }

/*windows controls*/

  class ButtonControl : public CommonControl
  {
    public:
        ButtonControl(int id):CommonControl(id) {};
    
        virtual bool Create(HWND hDlg, RECT* rect)
        {
            
            	hWnd = CreateWindowEx( 
		            0,  "BUTTON",   // Predefined class; Unicode assumed. 
		            "no text here now",		//  
		            WS_VISIBLE | WS_CHILD | ES_LEFT  | BS_PUSHBUTTON | BS_USERBUTTON,  // Styles. BS_OWNERDRAW?
		            rect->left,         // x position. 
		            rect->top,         // y position. 
		            rect->right,        // width.
		            rect->bottom,        // height.
		            hDlg,       // Parent window.
		            (HMENU) controlID,
		            (HINSTANCE)GetWindowLongPtr(hDlg, GWLP_HINSTANCE), 
		            NULL);      // Pointer not needed.
          
              return true;
        }

     bool ButtonControl::OnCommand(int notifyCode, WPARAM wParam, LPARAM lParam)
     {
        if (this->notifyProc != NULL)
         notifyProc(this, USER_CHANGED);
        return true;
     }
  };
   
  enum  EditFlags  {
        None     = 0,
        ReadOnly = 1,
        MultiLine =2,
        Password = 4,
    } ;

 class EditControl : public CommonControl
  {
    DWORD editFlags;

    public:
        EditControl(int id, DWORD flags ):CommonControl(id) 
          {editFlags = flags;};
    
      virtual bool Create(HWND hDlg, RECT* rect)
        {
             
            	hWnd = CreateWindowEx( 
		            0,  "EDIT",   // Predefined class; Unicode assumed. 
		            "no text here now",		//  
		            WS_VISIBLE | WS_CHILD | ES_LEFT  | editFlags, 
		            rect->left,         // x position. 
		            rect->top,         // y position. 
		            rect->right,        // width.
		            rect->bottom,        // height.
		            hDlg,       // Parent window.
		            (HMENU) controlID,
		            (HINSTANCE)GetWindowLongPtr(hDlg, GWLP_HINSTANCE), 
		            NULL);      // Pointer not needed.
          
              return true;
        }
  };

/*end windows controls*/



//factory for creating control
CommonControl *  CreateControl(char * itemType, int controlID)
{

    if (strcmpi(itemType, TEXT_PUSHBUTTON) == 0)
      return new ButtonControl(controlID);
    
   //edit boxes 
    if (strcmpi(itemType, TEXT_EDIT))
      return new EditControl(controlID, (int) EditFlags::None);
    if (strcmpi(itemType,  TEXT_EDIT_READONLY))
      return new EditControl(controlID, ES_READONLY );
    if (strcmpi(itemType, TEXT_EDIT_MULTILINE))
      return new EditControl(controlID,  ES_MULTILINE);
    if (strcmpi(itemType, TEXT_EDIT_MULTILINE_READONLY))
      return new EditControl(controlID,  ES_MULTILINE | ES_READONLY);
    
    
    
    return new CommonControl(controlID);
}


//mydebug

/* Initialize static variables */
	static WNDPROC defWndProc = NULL;
	static bool	registered = false;

  /*
  HWND HwndButtonAdd = CreateWindowEx( 
		0, L"BUTTON",   // Predefined class; Unicode assumed. 
		L"Add",		//  
		WS_VISIBLE | WS_CHILD | ES_LEFT,  // Styles. 
		30,         // x position. 
		130,         // y position. 
		100,        // width.
		20,        // height.
		hDlg,       // Parent window.
		(HMENU) ID_ButtonAdd,
		(HINSTANCE)GetWindowLongPtr(hDlg, GWLP_HINSTANCE), 
		NULL);      // Pointer not needed.

	HWND HwndButtonShow = CreateWindowEx( 
		0, L"BUTTON",   // Predefined class; Unicode assumed. 
		L"Show Dialog",		//  
		WS_VISIBLE | WS_CHILD | ES_LEFT,  // Styles. 
		30,         // x position. 
		160,         // y position. 
		100,        // width.
		20,        // height.
		hDlg,       // Parent window.
		(HMENU) ID_ButtonShow,
		(HINSTANCE)GetWindowLongPtr(hDlg, GWLP_HINSTANCE), 
		NULL);      // Pointer not needed.

	SendMessage(fHWndXEdit, (UINT) WM_SETTEXT, 0, (LPARAM)L"50"); 

	SendMessage(fHwndYEdit, (UINT) WM_SETTEXT, 0, (LPARAM)L"50");

	SendMessage(fHwndResultEdit, (UINT) WM_SETTEXT, 0, (LPARAM)L"100"); 
  */
#define CustomEditClassName "CustomEdit"
/* Declaration */
	LRESULT CALLBACK CustomEditWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


	void RegisterWithOS(HINSTANCE inAppInstance)
	{
	//	if(!registered)
		{
			WNDCLASSEX customEditClass;
			customEditClass.cbSize = sizeof(customEditClass);

			::GetClassInfoEx(NULL, "EDIT", &customEditClass);

			customEditClass.hInstance = inAppInstance;
			customEditClass.lpszClassName = CustomEditClassName;
			defWndProc = customEditClass.lpfnWndProc;
			customEditClass.lpfnWndProc = CustomEditWndProc;

			::RegisterClassEx(&customEditClass);

			registered = true;
		}
	}



HWND CreateCustomEdit(DWORD inExStyle, LPCTSTR inWindowName, DWORD inStyle, int x, int y, int inWidth, int inHeight, 
									HWND inParent, HMENU inMenu, HINSTANCE inHinstance, LPVOID inLpParam)
{
//	if(!registered)
	{
		RegisterWithOS(inHinstance);
	}

	return ::CreateWindowEx(	inExStyle
						,CustomEditClassName
						,inWindowName
						,inStyle
						,x
						,y
						,inWidth
						,inHeight
						,inParent
						,inMenu
						,inHinstance
						,inLpParam
						);
	
}

HWND CreateButton(HWND hDlg)
{
  HINSTANCE inHinstance = (HINSTANCE)GetWindowLongPtr(hDlg, GWLP_HINSTANCE);


	return ::CreateWindowEx(		0, 
             "BUTTON",   
						"buttn name",
						WS_VISIBLE | WS_CHILD | ES_LEFT | BS_PUSHBUTTON | BS_USERBUTTON,  // Styles. BS_OWNERDRAW?
		        30,         // x position. 
		        130,         // y position. 
		        100,        // width.
		        20,        // height.
		        hDlg,       // Parent window.
		        (HMENU) 1, //TODO id!
						(HINSTANCE)GetWindowLongPtr(hDlg, GWLP_HINSTANCE), 
						NULL
						);
	
}

HWND CreateItem( char * itemType, HWND hDlg)
{
  if (strcmp(TEXT_PUSHBUTTON, itemType) == 0)
    return CreateButton(hDlg);


  return CreateCustomEdit( 

		0, //L"EDIT",   // Predefined class; Unicode assumed. 
		"",		//  
		WS_VISIBLE | WS_CHILD | ES_RIGHT /*| ES_NUMBER*/ | WS_TABSTOP,  // Styles. 
		0,         // x position. 
		0,         // y position. 
		100,        // width.
		20,        // height.
		hDlg,       // Parent window.
		NULL, //(HMENU) ID_YEdit,
		(HINSTANCE)GetWindowLongPtr(hDlg, GWLP_HINSTANCE), 
		NULL);     
}

LRESULT CALLBACK CustomEditWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch(msg)
		{
			case WM_KEYDOWN:
			{
				/* Process only TAB and SHIFT + TAB */
				if( wParam == VK_TAB )
				{
					HWND parent = ::GetParent(hWnd);
					/* Poor implementation - Assumes that parent of edit knows about next focusable control */
					HWND nextFocusableItem = ::GetNextDlgTabItem(parent, hWnd, (::GetKeyState(VK_SHIFT) & 0x8000) != 0 ? true : false);
					if(nextFocusableItem)	::SetFocus(nextFocusableItem);
				
					/* Tell that we processed the message */
					return 0;
				}
			}
		}

		/* For all other messages call default window proc */
		return CallWindowProc(defWndProc, hWnd, msg, wParam, lParam);
	}



//end mydebug
