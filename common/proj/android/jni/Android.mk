LOCAL_PATH := $(call my-dir)

# Include prebuilt shared libraries
MY_THIRD_PARTY_PROJ_PATH := ../../../../third_party
MY_THIRD_PARTY_LIB_PATH := $(MY_THIRD_PARTY_PROJ_PATH)/build/android/libs/$(TARGET_ARCH_ABI)

# libzip
include $(CLEAR_VARS)
LOCAL_MODULE := zip-prebuilt
LOCAL_SRC_FILES := $(MY_THIRD_PARTY_LIB_PATH)/libzip.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(MY_THIRD_PARTY_PROJ_PATH)/libzip/include
LOCAL_EXPORT_LDLIBS := -lz -llog
#LOCAL_EXPORT_CFLAGS := -DDEBUG=1
include $(PREBUILT_SHARED_LIBRARY)

# libyajl
include $(CLEAR_VARS)
LOCAL_MODULE := yajl-prebuilt
LOCAL_SRC_FILES := $(MY_THIRD_PARTY_LIB_PATH)/libyajl.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(MY_THIRD_PARTY_PROJ_PATH)/yajl/include
include $(PREBUILT_SHARED_LIBRARY)


# build JHCommon.so
include $(CLEAR_VARS)
LOCAL_MODULE := JHCommon
LOCAL_CPP_EXTENSION := .cpp .mm

MY_PREFIX := $(LOCAL_PATH)/
MY_RELATIVE_PATH := ../../../
MY_ABS_PATH := $(MY_PREFIX)/$(MY_RELATIVE_PATH)

# Glob all the source files in all subdirectories recursively.
# $(wildcard $(MY_ABS_PATH)/include/*), $(wildcard $(MY_ABS_PATH)/src/*.cpp)
LOCAL_C_INCLUDES := $(shell find $(MY_ABS_PATH)/include -not -ipath ".*\.svn*"  -type d)
MY_SRC_FILES := $(shell find $(MY_ABS_PATH)/src -iregex ".*\.cpp" -o -iregex ".*\.mm"  -type f)
LOCAL_SRC_FILES := $(MY_SRC_FILES:$(MY_PREFIX)%=%)

LOCAL_SHARED_LIBRARIES := zip-prebuilt

include $(BUILD_SHARED_LIBRARY)
