#include <jni.h>
#include <string>
#include <sys/syscall.h>
#include <unistd.h>
#include <android/log.h>
#include "xhook.h"
#include "libjson/json.h"
//#if !defined (myhook)
#define TAG "360Qiyemono"
#define DUALLOGD(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__);//printf(__VA_ARGS__)
int (*oldopen)(const char *pathname, int flags,...);
//int newopen(const char *pathname, int flags,...);

void removeendl(std::string &s)
{
    while(1)
    {
        std::string::iterator it = find(s.begin(), s.end(), '\n');
        if(it == s.end())
        {
            break;
        }
        s.erase(it);
    }
    while(1)
    {
        std::string::iterator it = find(s.begin(), s.end(), '\t');
        if(it == s.end())
        {
            break;
        }
        s.erase(it);
    }
}
int newopen(const char * pathname, int flags,...){
    int ret;
    //ret = oldopenn(pathname,flags,mode);

    //parameter["mode"]=mode;
    //ret = oldopen(pathname, flags);
    // Handle the optional third argumen
    mode_t mode=NULL;
    va_list args={0};
    va_start(args, flags);
    //DUALLOGD(inttopchar(sizeof(args)));
    if(sizeof(args)==0){
        ret=oldopen(pathname,flags);
    }else{
        mode = va_arg(args, mode_t);
        ret=oldopen(pathname,flags,mode);
    }

    //mode = (mode_t)va_arg(args, int);
    va_end(args);
    DUALLOGD("%s",pathname)
    //logshow("libc.so","open",parameter,"");//(char*)ret);inttopchar(ret)
    return ret;
}
extern "C" JNIEXPORT jstring

JNICALL
Java_com_qiye_txz_plthook_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    xhook_register(".*\\.so$", "open",(void*)&newopen,    (void**)&oldopen);
    xhook_refresh(false);
    std::string hello = "Hello from C++";
    return NULL;
}
