#include "StdHeaders.h"
#include "ScriptographerEngine.h"
#include "com_scriptographer_widget_Item.h"

/*
 * com.scriptographer.widget.Item
 */

/*
 * int nativeCreate(com.scriptographer.widget.Dialog arg1, java.lang.String arg2, int arg3)
 */


//mydebug

/* Initialize static variables */
	static WNDPROC defWndProc = NULL;
	static bool	registered = false;


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



HWND CreateItem( char * itemType, HWND hDlg)
{
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












JNIEXPORT jint JNICALL Java_com_scriptographer_widget_Item_nativeCreate(
		JNIEnv *env, jobject obj, jobject dialogObj, jstring type, jint options) {
	try {
	  AIPanelRef fPanel = gEngine->getAIPanelRef(env, dialogObj);

		char *itemType = gEngine->convertString(env, type);
		// create with default dimensions:
		/*DEFINE_ADM_RECT(rect, 0, 0, 100, 100);
		DEFINE_CALLBACK_PROC(Item_onInit);
		ADMItemRef item = sADMItem->Create(dialog, kADMUniqueItemID, itemType, &rect, 
      (ADMItemInitProc) CALLBACK_PROC(Item_onInit), env->NewGlobalRef(obj), options);*/
    
	  AIPanelPlatformWindow hDlg = NULL;
	  sAIPanel->GetPlatformWindow(fPanel, hDlg);

#ifdef WIN_ENV
 
    HWND item = CreateItem(itemType, hDlg);

#endif

 		delete itemType;

    if (item == NULL)
			throw new StringException("Unable to create dialog item.");



		return (jint) item;
	} EXCEPTION_CONVERT(env);
	return 0;
}

/*
 * java.lang.String nativeInit(int arg1, boolean arg2)
 */
JNIEXPORT jstring JNICALL Java_com_scriptographer_widget_Item_nativeInit(
		JNIEnv *env, jobject obj, jint arg1, jboolean arg2) {
	try {
		// TODO: define nativeInit
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * void nativeDestroy(int arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_nativeDestroy(
		JNIEnv *env, jobject obj, jint arg1) {
	try {
		// TODO: define nativeDestroy
	} EXCEPTION_CONVERT(env);
}

/*
 * void nativeSetTrackCallback(boolean arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_nativeSetTrackCallback(
		JNIEnv *env, jobject obj, jboolean arg1) {
	try {
		// TODO: define nativeSetTrackCallback
	} EXCEPTION_CONVERT(env);
}

/*
 * void nativeSetDrawCallback(boolean arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_nativeSetDrawCallback(
		JNIEnv *env, jobject obj, jboolean arg1) {
	try {
		// TODO: define nativeSetDrawCallback
	} EXCEPTION_CONVERT(env);
}

/*
 * int getTrackMask()
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_widget_Item_getTrackMask(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define getTrackMask
	} EXCEPTION_CONVERT(env);
	return 0;
}

/*
 * void setTrackMask(int arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_setTrackMask(
		JNIEnv *env, jobject obj, jint arg1) {
	try {
		// TODO: define setTrackMask
	} EXCEPTION_CONVERT(env);
}

/*
 * void nativeSetStyle(int arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_nativeSetStyle(
		JNIEnv *env, jobject obj, jint arg1) {
	try {
		// TODO: define nativeSetStyle
	} EXCEPTION_CONVERT(env);
}

/*
 * int nativeGetStyle()
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_widget_Item_nativeGetStyle(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define nativeGetStyle
	} EXCEPTION_CONVERT(env);
	return 0;
}

/*
 * int getChildItemHandle(int arg1)
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_widget_Item_getChildItemHandle(
		JNIEnv *env, jobject obj, jint arg1) {
	try {
		// TODO: define getChildItemHandle
	} EXCEPTION_CONVERT(env);
	return 0;
}

/*
 * boolean isVisible()
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_widget_Item_isVisible(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define isVisible
	} EXCEPTION_CONVERT(env);
	return false;
}

/*
 * void setVisible(boolean arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_setVisible(
		JNIEnv *env, jobject obj, jboolean arg1) {
	try {
		// TODO: define setVisible
	} EXCEPTION_CONVERT(env);
}

/*
 * boolean isEnabled()
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_widget_Item_isEnabled(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define isEnabled
	} EXCEPTION_CONVERT(env);
	return false;
}

/*
 * void setEnabled(boolean arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_setEnabled(
		JNIEnv *env, jobject obj, jboolean arg1) {
	try {
		// TODO: define setEnabled
	} EXCEPTION_CONVERT(env);
}

/*
 * boolean isActive()
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_widget_Item_isActive(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define isActive
	} EXCEPTION_CONVERT(env);
	return false;
}

/*
 * void setActive(boolean arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_setActive(
		JNIEnv *env, jobject obj, jboolean arg1) {
	try {
		// TODO: define setActive
	} EXCEPTION_CONVERT(env);
}

/*
 * boolean isKnown()
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_widget_Item_isKnown(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define isKnown
	} EXCEPTION_CONVERT(env);
	return false;
}

/*
 * void setKnown(boolean arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_setKnown(
		JNIEnv *env, jobject obj, jboolean arg1) {
	try {
		// TODO: define setKnown
	} EXCEPTION_CONVERT(env);
}

/*
 * boolean wantsFocus()
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_widget_Item_wantsFocus(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define wantsFocus
	} EXCEPTION_CONVERT(env);
	return false;
}

/*
 * void setWantsFocus(boolean arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_setWantsFocus(
		JNIEnv *env, jobject obj, jboolean arg1) {
	try {
		// TODO: define setWantsFocus
	} EXCEPTION_CONVERT(env);
}

/*
 * com.scriptographer.ui.Rectangle nativeGetBounds()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_widget_Item_nativeGetBounds(
		JNIEnv *env, jobject obj) {
	try {
		
    HWND item = gEngine->getItemHandle(env, obj);
		RECT rt;
		GetWindowRect(item, &rt);
		return gEngine->convertRectangle(env, &rt);
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * void nativeSetBounds(int arg1, int arg2, int arg3, int arg4)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_nativeSetBounds(
		JNIEnv *env, jobject obj, jint x, jint y, jint width, jint height) {
	try {
		
    HWND item = gEngine->getItemHandle(env, obj);
		RECT rt;
		SetWindowPos(item, NULL, x, y, width, height, SWP_SHOWWINDOW);
	} EXCEPTION_CONVERT(env);
}

/*
 * void nativeSetSize(int arg1, int arg2)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_nativeSetSize(
		JNIEnv *env, jobject obj, jint arg1, jint arg2) {
	try {
		// TODO: define nativeSetSize
	} EXCEPTION_CONVERT(env);
}

/*
 * com.scriptographer.ui.Point localToScreen(int arg1, int arg2)
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_widget_Item_localToScreen__II(
		JNIEnv *env, jobject obj, jint arg1, jint arg2) {
	try {
		// TODO: define localToScreen
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * com.scriptographer.ui.Point screenToLocal(int arg1, int arg2)
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_widget_Item_screenToLocal__II(
		JNIEnv *env, jobject obj, jint arg1, jint arg2) {
	try {
		// TODO: define screenToLocal
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * com.scriptographer.ui.Rectangle localToScreen(int arg1, int arg2, int arg3, int arg4)
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_widget_Item_localToScreen__IIII(
		JNIEnv *env, jobject obj, jint arg1, jint arg2, jint arg3, jint arg4) {
	try {
		// TODO: define localToScreen
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * com.scriptographer.ui.Rectangle screenToLocal(int arg1, int arg2, int arg3, int arg4)
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_widget_Item_screenToLocal__IIII(
		JNIEnv *env, jobject obj, jint arg1, jint arg2, jint arg3, jint arg4) {
	try {
		// TODO: define screenToLocal
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * void invalidate()
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_invalidate__(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define invalidate
	} EXCEPTION_CONVERT(env);
}

/*
 * void invalidate(int arg1, int arg2, int arg3, int arg4)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_invalidate__IIII(
		JNIEnv *env, jobject obj, jint arg1, jint arg2, jint arg3, jint arg4) {
	try {
		// TODO: define invalidate
	} EXCEPTION_CONVERT(env);
}

/*
 * void update()
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_update(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define update
	} EXCEPTION_CONVERT(env);
}

/*
 * int nativeGetFont()
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_widget_Item_nativeGetFont(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define nativeGetFont
	} EXCEPTION_CONVERT(env);
	return 0;
}

/*
 * void nativeSetFont(int arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_nativeSetFont(
		JNIEnv *env, jobject obj, jint arg1) {
	try {
		// TODO: define nativeSetFont
	} EXCEPTION_CONVERT(env);
}

/*
 * void nativeSetBackgroundColor(int arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_nativeSetBackgroundColor(
		JNIEnv *env, jobject obj, jint arg1) {
	try {
		// TODO: define nativeSetBackgroundColor
	} EXCEPTION_CONVERT(env);
}

/*
 * int nativeGetBackgroundColor()
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_widget_Item_nativeGetBackgroundColor(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define nativeGetBackgroundColor
	} EXCEPTION_CONVERT(env);
	return 0;
}

/*
 * int nativeGetCursor()
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_widget_Item_nativeGetCursor(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define nativeGetCursor
	} EXCEPTION_CONVERT(env);
	return 0;
}

/*
 * void nativeSetCursor(int arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_nativeSetCursor(
		JNIEnv *env, jobject obj, jint arg1) {
	try {
		// TODO: define nativeSetCursor
	} EXCEPTION_CONVERT(env);
}

/*
 * void nativeSetTooltip(java.lang.String arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_nativeSetTooltip(
		JNIEnv *env, jobject obj, jstring arg1) {
	try {
		// TODO: define nativeSetTooltip
	} EXCEPTION_CONVERT(env);
}

/*
 * boolean isToolTipEnabled()
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_widget_Item_isToolTipEnabled(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define isToolTipEnabled
	} EXCEPTION_CONVERT(env);
	return false;
}

/*
 * void setToolTipEnabled(boolean arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_setToolTipEnabled(
		JNIEnv *env, jobject obj, jboolean arg1) {
	try {
		// TODO: define setToolTipEnabled
	} EXCEPTION_CONVERT(env);
}

/*
 * void showToolTip(int arg1, int arg2)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_showToolTip(
		JNIEnv *env, jobject obj, jint arg1, jint arg2) {
	try {
		// TODO: define showToolTip
	} EXCEPTION_CONVERT(env);
}

/*
 * void hideToolTip()
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_hideToolTip(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define hideToolTip
	} EXCEPTION_CONVERT(env);
}
