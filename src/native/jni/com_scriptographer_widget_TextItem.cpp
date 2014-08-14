#include "StdHeaders.h"
#include "ScriptographerEngine.h"
#include "com_scriptographer_widget_TextItem.h"

/*
 * com.scriptographer.widget.TextItem
 */

/*
 * void nativeSetText(java.lang.String arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_TextItem_nativeSetText(
		JNIEnv *env, jobject obj, jstring text) {
	try {
	    CCommonControl *item = gEngine->getItemObject(env, obj);
		
      if (text != NULL) {
			//  TCHAR *chars = gEngine->convertString_ASUnicode(env, text);
			//  item->SetText(chars);
			//  delete chars;
		} else {
			//  item->SetText("");
		}



	} EXCEPTION_CONVERT(env);
}
