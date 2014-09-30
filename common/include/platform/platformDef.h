//
//  platformdef.h
//
//  Created by Jingshu Huang on 12/20/12.
//  Copyright (c) HuangImage 2013. All rights reserved.
//

#ifndef _platformdef_h
#define _platformdef_h

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>


using namespace std;


#ifdef __APPLE__  // iOS specific

#define  LOGI(...)  printf(__VA_ARGS__); printf("\n")
#define  LOGE(...)  printf(__VA_ARGS__); printf("\n")
#define  LOGD(...)  printf(__VA_ARGS__); printf("\n")
//#define assert(condition) if(!(condition)) { printf("failed -- " #condition); }

#include <pthread.h>
//#include <ext/hash_map>
//#include <ext/hash_set>

//using namespace __gnu_cxx;

//// Add support for hash_map<string, ...>
//namespace __gnu_cxx
//{
//    template<> struct hash<std::string>
//    {
//        size_t operator()(const std::string& x) const
//        {
//            return hash<const char*>()(x.c_str());
//        }
//    };
//
//}


inline static pid_t getThreadID() {
//    return pthread_self();
    return getpid(); // TODO(jhuang): fix this.
}

#else // Android specific

#include <jni.h>
#include <android/log.h>
#include <unistd.h>  // for gettid(). Not in <sys/types.h>
//#include <hash_map>
//#include <hash_set>

using namespace std::tr1;

// Note this is tag only for JNI. Java one is defined in AppConfig.java
#define  LOG_TAG    "JHEngineJni"

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

static pid_t getThreadID() {
    return gettid();
}

#endif


////  Define for both platforms  //////

// Get only the basename for a file
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)


typedef unsigned char  byte;
typedef unsigned char  uchar;
typedef unsigned char  uchar4[4];
typedef unsigned short ushort;
typedef unsigned int   uint;

typedef float float2[2];
typedef float float3[3];
typedef float float4[4];
typedef float float9[9];
typedef float float12[12];
typedef float float16[16];
typedef float float4x4[4][4];

typedef float Radian;
typedef float Degree;

typedef size_t stringId;

#define kRadianPerDegree M_PI / 180.f
#define deg2rad(X) (kRadianPerDegree * X)

typedef unordered_map<string, string> KeyValuePairStringMap;
typedef unordered_map<string, void*>  KeyValuePairObjectMap;

//template <typename T>
//class KeyValuePairMapT {
//public:
//    typedef unordered_map<string, T> PairMap;
//    PairMap map;
//};
//typedef KeyValuePairMapT<string>  KeyValuePairStringMap;
//typedef KeyValuePairMapT<void*>   KeyValuePairObjectMap;


struct eqstr {
    bool operator()(const char* s1, const char* s2) const {
        return strcmp(s1, s2) == 0;
    }
};

    
#endif
