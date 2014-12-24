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
#include "ScriptographerPlugin.h"
#include <hash_map>
#include "commonctrls.h"
#include <CommCtrl.h>
#pragma comment(lib,"comctl32.lib")
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

LRESULT CALLBACK CDialog::StaticWndProc(HWND hWnd, UINT uMsg,WPARAM wParam, LPARAM lParam) {

  CDialog * dlg = (CDialog*)GetWindowLongPtr(hWnd,GWLP_USERDATA);
  if(!dlg) return DefWindowProc(hWnd,uMsg,wParam,lParam);

		// Consider this window activated if we either receive a WM_NCACTIVATE
		// message or if a WM_PARENTNOTIFY with a mouse message is received
		// (see ScriptographerPlugin::appWindowProc for more information).
		if (uMsg == WM_NCACTIVATE || uMsg == WM_PARENTNOTIFY && wParam > 0x200) {
			//jobject obj = gEngine->getDialogObject(it->second.panel);
			//TODOgEngine->callOnNotify(obj, kADMWindowActivateNotifier);
          
		}
    return dlg->WndProc(hWnd, uMsg, wParam, lParam);
   
}

LRESULT CDialog::WndProc(HWND hWnd, UINT uMsg,WPARAM wParam, LPARAM lParam)
{
  //deal with usual messages
	switch(uMsg)
	{
    case WM_COMMAND:
      OnCommand(hWnd, wParam, lParam);
      break;
    case WM_PARENTNOTIFY:
      OnParentNotify(hWnd, wParam, lParam);
      break;

	default:
		return DefWindowProc(hWnd,uMsg,wParam,lParam);
	};
	return 0;
}



   bool CDialog::OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
   {
      int nNotifyCode =HIWORD(wParam); 
     int id = LOWORD(wParam);
     HWND child = (HWND)lParam;
     return GetControl(id)->OnCommand(nNotifyCode, wParam, lParam);
   }
  
 
   bool CDialog::OnParentNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
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
  
 void PanelClosedNotifyProc(AIPanelRef fPanel)
{
	if (gEngine != NULL) {

		JNIEnv *env = gEngine->getEnv();
		try {
			AIPanelUserData uData;
		
      //HERE CALL Dialog_OnDestroy!!!!

			AIErr err = sAIPanel->GetUserData(fPanel, uData);

			jobject obj = (jobject) uData;
			gEngine->callOnDestroy(obj);
			// Clear the handle:
			gEngine->setIntField(env, obj,
					gEngine->fid_ui_NativeObject_handle, 0);
			env->DeleteGlobalRef(obj);

      //todod:common code with destroy
      	AIPanelPlatformWindow panelPlatfromWindow = NULL;
			err = sAIPanel->GetPlatformWindow(fPanel, panelPlatfromWindow);

			if(panelPlatfromWindow)
			{
				//RemovePropA(panelPlatfromWindow, "TPNL");
        DialogDataMap::iterator it = dialogDataMap.find(panelPlatfromWindow);
	      if (it != dialogDataMap.end()) {
          SetWindowLongPtr(panelPlatfromWindow, GWLP_WNDPROC, (LONG_PTR)(it->second.defaultProc));
          dialogDataMap.erase(it);
        }
      }

		} EXCEPTION_CATCH_REPORT(env);
	}
}


   CDialog::CDialog( AIPanelRef panel )
  {
    nextid = 33000;
    fPanel = panel;
    AIPanelPlatformWindow wnd;
    sAIPanel->GetPlatformWindow(fPanel, wnd);
    hWnd = (HWND)wnd;
  }

 

   CDialog * CDialog::CreateCDialog(ASUnicode * dlgName, int style, CControlInitProc Dialog_onInit, void * userData, int options)
   {
      	AIPanelRef fPanel;
        AIPanelFlyoutMenuRef fPanelFlyoutMenu;
        ASErr error = sAIPanelFlyoutMenu->Create(fPanelFlyoutMenu);
	     if (error)
	      	 throw new StringException("Unable to create dialog menu.");

        AISize pnSize = {240, 320};
        error = sAIPanel->Create(gPlugin->getPluginRef(), 
				ai::UnicodeString("kEmptyDialogID"), 
				ai::UnicodeString("qwe"), 3,
				pnSize,
				true, 
				 fPanelFlyoutMenu, //?
				userData, //user data
				fPanel);
	      if (error)
	      	 throw new StringException("Unable to create dialog panel.");

       CDialog * dlg = new CDialog(fPanel);
       dlg->SetWindowProc();
       dlg->SetInitProc(Dialog_onInit);
       dlg->OnCreate();

       error = sAIPanel->SetClosedNotifyProc(fPanel, PanelClosedNotifyProc);
    
       return dlg;

   }
   void CDialog::SetWindowProc()
   {
        oldWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hWnd, GWLP_WNDPROC,  reinterpret_cast<LONG_PTR>(StaticWndProc)));
        SetWindowLongPtr(hWnd,GWLP_USERDATA,(LONG_PTR)this);
  
        //mydebug
	      DialogData data = {fPanel, oldWndProc };
	      dialogDataMap[hWnd] = data;
 
   
   }

void CDialog::Destroy()
{
	if (gEngine != NULL) {

		JNIEnv *env = gEngine->getEnv();
  
    AIErr error = kNoErr;
		
    if(fPanel)
		{
      
      AIPanelUserData uData;
			
			AIErr err = sAIPanel->GetUserData(fPanel, uData);

			jobject obj = (jobject) uData;
			gEngine->callOnDestroy(obj);
			// Clear the handle:
			gEngine->setIntField(env, obj, gEngine->fid_ui_NativeObject_handle, 0);
			env->DeleteGlobalRef(obj);

      error = sAIPanel->SetClosedNotifyProc(fPanel, NULL);

       SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)(oldWndProc));
       SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)(0));

       DestroyWindow(hWnd);
	
			AIPanelFlyoutMenuRef fPanelFlyoutMenu;

			sAIPanel->GetFlyoutMenu(fPanel, fPanelFlyoutMenu);

			error = sAIPanel->Destroy(fPanel);
			fPanel = NULL;

			
			error = sAIPanelFlyoutMenu->Destroy(fPanelFlyoutMenu);
			fPanelFlyoutMenu = NULL;

		}

		//TODO call back call on destoy - from win proc notifier!?!
  }

}

   CCommonControl *  CreateControl(char * itemType, int controlID);

   CCommonControl * CDialog::CreateItem(char * itemType,  RECT* rect, CControlInitProc Item_onInit, void * userData)
  {
      int ctrlId = nextid++;
      CCommonControl * ctrl = CreateControl(itemType, ctrlId);
      ctrl->SetInitProc(Item_onInit);
      ctrl->SetUserData(userData);
      ctrlMap[ctrlId] = ctrl;

      ctrl->Create(hWnd, rect);

      return ctrl;
  }





  bool CCommonControl::Create(HWND hDlg, RECT* rect)
  {
    return false;
  }

  bool CCommonControl::OnCommand(int notifyCode, WPARAM wParam, LPARAM lParam)
  {
      return false;
  }





/*windows controls*/

  class ButtonControl : public CCommonControl
  {
    public:
        ButtonControl(int id):CCommonControl(id) {};
    
        virtual bool Create(HWND hDlg, RECT* rect)
        {
            
            	hWnd = CreateWindowEx( 
		            0,  
                "BUTTON",   // Predefined class; Unicode assumed. 
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
        return OnNotify(USER_CHANGED);
        return true;
     }
  };
   
  class CheckBoxControl : public CCommonControl
  {
    public:
        CheckBoxControl(int id):CCommonControl(id) {};
    
        virtual bool Create(HWND hDlg, RECT* rect)
        {
            
            	hWnd = CreateWindowEx( 
		            0,  
                "BUTTON",   // Predefined class; Unicode assumed. 
		            "no text here now",		//  
		            WS_VISIBLE | WS_CHILD | ES_LEFT   | BS_CHECKBOX,  // Styles. BS_OWNERDRAW?
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

     bool CheckBoxControl::OnCommand(int notifyCode, WPARAM wParam, LPARAM lParam)
     {
       /*switch (LOWORD(wParam)) {
    case 1:
        checked = IsDlgButtonChecked(hwnd, 1);
        if (checked) {
            CheckDlgButton(hwnd, 1, BST_UNCHECKED);
        } else {
            CheckDlgButton(hwnd, 1, BST_CHECKED);
    break;*/
        return OnNotify(USER_CHANGED);
        return true;
     }
  };
  
  class RadioButtonControl : public CCommonControl
  {
    public:
        RadioButtonControl(int id):CCommonControl(id) {};
    
        virtual bool Create(HWND hDlg, RECT* rect)
        {
            
            	hWnd = CreateWindowEx( 
		            0,  
                "BUTTON",   // Predefined class; Unicode assumed. 
		            "no text here now",		//  
		            WS_VISIBLE | WS_CHILD | ES_LEFT   | BS_RADIOBUTTON,  // Styles. BS_OWNERDRAW?
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

     bool RadioButtonControl::OnCommand(int notifyCode, WPARAM wParam, LPARAM lParam)
     {
       /*switch (LOWORD(wParam)) {
    case 1:
        checked = IsDlgButtonChecked(hwnd, 1);
        if (checked) {
            CheckDlgButton(hwnd, 1, BST_UNCHECKED);
        } else {
            CheckDlgButton(hwnd, 1, BST_CHECKED);
    break;*/
        return OnNotify(USER_CHANGED);
        return true;
     }
  };

  enum  EditFlags  {
        None     = 0,
        ReadOnly = 1,
        MultiLine =2,
        Password = 4,
    } ;

 class EditControl : public CCommonControl
  {
    DWORD editFlags;

    public:
        EditControl(int id, DWORD flags ):CCommonControl(id) 
          {editFlags = flags;};
    
      virtual bool Create(HWND hDlg, RECT* rect)
        {
             
            	hWnd = CreateWindowEx( 
		            0,  
                "EDIT",   // Predefined class; Unicode assumed. 
		            "no text here now",		//  
		            WS_VISIBLE | WS_CHILD | ES_LEFT | editFlags, 
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

 class StaticControl : public CCommonControl
  {
    DWORD editFlags;

    public:
        StaticControl(int id, DWORD flags ):CCommonControl(id) 
          {editFlags = flags;};
    
      virtual bool Create(HWND hDlg, RECT* rect)
        {
             
            	hWnd = CreateWindowEx( 
		            0,  "STATIC",   // Predefined class; Unicode assumed. 
		            "no text here now",		//  
		            WS_VISIBLE | WS_CHILD | SS_LEFT  | editFlags, 
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
CCommonControl *  CreateControl(char * itemType, int controlID)
{
    //push button
    if (strcmpi(itemType, TEXT_PUSHBUTTON) == 0)
      return new ButtonControl(controlID);
    
   //edit boxes 
    if (strcmpi(itemType, TEXT_EDIT)  == 0)
      return new EditControl(controlID, (int) EditFlags::None);
    if (strcmpi(itemType,  TEXT_EDIT_READONLY)  == 0)
      return new EditControl(controlID, ES_READONLY );
    if (strcmpi(itemType, TEXT_EDIT_MULTILINE)  == 0)
      return new EditControl(controlID,  ES_MULTILINE);
    if (strcmpi(itemType, TEXT_EDIT_MULTILINE_READONLY)  == 0)
      return new EditControl(controlID,  ES_MULTILINE | ES_READONLY);
    
    //static
    if (strcmpi(itemType, TEXT_STATIC)  == 0 ||  (strcmpi(itemType, TEXT_STATIC_MULTILINE)  == 0))
      return new StaticControl(controlID, (int) EditFlags::None);
    
    
     

     if (strcmpi(itemType, TEXT_CHECKBOX)  == 0)
      return new CheckBoxControl(controlID);
    
      if (strcmpi(itemType, PICTURE_CHECKBOX)  == 0)
      return new CheckBoxControl(controlID);

    return new CCommonControl(controlID);
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

//HWND CreateItem( char * itemType, HWND hDlg)
//{
//  if (strcmp(TEXT_PUSHBUTTON, itemType) == 0)
//    return CreateButton(hDlg);
//
//
//  return CreateCustomEdit( 
//
//		0, //L"EDIT",   // Predefined class; Unicode assumed. 
//		"",		//  
//		WS_VISIBLE | WS_CHILD | ES_RIGHT /*| ES_NUMBER*/ | WS_TABSTOP,  // Styles. 
//		0,         // x position. 
//		0,         // y position. 
//		100,        // width.
//		20,        // height.
//		hDlg,       // Parent window.
//		NULL, //(HMENU) ID_YEdit,
//		(HINSTANCE)GetWindowLongPtr(hDlg, GWLP_HINSTANCE), 
//		NULL);     
//}

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
