LOCAL_PATH := $(call my-dir)

# Include prebuilt shared third_party libraries
MY_THIRD_PARTY_PROJ_PATH := ../../../../third_party
MY_THIRD_PARTY_LIB_PATH := $(MY_THIRD_PARTY_PROJ_PATH)/build/android/libs/$(TARGET_ARCH_ABI)

include $(CLEAR_VARS)
LOCAL_MODULE := zip-prebuilt
LOCAL_SRC_FILES := $(MY_THIRD_PARTY_LIB_PATH)/libzip.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(MY_THIRD_PARTY_PROJ_PATH)/libzip/include
LOCAL_EXPORT_LDLIBS := -lz -llog
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := yajl-prebuilt
LOCAL_SRC_FILES := $(MY_THIRD_PARTY_LIB_PATH)/libyajl.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(MY_THIRD_PARTY_PROJ_PATH)/yajl/include
include $(PREBUILT_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := protobuf-prebuilt
LOCAL_SRC_FILES := $(MY_THIRD_PARTY_LIB_PATH)/libprotobuf.so
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(MY_THIRD_PARTY_PROJ_PATH)/protobuf-2.4.1/include
include $(PREBUILT_SHARED_LIBRARY)

# Include prebuilt shared library JHCommon.so
MY_JHCOMMON_PROJ_PATH := ../../../../common
MY_JHCOMMON_LIB_PATH := $(MY_JHCOMMON_PROJ_PATH)/proj/android/libs/$(TARGET_ARCH_ABI)

include $(CLEAR_VARS)
LOCAL_MODULE := JHCommon-prebuilt
LOCAL_SRC_FILES := $(MY_JHCOMMON_LIB_PATH)/libJHCommon.so
LOCAL_EXPORT_C_INCLUDES := $(shell find $(LOCAL_PATH)/$(MY_JHCOMMON_PROJ_PATH)/include -not -ipath ".*\.svn*"  -type d)
include $(PREBUILT_SHARED_LIBRARY)


# build JHRenderEngine.so
include $(CLEAR_VARS)
LOCAL_MODULE := JHRenderEngine
LOCAL_CPP_EXTENSION := .cc .cpp .mm

MY_PREFIX := $(LOCAL_PATH)/
MY_RELATIVE_PATH := ../../../
MY_ABS_PATH := $(MY_PREFIX)/$(MY_RELATIVE_PATH)

LOCAL_C_INCLUDES := \
    $(MY_ABS_PATH)/../third_party  \
	$(shell find $(MY_ABS_PATH)/include -not -ipath ".*\.svn*"  -type d)
MY_SRC_FILES := $(shell find $(MY_ABS_PATH)/src -iregex ".*\.cpp" -o -iregex ".*\.cc" -o -iregex ".*\.mm"  -type f)
LOCAL_SRC_FILES := $(MY_SRC_FILES:$(MY_PREFIX)%=%)

LOCAL_SHARED_LIBRARIES := zip-prebuilt protobuf-prebuilt yajl-prebuilt JHCommon-prebuilt
LOCAL_LDLIBS := -lGLESv2
#LOCAL_LDLIBS := -lGLESv2 -llog -lz -lstdc++ -lc -lm -landroid -ldl

include $(BUILD_SHARED_LIBRARY)
