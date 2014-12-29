#include "StdHeaders.h"
#include "ScriptographerEngine.h"
#include "ScriptographerPlugin.h"
#include "com_scriptographer_widget_Dialog.h"
#include <Uxtheme.h>
#include "uiGlobals.h"
#include <AIMenuGroups.h> //temp

//my debug
DialogDataMap dialogDataMap;
//end


/*
 * com.scriptographer.widget.Dialog
 */
#define CHECK_ERROR  if (error) {return error;}

void ASAPI Dialog_onDestroy(CControl * ctrl) {
	if (gEngine != NULL) {
		JNIEnv *env = gEngine->getEnv();
		try {
      CDialog* dlg = reinterpret_cast<CDialog*> (ctrl);
			jobject obj = gEngine->getDialogObject(dlg);
			gEngine->callOnDestroy(obj);
			// Clear the handle:
			gEngine->setIntField(env, obj,
					gEngine->fid_ui_NativeObject_handle, 0);
			env->DeleteGlobalRef(obj);
		} EXCEPTION_CATCH_REPORT(env);
	}
}


void ASAPI Dialog_onNotify(CControl * ctrl, char * notifier) {

   CDialog* dlg = reinterpret_cast<CDialog*> (ctrl);


	dlg->DefaultNotify(notifier);
	if (gEngine != NULL) {
		jobject obj = gEngine->getDialogObject(dlg);
		gEngine->callOnNotify(obj, notifier);
	}
}

ASBoolean ASAPI Dialog_onTrack(CControl * ctrl, int tracker) {
  CDialog* dlg = reinterpret_cast<CDialog*> (ctrl);

	jobject obj = gEngine->getDialogObject(dlg);
	ASBoolean ret = gEngine->callOnTrack(obj, tracker);
	if (ret)
		ret = dlg->DefaultTrack(tracker);
	return ret;
}
 
void ASAPI Dialog_onDraw(CControl * ctrl, int drawer) {
   CDialog* dlg = reinterpret_cast<CDialog*> (ctrl);

	jobject obj = gEngine->getDialogObject(dlg);
	ASBoolean ret = gEngine->callOnDraw(obj, drawer);
	//if (ret)
	//	dlg->DefaultDraw(dialog, drawer);
}

ASErr ASAPI Dialog_onInit(CControl * ctrl) {

  CDialog* dlg = reinterpret_cast<CDialog*> (ctrl);
	// Hide the dialog by default:
  AIPanelRef panel = dlg->Panel();
	sAIPanel->Show(panel, false);
	
	// Attach the dialog-level callbacks
	DEFINE_CALLBACK_PROC(Dialog_onDestroy);
	dlg->SetDestroyProc(Dialog_onDestroy);
	
	DEFINE_CALLBACK_PROC(Dialog_onNotify);
	dlg->SetNotifyProc( Dialog_onNotify);
	
	// Resize handler:
	/*ADMItemRef resizeItemRef = sADMDialog->GetItem(dialog, kADMResizeItemID);
	if (resizeItemRef) {
		DEFINE_CALLBACK_PROC(Dialog_onSizeChanged);
		sADMItem->SetNotifyProc(resizeItemRef,
				(ADMItemNotifyProc) CALLBACK_PROC(Dialog_onSizeChanged));
	}*/
	
	// Execute a one-shot timer right after creation of the dialog,
	// to run initialize()
	// Call onNotify with kADMInitializeNotifier
	JNIEnv *env = gEngine->getEnv();
	try {
		jobject obj = gEngine->getDialogObject(dlg);
		gEngine->callOnNotify(obj, kUIInitializeNotifier);
	} EXCEPTION_CATCH_REPORT(env);
  
	return kNoErr;
}



/*
 * int nativeCreate(java.lang.String arg1, int arg2, int arg3)
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_widget_Dialog_nativeCreate(
  JNIEnv *env, jobject obj, jstring name, jint style, jint options)
{
  try {
     char *chars = gEngine->convertString(env, name);
     ai::UnicodeString str =  ai::UnicodeString(chars); 

    CDialog * dialog = CDialog::CreateCDialog(str, style, Dialog_onInit, env->NewGlobalRef(obj), options);
		delete chars;

    //mydebug
    sAIMenu->AddMenuItemZString(gPlugin->getPluginRef(), "A my test Panel", kOtherPalettesMenuGroup, ZREF("A Third Party Panel"),
										kMenuItemNoOptions, &fEmptyPanelPanelMenuItemHandle);


    if (dialog == NULL)
			throw new StringException("Unable to create dialog.");

		return (jint) dialog;
	} EXCEPTION_CONVERT(env);
	return 0;

}


#ifdef test
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
	
	
  Dialog_onInit(fPanel);

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
#endif

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
	
		CDialog * dlg = gEngine->getDialog(env, obj);
    if (dlg)
    {
      dlg->Destroy();
    }


      
	
	} EXCEPTION_CONVERT(env);
}

#ifdef likeadm
/*
 * void nativeSetTrackCallback(boolean enabled)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_Dialog_nativeSetTrackCallback(
		JNIEnv *env, jobject obj, jboolean enabled) {
	try {
		/*ADMDialogRef dialog = gEngine->getDialogHandle(env, obj);
		DEFINE_CALLBACK_PROC(Dialog_onTrack);
		sADMDialog->SetTrackProc(dialog, enabled
				? (ADMDialogTrackProc) CALLBACK_PROC(Dialog_onTrack) : NULL);*/
     gPlugin->log("nativeSetTrackCallback");
	} EXCEPTION_CONVERT(env);
}

/*
 * boolean defaultTrack(com.scriptographer.adm.Tracker tracker)
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_adm_Dialog_defaultTrack(
		JNIEnv *env, jobject obj, jobject tracker) {
	try {
	/*	ADMDialogRef dialog = gEngine->getDialogHandle(env, obj);
		return sADMDialog->DefaultTrack(dialog,
				gEngine->getTrackerHandle(env, tracker));*/
    gPlugin->log("defaultTrack");
	} EXCEPTION_CONVERT(env);
	return false;
}
#endif
/*
 * boolean nativeIsVisible()
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_widget_Dialog_nativeIsVisible(
  JNIEnv * env, jobject obj)
{
try {
		CDialog * dlg = gEngine->getDialog(env, obj);
		if (dlg)
    {
		  AIBoolean bVis;
		  AIErr error = sAIPanel->IsShown (dlg->Panel(), bVis);
		  if (error)
			  return false;
		  return bVis;
    }
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
		CDialog * dlg = gEngine->getDialog(env, obj);
		if (dlg)
    {
		  AIBoolean bVis;
      AIErr error;
		  error = sAIPanel->Show(dlg->Panel(), isVisible);
    }
	} EXCEPTION_CONVERT(env);
	
}

/*
 * void nativeSetActive(boolean arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_nativeSetActive(
		JNIEnv *env, jobject obj, jboolean arg1) {
	try {
		// TODO: define nativeSetActive
    gPlugin->log("nativeSetActive");
	} EXCEPTION_CONVERT(env);
}

/*
 * com.scriptographer.ui.Size nativeGetSize()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_widget_Dialog_nativeGetSize(
  JNIEnv *env, jobject obj) {
	
	  try {
		CDialog * dlg = gEngine->getDialog(env, obj);
		if (dlg)
    {
		  AISize size;
		  AIErr error = sAIPanel->GetSize(dlg->Panel(), size);

      // gPlugin->log("nativeGetSize %f %f", size.width, size.height);
       //return gEngine->convertUISize(env, size.width, size.height);
      RECT rc;
      dlg->GetClientRect(&rc);
		  return gEngine->convertUISize(env,    rc.right -rc.left, rc.bottom - rc.top);
    }
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * void nativeSetSize(int arg1, int arg2)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_nativeSetSize(
  JNIEnv *env, jobject obj, jint width, jint height) {
	try {

    CDialog * dlg = gEngine->getDialog(env, obj);
		if (dlg)
    {
      AISize size;
      size.width = width;
      size.height = height;
		  AIErr error = sAIPanel->SetSize(dlg->Panel(), size);
    }
	} EXCEPTION_CONVERT(env);
}

/*
 * com.scriptographer.ui.Rectangle nativeGetBounds()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_widget_Dialog_nativeGetBounds(
  JNIEnv * env, jobject obj)
{
	try {
		CDialog * dlg = gEngine->getDialog(env, obj);
		if (dlg)
    {
      RECT rc;
      dlg->GetBounds(&rc);

      gPlugin->log("nativeGetBounds %d %d %d %d", rc.left, rc.top, rc.right, rc.bottom);

			return gEngine->convertRectangle(env, &rc);
    }
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * void nativeSetBounds(int arg1, int arg2, int arg3, int arg4)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_nativeSetBounds(
		JNIEnv *env, jobject obj, jint x, jint y, jint width, jint height) {
	try {
		CDialog * dlg = gEngine->getDialog(env, obj);
		if (dlg)
    {
      DEFINE_UI_RECT(rc, x, y, width, height);
        gPlugin->log("dlg nativeSetBounds %d %d %d %d",rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
      dlg->SetBounds(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
    }
  
  } EXCEPTION_CONVERT(env);
}

/*
 * com.scriptographer.ui.Point getPosition()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_widget_Dialog_getPosition(
		JNIEnv *env, jobject obj) {
	try {
	  CDialog * dlg = gEngine->getDialog(env, obj);
		if (dlg)
    {
		 RECT rc;
      dlg->GetBounds(&rc);
      POINT pt;
      pt.x = rc.left;
      pt.y = rc.top;
      return gEngine->convertPoint(env, &pt);

    }
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * void setPosition(int arg1, int arg2)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_setPosition(
		JNIEnv *env, jobject obj, jint x, jint y) {
	try {

		  CDialog * dlg = gEngine->getDialog(env, obj);
		  if (dlg)
      {
		    AIPoint point; 
        point.h = x < 0 ? 0 : x; 
        point.v = y < 0 ? 0 : y; ;
		    AIErr error = sAIPanel->Move(dlg->Panel(), point);
		   
      }
	} EXCEPTION_CONVERT(env);
}

/*
 * void invalidate()
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_invalidate__(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define invalidate
    gPlugin->log("invalidate");
	} EXCEPTION_CONVERT(env);
}

/*
 * void invalidate(int arg1, int arg2, int arg3, int arg4)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_invalidate__IIII(
		JNIEnv *env, jobject obj, jint arg1, jint arg2, jint arg3, jint arg4) {
	try {
			//todo: adm ADMFont enum implementation?
    gPlugin->log("invalidate");
	} EXCEPTION_CONVERT(env);
}

/*
 * void update()
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_update(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define update
    gPlugin->log("update");
	} EXCEPTION_CONVERT(env);
}

/*
 * int nativeGetFont()
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_widget_Dialog_nativeGetFont(
		JNIEnv *env, jobject obj) {
	try {
		//todo: adm ADMFont enum implementation?
    //call SystemParametersInfo(SPI_GETNONCLIENTMETRICS,sizeof(NONCLIENTMETRICS), &ncm, 0);
    //and compare ting with dialog deafultfont?
    //for now always return normal font = 0
    gPlugin->log("nativeGetFont");
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
    gPlugin->log("nativeSetFont %d", arg1);
	} EXCEPTION_CONVERT(env);
}

/*
 * void nativeSetName(java.lang.String arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_nativeSetName(
		JNIEnv *env, jobject obj, jstring arg1) {
	try {
		// TODO: define nativeSetName
    gPlugin->log("nativeSetName");
	} EXCEPTION_CONVERT(env);
}

/*
 * void nativeSetMinimumSize(int arg1, int arg2)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_nativeSetMinimumSize(
		JNIEnv *env, jobject obj, jint width, jint height) {
	try {
	CDialog * dlg = gEngine->getDialog(env, obj);
		if (dlg)
    {
      AISize size;
      size.width = width;
      size.height = height;
		  AIErr error = sAIPanel->SetMinimumSize(dlg->Panel(), size);
    }
	} EXCEPTION_CONVERT(env);
}

/*
 * void nativeSetMaximumSize(int arg1, int arg2)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_nativeSetMaximumSize(
		JNIEnv *env, jobject obj, jint width, jint height) {
	try {
  CDialog * dlg = gEngine->getDialog(env, obj);
		if (dlg)
    {
      AISize size;
      size.width = width;
      size.height = height;
		  AIErr error = sAIPanel->SetMaximumSize(dlg->Panel(), size);
    }
	} EXCEPTION_CONVERT(env);
}

/*
 * boolean isEnabled()
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_widget_Dialog_isEnabled(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define isEnabled
    gPlugin->log("isEnabled");
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
    gPlugin->log("setEnabled");
	} EXCEPTION_CONVERT(env);
}

/*
 * void nativeSetTitle(java.lang.String arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_nativeSetTitle(
		JNIEnv *env, jobject obj, jstring title) {
	try {
	
    CDialog * dlg = gEngine->getDialog(env, obj);
		if (dlg)
    {
	 		  if (title != NULL) {
		  	  char *chars = gEngine->convertString(env, title);
          ai::UnicodeString str =  ai::UnicodeString(chars); 

			    sAIPanel->SetTitle(dlg->Panel(), str);
			  delete chars;
		  } else {
			    sAIPanel->SetTitle(dlg->Panel(), ai::UnicodeString(""));
		  }
    }
	} EXCEPTION_CONVERT(env);
}

/*
 * boolean defaultTrack(com.scriptographer.widget.Tracker arg1)
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_widget_Dialog_defaultTrack(
		JNIEnv *env, jobject obj, jobject arg1) {
	try {
		// TODO: define defaultTrack
     gPlugin->log("defaultTrack");
	} EXCEPTION_CONVERT(env);
	return false;
}

/*
 * int getItemHandle(int arg1)
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_widget_Dialog_getItemHandle(
		JNIEnv *env, jobject obj, jint arg1) {
	try {
		// TODO: define getItemHandle
     gPlugin->log("getItemHandle");
	} EXCEPTION_CONVERT(env);
	return 0;
}

/*
 * com.scriptographer.widget.Item getDefaultItem()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_widget_Dialog_getDefaultItem(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define getDefaultItem
     gPlugin->log("getDefaultItem");
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * void setDefaultItem(com.scriptographer.widget.Item arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_setDefaultItem(
		JNIEnv *env, jobject obj, jobject arg1) {
	try {
		// TODO: define setDefaultItem
    gPlugin->log("setDefaultItem");
	} EXCEPTION_CONVERT(env);
}

/*
 * com.scriptographer.widget.Item getCancelItem()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_widget_Dialog_getCancelItem(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define getCancelItem
    gPlugin->log("getCancelItem");
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * void setCancelItem(com.scriptographer.widget.Item arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Dialog_setCancelItem(
		JNIEnv *env, jobject obj, jobject arg1) {
	try {
		// TODO: define setCancelItem
    gPlugin->log("setCancelItem");
	} EXCEPTION_CONVERT(env);
}

/*
 * java.io.File nativeFileDialog(java.lang.String arg1, java.lang.String arg2, java.io.File arg3, java.lang.String arg4, boolean arg5)
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_widget_Dialog_nativeFileDialog(
		JNIEnv *env, jclass cls, jstring arg1, jstring arg2, jobject arg3, jstring arg4, jboolean arg5) {
	try {
		// TODO: define nativeFileDialog
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * java.io.File chooseDirectory(java.lang.String arg1, java.io.File arg2)
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_widget_Dialog_chooseDirectory(
		JNIEnv *env, jclass cls, jstring arg1, jobject arg2) {
	try {
		// TODO: define chooseDirectory
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * com.scriptographer.ai.Color chooseColor(com.scriptographer.ai.Color arg1)
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_widget_Dialog_chooseColor(
		JNIEnv *env, jclass cls, jobject arg1) {
	try {
		// TODO: define chooseColor
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * com.scriptographer.ui.Rectangle getPaletteLayoutBounds()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_widget_Dialog_getPaletteLayoutBounds(
		JNIEnv *env, jclass cls) {
	try {
		// TODO: define getPaletteLayoutBounds
     gPlugin->log("getPaletteLayoutBounds");
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * com.scriptographer.ui.Size getScreenSize()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_widget_Dialog_getScreenSize(
		JNIEnv *env, jclass cls) {
	try {
	
#ifdef WIN_ENV
		RECT rect;
		GetWindowRect(GetDesktopWindow(), &rect);
#endif // WIN_ENV
		return gEngine->convertUISize(env, rect.right - rect.left, rect.bottom - rect.top);
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * com.scriptographer.ui.Size nativeGetTextSize(java.lang.String arg1)
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_widget_Dialog_nativeGetTextSize(
		JNIEnv *env, jobject obj, jstring text) {
	try {
		  CDialog * dlg = gEngine->getDialog(env, obj);
		if (dlg)
    {
    /*   char *chars = gEngine->convertString(env, text);
       SIZE size = dlg->GetTextSize(chars);
			  delete chars;
        return gEngine->convertUISize(env, size.cx, size.cy);
      */
       ai::UnicodeString str = gEngine->convertString_UnicodeString(env, text); 
       SIZE size = dlg->GetTextSize((LPWSTR)str.as_ASUnicode().c_str());
			
        gPlugin->log("GetTextSize   %d %d",   size.cx, size.cy);
      //delete chars;
        return gEngine->convertUISize(env, size.cx, size.cy);
    }

	} EXCEPTION_CONVERT(env);
	return NULL;
}
