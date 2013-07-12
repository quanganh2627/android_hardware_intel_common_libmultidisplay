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

package com.intel.multidisplay;

import java.io.IOException;
import android.util.Slog;


public class DisplaySetting {
    private final String TAG = "MultiDisplay";
    private final boolean LOG = true;

    // mode setting policy
    public static final int HDMI_ON_NOT_ALLOWED  = 0;
    public static final int HDMI_ON_ALLOWED = 1;

    /// Don't change the enum, pls align with MultiDisplayType.h

    /// Phone call state
    public static final int PHONE_STATE_OFF = 0;
    public static final int PHONE_STATE_ON  = 1;

    /// display mode
    public static final int DVI_CONNECTED_BIT   = 1;
    public static final int HDMI_CONNECTED_BIT  = 1 << 1;
    public static final int EXTENDED_MODE_BIT   = 1 << 2;
    public static final int WIDI_CONNECTED_BIT  = 1 << 3;

    /// MDS message type
    public static final int MDS_MSG_MODE_CHANGE = 1 << 2;
    public static final int MDS_MSG_HOT_PLUG    = 1 << 3;

    /// MDS display capability
    public static final int DISPLAY_PRIMARY  = 0;
    public static final int DISPLAY_EXTERNAL = 1;
    public static final int DISPLAY_VIRTUAL  = 2;
    public static final int HW_SUPPORT_HDMI = 1 << DISPLAY_EXTERNAL;
    public static final int HW_SUPPORT_WIDI = 1 << DISPLAY_VIRTUAL;

    private static onMdsMessageListener mListener = null;

    private static native boolean native_InitMDSClient(DisplaySetting thiz);
    private static native boolean native_DeInitMDSClient();
    private static native int     native_getMode();
    private static native int     native_getHdmiTiming(int width[],
                                                int height[], int refresh[],
                                                int interlace[], int ratio[]);
    private static native boolean native_setHdmiTiming(int width, int height,
                            int refresh, int interlace, int ratio);
    private static native int     native_getHdmiInfoCount();
    private static native boolean native_setHdmiScaleType(int Type);
    private static native boolean native_setHdmiOverscan(int h, int v);
    private static native int     native_getHdmiDeviceChange();
    private static native int     native_getDisplayCapability();
    private static native int     native_setPhoneState(int state);

    public DisplaySetting() {
        if (LOG) Slog.i(TAG, "Create a new DisplaySetting");
        native_InitMDSClient(this);
    }

    @Override
    protected void finalize() { native_DeInitMDSClient(); }

    public int getMode() {
        return native_getMode();
    }

    public void onMdsMessage(int event, int value) {
        if (mListener != null) {
            mListener.onMdsMessage(event, value);
        }
    }

    public void setMdsMessageListener(onMdsMessageListener listener) {
        if (mListener == null) {
            mListener = listener;
        }
    }

    public int getHdmiTiming(int width[], int height[],
                             int refresh[], int interlace[], int ratio[]) {
        return native_getHdmiTiming(width, height,
                                    refresh, interlace, ratio);
    }

    public boolean setHdmiTiming(int width, int height,
                        int refresh, int interlace, int ratio) {
        return native_setHdmiTiming(width, height,
                            refresh, interlace, ratio);
    }

    public interface onMdsMessageListener {
        boolean onMdsMessage(int event, int value);
    }

    public int getHdmiInfoCount() {
        return native_getHdmiInfoCount();
    }

    public boolean setHdmiScaleType(int Type) {
        return native_setHdmiScaleType(Type);
    }

    public boolean setHdmiOverscan(int hValue, int vValue) {
        return native_setHdmiOverscan(hValue, vValue);
    }

    public int getHdmiDeviceChange() {
        return native_getHdmiDeviceChange();
    }

    public int getDisplayCapability() {
        return native_getDisplayCapability();
    }

    public int setPhoneState(int phoneState) {
        return native_setPhoneState(phoneState);
    }
}
