// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// MediaStreamProvider is used to capture media of the types defined in
// MediaStreamType. There is only one MediaStreamProvider instance per media
// type and a MediaStreamProvider instance can have only one registered
// listener.
// The MediaStreamManager is expected to be called on Browser::IO thread and
// the listener will be called on the same thread.

#ifndef CONTENT_BROWSER_RENDERER_HOST_MEDIA_MEDIA_STREAM_PROVIDER_H_
#define CONTENT_BROWSER_RENDERER_HOST_MEDIA_MEDIA_STREAM_PROVIDER_H_

#include <list>
#include <string>

#include "content/common/media/media_stream_options.h"

namespace media_stream {

enum MediaStreamProviderError {
  kMediaStreamOk = 0,
  kInvalidMediaStreamType,
  kInvalidSession,
  kUnknownSession,
  kDeviceNotAvailable,
  kDeviceAlreadyInUse,
  kUnknownError
};

enum { kInvalidMediaCaptureSessionId = 0xFFFFFFFF };

// Callback class used by MediaStreamProvider.
class MediaStreamProviderListener {
 public:
  // Called by a MediaStreamProvider when a stream has been opened.
  virtual void Opened(MediaStreamType stream_type,
                      int capture_session_id) = 0;

  // Called by a MediaStreamProvider when a stream has been closed.
  virtual void Closed(MediaStreamType stream_type,
                      int capture_session_id) = 0;

  // Called by a MediaStreamProvider when available devices has been enumerated.
  virtual void DevicesEnumerated(MediaStreamType stream_type,
                                 const StreamDeviceInfoArray& devices) = 0;

  // Called by a MediaStreamProvider when an error has occured.
  virtual void Error(MediaStreamType stream_type,
                     int capture_session_id,
                     MediaStreamProviderError error) = 0;

 protected:
  virtual ~MediaStreamProviderListener() {}
};

// Implemented by a manager class providing captured media.
class MediaStreamProvider {
 public:
  // Registers a listener, only one listener is allowed.
  virtual void Register(MediaStreamProviderListener* listener) = 0;

  // Unregisters the previously registered listener.
  virtual void Unregister() = 0;

  // Enumerates existing capture devices and calls |DevicesEnumerated|.
  virtual void EnumerateDevices() = 0;

  // Opens the specified device. The device is not started and it is still
  // possible for other applications to open the device before the device is
  // started. |Opened| is called when the device is opened.
  // kInvalidMediaCaptureSessionId is returned on error.
  virtual int Open(const StreamDeviceInfo& device) = 0;

  // Closes the specified device and calls |Closed| when done.
  virtual void Close(int capture_session_id) = 0;

 protected:
  virtual ~MediaStreamProvider() {}
};

}  // namespace media_stream

#endif  // CONTENT_BROWSER_RENDERER_HOST_MEDIA_MEDIA_STREAM_PROVIDER_H_
