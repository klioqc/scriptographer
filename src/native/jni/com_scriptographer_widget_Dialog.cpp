#include "StdHeaders.h"
#include "ScriptographerEngine.h"
#include "ScriptographerPlugin.h"
#include "com_scriptographer_widget_Dialog.h"
#include <Uxtheme.h>

#include <AIMenuGroups.h> //temp


/*
 * com.scriptographer.widget.Dialog
 */
#define CHECK_ERROR  if (error) {return error;}

void PanelClosedNotifyProc(AIPanelRef fPanel)
{
	if (gEngine != NULL) {
		
		JNIEnv *env = gEngine->getEnv();
		try {
			AIPanelUserData uData;
			
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

#ifdef WIN_ENV

// Set up a data structure and a hash map that keeps track of dialog references
// and default window procs per hWnd of native dialog windows.
// This is needed on Windows < CS3 to capture dialog activation and internally
// fire the required events again.

#include <hash_map>

using namespace stdext;
//mydebug
//typedef struct {
//	AIPanelRef panel;
//	WNDPROC defaultProc;
//} DialogData;
//
//typedef hash_map<HWND, DialogData> DialogDataMap;
//end mydebug

DialogDataMap dialogDataMap;

LRESULT CALLBACK Dialog_windowProc(HWND hWnd, UINT uMsg,
		WPARAM wParam, LPARAM lParam) {
	DialogDataMap::iterator it = dialogDataMap.find(hWnd);
	if (it != dialogDataMap.end()) {
		// Consider this window activated if we either receive a WM_NCACTIVATE
		// message or if a WM_PARENTNOTIFY with a mouse message is received
		// (see ScriptographerPlugin::appWindowProc for more information).
		if (uMsg == WM_NCACTIVATE || uMsg == WM_PARENTNOTIFY && wParam > 0x200) {
			//jobject obj = gEngine->getDialogObject(it->second.panel);
			//TODOgEngine->callOnNotify(obj, kADMWindowActivateNotifier);
		}
		return ::CallWindowProc(it->second.defaultProc, hWnd, uMsg, wParam,
				lParam);
	}
	return 0;
}

#endif // WIN_ENV_INSTALL_WNDPROC


AIErr SetIcon(		AIPanelRef fPanel);
AIErr GetIcon(AIDataFilter* dataFilterIn, string* buffStrIn, size_t* lenIn);

/*
 * int nativeCreate(java.lang.String arg1, int arg2, int arg3)
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_widget_Dialog_nativeCreate(
  JNIEnv *env, jobject obj, jstring name, jint style, jint options)
{

	/*try {
		char *str = gEngine->convertString(env, name);
		DEFINE_CALLBACK_PROC(Dialog_onInit);
		ADMDialogRef dialog = sADMDialog->Create(gPlugin->getPluginRef(), str,
				kEmptyDialogID, (ADMDialogStyle) style,
				(ADMDialogInitProc) CALLBACK_PROC(Dialog_onInit),
				env->NewGlobalRef(obj), options);
		delete str;
		if (dialog == NULL)
			throw new StringException("Unable to create dialog.");
		return (jint) dialog;
	} EXCEPTION_CONVERT(env);
	return 0;*/

	ASErr error ; // sAIMenu->AddMenuItemZString(gPlugin->getPluginRef(), "123Third Party Panel", kOtherPalettesMenuGroup, ZREF("123Third Party Panel"),
					//					kMenuItemNoOptions, &fEmptyPanelPanelMenuItemHandle);

	try {

		AIPanelRef fPanel;

		AIPanelFlyoutMenuRef fPanelFlyoutMenu;


		//todo:
		AISize pnSize = {240, 320};
	
			char *str = gEngine->convertString(env, name);
				/** Creates a new panel.
	@param inPluginRef		The plug-in that is creating the panel.
	@param inID				A unique identifier for the new panel.
	@param inTitle			The title of the panel.
	@param inStateCount 	The number of host layouts for the panel; must be at least 1.
	@param inMinSize		Minimum size of the panel.
	@param isResizable		True to make the panel resizable.
	@param inFlyoutMenu		The flyout menu for the panel, or NULL to hide the flyout icon.
	@param inUserData		Developer-defined data to be associated with the panel.
	@param outPanel			[out] A buffer in which to return the new panel object.
 	*/

      	error = sAIMenu->AddMenuItemZString(gPlugin->getPluginRef(), "A my test Panel", kOtherPalettesMenuGroup, ZREF("A Third Party Panel"),
										kMenuItemNoOptions, &fEmptyPanelPanelMenuItemHandle);
	if (error)
		return error;

  fPanelFlyoutMenu = NULL;
	error = sAIPanelFlyoutMenu->Create(fPanelFlyoutMenu);
	if (error)
		return error;

	//error = sAIPanelFlyoutMenu->AppendItem(fPanelFlyoutMenu, 1, ai::UnicodeString("First Item"));
 //   CHECK_ERROR
	//error = sAIPanelFlyoutMenu->AppendItem(fPanelFlyoutMenu, 3, ai::UnicodeString("Third Item"));
 //   CHECK_ERROR
	//error = sAIPanelFlyoutMenu->InsertItem(fPanelFlyoutMenu, 3, 2, ai::UnicodeString("Second Item"));
 //   CHECK_ERROR
	//error = sAIPanelFlyoutMenu->InsertSeparator(fPanelFlyoutMenu, 3, 5);
 //   CHECK_ERROR
	//error = sAIPanelFlyoutMenu->AppendItem(fPanelFlyoutMenu, 4, ai::UnicodeString("Fourth Item"));
 //   CHECK_ERROR
 //   
	//error = sAIPanelFlyoutMenu->SetItemEnabled(fPanelFlyoutMenu, 4, false);
 //   CHECK_ERROR
	//error = sAIPanelFlyoutMenu->SetItemMark(fPanelFlyoutMenu, 1 , kAIPanelFlyoutMenuItemMark_BULLET);
 //   CHECK_ERROR
	//error = sAIPanelFlyoutMenu->SetItemMark(fPanelFlyoutMenu, 2 , kAIPanelFlyoutMenuItemMark_CHECK);
 //   CHECK_ERROR
	//error = sAIPanelFlyoutMenu->SetItemMark(fPanelFlyoutMenu, 3 , kAIPanelFlyoutMenuItemMark_DASH);
	//CHECK_ERROR





			error = sAIPanel->Create(gPlugin->getPluginRef(), 
				ai::UnicodeString("kEmptyDialogID"), 
				ai::UnicodeString("default"), 3,
				pnSize,
				true, 
				 fPanelFlyoutMenu, 
				
				env->NewGlobalRef(obj), //user data
				
				fPanel);
	
		delete str;

	if (error)
			throw new StringException("Unable to create dialog.");
  	error = sAIPanel->Show(fPanel, true);

	AISize minSize = {50, 50};
	AISize maxSize = {800, 800};
	AISize prefConstSize = {100, 100};
	AISize prefUnconstSize = {600, 600};

	error = sAIPanel->SetSizes(fPanel, minSize, prefUnconstSize, prefConstSize, maxSize);
	
	
	//AIPanelPlatformWindow hDlg = NULL;
	//error = sAIPanel->GetPlatformWindow(fPanel, hDlg);

	//if (error)
	//	return error;

	error = sAIPanel->SetClosedNotifyProc(fPanel, PanelClosedNotifyProc);

	//::SetPropA(hDlg, "TPNL", this);
	//fDefaultWindProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hDlg, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(EmptyPanelPlugin::NewWindowProc)));

#ifdef WIN_ENV
	AIPanelPlatformWindow hDlg = NULL;
	error = sAIPanel->GetPlatformWindow(fPanel, hDlg);

	HWND hWnd = (HWND) hDlg;
	/*WNDPROC defaultProc = (WNDPROC) ::SetWindowLong(hWnd, GWLP_WNDPROC,
			(LONG) Dialog_windowProc);
*/
	WNDPROC defaultProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(hDlg, GWLP_WNDPROC,
		reinterpret_cast<LONG_PTR>(Dialog_windowProc)));

	DialogData data = { fPanel, defaultProc };
	dialogDataMap[hWnd] = data;

#endif // WIN_ENV

  //error = SetIcon(fPanel);
    

	return (jint) fPanel;
	} EXCEPTION_CONVERT(env);
	return 0;
}


#define bufMax 1024



AIErr SetIcon(		AIPanelRef fPanel)
{
	AIErr error = kNoErr;
	AIDataFilterSuite* sAIDataFilter = NULL;
	error = sSPBasic->AcquireSuite(kAIDataFilterSuite, kAIDataFilterVersion, (const void **)&sAIDataFilter);
	if (sAIDataFilter)
	{
		AIDataFilter *dataFilter(NULL);
		error = sAIDataFilter->NewResourceDataFilter(gPlugin->getPluginRef(), 'Icon', 0, "kEmptyIconID", &dataFilter);	
        AIDataFilter *dataFilterHiDPI(NULL);
        error = sAIDataFilter->NewResourceDataFilter(gPlugin->getPluginRef(), 'Icon', 0, "kEmptyIconID", &dataFilterHiDPI);

		if (error == kNoErr && dataFilter)
		{
            
			error = sAIDataFilter->LinkDataFilter(NULL, dataFilter);
            error = sAIDataFilter->LinkDataFilter(NULL, dataFilterHiDPI);
			
			size_t len = 0;
			string buffStr;
            GetIcon(dataFilter, &buffStr, &len);

            string buffStrHIDPI;
            size_t lenHIDPI = 0;
            GetIcon(dataFilterHiDPI, &buffStrHIDPI, &lenHIDPI);
            
            ai::AutoBuffer<const ai::uint8*> inPNGData(2);
			inPNGData[(size_t)0] = (ai::uint8*)buffStr.c_str();//regular
			inPNGData[(size_t)1] = (ai::uint8*)buffStrHIDPI.c_str();//hidpi
			ai::AutoBuffer<ai::uint32> size(2);
			size[(size_t)0] = len;//regular
			size[(size_t)1] = lenHIDPI;//hidpi

			//                          light			dark
			sAIPanel->SetIcon(fPanel, inPNGData, size, inPNGData, size);
		}

		error = sAIDataFilter->UnlinkDataFilter(dataFilter, NULL);
		error = sAIDataFilter->UnlinkDataFilter(dataFilterHiDPI, NULL);
		sSPBasic->ReleaseSuite(kAIDataFilterSuite, kAIDataFilterVersion);
	}
	return error;
}

AIErr GetIcon(AIDataFilter* dataFilterIn, string* buffStrIn, size_t* lenIn)
{
    AIErr error = kNoErr;
    AIDataFilterSuite* sAIDataFilter = NULL;
	error = sSPBasic->AcquireSuite(kAIDataFilterSuite, kAIDataFilterVersion, (const void **)&sAIDataFilter);

    error = sAIDataFilter->LinkDataFilter(NULL, dataFilterIn);
    
    char buf[bufMax];
    
    for(;;)
    {
        size_t count = bufMax;
        const ASErr error = sAIDataFilter->ReadDataFilter(dataFilterIn, buf, &count);
        if (error)
        {
            return error;
        }
        *lenIn += count;
        buffStrIn->append(buf, count);
        if (count != bufMax)
        {
            break;
        }
    }
}

#ifdef test
LRESULT CALLBACK EmptyPanelPlugin::NewWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	EmptyPanelPlugin* panel = (EmptyPanelPlugin*)GetPropA(hWnd, "TPNL");
	LRESULT result = 0;
	bool handled = panel->PanelWindowProc(result, hWnd, msg, wParam, lParam);
	if(!handled)
		result =  panel->CallDefaultWindowProc(hWnd, msg, wParam, lParam);
	return result;
}

bool EmptyPanelPlugin::PanelWindowProc(LRESULT& result, HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	bool handled = false;
	switch (msg) 
	{
	case WM_COMMAND: 
		{
			if(wParam == ID_ButtonAdd)
			{
				TCHAR XStr[255] ;
				SendMessage(fHWndXEdit, (UINT) WM_GETTEXT, 255, (LPARAM)XStr); 

				TCHAR YStr[255];
				SendMessage(fHwndYEdit, (UINT) WM_GETTEXT, 255, (LPARAM)YStr); 
				
				ai::NumberFormat numFormat;
				ASInt32 x = 0;
				sAIStringFormatUtils->StringToInteger(numFormat, ai::UnicodeString(XStr), x);
				ASInt32 y = 0;
				sAIStringFormatUtils->StringToInteger(numFormat, ai::UnicodeString(YStr), y);

				ASInt32 result = x + y;
				ai::UnicodeString strResult;
				sAIStringFormatUtils->IntegerToString(numFormat, result, strResult);
				SendMessage(fHwndResultEdit, (UINT) WM_SETTEXT, 0, (LPARAM)strResult.as_ASUnicode().c_str());

				handled = true;
			}
			else if(wParam == ID_ButtonShow)
			{
				sAIUser->MessageAlert(ai::UnicodeString("This is a Dialog"));
				handled = true;
			}
		}
	}
	return handled;
}

LRESULT CALLBACK EmptyPanelPlugin::CallDefaultWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(fDefaultWindProc)
		return ::CallWindowProc(fDefaultWindProc, hWnd, msg, wParam, lParam);
	else
	{
		return 0; 
	}
}

#endif

/*
 * void nativeDestroy(int arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_nativeDestroy(
		JNIEnv *env, jobject obj, jint arg1) {
	try {
	
		AIPanelRef fPanel = gEngine->getAIPanelRef(env, obj);
		AIErr error = kNoErr;
		if(fPanel)
		{
      
      AIPanelUserData uData;
			
			AIErr err = sAIPanel->GetUserData(fPanel, uData);

			jobject obj = (jobject) uData;
			gEngine->callOnDestroy(obj);
			// Clear the handle:
			gEngine->setIntField(env, obj,
					gEngine->fid_ui_NativeObject_handle, 0);
			env->DeleteGlobalRef(obj);

      error = sAIPanel->SetClosedNotifyProc(fPanel, NULL);

    #ifdef WIN_ENV
			
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
        DestroyWindow(panelPlatfromWindow);
			}
		#endif
/*
			result = sAIPanel->Destroy(fPanel);
			fPanel = NULL;
		*/


    
			AIPanelFlyoutMenuRef fPanelFlyoutMenu;

			sAIPanel->GetFlyoutMenu(fPanel, fPanelFlyoutMenu);

			error = sAIPanel->Destroy(fPanel);
			fPanel = NULL;

			
			error = sAIPanelFlyoutMenu->Destroy(fPanelFlyoutMenu);
			fPanelFlyoutMenu = NULL;

		}

		//TODO call back call on destoy - from win proc notifier!?!

		
    JNIEnv *env = gEngine->getEnv();

      
	
	} EXCEPTION_CONVERT(env);
}



/*
 * boolean nativeIsVisible()
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_widget_Dialog_nativeIsVisible(
  JNIEnv * env, jobject obj)
{
try {
		AIPanelRef fPanel = gEngine->getAIPanelRef(env, obj);
		
		AIPanelPlatformWindow hDlg = NULL;
		AIBoolean bVis;
		AIErr error = sAIPanel->IsShown (fPanel, bVis);
		if (error)
			return false;
		return bVis;

	} EXCEPTION_CONVERT(env);
	return false;
}

/*
 * void nativeSetVisible(boolean arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_nativeSetVisible(
  JNIEnv * env, jobject obj, jboolean isVisible)
{
	try {
		AIPanelRef fPanel = gEngine->getAIPanelRef(env, obj);
		
		AIPanelPlatformWindow hDlg = NULL;
		AIBoolean bVis;
    AIErr error;
		error = sAIPanel->Show(fPanel,isVisible == 1);
	} EXCEPTION_CONVERT(env);
	
}

/*
 * void nativeSetActive(boolean arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_nativeSetActive(
		JNIEnv *env, jobject obj, jboolean arg1) {
	try {
		// TODO: define nativeSetActive
	} EXCEPTION_CONVERT(env);
}

/*
 * com.scriptographer.ui.Size nativeGetSize()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_widget_Dialog_nativeGetSize(
  JNIEnv *env, jobject obj) {
	
	  try {
		AIPanelRef fPanel = gEngine->getAIPanelRef(env, obj);
		
		AIPanelPlatformWindow hDlg = NULL;
		AISize size;
		AIErr error = sAIPanel->GetSize(fPanel, size);
		return gEngine->convertSize(env, &size);
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * void nativeSetSize(int arg1, int arg2)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_nativeSetSize(
		JNIEnv *env, jobject obj, jint arg1, jint arg2) {
	try {
		// TODO: define nativeSetSize
	} EXCEPTION_CONVERT(env);
}

/*
 * com.scriptographer.ui.Rectangle nativeGetBounds()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_widget_Dialog_nativeGetBounds(
  JNIEnv * env, jobject obj)
{
	try {
		AIPanelRef fPanel = gEngine->getAIPanelRef(env, obj);
		
		AIPanelPlatformWindow hDlg = NULL;
		AIErr error = sAIPanel->GetPlatformWindow(fPanel, hDlg);
		//win:
#ifdef WIN_ENV
			RECT rc;
			if (!GetWindowRect(hDlg, &rc))
				throw new StringException("Error calling GetWindowRect.");
			return gEngine->convertRectangle(env, &rc);
#endif
		//todo MAC

	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * void nativeSetBounds(int arg1, int arg2, int arg3, int arg4)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_nativeSetBounds(
		JNIEnv *env, jobject obj, jint x, jint y, jint width, jint height) {
	try {
      	AIPanelRef fPanel = gEngine->getAIPanelRef(env, obj);
	    //  AISize size = new AISize(x, y, width, height);

        
        //sAIPanel->S(fPanel, .SetMinimumSize();


#ifdef WIN_ENV
  

#endif
  
  
  } EXCEPTION_CONVERT(env);
}

/*
 * com.scriptographer.ui.Point getPosition()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_widget_Dialog_getPosition(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define getPosition
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * void setPosition(int arg1, int arg2)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_setPosition(
		JNIEnv *env, jobject obj, jint arg1, jint arg2) {
	try {
		// TODO: define setPosition
	} EXCEPTION_CONVERT(env);
}

/*
 * void invalidate()
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_invalidate__(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define invalidate
	} EXCEPTION_CONVERT(env);
}

/*
 * void invalidate(int arg1, int arg2, int arg3, int arg4)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_invalidate__IIII(
		JNIEnv *env, jobject obj, jint arg1, jint arg2, jint arg3, jint arg4) {
	try {
			//todo: adm ADMFont enum implementation?
	} EXCEPTION_CONVERT(env);
}

/*
 * void update()
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_update(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define update
	} EXCEPTION_CONVERT(env);
}

/*
 * int nativeGetFont()
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_widget_Dialog_nativeGetFont(
		JNIEnv *env, jobject obj) {
	try {
		//todo: adm ADMFont enum implementation?
		return 0;
	} EXCEPTION_CONVERT(env);
	return 0;
}

/*
 * void nativeSetFont(int arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_nativeSetFont(
		JNIEnv *env, jobject obj, jint arg1) {
	try {
		// TODO: define nativeSetFont
	} EXCEPTION_CONVERT(env);
}

/*
 * void nativeSetName(java.lang.String arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_nativeSetName(
		JNIEnv *env, jobject obj, jstring arg1) {
	try {
		// TODO: define nativeSetName
	} EXCEPTION_CONVERT(env);
}

/*
 * void nativeSetMinimumSize(int arg1, int arg2)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_nativeSetMinimumSize(
		JNIEnv *env, jobject obj, jint arg1, jint arg2) {
	try {
		// TODO: define nativeSetMinimumSize
	} EXCEPTION_CONVERT(env);
}

/*
 * void nativeSetMaximumSize(int arg1, int arg2)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_nativeSetMaximumSize(
		JNIEnv *env, jobject obj, jint arg1, jint arg2) {
	try {
		// TODO: define nativeSetMaximumSize
	} EXCEPTION_CONVERT(env);
}

/*
 * boolean isEnabled()
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_widget_Dialog_isEnabled(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define isEnabled
	} EXCEPTION_CONVERT(env);
	return false;
}

/*
 * void setEnabled(boolean arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_setEnabled(
		JNIEnv *env, jobject obj, jboolean arg1) {
	try {
		// TODO: define setEnabled
	} EXCEPTION_CONVERT(env);
}

/*
 * void nativeSetTitle(java.lang.String arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_nativeSetTitle(
		JNIEnv *env, jobject obj, jstring title) {
	try {
	
    AIPanelRef fPanel = gEngine->getAIPanelRef(env, obj);
	 
		if (title != NULL) {
		  	char *chars = gEngine->convertString(env, title);
        ai::UnicodeString str =  ai::UnicodeString(chars); 

			  sAIPanel->SetTitle(fPanel, str);
			delete chars;
		} else {
			  sAIPanel->SetTitle(fPanel, ai::UnicodeString(""));
		}

	} EXCEPTION_CONVERT(env);
}