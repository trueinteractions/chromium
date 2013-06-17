// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.chromium.android_webview;

import org.chromium.base.JNINamespace;

/**
 * Exposes a subset of Chromium form database to Webview database for managing autocomplete
 * functionality.
 */
@JNINamespace("android_webview")
public class AwFormDatabase {

    public static void clearFormData() {
        nativeClearFormData();
    }

    //--------------------------------------------------------------------------------------------
    //  Native methods
    //--------------------------------------------------------------------------------------------
    private static native void nativeClearFormData();
}
