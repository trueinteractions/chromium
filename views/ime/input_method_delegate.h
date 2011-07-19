// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef VIEWS_IME_INPUT_METHOD_DELEGATE_H_
#define VIEWS_IME_INPUT_METHOD_DELEGATE_H_
#pragma once

namespace views {

class KeyEvent;

namespace internal {

// An interface implemented by the object that handles events sent back from an
// InputMethod implementation.
class InputMethodDelegate {
 public:
  virtual ~InputMethodDelegate() {}

  // Dispatch a key event already processed by the input method.
  virtual void DispatchKeyEventPostIME(const KeyEvent& key) = 0;

};

}  // namespace internal
}  // namespace views

#endif  // VIEWS_IME_INPUT_METHOD_DELEGATE_H_
