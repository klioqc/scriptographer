/*
 * Scriptographer
 *
 * This file is part of Scriptographer, a Plugin for Adobe Illustrator.
 *
 * Copyright (c) 2002-2007 Juerg Lehni, http://www.scratchdisk.com.
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
 * File created on 16.02.2005.
 *
 * $Id: AbstractExtendedList.java 240 2007-02-17 15:14:26Z lehni $
 */

package com.scratchdisk.list;


/**
 * @author lehni
 */
public abstract class AbstractExtendedList implements ExtendedList {

	public Object add(Object element) {
		return add(size(), element);
	}

	public Object getFirst() {
		return get(0);
	}
	
	public Object getLast() {
		return get(size() - 1);
	}

	public Object removeFirst() {
		return this.remove(0);		
	}
	
	public Object removeLast() {
		return this.remove(size() - 1);
	}

	public int indexOf(Object element) {
		int size = size();
		for (int i = 0; i < size; i++) {
			Object obj = get(i);
			if (obj == null && element == null || obj.equals(element))
				return i;
		}
		return -1;
	}

	public int lastIndexOf(Object element) {
		for (int i = size() - 1; i >= 0; i--) {
			Object obj = get(i);
			if (obj == null && element == null || obj.equals(element))
				return i;
		}
		return -1;
	}

	public boolean contains(Object element) {
		return indexOf(element) != -1;
	}

	public boolean remove(Object element) {
		int index = indexOf(element);
		if (index >= 0) {
			return remove(index) != null;
		}
		return false;
	}

	public void remove(int fromIndex, int toIndex) {
		for (int i = toIndex - 1; i >= fromIndex; i--) {
			remove(i);
		}
	}

	public final void removeAll() {
		remove(0, size());
	}

	public boolean addAll(int index, ExtendedList elements) {
		boolean modified = false;
		int size = elements.size();
		for (int i = 0; i < size; i++) {
			if (add(index++, elements.get(i)) != null)
				modified = true;
		}
		return modified;
	}

	public final boolean addAll(ExtendedList elements) {
		return addAll(size(), elements);
	}

	public final boolean addAll(int index, Object[] elements) {
		return addAll(index, Lists.asList(elements));
	}

	public final boolean addAll(Object[] elements) {
		return addAll(size(), elements);
	}

	public Object[] toArray(Object[] array) {
		if (array == null)
			array = new Object[size()];
		for (int i = 0; i < array.length; i++)
			array[i] = get(i);
		return array;
	}

	public final Object[] toArray() {
		return toArray(null);
	}

	public ExtendedList getSubList(int fromIndex, int toIndex) {
		return Lists.createSubList(this, fromIndex, toIndex);
	}

	public boolean isEmpty() {
		return size() == 0;
	}

	public boolean retainAll(ExtendedList elements) {
		boolean modified = false;
		for (int i = size() - 1; i >= 0; i--) {
			Object obj = get(i);
			if(!elements.contains(obj) && remove(i) != null)
				modified = true;
		}
		return modified;
	}

	public final boolean retainAll(Object[] elements) {
		return retainAll(elements);
	}

	public boolean removeAll(ExtendedList elements) {
		boolean modified = false;
		for (int i = size() - 1; i >= 0; i--) {
			Object obj = get(i);
			if(elements.contains(obj) && remove(i) != null)
				modified = true;
		}
		return modified;
	}

	public final boolean removeAll(Object[] elements) {
		return removeAll(elements);
	}

	public boolean containsAll(ExtendedList elements) {
		for (int i = elements.size() - 1; i >= 0; i--) {
			if (!contains(elements.get(i)))
				return false;
		}
		return true;
	}

	public final boolean containsAll(Object[] elements) {
		return removeAll(elements);
	}

	public String toString() {
		StringBuffer buf = new StringBuffer(256);
		buf.append("[ ");
		int size = size();
		for (int i = 0; i < size; i++) {
			Object obj = get(i);
			if (i > 0) buf.append(", ");
			buf.append(obj.toString());
		}
		buf.append(" ]");
		return buf.toString();
	}
}