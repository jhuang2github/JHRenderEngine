LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := protobuf
LOCAL_MODULE_TAGS := optional

LOCAL_CPP_EXTENSION := .cc

MY_PREFIX := $(LOCAL_PATH)/
MY_RELATIVE_PATH := ../..
MY_ABS_PATH := $(MY_PREFIX)/$(MY_RELATIVE_PATH)
LOCAL_C_INCLUDES := $(MY_ABS_PATH)/include
#MY_SRC_FILES := $(wildcard $(MY_ABS_PATH)/src/google/protobuf/*.cc)
#LOCAL_SRC_FILES := $(MY_SRC_FILES:$(MY_PREFIX)%=%)

MY_SRC_PATH := $(MY_RELATIVE_PATH)/src/google/protobuf
CC_LITE_SRC_FILES := \
    $(MY_SRC_PATH)/stubs/common.cc                              \
    $(MY_SRC_PATH)/stubs/once.cc                                \
    $(MY_SRC_PATH)/extension_set.cc                             \
    $(MY_SRC_PATH)/generated_message_util.cc                    \
    $(MY_SRC_PATH)/message_lite.cc                              \
    $(MY_SRC_PATH)/repeated_field.cc                            \
    $(MY_SRC_PATH)/wire_format_lite.cc                          \
    $(MY_SRC_PATH)/io/coded_stream.cc                           \
    $(MY_SRC_PATH)/io/zero_copy_stream.cc                       \
    $(MY_SRC_PATH)/io/zero_copy_stream_impl_lite.cc

LOCAL_SRC_FILES := $(CC_LITE_SRC_FILES)


#libstdc++
#LOCAL_SHARED_LIBRARIES := libz libcutils libutils
#LOCAL_LDLIBS := -lz -llog

include $(BUILD_SHARED_LIBRARY)
#include $(BUILD_STATIC_LIBRARY)



# stlport conflicts with the host stl library
#ifneq ($(TARGET_SIMULATOR),true)
#LOCAL_C_INCLUDES += external/stlport/stlport
#LOCAL_SHARED_LIBRARIES += libstlport
#endif

#LOCAL_CFLAGS := -DGOOGLE_PROTOBUF_NO_RTTI

