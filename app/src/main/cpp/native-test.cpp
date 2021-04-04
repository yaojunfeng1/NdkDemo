#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_com_yao_ndkdemo_TestActivity_stringFromJNI1(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++1";
    return env->NewStringUTF(hello.c_str());
}
