#include <jni.h>
#include "javajni_JavaJNI.h"
#include <stdio.h>

JNIEXPORT void JNICALL Java_javajni_JavaJNI_nativePrint(JNIEnv *, jobject)
{
    printf("Hello there!\n");
}
