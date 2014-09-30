CACHED_LOCAL_PATH := $(call my-dir)
include $(CACHED_LOCAL_PATH)/../../../libzip/proj/android/Android.mk
include $(CACHED_LOCAL_PATH)/../../../protobuf-2.4.1/proj/android/Android.mk
include $(CACHED_LOCAL_PATH)/../../../yajl/proj/android/Android.mk
LOCAL_PATH := $(CACHED_LOCAL_PATH)
