/*
 * Copyright (c) 2012-2013, Intel Corporation. All rights reserved.
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

#ifndef __MULTIDISPLAY_COMPOSER_H__
#define __MULTIDISPLAY_COMPOSER_H__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/inotify.h>
#include <sys/limits.h>
#include <sys/poll.h>
#include <linux/input.h>
#include <errno.h>
#include <utils/threads.h>
#include <utils/Vector.h>
#include <display/IExtendDisplayListener.h>
#include <display/MultiDisplayType.h>

using namespace android;

class MultiDisplayListener {
private:
    int   mMsg;
    char* mName;
    sp<IExtendDisplayListener> mIEListener;
public:
    MultiDisplayListener(int msg, const char* client, sp<IExtendDisplayListener>);
    ~MultiDisplayListener();
    inline char* getName() {
        return mName;
    }
    inline int getMsg() {
        return mMsg;
    }
    inline sp<IExtendDisplayListener> getIEListener() {
        return mIEListener;
    }
    inline bool checkMsg(int msg) {
        if (msg & mMsg)
            return true;
        return false;
    }
};

class MultiDisplayComposer : public Thread
{
public:
    MultiDisplayComposer();
    ~MultiDisplayComposer();

    int getMode(bool wait);

    int setModePolicy(int);
    int notifyWidi(bool);
    int notifyMipi(bool);
    int isMdsSurface(int* nw);
    int notifyHotPlug();
    int setHdmiPowerOff();
    int prepareForVideo(int);
    int updateVideoInfo(MDSVideoSourceInfo*);

    int registerListener(sp<IExtendDisplayListener>, void *, const char *, int);
    int unregisterListener(void *);

    int getHdmiModeInfo(int* widht, int* height, int* refresh, int* interlace, int* ratio);
    int setHdmiModeInfo(int widht, int height, int refresh, int interlace, int ratio);
    int setHdmiScaleType(int type);
    int setHdmiScaleStep(int hValue, int vValue);
    int getHdmiDeviceChange();
    int getVideoInfo(int* dw, int* dh, int* fps, int* interlace);
    int getDisplayCapability();
    void setWidiOrientationInfo(int orientation);
    int enablePlayInBackground(bool on, int playerId);
    int setNativeSurface(int* surface);
    int isPlayInBackgroundEnabled();
    int getBackgroundPlayerId();

private:
    enum {
        NO_MIPI_REQ  = -1, // No mipi op request (default)
        MIPI_OFF_REQ = 0,  // Turn off mipi request
        MIPI_ON_REQ  = 1,  // Turn on mipi request
    };

    bool mDrmInit;
    int mMode;
    int mDisplayCapability;
    int mHdmiPolicy;
    int mMipiPolicy;
    bool mMipiOn;
    int  mMipiReq;
    bool mWidiVideoExt;
    MDSVideoSourceInfo mVideo;
    mutable Mutex mLock;
    Condition mMipiCon;
    mutable Mutex mMipiLock;
    KeyedVector<void *, MultiDisplayListener* > mListener;
    bool mEnablePlayInBackground;
    int* mNativeSurface;
    int mBackgroundPlayerId;
    mutable Mutex mBackgroundPlayLock;
    int mConnectStatus;

    // HDMI Scaling mode and scaling calibriation
    sp<IBinder> mSurfaceComposer;
    uint32_t mScaleMode;
    uint32_t mScaleStepX;
    uint32_t mScaleStepY;
    int setDisplayScalingLocked(uint32_t mode,
         uint32_t stepx, uint32_t stepy);

    void initialize_l();
    int setHdmiMode_l();
    int setMipiMode_l(bool);
    int setModePolicy_l(int);
    int getHdmiPlug_l();
    int isHwcSetUp_l();

    void broadcastMessage_l(int msg, void* value, int size);
    int  setHdmiTiming_l(void* value, int size);

    virtual bool threadLoop();
    inline bool checkMode(int value, int bit) {
        if ((value & bit) == bit)
            return true;
        return false;
    }

    static int widi_rm_notifier_handler(void* cookie, int cmd, int data);
    bool isHdmiTimingDynamicSettingEnable_l();
};


#endif

