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
 * File created on 29.12.2004.
 */

package com.scriptographer.adm;

import java.io.IOException;


import java.io.File;
import java.net.URL;

import com.scriptographer.ui.ImageBase;
import com.scriptographer.ui.ImageType;



/**
 * @author lehni
 */
public class Image extends ImageBase {
	
	/**
	 * @param file
	 * @throws IOException
	 */
	public Image(File file) throws IOException {
		super(file);
		// TODO Auto-generated constructor stub
	}


	/**
	 * @param width
	 * @param height
	 * @param rgb
	 */
	public Image(int width, int height, ImageType rgb) {
		// TODO Auto-generated constructor stub
		super(width, height, rgb);
	}


	/**
	 * @param resource
	 * @throws IOException 
	 */
	public Image(URL resource) throws IOException {
		super(resource);
	}

	public Object clone() {
		Image copy = new Image(width, height, type);
		copy.nativeSetPixels(handle, byteWidth * height);
		return copy;
	}

	public int createIconHandle() {
		Image img = (Image) clone();
		int handle = img.nativeCreateIcon();
		// clear the handle so that nothing happens in finalize or destroy!
		img.handle = 0;
		img.iconHandle = 0;
		return handle;
	}
	
	private Drawer drawer;

	
	public Drawer getDrawer() {
		if (drawer == null)
			drawer = new Drawer(nativeBeginDrawer(), this, false);
		return drawer;
	}
	
	public void dispose() {
		if (handle != 0) {
			if (drawer != null) {
				drawer.dispose();
				drawer = null;
			}
			nativeDestroy(handle, iconHandle);
			handle = 0;
			iconHandle = 0;
		}
	}
	
	public void finalize() {
		dispose();
	}
	
	/**
	 * called by Drawer.dispose
	 */
	protected void endDrawer() {
		if (drawer != null) {
			nativeEndDrawer();
			drawer = null;
		}
	}
	

	protected native int nativeCreate(int width, int height, int type);

	protected native void nativeDestroy(int handle, int iconHandle);

	protected native void nativeSetPixels(int[] data, int width, int height,
			int byteWidth);

	protected native void nativeGetPixels(int[] data, int width, int height,
			int byteWidth);

	protected native void nativeSetPixels(int handle, int numBytes);

	protected native int nativeCreateIcon();
	protected native int nativeBeginDrawer();

	protected native void nativeEndDrawer();

}
