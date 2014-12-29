#include "StdHeaders.h"
#include "ScriptographerEngine.h"
#include "ScriptographerPlugin.h"
#include "com_scriptographer_widget_Item.h"
#include "uiGlobals.h"
#include "commonctrls.h"

void Item_onDestroy(CControl* inItem);
void Item_onNotify(CControl* inItem, char * notifier);


int Item_onInit(CControl* inItem) {
	// Attach the item-level callbacks
	DEFINE_CALLBACK_PROC(Item_onDestroy);
  inItem->SetDestroyProc((CControlDestroyProc) CALLBACK_PROC(Item_onDestroy));
	
	DEFINE_CALLBACK_PROC(Item_onNotify);
	inItem->SetNotifyProc((CControlNotifyProc) CALLBACK_PROC(Item_onNotify));

	// Call onNotify with kADMInitializeNotifier
	JNIEnv *env = gEngine->getEnv();
	try {
		CCommonControl * ctrl = reinterpret_cast<CCommonControl*>(inItem);
		jobject obj = gEngine->getItemObject(ctrl);
		gEngine->callOnNotify(obj, kUIInitializeNotifier);
	} EXCEPTION_CATCH_REPORT(env);
	return kNoErr;

}
void Item_onDestroy(CControl* inItem) {
	if (gEngine != NULL) {
		JNIEnv *env = gEngine->getEnv();
		try {
      CCommonControl * ctrl = reinterpret_cast<CCommonControl*>(inItem);
			jobject obj = gEngine->getItemObject(ctrl);
			gEngine->callOnDestroy(obj);
			// clear the handle:
			gEngine->setIntField(env, obj, gEngine->fid_ui_NativeObject_handle, 0);

			// is this a list or hierarchy list?
			// if so, call its destroy function, as this is not automatically done:
			// SetUserData needs to be called again as the user data is not valid anymore here:

		/*	if (env->IsInstanceOf(obj, gEngine->cls_adm_ListItem)) {
				if (env->IsInstanceOf(obj, gEngine->cls_adm_HierarchyListBox)) {
					ADMHierarchyListRef list = gEngine->getHierarchyListBoxHandle(env, obj);
					sADMHierarchyList->SetUserData(list, obj);
					HierarchyListBox_onDestroy(list);
				} else {
					ADMListRef list = gEngine->getListBoxHandle(env, obj);
					sADMList->SetUserData(list, obj);
					ListItem_onDestroy(list);
				}
			}*/
			env->DeleteGlobalRef(obj);
		} EXCEPTION_CATCH_REPORT(env);
  }
}

void Item_onNotify(CControl* inItem, char * notifier) {
	//sADMItem->DefaultNotify(item, notifier);
	if (gEngine != NULL) {

		CCommonControl * ctrl = reinterpret_cast<CCommonControl*>(inItem);
		jobject obj = gEngine->getItemObject(ctrl);

		gEngine->callOnNotify(obj, notifier);
	}
}

/*
ASBoolean ASAPI Item_onTrack(ADMItemRef item, ADMTrackerRef tracker) {
	jobject obj = gEngine->getItemObject(item);
	ASBoolean ret = gEngine->callOnTrack(obj, tracker);
	if (ret)
		ret = sADMItem->DefaultTrack(item, tracker);
	return ret;
}

void ASAPI Item_onDraw(ADMItemRef item, ADMDrawerRef drawer) {
	jobject obj = gEngine->getItemObject(item);
	ASBoolean ret = gEngine->callOnDraw(obj, drawer);
	if (ret)
		sADMItem->DefaultDraw(item, drawer);
}


*/





JNIEXPORT jint JNICALL Java_com_scriptographer_widget_Item_nativeCreate(
		JNIEnv *env, jobject obj, jobject dialogObj, jstring type, jint options) {
	try {
	  CDialog * dlg = gEngine->getDialog(env, dialogObj);
    if (dlg)
    {
		    char * itemType =  gEngine->convertString(env, type);
    
        DEFINE_UI_RECT(rect, 0, 0, 100, 100);
		    DEFINE_CALLBACK_PROC(Item_onInit);
    
        CControl * item = dlg->CreateItem(itemType, &rect, (CControlInitProc)CALLBACK_PROC(Item_onInit), env->NewGlobalRef(obj));

        delete itemType;

      if (item == NULL)
			  throw new StringException("Unable to create dialog item.");

		  return (jint) item;
    }

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
     gPlugin->log("nativeInit");
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
      gPlugin->log("nativeDestroy");
	} EXCEPTION_CONVERT(env);
}

/*
 * void nativeSetTrackCallback(boolean arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_nativeSetTrackCallback(
		JNIEnv *env, jobject obj, jboolean arg1) {
	try {
		// TODO: define nativeSetTrackCallback
      gPlugin->log("nativeSetTrackCallback");
	} EXCEPTION_CONVERT(env);
}


/*
 * int getTrackMask()
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_widget_Item_getTrackMask(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define getTrackMask
    gPlugin->log("getTrackMask");
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
      gPlugin->log("setTrackMask");
	} EXCEPTION_CONVERT(env);
}

/*
 * void nativeSetStyle(int arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_nativeSetStyle(
		JNIEnv *env, jobject obj, jint arg1) {
	try {
		// TODO: define nativeSetStyle
      gPlugin->log("nativeSetStyle");
	} EXCEPTION_CONVERT(env);
}

/*
 * int nativeGetStyle()
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_widget_Item_nativeGetStyle(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define nativeGetStyle
      gPlugin->log("nativeGetStyle");
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
      gPlugin->log("getChildItemHandle");
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
      gPlugin->log("isVisible");
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
    gPlugin->log("setVisible");
	} EXCEPTION_CONVERT(env);
}

/*
 * boolean isEnabled()
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_widget_Item_isEnabled(
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
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_setEnabled(
		JNIEnv *env, jobject obj, jboolean arg1) {
	try {
		// TODO: define setEnabled
     gPlugin->log("setEnabled");
	} EXCEPTION_CONVERT(env);
}

/*
 * boolean isActive()
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_widget_Item_isActive(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define isActive
     gPlugin->log("isActive");
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
     gPlugin->log("setActive");
	} EXCEPTION_CONVERT(env);
}

/*
 * boolean isKnown()
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_widget_Item_isKnown(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define isKnown
    gPlugin->log("isKnown");
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
    gPlugin->log("setKnown");
	} EXCEPTION_CONVERT(env);
}

/*
 * boolean wantsFocus()
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_widget_Item_wantsFocus(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define wantsFocus
    gPlugin->log("wantsFocus");
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
    gPlugin->log("setWantsFocus");
	} EXCEPTION_CONVERT(env);
}

/*
 * com.scriptographer.ui.Rectangle nativeGetBounds()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_widget_Item_nativeGetBounds(
		JNIEnv *env, jobject obj) {
	try {
		
    CCommonControl * item = gEngine->getItemObject(env, obj);
		RECT rt;
    item->GetBounds(&rt);
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
		
    CCommonControl * item = gEngine->getItemObject(env, obj);
    item->SetBounds( x, y, width , height);
	  gPlugin->log("SetBounds %d %d %d %d", x, y, width, height );
	} EXCEPTION_CONVERT(env);
}

/*
 * void nativeSetSize(int arg1, int arg2)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_nativeSetSize(JNIEnv *env, jobject obj, jint width, jint height) {
	try {
		 CCommonControl * item = gEngine->getItemObject(env, obj);
	   item->SetWindowSize(width, height);
	 gPlugin->log("setSize %d %d", width, height );
	} EXCEPTION_CONVERT(env);
}

/*
 * com.scriptographer.ui.Point localToScreen(int arg1, int arg2)
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_widget_Item_localToScreen__II(
		JNIEnv *env, jobject obj, jint arg1, jint arg2) {
	try {
		// TODO: define localToScreen
    gPlugin->log("localToScreen");
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
    gPlugin->log("screenToLocal");
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
    gPlugin->log("localToScreen");
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
     gPlugin->log("screenToLocal");
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
     gPlugin->log("invalidate");
	} EXCEPTION_CONVERT(env);
}

/*
 * void invalidate(int arg1, int arg2, int arg3, int arg4)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_invalidate__IIII(
		JNIEnv *env, jobject obj, jint arg1, jint arg2, jint arg3, jint arg4) {
	try {
		// TODO: define invalidate
     gPlugin->log("invalidate");
	} EXCEPTION_CONVERT(env);
}

/*
 * void update()
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_update(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define update
     gPlugin->log("update");
	} EXCEPTION_CONVERT(env);
}

/*
 * int nativeGetFont()
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_widget_Item_nativeGetFont(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define nativeGetFont
     gPlugin->log("nativeGetFont");
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
     gPlugin->log("nativeSetFont");
	} EXCEPTION_CONVERT(env);
}

/*
 * void nativeSetBackgroundColor(int arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_nativeSetBackgroundColor(
		JNIEnv *env, jobject obj, jint arg1) {
	try {
		// TODO: define nativeSetBackgroundColor
      gPlugin->log("nativeSetBackgroundColor");
	} EXCEPTION_CONVERT(env);
}

/*
 * int nativeGetBackgroundColor()
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_widget_Item_nativeGetBackgroundColor(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define nativeGetBackgroundColor
     gPlugin->log("nativeGetBackgroundColor");
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
     gPlugin->log("nativeGetCursor");
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
       gPlugin->log("nativeSetCursor");
	} EXCEPTION_CONVERT(env);
}

/*
 * void nativeSetTooltip(java.lang.String arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_nativeSetTooltip(
		JNIEnv *env, jobject obj, jstring arg1) {
	try {
		// TODO: define nativeSetTooltip
       gPlugin->log("nativeSetTooltip");
	} EXCEPTION_CONVERT(env);
}

/*
 * boolean isToolTipEnabled()
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_widget_Item_isToolTipEnabled(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define isToolTipEnabled
       gPlugin->log("isToolTipEnabled");
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
       gPlugin->log("setToolTipEnabled");
	} EXCEPTION_CONVERT(env);
}

/*
 * void showToolTip(int arg1, int arg2)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_showToolTip(
		JNIEnv *env, jobject obj, jint arg1, jint arg2) {
	try {
		// TODO: define showToolTip
       gPlugin->log("showToolTip");
	} EXCEPTION_CONVERT(env);
}

/*
 * void hideToolTip()
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_hideToolTip(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define hideToolTip
       gPlugin->log("hideToolTip");
	} EXCEPTION_CONVERT(env);
}

/*
 * void nativeSetDrawCallback(boolean arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Item_nativeSetDrawCallback(
		JNIEnv *env, jobject obj, jboolean arg1) {
	try {
		// TODO: define nativeSetDrawCallback
       gPlugin->log("nativeSetDrawCallback");
	} EXCEPTION_CONVERT(env);
}

/*
 * boolean defaultTrack(com.scriptographer.widget.Tracker arg1)
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_widget_Item_defaultTrack(
		JNIEnv *env, jobject obj, jobject arg1) {
	try {
		// TODO: define defaultTrack
    gPlugin->log("defaultTrack");
	} EXCEPTION_CONVERT(env);
	return false;
}

/*
 * com.scriptographer.ui.Size nativeGetTextSize(java.lang.String arg1)
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_widget_Item_nativeGetTextSize(
		JNIEnv *env, jobject obj, jstring text) {
	try {
		
     CCommonControl * item = gEngine->getItemObject(env, obj);
		if (item)
    {
       //char *chars = gEngine->convertString(env, text);
       ai::UnicodeString str = gEngine->convertString_UnicodeString(env, text); 
       SIZE size = item->GetTextSize((LPWSTR)str.as_ASUnicode().c_str());
			
        gPlugin->log("GetTextSize  %d %d",  size.cx, size.cy);
      //delete chars;
        return gEngine->convertUISize(env, size.cx, size.cy);
    }
      
	} EXCEPTION_CONVERT(env);
	return NULL;
}
