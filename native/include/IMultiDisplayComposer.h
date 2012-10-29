/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef __I_MULTIDISPLAY_COMPOSER_H__
#define __I_MULTIDISPLAY_COMPOSER_H__

#include <utils/RefBase.h>
#include <binder/IInterface.h>
#include <binder/Parcel.h>
#include <display/MultiDisplayType.h>
#include <display/IExtendDisplayModeChangeListener.h>

namespace android {

class IMultiDisplayComposer: public IInterface {
public:
    DECLARE_META_INTERFACE(MultiDisplayComposer);

    virtual int getMode(bool wait) = 0;
    virtual int setModePolicy(int) = 0;
    virtual int updateVideoInfo(MDSVideoInfo*) = 0;
    virtual int notifyWidi(bool) = 0;
    virtual int notifyMipi(bool) = 0;
    virtual int notifyHotPlug() = 0;
    virtual int setHdmiPowerOff() = 0;

    virtual int registerModeChangeListener(sp<IExtendDisplayModeChangeListener>, void *) = 0;
    virtual int unregisterModeChangeListener(sp<IExtendDisplayModeChangeListener>, void *) = 0;
    virtual int getHdmiModeInfo(int* widht, int* height, int* refresh, int* interlace, int* ratio) = 0;
    virtual int setHdmiModeInfo(int widht, int height, int refresh, int interlace, int ratio) = 0;
    virtual int setHdmiScaleType(int type) = 0;
    virtual int setHdmiScaleStep(int hValue, int vValue) = 0;
    virtual int getHdmiDeviceChange() = 0;
    virtual int getVideoInfo(int* dw, int* dh, int* fps, int* interlace) = 0;
    virtual int getDisplayCapability() = 0;
};

class BpMultiDisplayComposer:public BpInterface<IMultiDisplayComposer> {
public:
    BpMultiDisplayComposer(const sp<IBinder>& impl)
        : BpInterface<IMultiDisplayComposer>(impl) {}
    virtual int getMode(bool wait);
    virtual int setModePolicy(int);
    virtual int updateVideoInfo(MDSVideoInfo*);
    virtual int notifyWidi(bool);
    virtual int notifyMipi(bool);
    virtual int notifyHotPlug();
    virtual int setHdmiPowerOff();

    virtual int registerModeChangeListener(sp<IExtendDisplayModeChangeListener>, void *);
    virtual int unregisterModeChangeListener(sp<IExtendDisplayModeChangeListener>, void *);

    virtual int getHdmiModeInfo(int* widht, int* height, int* refresh, int* interlace, int* ratio);
    virtual int setHdmiModeInfo(int widht, int height, int refresh, int interlace, int ratio);
    virtual int setHdmiScaleType(int type);
    virtual int setHdmiScaleStep(int hValue, int vValue);
    virtual int getHdmiDeviceChange();
    virtual int getVideoInfo(int* dw, int* dh, int* fps, int* interlace);
    virtual int getDisplayCapability();
};

class BnMultiDisplayComposer:public BnInterface<IMultiDisplayComposer> {
public:
    virtual status_t onTransact(uint32_t code,
                                const Parcel& data,
                                Parcel* replay,
                                uint32_t flags = 0);
};
}; // android namespace
#endif

