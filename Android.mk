LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := peeknpoke
LOCAL_MODULE_TAGS := optional
LOCAL_PRELINK_MODULE := false
LOCAL_SRC_FILES := \
    intel_scu_ipcutil_test.c \
    mem_dump.c \
    gen_utils.c \
    peeknpoke.c \
    msr.c \
    i2c.c \
    port.c
LOCAL_PROPRIETARY_MODULE := true
LOCAL_FORCE_STATIC_EXECUTABLE := true
LOCAL_STATIC_LIBRARIES := libc liblog
include $(BUILD_EXECUTABLE)
