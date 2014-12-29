#include "StdHeaders.h"
#include "ScriptographerEngine.h"
#include "com_scriptographer_widget_TextValueItem.h"

#include "ScriptographerPlugin.h"

/*
 * com.scriptographer.widget.TextValueItem
 */

/*
 * void setText(java.lang.String arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_TextValueItem_setText(
		JNIEnv *env, jobject obj, jstring text) {
try {
	
    CCommonControl * item = gEngine->getItemObject(env, obj);
		if (text != NULL) {
		 char *chars = gEngine->convertString(env, text);
      ai::UnicodeString str =  ai::UnicodeString(chars);
			item->SetText(str);
			delete chars;
		} else {
			item->SetText("");
		}
	} EXCEPTION_CONVERT(env);
}

/*
 * java.lang.String getText()
 */
JNIEXPORT jstring JNICALL Java_com_scriptographer_widget_TextValueItem_getText(
		JNIEnv *env, jobject obj) {
	try {
		CCommonControl * item = gEngine->getItemObject(env, obj);
    if (item != NULL)
    {
     
		    long len = item->GetTextLen();
		    ASUnicode *chars = new ASUnicode[len];
		    item->GetText((LPWSTR)chars, len);
		    jstring res = gEngine->convertString(env, chars, len);
		    delete chars;
		    return res;
    }
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * void nativeSetJustification(int arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_TextValueItem_nativeSetJustification(
		JNIEnv *env, jobject obj, jint arg1) {
	try {
	  gPlugin->log("nativeGetJustification");
	} EXCEPTION_CONVERT(env);
}

/*
 * int nativeGetJustification()
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_widget_TextValueItem_nativeGetJustification(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define nativeGetJustification
     gPlugin->log("nativeGetJustification");
	} EXCEPTION_CONVERT(env);
	return 0;
}

/*
 * void nativeSetUnits(int arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_TextValueItem_nativeSetUnits(
		JNIEnv *env, jobject obj, jint arg1) {
	try {
		// TODO: define nativeSetUnits
     gPlugin->log("nativeSetUnits");
	} EXCEPTION_CONVERT(env);
}

/*
 * int nativeGetUnits()
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_widget_TextValueItem_nativeGetUnits(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define nativeGetUnits
    gPlugin->log("nativeGetUnits");
	} EXCEPTION_CONVERT(env);
	return 0;
}

/*
 * void setShowUnits(boolean arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_TextValueItem_setShowUnits(
		JNIEnv *env, jobject obj, jboolean arg1) {
	try {
		// TODO: define setShowUnits
        gPlugin->log("setShowUnits");
	} EXCEPTION_CONVERT(env);
}

/*
 * boolean getShowUnits()
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_widget_TextValueItem_getShowUnits(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define getShowUnits
      gPlugin->log("getShowUnits");
	} EXCEPTION_CONVERT(env);
	return false;
}
