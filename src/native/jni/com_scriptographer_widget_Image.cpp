#include "StdHeaders.h"
#include "ScriptographerEngine.h"
#include "com_scriptographer_widget_Image.h"

/*
 * com.scriptographer.widget.Image
 */

/*
 * int nativeCreate(int arg1, int arg2, int arg3)
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_widget_Image_nativeCreate(
		JNIEnv *env, jobject obj, jint width, jint height, jint type) {
	try {

#ifdef WIN_ENV

		HBITMAP image = NULL;
		
    switch(type) {
			case 0:
				image = CreateBitmap(width, height, 8,8,NULL);
				break;
			case 1:
         
          image = CreateCompatibleBitmap ( GetDC(NULL), width, height );
				//image = sADMImage->Create(width, height, kADMImageHasAlphaChannelOption);
				break;
			case 2:
        image = CreateBitmap(width, height, 8,8,NULL);
			
				break;
			case 3:
        image = CreateBitmap(width, height, 8,8,NULL);
			//	image = sADMImage->CreateOffscreen(width, height, kADMImageHasAlphaChannelOption);
				break;
		}

		gEngine->setIntField(env, obj, gEngine->fid_widget_Image_byteWidth, width / 8);
		gEngine->setIntField(env, obj, gEngine->fid_widget_Image_bitsPerPixel,  8);
  
		return (jint)image;

 #endif
	} EXCEPTION_CONVERT(env)
	return 0;
}

/*
 * void nativeDestroy(int arg1, int arg2)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Image_nativeDestroy(
		JNIEnv *env, jobject obj, jint handle, jint iconHandle) {
	try {
		if (handle != 0) DeleteObject((HGDIOBJ)handle);
		if (iconHandle != 0) DeleteObject((HGDIOBJ) iconHandle);
	} EXCEPTION_CONVERT(env);
}

/*
 * void nativeSetPixels(int[] arg1, int arg2, int arg3, int arg4)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Image_nativeSetPixels___3IIII(
		JNIEnv *env, jobject obj, jintArray data, jint width, jint height, jint byteWidth) {
	try {
		HBITMAP image = gEngine->getImageHandle(env, obj);
		jint len = env->GetArrayLength(data);
		char *src = (char *) env->GetPrimitiveArrayCritical(data, 0);
		if (data == NULL) EXCEPTION_CHECK(env);
		//char *dst = (char *) sADMImage->BeginBaseAddressAccess(image); 
	/*
		// we're copying int rgb(a) values, so *4:
		width *= 4;
		for (int y = 0; y < height; y++) {
#ifdef __i386__ // TODO: figure out when ARGB to RGBA switch is needed and when not
			for (int x = 0; x < width; x += 4) {
				dst[x + 0] = src[x + 3]; // A
				dst[x + 1] = src[x + 2]; // R
				dst[x + 2] = src[x + 1]; // G
				dst[x + 3] = src[x + 0]; // B
			}
#else
			memcpy(dst, src, width);
#endif
			src += width;
			dst += byteWidth;
		}
    */
		env->ReleasePrimitiveArrayCritical(data, src, 0);
	//	sADMImage->EndBaseAddressAccess(image); 
	} EXCEPTION_CONVERT(env);
}

/*
 * void nativeGetPixels(int[] arg1, int arg2, int arg3, int arg4)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Image_nativeGetPixels(JNIEnv *env, jobject obj, jintArray data, jint width, jint height, jint byteWidth) {
	try {
		HBITMAP image = gEngine->getImageHandle(env, obj);
		jint len = env->GetArrayLength(data);
		char *dst = (char *)env->GetPrimitiveArrayCritical(data, 0);
		if (data == NULL) EXCEPTION_CHECK(env);
	/*	char *src = (char *)sADMImage->BeginBaseAddressAccess(image); 
		
		// we're copying int rgb(a) values, so *4:
		width *= 4;
		for (int y = 0; y < height; y++) {
#ifdef __i386__ // TODO: figure out when ARGB to RGBA switch is needed and when not
			for (int x = 0; x < width; x += 4) {
				dst[x + 0] = src[x + 3]; // B
				dst[x + 1] = src[x + 2]; // G
				dst[x + 2] = src[x + 1]; // R
				dst[x + 3] = src[x + 0]; // A
			}
#else
			memcpy(dst, src, width);
#endif
			src += byteWidth;
			dst += width;
		}
		*/
		env->ReleasePrimitiveArrayCritical(data, dst, 0);
		//sADMImage->EndBaseAddressAccess(image); 
	} EXCEPTION_CONVERT(env);
}
/*
 * void nativeSetPixels(int arg1, int arg2)
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Image_nativeSetPixels__II(
		JNIEnv *env, jobject obj, jint handle, jint numBytes) {
try {
		HBITMAP dstImage = gEngine->getImageHandle(env, obj);
		HBITMAP srcImage = (HBITMAP) handle;
	/*	char *src = (char *) sADMImage->BeginBaseAddressAccess(srcImage); 
		char *dst = (char *) sADMImage->BeginBaseAddressAccess(dstImage); 
		
		memcpy(dst, src, numBytes);
		
		sADMImage->EndBaseAddressAccess(srcImage); 
		sADMImage->EndBaseAddressAccess(dstImage); */
	} EXCEPTION_CONVERT(env); 
}

/*
 * int nativeCreateIcon()
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_widget_Image_nativeCreateIcon(
		JNIEnv *env, jobject obj) {
  try {
	  HBITMAP image = gEngine->getImageHandle(env, obj);
		
	} EXCEPTION_CONVERT(env);
  return 0;
}

/*
 * int nativeBeginDrawer()
 */
JNIEXPORT jint JNICALL Java_com_scriptographer_widget_Image_nativeBeginDrawer(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define nativeBeginDrawer
	} EXCEPTION_CONVERT(env);
	return 0;
}

/*
 * void nativeEndDrawer()
 */
JNIEXPORT void JNICALL Java_com_scriptographer_widget_Image_nativeEndDrawer(
		JNIEnv *env, jobject obj) {
	try {
		// TODO: define nativeEndDrawer
	} EXCEPTION_CONVERT(env);
}
