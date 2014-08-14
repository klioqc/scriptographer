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

#include "stdHeaders.h"
#include <TCHAR.H>
#include "ScriptographerEngine.h"
#include "loadJava.h"
#include <sys/stat.h>

#define JRE_KEY "Software\\JavaSoft\\Java Runtime Environment"

void getJVMPath(const TCHAR *jrePath, const TCHAR *jvmType, TCHAR *jvmPath) {
	struct _stat s;
	_stprintf(jvmPath, _T("%s\\bin\\%s\\jvm.dll") , jrePath, jvmType);
	if (_tstat(jvmPath, &s) != 0)
		throw new StringException("No JVM of type `%s' found at `%s'", jvmType, jvmPath);
}

bool getStringFromRegistry(HKEY key, const TCHAR *name, TCHAR *buf, jint bufsize) {
	DWORD type, size;
	return (RegQueryValueEx(key, name, 0, &type, 0, &size) == 0
		&& type == REG_SZ
		&& (size < (unsigned int)bufsize)
		&& RegQueryValueEx(key, name, 0, 0, (unsigned char*) buf, &size) == 0);
}

void getJREPath(TCHAR *jrePath) {
	HKEY key, subkey;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T(JRE_KEY), 0, KEY_READ, &key) != 0)
		throw new StringException("Error opening registry key '" JRE_KEY);

	TCHAR version[64];
	if (!getStringFromRegistry(key, _T("CurrentVersion"), version, sizeof(version))) {
		RegCloseKey(key);
		throw new StringException("Failed to read registry key '" JRE_KEY "\\CurrentVersion'");
	}

	if (RegOpenKeyEx(key, version, 0, KEY_READ, &subkey) != 0) {
		RegCloseKey(key);
		throw new StringException("Error opening registry key '" JRE_KEY "\\%s'", version);
	}

	if (!getStringFromRegistry(subkey, _T("JavaHome"), jrePath, MAX_PATH)) {
		RegCloseKey(key);
		RegCloseKey(subkey);
		throw new StringException("Failed to read registry key '" JRE_KEY "\\%s\\JavaHome'", version);
	}

	RegCloseKey(key);
	RegCloseKey(subkey);
}

void loadJavaVM(const TCHAR *jvmType, CreateJavaVMProc *createJavaVM, GetDefaultJavaVMInitArgsProc *getDefaultJavaVMInitArgs) {
	TCHAR jrePath[MAX_PATH];
	getJREPath(jrePath);

	TCHAR jvmPath[MAX_PATH];
	getJVMPath(jrePath, jvmType, jvmPath);

	// load the Java VM DLL
	HINSTANCE handle = LoadLibrary(jvmPath);

	if (handle == NULL)
		throw new StringException("Cannot load JVM at %s", jvmPath);

	// now get the function addresses
	*createJavaVM = (CreateJavaVMProc) GetProcAddress(handle, "JNI_CreateJavaVM");
	*getDefaultJavaVMInitArgs = (GetDefaultJavaVMInitArgsProc) GetProcAddress(handle, "JNI_GetDefaultJavaVMInitArgs");

	if (createJavaVM == NULL || getDefaultJavaVMInitArgs == NULL)
		throw new StringException("Cannot find JNI interfaces in: %s", jvmPath);
}

// Code below from http://forums.sun.com/thread.jspa?threadID=5403763

bool canAllocate(DWORD bytes) {
	LPVOID base = VirtualAlloc(NULL, bytes, MEM_RESERVE, PAGE_READWRITE);
	if (base == NULL)
			return false;
	VirtualFree(base, 0, MEM_RELEASE);
	return true;
}

int getMaxHeapAvailable(int maxPermSize, int maxHeapSize, int extraSize) {
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	// JVM aligns as follows: 
	// Quoted from size_t GenCollectorPolicy::compute_max_alignment() of JDK 7 hotspot code:
	//	  The card marking array and the offset arrays for old generations are
	//	  committed in os pages as well. Make sure they are entirely full (to
	//	  avoid partial page problems), e.g. if 512 bytes heap corresponds to 1
	//	  byte entry and the os page size is 4096, the maximum heap size should
	//	  be 512*4096 = 2MB aligned.

	// card_size computation from CardTableModRefBS::SomePublicConstants of JDK 7 hotspot code
	DWORD alignmentBytes = sysInfo.dwPageSize * (1 << 9);

	// Make it fit in the alignment structure
	DWORD maxHeapBytes = maxHeapSize + (maxHeapSize % alignmentBytes);
	DWORD originalMaxHeapBytes = maxHeapBytes;

	// Loop and decrement requested amount by one chunk
	// until the available amount is found
	int numMemChunks = maxHeapBytes / alignmentBytes;
	while (!canAllocate(maxHeapBytes + maxPermSize + extraSize) && numMemChunks > 0) {
		numMemChunks--;
		maxHeapBytes = numMemChunks * alignmentBytes;
	}

	if (numMemChunks == 0)
		return 0;

	// If we can allocate the requested size, return it instead of the
	// calculated size.
	return maxHeapBytes == originalMaxHeapBytes ? maxHeapSize : maxHeapBytes;
}

