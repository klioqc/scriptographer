/*
 * Scriptographer
 *
 * This file is part of Scriptographer, a Plugin for Adobe Illustrator.
 *
 * Copyright (c) 2002-2005 Juerg Lehni, http://www.scratchdisk.com.
 * All rights reserved.
 *
 * Please visit http://scriptographer.com/ for updates and contact.
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
 * File created on 08.04.2005.
 *
 * $RCSfile: DictionaryObject.java,v $
 * $Author: lehni $
 * $Revision: 1.1 $
 * $Date: 2005/04/08 21:56:40 $
 */

package com.scriptographer.ai;

import java.util.Map;

import org.mozilla.javascript.NativeObject;

import com.scriptographer.js.FunctionHelper;
import com.scriptographer.util.Handle;

public abstract class DictionaryObject extends AIObject {
	
	protected DictionaryObject(Handle handle) {
		super(handle);
	}
	
	protected DictionaryObject(int handle) {
		super(handle);
	}

	private Dictionary dictionary = null;
	
	public Dictionary getDictionary() {
		if (dictionary == null)
			dictionary = new Dictionary(this);
		else
			dictionary.checkUpdate();
		
		return dictionary;	
	}

	public void setDictionary(Map map) {
		Dictionary dictionary = getDictionary();
		dictionary.clear();
		dictionary.putAll(map);
	}
	
	public void setDictionary(NativeObject obj) {
		setDictionary(FunctionHelper.convertToMap(obj));	
	}
	
	protected abstract int getVersion();

	protected abstract void nativeGetDictionary(Dictionary dictionary);
	protected abstract void nativeSetDictionary(Dictionary dictionary);
}