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
 * File created on Oct 28, 2013.
 */

package com.scriptographer.script.rhino;
import com.scriptographer.ui.Border;
import com.scriptographer.ui.DialogColor;






import com.scriptographer.ai.Annotator;
import com.scriptographer.ai.AreaText;
import com.scriptographer.ai.Artboard;
import com.scriptographer.ai.CMYKColor;
import com.scriptographer.ai.CharacterStyle;
import com.scriptographer.ai.Color;
import com.scriptographer.ai.CompoundPath;
import com.scriptographer.ai.Curve;
import com.scriptographer.ai.Dictionary;
import com.scriptographer.ai.Document;
import com.scriptographer.ai.DocumentView;
import com.scriptographer.ai.FileFormat;
import com.scriptographer.ai.FillStyle;
import com.scriptographer.ai.FontFamily;
import com.scriptographer.ai.FontWeight;
import com.scriptographer.ai.Gradient;
import com.scriptographer.ai.GradientColor;
import com.scriptographer.ai.GradientStop;
import com.scriptographer.ai.GrayColor;
import com.scriptographer.ai.Group;
import com.scriptographer.ai.HitResult;
import com.scriptographer.ai.Layer;
import com.scriptographer.ai.Line;
import com.scriptographer.ai.LiveEffect;
import com.scriptographer.ai.LiveEffectParameters;
import com.scriptographer.ai.Matrix;
import com.scriptographer.ai.ParagraphStyle;
import com.scriptographer.ai.Path;
import com.scriptographer.ai.PathItem;
import com.scriptographer.ai.PathStyle;
import com.scriptographer.ai.PathText;
import com.scriptographer.ai.Pathfinder;
import com.scriptographer.ai.Pattern;
import com.scriptographer.ai.PatternColor;
import com.scriptographer.ai.PlacedFile;
import com.scriptographer.ai.PlacedSymbol;
import com.scriptographer.ai.PointText;
import com.scriptographer.ai.RGBColor;
import com.scriptographer.ai.Raster;
import com.scriptographer.ai.Segment;
import com.scriptographer.ai.StrokeStyle;
import com.scriptographer.ai.Swatch;
import com.scriptographer.ai.Symbol;
import com.scriptographer.ai.TextRange;
import com.scriptographer.ai.TextStory;
import com.scriptographer.ai.Timer;
import com.scriptographer.ai.Tool;
import com.scriptographer.ai.ToolHandler;
import com.scriptographer.ai.Tracing;
import com.scriptographer.ui.Dialog;
import com.scriptographer.ui.Key;
import com.scriptographer.ui.MenuGroup;
import com.scriptographer.ui.MenuItem;
import com.scriptographer.ui.Palette;


import com.scriptographer.widget.Button;
import com.scriptographer.widget.CheckBox;
import com.scriptographer.widget.Frame;
import com.scriptographer.widget.ImageCheckBox;
import com.scriptographer.widget.ImagePane;
import com.scriptographer.widget.FloatingDialog;
import com.scriptographer.widget.Image;
import com.scriptographer.widget.HierarchyListBox;
import com.scriptographer.widget.HierarchyListEntry;
import com.scriptographer.widget.ImageRadioButton;
import com.scriptographer.widget.ItemGroup;
import com.scriptographer.widget.ListBox;
import com.scriptographer.widget.ListEntry;
import com.scriptographer.widget.ListItem;
import com.scriptographer.widget.ModalDialog;
import com.scriptographer.widget.PopupDialog;
import com.scriptographer.widget.PopupList;
import com.scriptographer.widget.PopupMenu;
import com.scriptographer.widget.ProgressBar;
import com.scriptographer.widget.RadioButton;
import com.scriptographer.widget.ScrollBar;
import com.scriptographer.widget.Slider;
import com.scriptographer.widget.Spacer;
import com.scriptographer.widget.SpinEdit;
import com.scriptographer.widget.TextEdit;
import com.scriptographer.widget.TextPane;
import com.scriptographer.widget.TextValueItem;
import com.scriptographer.widget.ImageButton;
import com.scriptographer.widget.ToggleItem;
import com.scriptographer.widget.Tracker;

/**
 * @author Olga
 *
 */
public class WidgetClasses {
	final static Class classes[] = {
		//todo: move to common?
		// AI, alphabetically
		Annotator.class,
		AreaText.class,
		Artboard.class,
		CharacterStyle.class,
		Color.class,
		CMYKColor.class,

		CompoundPath.class,
		Curve.class,
		Dictionary.class,
		Document.class,
		DocumentView.class,
		FileFormat.class,
		FillStyle.class,
		FontFamily.class,
		FontWeight.class,
		Gradient.class,
		GradientColor.class,
		GradientStop.class,
		GrayColor.class,
		Group.class,
		HitResult.class,
		com.scriptographer.ai.Item.class,
		Layer.class,
		Line.class,
		LiveEffect.class,
		LiveEffectParameters.class,
		Matrix.class,
		ParagraphStyle.class,
		Path.class,
		Pathfinder.class,
		PathItem.class,
		PathStyle.class,
		PathText.class,
		Pattern.class,
		PatternColor.class,
		PlacedFile.class,
		com.scriptographer.ai.Point.class,
		PointText.class,
		Raster.class,
		com.scriptographer.ai.Rectangle.class,
		RGBColor.class,
		Segment.class,
		com.scriptographer.ai.Size.class,
		StrokeStyle.class,
		Swatch.class,
		Symbol.class,
		PlacedSymbol.class,
		com.scriptographer.ai.TextItem.class,
		TextRange.class,
		TextStory.class,
		Tool.class,
		ToolHandler.class,
		Tracing.class,
		Timer.class,
		
		// UI, alphabetically
		Dialog.class,
	
		Key.class,
		MenuGroup.class,
		MenuItem.class,
		Palette.class,
		
		Border.class,
		DialogColor.class,
		
		//not adm, but native 
		
		Button.class,	
		CheckBox.class,
	 	FloatingDialog.class,
		Frame.class,
		HierarchyListBox.class,
		HierarchyListEntry.class,
	 	Image.class,
		ImageButton.class,
		ImageCheckBox.class,
		ImageRadioButton.class,
		ImagePane.class,
		ItemGroup.class,
	 	ListBox.class,
	 	ListEntry.class,
		ListItem.class,
	 	ModalDialog.class,
	 	PopupDialog.class,
		PopupList.class,
		PopupMenu.class,
		ProgressBar.class,
		RadioButton.class,
		ScrollBar.class,
		Slider.class,
		Spacer.class,
		SpinEdit.class,
		TextPane.class,
		TextEdit.class,
		TextValueItem.class,
		
		ToggleItem.class,
		Tracker.class

	};
}
