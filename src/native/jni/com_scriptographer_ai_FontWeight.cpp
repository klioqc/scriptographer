/*
 * Scriptographer
 *
 * This file is part of Scriptographer, a Plugin for Adobe Illustrator.
 *
 * Copyright (c) 2002-2010 Juerg Lehni, http://www.scratchdisk.com.
 * All rights reserved.
 *
 * Please visit http://scriptographer.org/ for updates and contact.
 *
 * -- GPL LICENSE NOTICE --
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * -- GPL LICENSE NOTICE --
 *
 * $Id$
 */

#include "StdHeaders.h"
#include "ScriptographerEngine.h"
#include "com_scriptographer_ai_FontWeight.h"

/*
 * com.scriptographer.ai.FontWeight
 */

/*
 * java.lang.String nativeGetName()
 */
JNIEXPORT jstring JNICALL Java_com_scriptographer_ai_FontWeight_nativeGetName(JNIEnv *env, jobject obj, jint handle) {
	try {
		ASUnicode unicodeName[256];
		if (!sAIFont->GetFontStyleUINameUnicode((AIFontKey) handle, unicodeName, 256)) {
			// In some cases GetFontFamilyUINameUnicode does seem to return an empty
			// string, e.g on CS2 Windows. If so, fall back to non-unicode version.
			if (unicodeName[0] == 0) {
				char name[256];
				if (!sAIFont->GetFontStyleUIName((AIFontKey) handle, name, 256))
					return gEngine->convertString(env, name);
			}
			return gEngine->convertString(env, unicodeName);
		}
	} EXCEPTION_CONVERT(env);
	return NULL;
}

/*
 * int nativeGetFamily(int handle)
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_ai_FontWeight_nativeGetFamily(JNIEnv *env, jobject obj, jint handle) {
	try {
		AITypefaceKey family;
		short style;
		if (!sAIFont->TypefaceAndStyleFromFontKey((AIFontKey) handle, &family, &style))
			return (jint) family;
	} EXCEPTION_CONVERT(env);
	return 0;
}

/*
 * int nativeGetIndex(int handle)
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_ai_FontWeight_nativeGetIndex(JNIEnv *env, jobject obj, jint handle) {
	try {
		AITypefaceKey family;
		short style;
		if (!sAIFont->TypefaceAndStyleFromFontKey((AIFontKey) handle, &family, &style))
			return (jint) style;
	} EXCEPTION_CONVERT(env);
	return 0;
}

/*
 * boolean nativeIsValid(int handle)
 */
JNIEXPORT jboolean JNICALL Java_com_scriptographer_ai_FontWeight_nativeIsValid(JNIEnv *env, jobject obj, jint handle) {
	try {
		FontRef ref;
		if (!sAIFont->FontFromFontKey((AIFontKey) handle, &ref) && ref != NULL)
			return true;
	} EXCEPTION_CONVERT(env);
	return false;
}
