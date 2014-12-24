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
 * File created on 21.12.2004.
 */

package com.scriptographer.ai;

import java.util.ArrayList;
import java.util.EnumSet;

import com.scratchdisk.util.IntMap;
import com.scratchdisk.util.IntegerEnumUtils;
import com.scriptographer.ScriptographerException;
import com.scriptographer.sg.CoordinateSystem;
import com.scriptographer.ui.ImageBase;

/**
 * The Tool object refers to the Scriptographer tool in the Illustrator tool
 * palette and can be accessed through the global {@code tool} variable. All its
 * properties are also available in the global scope.
 * 
 * The global {@code tool} variable only exists in scripts that contain mouse
 * handler functions ({@link #getOnMouseDown()}, {@link #getOnMouseDrag()},
 * {@link #getOnMouseUp()}), which are automatically associated with the tool
 * button on execution.
 * 
 * Sample code:
 * <code>
 * var path;
 * 
 * // Only execute onMouseDrag when the mouse
 * // has moved at least 10 points:
 * tool.distanceThreshold = 10;
 * 
 * function onMouseDown(event) {
 * 	// Create a new path every time the mouse is clicked
 * 	path = new Path();
 * }
 * 
 * function onMouseDrag(event) {
 * 	// Add a point to the path every time the mouse is dragged
 * 	path.lineTo(event.point);
 * }
 * </code>
 * 
 * @author lehni
 */
public class Tool extends ToolHandler {
	// TODO: implement a way to set cursors?
	private int cursor = 128;

	/**
	 * tools maps tool handles to their wrappers.
	 */
	private static IntMap<Tool> tools = new IntMap<Tool>();
	private static ArrayList<Tool> unusedTools = null;

	private ImageBase image;
	private ImageBase rolloverImage;
	private ImageBase defaultImage;

	private String name;

	private String defaultTooltip;


	/**
	 * @jshide
	 */
	public Tool(String name, ImageBase image, EnumSet<ToolOption> options,
			Tool groupTool, Tool toolsetTool) {
		this.name = name;
		defaultImage = image;

		ArrayList<Tool> unusedTools = getUnusedTools();

		// Now see first whether there is an unusedEffect already that fits this
		// description
		int index = unusedTools.indexOf(this);
		if (index >= 0) {
			// Found one, let's reuse it's handle and remove the old effect from
			// the list:
			Tool tool = unusedTools.get(index);
			handle = tool.handle;
			tool.handle = 0;
			unusedTools.remove(index);
		} else {
			// No previously existing effect found, create a new one:
			handle = nativeCreate(name,
					image != null ? image.createIconHandle() : 0,
					IntegerEnumUtils.getFlags(options),
					groupTool != null ? groupTool.handle : 0,
					toolsetTool != null ? toolsetTool.handle : 0);
		}

		if (handle == 0)
			throw new ScriptographerException("Unable to create Tool.");

		reset();

		tools.put(handle, this);
	}

	public Tool(String name, ImageBase image, EnumSet<ToolOption> options,
			Tool groupTool) {
		this(name, image, options, groupTool, null);
	}

	public Tool(String name, ImageBase image, EnumSet<ToolOption> options) {
		this(name, image, options, null, null);
	}

	public Tool(String name, ImageBase image) {
		this(name, image, null, null, null);
	}

	public Tool(String name) {
		this(name, null, null, null, null);
	}

	private native int nativeCreate(String name, int iconHandle, int options,
			int groupHandle, int toolsetHandle);

	protected Tool(int handle, String name) {
		super(handle);
		// See resourceIds.h:
		this.name = name;
	}

	public void initialize() {
		super.initialize();
		setEventInterval(-1);
	}

	public void reset() {
		initialize();
		if (defaultImage != null)
			setImage(defaultImage);
		if (defaultTooltip != null)
			setTooltip(defaultTooltip);
	}

	/**
	 * Checks whether the input device of the user has a pressure feature (i.e.
	 * a drawing tablet)
	 */
	public native boolean hasPressure();

	/**
	 * Sets the fixed time delay between each call to the {@link #onMouseDrag}
	 * event. Setting this to an interval means the {@link #onMouseDrag} event
	 * is called repeatedly after the initial {@link #onMouseDown} until the
	 * user releases the mouse.
	 * 
	 * Sample code:
	 * <code>
	 * // Fire the onMouseDrag event once a second,
	 * // while the mouse button is down
	 * tool.eventInterval = 1000;
	 * </code>
	 * 
	 * @return the interval time in milliseconds
	 */
	public native int getEventInterval();
	
	public native void setEventInterval(int interval);
	
	/**
	 * @deprecated use Tool#setEventInterval instead.
	 * 
	 * @jshide
	 */
	public void setIdleEventInterval(int interval) {
		setEventInterval(interval);
	}
	
	/**
	 * @jshide
	 */
	public native String getTitle();

	/**
	 * @jshide
	 */
	public native void setTitle(String title);

	/**
	 * {@grouptitle Tool Button Styling}
	 * 
	 * The tooltip as seen when you hold the cursor over the tool button.
	 */
	public native String getTooltip();

	private native void nativeSetTooltip(String text);

	public void setTooltip(String text) {
		if (defaultTooltip == null)
			defaultTooltip = text;
		nativeSetTooltip(text);
	}

	private native int nativeGetOptions();

	private native void nativeSetOptions(int options);

	/**
	 * @jshide
	 */
	public native boolean getSelected();

	/**
	 * @jshide
	 */
	public native void setSelected(boolean selected);

	/**
	 * @jshide
	 */
	public EnumSet<ToolOption> getOptions() {
		return IntegerEnumUtils.getSet(ToolOption.class, nativeGetOptions());
	}

	/**
	 * @jshide
	 */
	public void setOptions(EnumSet<ToolOption> options) {
		nativeSetOptions(IntegerEnumUtils.getFlags(options));
	}

	public ImageBase getImage() {
		return image;
	}

	private native void nativeSetImage(int iconHandle);

	public void setImage(ImageBase image) {
		nativeSetImage(image != null ? image.createIconHandle() : 0);
		this.image = image;
		if (defaultImage == null)
			defaultImage = image;
	}

	public ImageBase getRolloverImage() {
		return rolloverImage;
	}

	private native void nativeSetRolloverImage(int iconHandle);

	public void setRolloverImage(ImageBase image) {
		nativeSetRolloverImage(image != null ? image.createIconHandle() : 0);
		this.rolloverImage = image;
	}

	public void onHandleEvent(ToolEventType type, Point pt, int pressure,
			int modifiers) {
		if (type == ToolEventType.MOUSEDOWN) {
			try {
				Item.collectCreatedItems();
				super.onHandleEvent(type, pt, pressure, modifiers);
				if (Item.hasCreatedItems())
					Document.getActiveDocument().redraw();
			} finally {
				Item.clearCreatedItems();
			}
		} else {
			super.onHandleEvent(type, pt, pressure, modifiers);
		}
	}

	/**
	 * To be called from the native environment. Returns the cursor
	 * id to be set, if any.
	 */
	private static int onHandleEvent(int handle, String selector,
			double x, double y, int pressure, int modifiers) {
		Tool tool = getTool(handle);
		ToolEventType type = ToolEventType.get(selector);
		if (tool != null && type != null) {
			// Make sure we use the right coordinate system to convert the
			// point. It is a bit a shame we have to introduce a local
			// convertPoint(topDown, x, y) function for this, but that's the
			// only easy way to use the native side's coordinate system handling
			// on the point and respect the tools / script's context correctly
			// before actually calling Document.beginExecution()
			Point point = convertPoint(
					CoordinateSystem.TOP_DOWN == (tool.script != null
							? tool.script.getCoordinateSystem()
							: null),
					// Activate the underlying Artboard if the mouse is pressed,
					// as Ai only activates on mouse-up.
					type == ToolEventType.MOUSEDOWN,
					// Update coordinates system when tool is selected, mouse
					// is clicked or released, and when the we're not in a drag
					// movement. During a drag movement, the coordinates won't
					// change and therefore do not need an update.
					type == ToolEventType.SELECT
					|| type == ToolEventType.MOUSEDOWN
					|| type == ToolEventType.MOUSEUP
					|| type == ToolEventType.MOUSEMOVE,
					x, y);
			tool.onHandleEvent(type, point, pressure, modifiers);
		}
		// Tell the native side to update the cursor
		return tool.cursor;
	}

	/*
	 * See the comment above in onHandleEvent
	 */
	private static native Point convertPoint(boolean topDownCoordinates,
			boolean activateArtboard, boolean updateCoordinates,
			double x, double y);

	private static Tool getTool(int handle) {
		return tools.get(handle);
	}

	public boolean equals(Object obj) {
		if (obj instanceof Tool) {
			Tool tool = (Tool) obj;
			return name.equals(tool.name);
		}
		return false;
	}

	private static ArrayList<Tool> getUnusedTools() {
		if (unusedTools == null)
			unusedTools = nativeGetTools();
		return unusedTools;
	}

	private static native ArrayList<Tool> nativeGetTools();
}
