LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := yajl

# Note build script run from the top of the NDK. So $(LOCAL_PATH) starts with "jni".
# $(LOCAL_C_INCLUDES) and $(LOCAL_SRC_FILES) are relative to $(LOCAL_PATH)

#MY_PREFIX := $(LOCAL_PATH)/
#LOCAL_C_INCLUDES := $(MY_PREFIX)/include
#MY_SRC_FILES := $(wildcard $(MY_PREFIX)/src/*.c)
MY_PREFIX := $(LOCAL_PATH)/
MY_RELATIVE_PATH := ../..
MY_ABS_PATH := $(MY_PREFIX)/$(MY_RELATIVE_PATH)
LOCAL_C_INCLUDES := $(MY_ABS_PATH)/include
MY_SRC_FILES := $(wildcard $(MY_ABS_PATH)/src/*.c)
LOCAL_SRC_FILES := $(MY_SRC_FILES:$(MY_PREFIX)%=%)

#LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_STATIC_LIBRARY)
