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
 * File created on 23.02.2005.
 */

package com.scriptographer.ai;

import java.util.ArrayList;

import com.scriptographer.ScriptographerEngine;
import com.scriptographer.ScriptographerException;
import com.scratchdisk.script.Callable;
import com.scratchdisk.util.IntMap;
import com.scratchdisk.util.SoftIntMap;
import com.scriptographer.adm.Drawer;

/**
 * @author lehni
 * 
 * @jshide
 */
public class Annotator extends NativeObject {
	private boolean active;

	private static IntMap<Annotator> annotators = new IntMap<Annotator>();
	private static ArrayList<Annotator> unusedAnnotators = null;
	// this is list of drawers that map viewports to created ADM Drawer objects:
	private static SoftIntMap<Drawer> drawers = new SoftIntMap<Drawer>();
	private static int counter = 0;

	public Annotator() {
		// now see first whether there is an unusedEffect already:
		ArrayList unusedAnnotators = getUnusedAnnotators();

		int index = unusedAnnotators.size() - 1;
		if (index >= 0) {
			Annotator annotator = (Annotator) unusedAnnotators.get(index);
			// found one, let's reuse it's handle and remove the old timer from
			// the list:
			handle = annotator.handle;
			annotator.handle = 0;
			unusedAnnotators.remove(index);
		} else {
			handle = nativeCreate("Scriptographer Annotator " + (counter++));
		}

		if (handle == 0)
			throw new ScriptographerException("Unable to create Annotator.");

		active = false;

		annotators.put(handle, this);
	}

	/**
	 * Called from the native environment.
	 */
	protected Annotator(int handle) {
		super(handle);
	}

	private native int nativeCreate(String name);

	/**
	 * @param active if {@code true}, activates the annotator, otherwise
	 *        deactivates it
	 */
	public void setActive(boolean active) {
		if (nativeSetActive(handle, active)) {
			this.active = active;
		}
	}

	public boolean isActive() {
		return active;
	}

	private native boolean nativeSetActive(int handle, boolean active);

	/**
	 * @jshide
	 */
	public void invalidate(int x, int y, int width, int height) {
		nativeInvalidate(handle, x, y, width, height);
	}

	public void invalidate(Rectangle rect) {
		// TODO: implement DocumentView and pass handle to it!
		nativeInvalidate(handle, (int) rect.x, (int) rect.y, (int) rect.width,
				(int) rect.height);
	}

	private native void nativeInvalidate(int viewHandle, int x, int y,
			int width, int height);

	public void dispose() {
		// see whether we're still linked:
		if (annotators.get(handle) == this) {
			// if so remove it and put it to the list of unused timers, for
			// later
			// recycling
			annotators.remove(handle);
			getUnusedAnnotators().add(this);
		}
	}

	/**
	 * @jshide
	 */
	public static void disposeAll() {
		// As remove() modifies the map, using an iterator is not possible here:
		Object[] annotators = Annotator.annotators.values().toArray();
		for (int i = 0; i < annotators.length; i++)
			((Annotator) annotators[i]).dispose();

		// also clean up the port drawers:
		Object[] drawers = Annotator.drawers.values().toArray();
		for (int i = 0; i < drawers.length; i++)
			((Drawer) drawers[i]).dispose();
	}

	private static ArrayList<Annotator> getUnusedAnnotators() {
		if (unusedAnnotators == null)
			unusedAnnotators = nativeGetAnnotators();
		return unusedAnnotators;
	}

	private static native ArrayList<Annotator> nativeGetAnnotators();

	private Callable onDraw = null;

	public void setOnDraw(Callable onDraw) {
		this.onDraw = onDraw;
		this.setActive(onDraw != null);
	}

	public Callable getOnDraw() {
		return onDraw;
	}

	protected void onDraw(Drawer drawer, DocumentView view) {
		if (onDraw != null)
			ScriptographerEngine.invoke(onDraw, this, drawer, view);
	}

	private Callable onInvalidate = null;

	public void setOnInvalidate(Callable onInvalidate) {
		this.onInvalidate = onInvalidate;
	}

	public Callable getOnInvalidate() {
		return onInvalidate;
	}

	protected void onInvalidate() {
		if (onInvalidate != null)
			ScriptographerEngine.invoke(onInvalidate, this);
	}

	/**
	 * To be called from the native environment:
	 */
	private static void onDraw(int handle, int portHandle, int viewHandle,
			int docHandle) {
		Annotator annotator = getAnnotator(handle);
		if (annotator != null) {
			annotator.onDraw(
					createDrawer(portHandle),
					DocumentView.wrapHandle(viewHandle,
							Document.wrapHandle(docHandle)));
		}
	}

	private static void onInvalidate(int handle) {
		Annotator annotator = getAnnotator(handle);
		if (annotator != null) {
			annotator.onInvalidate();
		}
	}

	private static Annotator getAnnotator(int handle) {
		return annotators.get(handle);
	}

	/**
	 * Returns a Drawer for the passed portHandle. The drawers are cashed and
	 * reused for the same port. TODO: Remove dependency from adm.Drawer and
	 * introduce a new ui.Drawer that makes the abstraction behind the scenes.
	 * Or: Use an interface called ui.Drawer and have different implementations
	 * sharing that common interface.
	 * 
	 * @param portHandle
	 */
	private static Drawer createDrawer(int portHandle) {
		Drawer drawer = drawers.get(portHandle);
		if (drawer == null) {
			drawer = nativeCreateDrawer(portHandle);
			drawers.put(portHandle, drawer);
		}
		return drawer;
	}

	private static native Drawer nativeCreateDrawer(int portHandle);

	protected void finalize() {
		dispose();
	}
}
