#include <jni.h>
#include <string>
#include <string.h>
// 引入md5头文件即可，不要引入md5.cpp
#include "md5.h"
// 导入android的打印log的包
#include <android/log.h>


// 额外附加的字符串
static char* EXTRA_SIGNATURE = "NOVATE" ;
// 声明命名空间
using namespace std;


// 校验签名
// 校验成功的变量
static int is_verify = 0 ;
// 包名
static char *PACKAGE_NAME = "com.yao.ndkdemo" ;
// 签名：实际中应该是打包后的签名，这里为了测试，是在MainActivity中直接用java的方式打印出的签名，然后下边用 c代码调用java代码
static char *APP_SIGNATURE = "308202e2308201ca020101300d06092a864886f70d010105050030373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b3009060355040613025553301e170d3139303832373135303733355a170d3439303831393135303733355a30373116301406035504030c0d416e64726f69642044656275673110300e060355040a0c07416e64726f6964310b300906035504061302555330820122300d06092a864886f70d01010105000382010f003082010a0282010100b6c41c1f251309b34434b67f02760263c86419198c764365686794924c472088ce319699258aeb5857f82872927161a551060d0cff8cd5fa6b377f938aa8f7b44b0504443565360f0cb059a2fac741d987546907c3ccaf8f6a00c0ac57370ceaf25c577129eee2fd10e2b7c3eef2c0aede4ae8d36857d9588ed11c11d868d728f9a6fb1f0211b063f3a2270c3bb5b53863d046cdb73c1b97b5dbbfebaf12e2e8222f585f1791f308ab11d5273155c5983fa804615c7a1f4f7ad4a5eb8508d23766720aa33a088cc0452088d989c0e14674b3eb5ed3f7476d9f08adde40bb1077e7fb9b9a4efa9f6f849399efd20f40df9d4da2ceb5be5cc61d0a195d690758870203010001300d06092a864886f70d0101050500038201010044fa9ac586b86f6dafb7eb5c1867b039ab69818573d287d6c5e79af23c65caba4451e62f7d55737c234e553d49ebcc693b3b7f72f5ed9b5e4d7e40c76aaf585ae744d51f1d4dc8e0f5527b85840f183a9bce0237c90f80b3d4c9ea07a29f5873eaaf4323072c7d9cec0c2b42c6f506cc50b19925f81cfcaf6819b0c0e5c6442e14a87914c44a3d2f3aa79eb22282a654379072b92fc88005b4488c8f1a5c45293bcbfce963518ad6099f2e242cd13918a283c58620e02fa73ecbd4e23fcc024517c1c41264a52d200ab8160805e8598314eac8efe166f5de58f122036a41af11cc9ecb16fba90aba8dd5b2649afd04590035c499df3284cfcb47d9e577456924" ;

extern "C"{

// 在这里对下边方法声明一下，或者写一个.h头文件也行
JNIEXPORT jstring JNICALL
Java_com_yao_ndkdemo_SignatureUtils_signatureParams(JNIEnv *env, jclass type, jstring params_);

// 在这里对下边方法声明一下
JNIEXPORT void JNICALL
Java_com_yao_ndkdemo_SignatureUtils_signatureVerify(JNIEnv *env, jclass type, jobject context);
}



JNIEXPORT jstring JNICALL
Java_com_yao_ndkdemo_SignatureUtils_signatureParams(JNIEnv *env, jclass type, jstring params_) {
    const char *params = env->GetStringUTFChars(params_, 0);


    // 在这里判断校验是否成功，如果成功直接return
    if(is_verify == 0){
        return env -> NewStringUTF("error_signature") ;
    }



    // TODO
    // MD5签名规则，就是说加点料：比如：
    // 1. 字符串前边加一个名字;
    string signature_str(params) ;
    signature_str.insert(0,EXTRA_SIGNATURE);
    // 2. 后边去掉两位; 或者其他，这个规则自己随便定义，目的就是一个：就是不想别人破解
    signature_str = signature_str.substr(0,signature_str.length()-2) ;

    // 3. md5加密 C++和Java是一样的，唯一不同的就是需要自己回收内存
    MD5_CTX *ctx = new MD5_CTX() ;
    // 初始化MD5
    MD5Init(ctx) ;
    MD5Update(ctx , (unsigned char *)signature_str.c_str() , signature_str.length()) ;
    // 会把加密的数据放到digest[16]字符串 中，
    unsigned char digest[16] = {0};
    MD5Final(digest , ctx) ;


    // 生成32位字符串
    char md5_str[32] ;
    for (int i = 0 ; i < 16 ; i++){
        // 不足的情况下补0，比如是f，就是0f；
        sprintf(md5_str , "%02x" , md5_str , digest[i]) ;
    }


    // 释放资源
    env->ReleaseStringUTFChars(params_, params);


    // 直接返回signature_str.c_str()即可
    return env->NewStringUTF(md5_str);
}


/**
 * 下边方法是自带的，可以注释掉，不需要下边方法
 */
/*extern "C"
JNIEXPORT jstring JNICALL
Java_cn_novate_ndk_day01_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject *//* this *//*) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}*/


/**
PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), PackageManager.GET_SIGNATURES);
Signature[] signatures = packageInfo.signatures;
return signatures[0].toCharsString();
 */

// C 调用 java 代码，这个其实就是 JNI的基础
JNIEXPORT void JNICALL
Java_com_yao_ndkdemo_SignatureUtils_signatureVerify(JNIEnv *env, jclass type, jobject context) {

    // TODO
    // 1. 获取包名
    jclass j_clz = env->GetObjectClass(context) ;
    // getPackageName:表示方法名；
    // ()Ljava/lang/String; ：表示方法参数；
    // ()：表示无参构造方法
    jmethodID j_mid = env->GetMethodID(j_clz , "getPackageName" , "()Ljava/lang/String;") ;
    jstring j_package_name = (jstring) env->CallObjectMethod(context , j_mid);

    // 2. 比对包名是否一样
    const char * c_package_name = env -> GetStringUTFChars(j_package_name , NULL) ;
    // strcmp方法: 字符串比较
    if(strcmp(c_package_name , PACKAGE_NAME) != 0){
        return;
    }

    // 打印log , %s表示占位符  打印结果：包名一致: cn.novate.ndk.day01
//    __android_log_print(ANDROID_LOG_ERROR , "JNI_TAG" , "包名一致: %s" , c_package_name) ;

    // 3. 获取签名
    // 3.1 获取 PackageManager
    j_mid = env -> GetMethodID(j_clz , "getPackageManager" , "()Landroid/content/pm/PackageManager;") ;
    jobject  package_manager = env->CallObjectMethod(context , j_mid) ;
    // 3.2 获取 PackageInfo
    j_clz = env->GetObjectClass(package_manager) ;
    j_mid = env->GetMethodID(j_clz , "getPackageInfo" , "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;") ;
    // 0x00000040 = 64
    jobject package_info = env->CallObjectMethod(package_manager , j_mid , j_package_name , 0x00000040) ;
    // 3.3 获取 signature数组
    j_clz = env -> GetObjectClass(package_info) ;
    jfieldID j_fid = env->GetFieldID(j_clz , "signatures" , "[Landroid/content/pm/Signature;") ;
    jobjectArray signatures = (jobjectArray) env->GetObjectField(package_info , j_fid);
    // 3.4 获取 signature[0]
    jobject signature_first = env->GetObjectArrayElement(signatures,0) ;
    // 3.5 调用 signatures[0].toCharsString()
    j_clz = env->GetObjectClass(signature_first) ;
    j_mid = env->GetMethodID(j_clz , "toCharsString" , "()Ljava/lang/String;") ;
    jstring j_signature_str = (jstring) env->CallObjectMethod(signature_first , j_mid);
    const char * c_signature_str = env->GetStringUTFChars(j_signature_str,NULL) ;
    // 比较字符串
    // 比较签名是否相等，如果不相等，则直接 return；
    if(strcmp(c_signature_str , APP_SIGNATURE) != 0){
        return;
    }


    // 打印下日志
//    __android_log_print(ANDROID_LOG_ERROR , "JNI_TAG" , "签名校验成功：%s" , c_signature_str) ;
    // 4. 比对签名是否一样，如果签名一样，就让 is_verify = 1 ;
    is_verify = 1 ;

}