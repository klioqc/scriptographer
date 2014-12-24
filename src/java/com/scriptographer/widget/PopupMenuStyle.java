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
 * 
 * File created on Apr 15, 2008.
 */

package com.scriptographer.widget;

import com.scratchdisk.util.IntegerEnum;

/**
 * ADMPopupMenuStyle
 * 
 * @author lehni
 *
 */
public enum PopupMenuStyle implements IntegerEnum {
	RIGHT(0),
	BOTTOM(1),
	ROUND(2),
	ROUND_HIERARCHY(4);

	protected int value;

	private PopupMenuStyle(int value) {
		this.value = value;
	}

	public int value() {
		return value;
	}
}
