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
 */

void loadJavaVM(const TCHAR *jvmType, CreateJavaVMProc *createJavaVM, GetDefaultJavaVMInitArgsProc *getDefaultJavaVMInitArgs);
int getMaxHeapAvailable(int maxPermSize, int maxHeapSize, int extraSize);