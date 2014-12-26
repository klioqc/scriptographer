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

package com.scriptographer.widget;

import java.awt.Container;
import java.awt.MediaTracker;
import java.awt.Toolkit;
import java.awt.image.BufferedImage;
import java.awt.image.DataBufferInt;
import java.awt.image.ImageProducer;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;

import com.scriptographer.ai.Raster;
import com.scriptographer.ui.ImageBase;
import com.scriptographer.ui.ImageType;
import com.scriptographer.ui.NativeObject;
import com.scriptographer.ui.Size;

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
		
	}
	
	public Image(URL url) throws IOException {
			super(url);
	}
	
	/**
	 * @param width
	 * @param height
	 * @param type
	 */
	public Image(int width, int height, ImageType type) {
		// TODO Auto-generated constructor stub
		super(width, height, type);
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
	
	protected native int nativeCreate(int width, int height, int type);

	protected native void nativeDestroy(int handle, int iconHandle);

	protected native void nativeSetPixels(int[] data, int width, int height,
			int byteWidth);

	protected native void nativeGetPixels(int[] data, int width, int height,
			int byteWidth);

	protected native void nativeSetPixels(int handle, int numBytes);

	protected native int nativeCreateIcon();
	
}
	
