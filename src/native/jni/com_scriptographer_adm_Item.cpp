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
#include "ScriptographerPlugin.h"
#include "ScriptographerEngine.h"
#include "uiGlobals.h"
#include "com_scriptographer_adm_Item.h"
#include "com_scriptographer_adm_Notifier.h"

/*
 * com.scriptographer.awt.Item
 */

/*
 * various callbacks for item:
 *
 */

ASErr ASAPI Item_onInit(ADMItemRef item) {
	// Attach the item-level callbacks
	DEFINE_CALLBACK_PROC(Item_onDestroy);
	sADMItem->SetDestroyProc(item, (ADMItemDestroyProc) CALLBACK_PROC(Item_onDestroy));
	
	DEFINE_CALLBACK_PROC(Item_onNotify);
	sADMItem->SetNotifyProc(item, (ADMItemNotifyProc) CALLBACK_PROC(Item_onNotify));

	// Call onNotify with kADMInitializeNotifier
	JNIEnv *env = gEngine->getEnv();
	try {
		jobject obj = gEngine->getItemObject(item);
		gEngine->callOnNotify(obj, kADMInitializeNotifier);
	} EXCEPTION_CATCH_REPORT(env);
	return kNoErr;
}

void ASAPI Item_onDestroy(ADMItemRef item) {
	if (gEngine != NULL) {
		JNIEnv *env = gEngine->getEnv();
		try {
			jobject obj = gEngine->getItemObject(item);
			gEngine->callOnDestroy(obj);
			// clear the handle:
			gEngine->setIntField(env, obj, gEngine->fid_ui_NativeObject_handle, 0);

			// is this a list or hierarchy list?
			// if so, call its destroy function, as this is not automatically done:
			// SetUserData needs to be called again as the user data is not valid anymore here:

			if (env->IsInstanceOf(obj, gEngine->cls_adm_ListItem)) {
				if (env->IsInstanceOf(obj, gEngine->cls_adm_HierarchyListBox)) {
					ADMHierarchyListRef list = gEngine->getHierarchyListBoxHandle(env, obj);
					sADMHierarchyList->SetUserData(list, obj);
					HierarchyListBox_onDestroy(list);
				} else {
					ADMListRef list = gEngine->getListBoxHandle(env, obj);
					sADMList->SetUserData(list, obj);
					ListItem_onDestroy(list);
				}
			}
			env->DeleteGlobalRef(obj);
		} EXCEPTION_CATCH_REPORT(env);
	}
}

void ASAPI Item_onNotify(ADMItemRef item, ADMNotifierRef notifier) {
	sADMItem->DefaultNotify(item, notifier);
	if (gEngine != NULL) {
		jobject obj = gEngine->getItemObject(item);
		gEngine->callOnNotify(obj, notifier);
	}
}

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

/*
 * int nativeCreate(com.scriptographer.adm.Dialog dialog, java.lang.String type, int options)
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_adm_Item_nativeCreate(JNIEnv *env, jobject obj, jobject dialogObj, jstring type, jint options) {
	try {
		ADMDialogRef dialog = gEngine->getDialogHandle(env, dialogObj);
		char *itemType = gEngine->convertString(env, type);
		// create with default dimensions:
		DEFINE_ADM_RECT(rect, 0, 0, 100, 100);
		DEFINE_CALLBACK_PROC(Item_onInit);
		ADMItemRef item = sADMItem->Create(dialog, kADMUniqueItemID, itemType, &rect, (ADMItemInitProc) CALLBACK_PROC(Item_onInit), env->NewGlobalRef(obj), options);
		delete itemType;
		if (item == NULL)
			throw new StringException("Unable to create dialog item.");

		return (jint) item;
	} EXCEPTION_CONVERT(env);
	return 0;
}

/*
 * String nativeInit(int handle, boolean isChild)
 */
JNIEXPORT jstring JNICALL Java_com_scriptographer_adm_Item_nativeInit(JNIEnv *env, jobject obj, jint handle, jboolean isChild) {
	try {
		sADMItem->SetUserData((ADMItemRef) handle, env->NewGlobalRef(obj));
		// We cannot override the ontifier callbacks on child items, as this would
		// break things like Spin Edits.
		if (!isChild)
			Item_onInit((ADMItemRef) handle);
		return gEngine->convertString(env, sADMItem->GetItemType((ADMItemRef) handle));
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * void nativeDestroy(int handle)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_Item_nativeDestroy(JNIEnv *env, jobject obj, jint handle) {
	try {
		sADMItem->Destroy((ADMItemRef) handle);
	} EXCEPTION_CONVERT(env);
}

/*
 * void nativeSetTrackCallback(boolean enabled)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_Item_nativeSetTrackCallback(JNIEnv *env, jobject obj, jboolean enabled) {
	try {
		DEFINE_CALLBACK_PROC(Item_onTrack);
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		sADMItem->SetTrackProc(item, enabled ? (ADMItemTrackProc) CALLBACK_PROC(Item_onTrack) : NULL);
	} EXCEPTION_CONVERT(env);
}

/*
 * boolean defaultTrack(com.scriptographer.adm.Tracker tracker)
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_adm_Item_defaultTrack(JNIEnv *env, jobject obj, jobject tracker) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		return sADMItem->DefaultTrack(item, gEngine->getTrackerHandle(env, tracker));
	} EXCEPTION_CONVERT(env);
	return false;
}

/*
 * void defaultDraw(com.scriptographer.adm.Drawer drawer)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_Item_defaultDraw(JNIEnv *env, jobject obj, jobject drawer) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		sADMItem->DefaultDraw(item, gEngine->getDrawerHandle(env, drawer));
	} EXCEPTION_CONVERT(env);
}

/*
 * int getTrackMask()
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_adm_Item_getTrackMask(JNIEnv *env, jobject obj) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		return sADMItem->GetMask(item);
	} EXCEPTION_CONVERT(env);
	return 0;
}

/*
 * void setTrackMask(int mask)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_Item_setTrackMask(JNIEnv *env, jobject obj, jint mask) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		sADMItem->SetMask(item, mask);
	} EXCEPTION_CONVERT(env);
}

/*
 * void nativeSetDrawCallback(boolean enabled)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_Item_nativeSetDrawCallback(JNIEnv *env, jobject obj, jboolean enabled) {
	try {
		DEFINE_CALLBACK_PROC(Item_onDraw);
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		sADMItem->SetDrawProc(item, enabled ? (ADMItemDrawProc) CALLBACK_PROC(Item_onDraw) : NULL);
	} EXCEPTION_CONVERT(env);
}


/*
 * void nativeSetStyle(int style)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_Item_nativeSetStyle(JNIEnv *env, jobject obj, jint style) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		sADMItem->SetItemStyle(item, (ADMItemStyle) style);
	} EXCEPTION_CONVERT(env);
}

/*
 * int nativeGetStyle()
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_adm_Item_nativeGetStyle(JNIEnv *env, jobject obj) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		return (jint) sADMItem->GetItemStyle(item);
	} EXCEPTION_CONVERT(env);
	return 0;
}

/*
 * com.scriptographer.adm.Size nativeGetSize()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_Item_nativeGetSize(JNIEnv *env, jobject obj) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		ADMRect rect;
		sADMItem->GetLocalRect(item, &rect);
		DEFINE_ADM_POINT(size, rect.right, rect.bottom);
		return gEngine->convertSize(env, &size);
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * void nativeSetSize(int width, int height)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_Item_nativeSetSize(JNIEnv *env, jobject obj, jint width, jint height) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		DEFINE_ADM_RECT(rect, 0, 0, width, height);
		sADMItem->SetLocalRect(item, &rect);
	} EXCEPTION_CONVERT(env);
}

/*
 * com.scriptographer.adm.Size nativeGetBestSize()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_Item_nativeGetBestSize(JNIEnv *env, jobject obj) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		ADMPoint size;
		sADMItem->GetBestSize(item, &size);
		return gEngine->convertSize(env, &size);
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * java.awt.Rectangle nativeGetBounds()
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_Item_nativeGetBounds(JNIEnv *env, jobject obj) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		ADMRect rt;
		sADMItem->GetBoundsRect(item, &rt);
		return gEngine->convertRectangle(env, &rt);
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * void nativeSetBounds(int x, int y, int width, int height)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_Item_nativeSetBounds(JNIEnv *env, jobject obj, jint x, jint y, jint width, jint height) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		DEFINE_ADM_RECT(rt, x, y, width, height);
		sADMItem->SetBoundsRect(item, &rt);
	} EXCEPTION_CONVERT(env);
}

/*
 * com.scriptographer.adm.Point localToScreen(int x, int y)
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_Item_localToScreen__II(JNIEnv *env, jobject obj, jint x, jint y) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		DEFINE_ADM_POINT(pt, x, y);
		sADMItem->LocalToScreenPoint(item, &pt);
		return gEngine->convertPoint(env, &pt);
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * com.scriptographer.adm.Point screenToLocal(int x, int y)
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_Item_screenToLocal__II(JNIEnv *env, jobject obj, jint x, jint y) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		DEFINE_ADM_POINT(pt, x, y);
		sADMItem->ScreenToLocalPoint(item, &pt);
		return gEngine->convertPoint(env, &pt);
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * java.awt.Rectangle localToScreen(int x, int y, int width, int height)
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_Item_localToScreen__IIII(JNIEnv *env, jobject obj, jint x, jint y, jint width, jint height) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		DEFINE_ADM_RECT(rt, x, y, width, height);
		sADMItem->LocalToScreenRect(item, &rt);
		return gEngine->convertRectangle(env, &rt);
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * java.awt.Rectangle screenToLocal(int x, int y, int width, int height)
 */
JNIEXPORT jobject JNICALL Java_com_scriptographer_adm_Item_screenToLocal__IIII(JNIEnv *env, jobject obj, jint x, jint y, jint width, jint height) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		DEFINE_ADM_RECT(rt, x, y, width, height);
		sADMItem->ScreenToLocalRect(item, &rt);
		return gEngine->convertRectangle(env, &rt);
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * void invalidate()
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_Item_invalidate__(JNIEnv *env, jobject obj) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		sADMItem->Invalidate(item);
	} EXCEPTION_CONVERT(env);
}

/*
 * void invalidate(int x, int y, int width, int height)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_Item_invalidate__IIII(JNIEnv *env, jobject obj, jint x, jint y, jint width, jint height) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		DEFINE_ADM_RECT(rt, x, y, width, height);
		sADMItem->InvalidateRect(item, &rt);
	} EXCEPTION_CONVERT(env);
}

/*
 * void update()
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_Item_update(JNIEnv *env, jobject obj) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		sADMItem->Update(item);
	} EXCEPTION_CONVERT(env);
}

/*
 * boolean isVisible()
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_adm_Item_isVisible(JNIEnv *env, jobject obj) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		return sADMItem->IsVisible(item);
	} EXCEPTION_CONVERT(env);
	return false;
}

/*
 * void setVisible(boolean visible)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_Item_setVisible(JNIEnv *env, jobject obj, jboolean visible) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		sADMItem->Show(item, visible);
	} EXCEPTION_CONVERT(env);
}

/*
 * boolean isActive()
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_adm_Item_isActive(JNIEnv *env, jobject obj) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		return sADMItem->IsActive(item);
	} EXCEPTION_CONVERT(env);
	return false;
}

/*
 * void setActive(boolean arg1)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_Item_setActive(JNIEnv *env, jobject obj, jboolean active) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		sADMItem->Activate(item, active);
	} EXCEPTION_CONVERT(env);
}

/*
 * boolean isEnabled()
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_adm_Item_isEnabled(JNIEnv *env, jobject obj) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		return sADMItem->IsEnabled(item);
	} EXCEPTION_CONVERT(env);
	return false;
}

/*
 * void seEnabled(boolean enabled)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_Item_setEnabled(JNIEnv *env, jobject obj, jboolean enabled) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		sADMItem->Enable(item, enabled);
	} EXCEPTION_CONVERT(env);
}

/*
 * boolean isKnown()
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_adm_Item_isKnown(JNIEnv *env, jobject obj) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		return sADMItem->IsKnown(item);
	} EXCEPTION_CONVERT(env);
	return false;
}

/*
 * void setKnown(boolean known)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_Item_setKnown(JNIEnv *env, jobject obj, jboolean known) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		sADMItem->Known(item, known);
	} EXCEPTION_CONVERT(env);
}

/*
 * int nativeGetCursor()
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_adm_Item_nativeGetCursor(JNIEnv *env, jobject obj) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		long cursor;
		const char* name;
		SPPluginRef pluginRef = sADMItem->GetPluginRef(item);
		sADMItem->GetCursorID(item, &pluginRef, &cursor, &name);
		return cursor;
	} EXCEPTION_CONVERT(env);
	return 0;
}

/*
 * void nativeSetCursor(int cursor)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_Item_nativeSetCursor(JNIEnv *env, jobject obj, jint cursor) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		if (cursor >= 0) {
			SPPluginRef pluginRef = sADMItem->GetPluginRef(item);
			sADMItem->SetCursorID(item, pluginRef, cursor, NULL);
		}
	} EXCEPTION_CONVERT(env);
}

/*
 * boolean wantsFocus()
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_adm_Item_wantsFocus(JNIEnv *env, jobject obj) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		return sADMItem->GetWantsFocus(item);
	} EXCEPTION_CONVERT(env);
	return false;
}

/*
 * void setWantsFocus(boolean wantsFocus)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_Item_setWantsFocus(JNIEnv *env, jobject obj, jboolean wantsFocus) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		sADMItem->SetWantsFocus(item, wantsFocus);
	} EXCEPTION_CONVERT(env);
}

/*
 * void nativeSetTooltip(java.lang.String toolTip)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_Item_nativeSetTooltip(JNIEnv *env, jobject obj, jstring toolTip) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		ASUnicode *chars = gEngine->convertString_ASUnicode(env, toolTip);
		sADMItem->SetTipStringW(item, chars);
		delete chars;
	} EXCEPTION_CONVERT(env);
}

/*
 * boolean isToolTipEnabled()
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_adm_Item_isToolTipEnabled(JNIEnv *env, jobject obj) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		return sADMItem->IsTipEnabled(item);
	} EXCEPTION_CONVERT(env);
	return false;
}

/*
 * void setToolTipEnabled(boolean enabled)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_Item_setToolTipEnabled(JNIEnv *env, jobject obj, jboolean enabled) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		sADMItem->EnableTip(item, enabled);
	} EXCEPTION_CONVERT(env);
}

/*
 * void showToolTip(int x, int y)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_Item_showToolTip(JNIEnv *env, jobject obj, jint x, jint y) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		DEFINE_ADM_POINT(pt, x, y);
		sADMItem->ShowToolTip(item, &pt);
	} EXCEPTION_CONVERT(env);
}

/*
 * void hideToolTip()
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_Item_hideToolTip(JNIEnv *env, jobject obj) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		sADMItem->HideToolTip(item);
	} EXCEPTION_CONVERT(env);
}

/*
 * void nativeSetFont(int font)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_Item_nativeSetFont(JNIEnv *env, jobject obj, jint font) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		sADMItem->SetFont(item, (ADMFont)font);
	} EXCEPTION_CONVERT(env);
}

/*
 * int nativeGetFont()
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_adm_Item_nativeGetFont(JNIEnv *env, jobject obj) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		return sADMItem->GetFont(item);
	} EXCEPTION_CONVERT(env);
	return 0;
}

/*
 * void nativeSetBackgroundColor(int color)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_adm_Item_nativeSetBackgroundColor(JNIEnv *env, jobject obj, jint color) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		sADMItem->SetBackColor(item, (ADMColor) color);
	} EXCEPTION_CONVERT(env);
}

/*
 * int nativeGetBackgroundColor()
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_adm_Item_nativeGetBackgroundColor(JNIEnv *env, jobject obj) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		return sADMItem->GetBackColor(item);
	} EXCEPTION_CONVERT(env);
	return 0;
}

/*
 * int getChildItemHandle(int itemID)
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_adm_Item_getChildItemHandle(JNIEnv *env, jobject obj, jint itemID) {
	try {
		ADMItemRef item = gEngine->getItemHandle(env, obj);
		return (jint) sADMItem->GetChildItem(item, itemID);
	} EXCEPTION_CONVERT(env);
	return 0;
}
