LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,/cygdrive/D/Android/cocos2d-x-3.4)
$(call import-add-path,/cygdrive/D/Android/cocos2d-x-3.4/external)
$(call import-add-path,/cygdrive/D/Android/cocos2d-x-3.4/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := sampleCPP/main.cpp \
                   sampleCPP/AppDelegate.cpp \
                   sampleCPP/C3bSceneManager.cpp \
                   sampleCPP/My3DLayer.cpp \
                   sampleCPP/My2DLayer.cpp \
                   sampleCPP/Effect3D.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_STATIC_LIBRARIES := cocos2dx_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)
