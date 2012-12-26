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
 * Author: tianyang.zhu@intel.com
 */

//#define LOG_NDEBUG 0

#include <utils/Log.h>
#include <binder/Parcel.h>
#include <binder/IServiceManager.h>
#include <display/IMultiDisplayComposer.h>
#include <display/MultiDisplayType.h>
#include <display/IExtendDisplayListener.h>

using namespace android;

int BpMultiDisplayComposer::getMode(bool wait) {
    Parcel data, reply;
    data.writeInterfaceToken(IMultiDisplayComposer::getInterfaceDescriptor());
    data.writeInt32(wait == true ? 1 : 0);
    remote()->transact(MDS_GET_MODE, data, &reply);
    return reply.readInt32();
}

int BpMultiDisplayComposer::setModePolicy(int policy) {
    Parcel data, reply;
    data.writeInterfaceToken(IMultiDisplayComposer::getInterfaceDescriptor());
    data.writeInt32(policy);
    remote()->transact(MDS_SET_MODEPOLICY, data, &reply);
    return reply.readInt32();
}

int BpMultiDisplayComposer::notifyWidi(bool on) {
    Parcel data, reply;
    data.writeInterfaceToken(IMultiDisplayComposer::getInterfaceDescriptor());
    data.writeInt32( on == true ? 1: 0);
    remote()->transact(MDS_NOTIFY_WIDI, data, &reply);
    return reply.readInt32();
}

int BpMultiDisplayComposer::notifyMipi(bool on) {
    Parcel data, reply;
    data.writeInterfaceToken(IMultiDisplayComposer::getInterfaceDescriptor());
    data.writeInt32( on == true ? 1: 0);
    remote()->transact(MDS_NOTIFY_MIPI, data, &reply);
    return reply.readInt32();
}

int BpMultiDisplayComposer::isMdsSurface(int* nw) {
    Parcel data, reply;
    data.writeInterfaceToken(IMultiDisplayComposer::getInterfaceDescriptor());
    if (nw == NULL) return MDS_ERROR;
    data.writeIntPtr((intptr_t)nw);
    remote()->transact(MDS_VERIFY_BACKGROUND_SURFACE, data, &reply);
    return reply.readInt32();
}

int BpMultiDisplayComposer::updateVideoInfo(MDSVideoInfo* info) {
    Parcel data, reply;
    data.writeInterfaceToken(IMultiDisplayComposer::getInterfaceDescriptor());
    if (info == NULL) return MDS_ERROR;
    int isplaying = (info->isplaying ? 1 : 0);
    int isprotected = (info->isprotected ? 1 : 0);
    int dsw = info->displayW;
    int dsh = info->displayH;
    int framerate = info->frameRate;
    int isinterlace = (info->isinterlace ? 1 : 0);
    data.writeInt32(isplaying);
    data.writeInt32(isprotected);
    data.writeInt32(framerate);
    data.writeInt32(dsw);
    data.writeInt32(dsh);
    data.writeInt32(isinterlace);
    remote()->transact(MDS_UPDATE_VIDEOINFO, data, &reply);
    return reply.readInt32();
}

int BpMultiDisplayComposer::notifyHotPlug() {
    Parcel data, reply;
    data.writeInterfaceToken(IMultiDisplayComposer::getInterfaceDescriptor());
    remote()->transact(MDS_NOTIFY_HOTPLUG, data, &reply);
    return reply.readInt32();
}

int BpMultiDisplayComposer::setHdmiPowerOff() {
    Parcel data, reply;
    data.writeInterfaceToken(IMultiDisplayComposer::getInterfaceDescriptor());
    remote()->transact(MDS_HDMI_POWER_OFF, data, &reply);
    return reply.readInt32();
}

int BpMultiDisplayComposer::registerListener(
        sp<IExtendDisplayListener> listener, void *handle, const char* name, int msg) {
    Parcel data, reply;
    if (!name || msg <= 0) {
        LOGE("%s: Parameter is null", __func__);
        return MDS_ERROR;
    }
    data.writeInterfaceToken(IMultiDisplayComposer::getInterfaceDescriptor());
    data.writeStrongBinder(listener->asBinder());
    data.writeIntPtr(reinterpret_cast<intptr_t>(handle));
    data.writeCString(name);
    data.writeInt32(msg);
    remote()->transact(MDS_REGISTER_LISTENER, data, &reply);
    return reply.readInt32();
}

int BpMultiDisplayComposer::unregisterListener(sp<IExtendDisplayListener> listener, void *handle) {
    Parcel data, reply;
    data.writeInterfaceToken(IMultiDisplayComposer::getInterfaceDescriptor());
    data.writeStrongBinder(listener->asBinder());
    data.writeIntPtr(reinterpret_cast<intptr_t>(handle));
    remote()->transact(MDS_UNREGISTER_LISTENER, data, &reply);
    return reply.readInt32();
}

int BpMultiDisplayComposer::setHdmiModeInfo(int width, int height, int refresh, int interlace, int ratio) {
    Parcel data, reply;
    data.writeInterfaceToken(IMultiDisplayComposer::getInterfaceDescriptor());
    data.writeInt32(width);
    data.writeInt32(height);
    data.writeInt32(refresh);
    data.writeInt32(interlace);
    data.writeInt32(ratio);
    remote()->transact(MDS_SET_HDMIMODE_INFO, data, &reply);
    return reply.readInt32();
}

int BpMultiDisplayComposer::getHdmiModeInfo(int* widthArray, int* heightArray,
        int* refreshArray, int* interlaceArray, int* ratioArray) {
    Parcel data, reply;
    data.writeInterfaceToken(IMultiDisplayComposer::getInterfaceDescriptor());
    if (widthArray == NULL || heightArray == NULL
        || refreshArray == NULL || interlaceArray == NULL
        || ratioArray == NULL) {
        remote()->transact(MDS_GET_HDMIMODE_INFO_COUNT, data, &reply);
        return reply.readInt32();
    }
    remote()->transact(MDS_GET_HDMIMODE_INFO, data, &reply);
    int count = reply.readInt32();
    for (int i = 0; i < count; i++) {
        *(widthArray + i)= reply.readInt32();
        *(heightArray + i)= reply.readInt32();
        *(refreshArray + i)= reply.readInt32();
        *(interlaceArray + i)= reply.readInt32();
        *(ratioArray + i)= reply.readInt32();
    }
    return count;
}

int BpMultiDisplayComposer::setHdmiScaleType(int Type) {
    Parcel data, reply;
    data.writeInterfaceToken(IMultiDisplayComposer::getInterfaceDescriptor());
    data.writeInt32(Type);
    remote()->transact(MDS_SET_HDMISCALE_TYPE, data, &reply);
    return reply.readInt32();
}

int BpMultiDisplayComposer::setHdmiScaleStep(int hValue, int vValue) {
    Parcel data, reply;
    data.writeInterfaceToken(IMultiDisplayComposer::getInterfaceDescriptor());
    data.writeInt32(hValue);
    data.writeInt32(vValue);
    remote()->transact(MDS_SET_HDMISCALE_STEP, data, &reply);
    return reply.readInt32();
}

int BpMultiDisplayComposer::getHdmiDeviceChange() {
    Parcel data, reply;
    data.writeInterfaceToken(IMultiDisplayComposer::getInterfaceDescriptor());
    remote()->transact(MDS_GET_HDMIDEVICE_CHANGE, data, &reply);
    return reply.readInt32();
}


int BpMultiDisplayComposer::getVideoInfo(int* dw, int* dh, int* fps, int* interlace) {
    Parcel data, reply;
    data.writeInterfaceToken(IMultiDisplayComposer::getInterfaceDescriptor());
    if (dw == NULL || dh == NULL
            || fps == NULL || interlace == NULL) {
        LOGE("%s: parameter is null", __func__);
        return MDS_ERROR;
    }
    data.writeIntPtr((intptr_t)dw);
    data.writeIntPtr((intptr_t)dh);
    data.writeIntPtr((intptr_t)fps);
    data.writeIntPtr((intptr_t)interlace);
    remote()->transact(MDS_GET_VIDEO_INFO, data, &reply);
    return reply.readInt32();
}

int BpMultiDisplayComposer::getDisplayCapability() {
    Parcel data, reply;
    data.writeInterfaceToken(IMultiDisplayComposer::getInterfaceDescriptor());
    remote()->transact(MDS_GET_DISPLAY_CAPABILITY, data, &reply);
    return reply.readInt32();
}


int BpMultiDisplayComposer::enablePlayInBackground(bool on, int playerId) {
    Parcel data, reply;
    data.writeInterfaceToken(IMultiDisplayComposer::getInterfaceDescriptor());
    data.writeInt32( on == true ? 1: 0);
    data.writeInt32(playerId);
    remote()->transact(MDS_SET_PLAYINBACKGROUND, data, &reply);
    return reply.readInt32();
}

int BpMultiDisplayComposer::setNativeSurface(int* surface) {
    Parcel data, reply;
    data.writeInterfaceToken(IMultiDisplayComposer::getInterfaceDescriptor());
    if (surface == NULL) {
        LOGE("%s: parameter is null", __func__);
        return -1;
    }
    data.writeIntPtr((intptr_t)surface);
    remote()->transact(MDS_SET_SURFACE, data, &reply);
    return reply.readInt32();
}

int BpMultiDisplayComposer::isPlayInBackgroundEnabled() {
    Parcel data, reply;
    data.writeInterfaceToken(IMultiDisplayComposer::getInterfaceDescriptor());
    remote()->transact(MDS_GET_PLAYINBACKGROUND, data, &reply);
    return reply.readInt32();
}

int BpMultiDisplayComposer::getBackgroundPlayerId() {
    Parcel data, reply;
    data.writeInterfaceToken(IMultiDisplayComposer::getInterfaceDescriptor());
    remote()->transact(MDS_GET_BACKGROUNDPLAYER_ID, data, &reply);
    return reply.readInt32();
}

int BpMultiDisplayComposer::setHdcpStatus(int Value) {
    Parcel data, reply;
    data.writeInterfaceToken(IMultiDisplayComposer::getInterfaceDescriptor());
    data.writeInt32(Value);
    remote()->transact(MDS_SET_HDMIHDCP, data, &reply);
    return reply.readInt32();
}

int BpMultiDisplayComposer::notifyScreenOff() {
    Parcel data, reply;
    data.writeInterfaceToken(IMultiDisplayComposer::getInterfaceDescriptor());
    remote()->transact(MDS_NOTIFY_SCREENOFF, data, &reply);
    return reply.readInt32();
}

IMPLEMENT_META_INTERFACE(MultiDisplayComposer,"com.intel.IMultiDisplayComposer");

status_t BnMultiDisplayComposer::onTransact(uint32_t code,
                                            const Parcel& data,
                                            Parcel* reply,
                                            uint32_t flags) {
    switch (code) {
    case MDS_GET_MODE: {
        CHECK_INTERFACE(IMultiDisplayComposer, data, reply);
        reply->writeInt32(getMode((data.readInt32() == 1 ? true : false)));
        return NO_ERROR;
    }
    break;
    case MDS_SET_MODEPOLICY: {
        CHECK_INTERFACE(IMultiDisplayComposer, data, reply);
        reply->writeInt32(setModePolicy(data.readInt32()));
        return NO_ERROR;
    }
    break;
    case MDS_NOTIFY_MIPI: {
        CHECK_INTERFACE(IMultiDisplayComposer, data, reply);
        reply->writeInt32(notifyMipi((data.readInt32() == 1 ? true : false)));
        return NO_ERROR;
    }
    break;
    case MDS_NOTIFY_WIDI: {
        CHECK_INTERFACE(IMultiDisplayComposer, data, reply);
        reply->writeInt32(notifyWidi((data.readInt32() == 1 ? true : false)));
        return NO_ERROR;
    }
    break;
    case MDS_UPDATE_VIDEOINFO: {
        CHECK_INTERFACE(IMultiDisplayComposer, data, reply);
        MDSVideoInfo info;
        info.isplaying = (data.readInt32() == 0 ? false : true);
        info.isprotected = (data.readInt32() == 0 ? false : true);
        info.frameRate = data.readInt32();
        info.displayW = data.readInt32();
        info.displayH = data.readInt32();
        info.isinterlace = (data.readInt32() == 0 ? false : true);
        reply->writeInt32(updateVideoInfo(&info));
        return NO_ERROR;
    }
    break;
    case MDS_NOTIFY_HOTPLUG: {
        CHECK_INTERFACE(IMultiDisplayComposer, data, reply);
        reply->writeInt32(notifyHotPlug());
        return NO_ERROR;
    }
    break;
    case MDS_VERIFY_BACKGROUND_SURFACE: {
        CHECK_INTERFACE(IMultiDisplayComposer, data, reply);
        int *nw = NULL;
        nw = (int*)data.readIntPtr();
        reply->writeInt32((isMdsSurface(nw) == 1 ? true : false));
        return NO_ERROR;
    }
    case MDS_HDMI_POWER_OFF: {
        CHECK_INTERFACE(IMultiDisplayComposer, data, reply);
        reply->writeInt32(setHdmiPowerOff());
        return NO_ERROR;
    }
    break;
    case MDS_REGISTER_LISTENER: {
        CHECK_INTERFACE(IMultiDisplayComposer, data, reply);
        sp<IBinder> listener = data.readStrongBinder();
        void* handle = reinterpret_cast<void *>(data.readIntPtr());
        const char* client = data.readCString();
        int msg = data.readInt32();
        int ret = registerListener(
                    interface_cast<IExtendDisplayListener>(listener), handle, client, msg);
        reply->writeInt32(ret);
        return NO_ERROR;
    }
    break;
    case MDS_UNREGISTER_LISTENER: {
        CHECK_INTERFACE(IMultiDisplayComposer, data, reply);
        sp<IBinder> listener = data.readStrongBinder();
        int ret = unregisterListener(
                    interface_cast<IExtendDisplayListener>(listener),
                    reinterpret_cast<void *>(data.readIntPtr()));
        reply->writeInt32(ret);
        return NO_ERROR;
    }
    break;
    case MDS_SET_HDMIMODE_INFO: {
        CHECK_INTERFACE(IMultiDisplayComposer, data, reply);
        int width, height, refresh, interlace, ratio, ret;
        width = data.readInt32();
        height = data.readInt32();
        refresh = data.readInt32();
        interlace = data.readInt32();
        ratio = data.readInt32();
        ret = setHdmiModeInfo(width, height, refresh, interlace, ratio);
        reply->writeInt32(ret);
        return NO_ERROR;
    }
    break;
    case MDS_GET_HDMIMODE_INFO: {
        CHECK_INTERFACE(IMultiDisplayComposer, data, reply);
        int width[HDMI_TIMING_MAX];
        int height[HDMI_TIMING_MAX];
        int refresh[HDMI_TIMING_MAX];
        int interlace[HDMI_TIMING_MAX];
        int ratio[HDMI_TIMING_MAX];
        memset(width, 0 , HDMI_TIMING_MAX * sizeof(int));
        memset(height, 0 , HDMI_TIMING_MAX * sizeof(int));
        memset(refresh, 0 , HDMI_TIMING_MAX * sizeof(int));
        memset(interlace, 0 , HDMI_TIMING_MAX * sizeof(int));
        memset(ratio, 0 , HDMI_TIMING_MAX * sizeof(int));
        int count = getHdmiModeInfo(width, height, refresh, interlace, ratio);
        if (count < 0 || count > HDMI_TIMING_MAX)
            count = -1;
        reply->writeInt32(count);
        for(int i = 0; i < count; i++) {
            reply->writeInt32(*(width + i));
            reply->writeInt32(*(height + i));
            reply->writeInt32(*(refresh + i));
            reply->writeInt32(*(interlace + i));
            reply->writeInt32(*(ratio + i));
        }
        return NO_ERROR;
    }
    break;
    case MDS_GET_HDMIMODE_INFO_COUNT: {
        CHECK_INTERFACE(IMultiDisplayComposer, data, reply);
        reply->writeInt32(getHdmiModeInfo(NULL, NULL, NULL, NULL, NULL));
        return NO_ERROR;
    }
    break;
    case MDS_SET_HDMISCALE_TYPE: {
        CHECK_INTERFACE(IMultiDisplayComposer, data, reply);
        int type;
        type = data.readInt32();
        int ret = setHdmiScaleType(type);
        reply->writeInt32(ret);
        return NO_ERROR;
    }
    break;
    case MDS_SET_HDMISCALE_STEP: {
        CHECK_INTERFACE(IMultiDisplayComposer, data, reply);
        int hValue, vValue;
        hValue = data.readInt32();
        vValue = data.readInt32();
        int ret = setHdmiScaleStep(hValue, vValue);
        reply->writeInt32(ret);
        return NO_ERROR;
    }
    break;
    case MDS_GET_HDMIDEVICE_CHANGE: {
        CHECK_INTERFACE(IMultiDisplayComposer, data, reply);
        int ret = getHdmiDeviceChange();
        reply->writeInt32(ret);
        return NO_ERROR;
    }
    break;
    case MDS_GET_VIDEO_INFO: {
        CHECK_INTERFACE(IMultiDisplayComposer, data, reply);
        int *width, *height, *fps, *interlace;
        width = height = fps = interlace = NULL;
        width = (int*)data.readIntPtr();
        height = (int*)data.readIntPtr();
        fps = (int*)data.readIntPtr();
        interlace = (int*)data.readIntPtr();
        int ret = getVideoInfo(width, height, fps, interlace);
        reply->writeInt32(ret);
        return NO_ERROR;
    }
    break;
    case MDS_GET_DISPLAY_CAPABILITY: {
        CHECK_INTERFACE(IMultiDisplayComposer, data, reply);
        int ret = getDisplayCapability();
        reply->writeInt32(ret);
        return NO_ERROR;
    }
    break;

    case MDS_SET_PLAYINBACKGROUND: {
        CHECK_INTERFACE(IMultiDisplayComposer, data, reply);
        bool on;
        on = (data.readInt32() == 0 ? false : true);
        int playerId = data.readInt32();
        int ret = enablePlayInBackground(on, playerId);
        reply->writeInt32(ret);
        return NO_ERROR;
    }
    break;

    case MDS_SET_SURFACE: {
        CHECK_INTERFACE(IMultiDisplayComposer, data, reply);
        int* ptr = NULL;
        ptr = (int*)data.readIntPtr();
        int ret = setNativeSurface(ptr);
        reply->writeInt32(ret);
        return NO_ERROR;
    }
    break;

    case MDS_GET_PLAYINBACKGROUND: {
        CHECK_INTERFACE(IMultiDisplayComposer, data, reply);
        int ret = isPlayInBackgroundEnabled();
        reply->writeInt32(ret);
        return NO_ERROR;
    }
    break;

    case MDS_GET_BACKGROUNDPLAYER_ID: {
        CHECK_INTERFACE(IMultiDisplayComposer, data, reply);
        int ret = getBackgroundPlayerId();
        reply->writeInt32(ret);
        return NO_ERROR;
    }

    case MDS_SET_HDMIHDCP: {
        CHECK_INTERFACE(IMultiDisplayComposer, data, reply);
        int value;
        value = data.readInt32();
        int ret = setHdcpStatus(value);
        reply->writeInt32(ret);
        return NO_ERROR;
    }
    break;

    case MDS_NOTIFY_SCREENOFF: {
        CHECK_INTERFACE(IMultiDisplayComposer, data, reply);
        int ret = notifyScreenOff();
        reply->writeInt32(ret);
        return NO_ERROR;
    }
    break;

    default:
        return BBinder::onTransact(code, data, reply, flags);
    }
    return NO_ERROR;
}
