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
 * File created on Feb 23, 2010.
 */

package com.scriptographer.ui;

import java.util.LinkedHashMap;
import java.util.Map;

import com.scratchdisk.script.Callable;
import com.scratchdisk.script.PropertyObserver;
import com.scratchdisk.script.ScriptEngine;
import com.scriptographer.CommitManager;
import com.scriptographer.Committable;
import com.scriptographer.ScriptographerEngine;

/**
 * @author lehni
 * 
 */
public class Palette implements PropertyObserver, Committable {

	private String title;
	private Map<String, Object> values;
	private Map<String, Object> components;
	private boolean sizeChanged;
	private boolean hasLabels;

	private PaletteProxy proxy;

	public Palette(String title, Map<String, Object> components,
			Map<String, Object> values) {
		if (values != null) {
			// Observer all existing properties for changes
			for (Object key : values.keySet())
				ScriptEngine.observeChanges(values, key, this);	
		} else {
			values = new LinkedHashMap<String, Object>();
		}
		if (components == null)
			components = new LinkedHashMap<String, Object>();
		this.title = title;
		this.values = values;
		this.components = components;
		Component[] comps =
			Component.getComponents(components, values);
		hasLabels = false;
		// Store references to palette in each component, and determine if there
		// are labels
		for (Component component : comps) {
			if (component != null) {
				component.palette = this;
				String label = component.getLabel();
				if (label != null && !"".equals(label))
					hasLabels = true;
			}
		}
		proxy = UiFactory.getInstance().createPalette(this, comps);
	}

	public Palette(String title, Map<String, Object> components) {
		this(title, components, null);
	}

	public Map<String, Object> getValues() {
		return values;
	}

	/**
	 * The components that belong to the palette.
	 */
	public Map<String, Object> getComponents() {
		return components;
	}

	/**
	 * The title that appears at the top of the palette window.
	 */
	public String getTitle() {
		return title;
	}

	/**
	 * @jshide
	 */
	public boolean hasLabels() {
		return hasLabels;
	}

	/**
	 * Resets the values of the components to their {@link Component#getDefaultValue}.
	 */
	public void reset() {
		for (Object component : components.values()) {
			if (component instanceof Component)
				((Component) component).reset();
		}
	}

	/**
	 * @jshide
	 */
	public Component getComponent(String name) {
		// components only contains PaletteComponent after initialization,
		// but is not declared in this way as the passed components object
		// is reused and PaletteComponent are put pack into it. This gives
		// easy access to them on the Scripting side.
		Object component = components.get(name);
		return component instanceof Component ? (Component) component : null;
	}

	private Callable onChange = null;
	private boolean isChanging = false;

	/**
	 * The function that is called whenever the value of one of the
	 * {@link Palette#getComponents()} changes. The function receives the
	 * component of which the value has changed as an argument.
	 */
	public Callable getOnChange() {
		return onChange;
	}

	public void setOnChange(Callable onChange) {
		this.onChange = onChange;
	}

	protected void onChange(Component component, String name, Object value,
			boolean callback) {
		if (!values.containsKey(name)) {
			// Make sure we observe new fields too. This has to do with the
			// nature of change observing on JavaScript, where observers can
			// only be installed for existing properties.
			// So add a null property under that name now, then register the 
			// observer to make sure it can register. The change after then
			// trigers straight away.
			values.put(name, null);
			ScriptEngine.observeChanges(values, name, this);	
		}
		isChanging  = true;
		values.put(name, value);
		isChanging = false;
		// Use CommitManager functionality to update this dialog once
		// after all value changes.
		CommitManager.markDirty(this, this);
		if (callback && onChange != null)
			ScriptographerEngine.invoke(onChange, this, component);
	}

	protected void onSizeChanged() {
		sizeChanged = true;
		// Use CommitManager functionality to update this dialog once
		// after all value changes.
		CommitManager.markDirty(this, this);
	}

	public void commit(boolean endExecution) {
		if (proxy != null)
			proxy.update(sizeChanged);
		sizeChanged = false;
	}

	/**
	 * @jshide
	 */
	public void onChangeProperty(Map object, Object key, Object value) {
		if (!isChanging) {
			Component component = getComponent(key.toString());
			if (component != null)
				component.setValue(value);
		}
	}
}
