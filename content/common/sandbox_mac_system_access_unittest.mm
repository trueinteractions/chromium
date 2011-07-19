// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Cocoa/Cocoa.h>

#include "base/logging.h"
#include "base/sys_string_conversions.h"
#include "content/common/sandbox_mac.h"
#include "content/common/sandbox_mac_unittest_helper.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace {

using sandboxtest::MacSandboxTest;

//--------------------- Clipboard Sandboxing ----------------------
// Test case for checking sandboxing of clipboard access.
class MacSandboxedClipboardTestCase : public sandboxtest::MacSandboxTestCase {
 public:
  MacSandboxedClipboardTestCase();
  virtual ~MacSandboxedClipboardTestCase();

  virtual bool SandboxedTest();

  virtual void SetTestData(const char* test_data);
 private:
  NSString* clipboard_name_;
};

REGISTER_SANDBOX_TEST_CASE(MacSandboxedClipboardTestCase);

MacSandboxedClipboardTestCase::MacSandboxedClipboardTestCase() :
    clipboard_name_(nil) {}

MacSandboxedClipboardTestCase::~MacSandboxedClipboardTestCase() {
  [clipboard_name_ release];
}

bool MacSandboxedClipboardTestCase::SandboxedTest() {
  // Shouldn't be able to open the pasteboard in the sandbox.

  if ([clipboard_name_ length] == 0) {
    LOG(ERROR) << "Clipboard name is empty";
    return false;
  }

  NSPasteboard* pb = [NSPasteboard pasteboardWithName:clipboard_name_];
  if (pb != nil) {
    LOG(ERROR) << "Was able to access named clipboard";
    return false;
  }

  pb = [NSPasteboard generalPasteboard];
  if (pb != nil) {
    LOG(ERROR) << "Was able to access system clipboard";
    return false;
  }

  return true;
}

void MacSandboxedClipboardTestCase::SetTestData(const char* test_data) {
  clipboard_name_ = [base::SysUTF8ToNSString(test_data) retain];
}

TEST_F(MacSandboxTest, ClipboardAccess) {
  NSPasteboard* pb = [NSPasteboard pasteboardWithUniqueName];
  EXPECT_EQ([[pb types] count], 0U);

  std::string pasteboard_name = base::SysNSStringToUTF8([pb name]);
  EXPECT_TRUE(RunTestInAllSandboxTypes("MacSandboxedClipboardTestCase",
                  pasteboard_name.c_str()));

  // After executing the test, the clipboard should still be empty.
  EXPECT_EQ([[pb types] count], 0U);
}

//--------------------- File Access Sandboxing ----------------------
// Test case for checking sandboxing of filesystem apis.
class MacSandboxedFileAccessTestCase : public sandboxtest::MacSandboxTestCase {
 public:
  virtual bool SandboxedTest();
};

REGISTER_SANDBOX_TEST_CASE(MacSandboxedFileAccessTestCase);

bool MacSandboxedFileAccessTestCase::SandboxedTest() {
  return open("/etc/passwd", O_RDONLY) == -1;
}

TEST_F(MacSandboxTest, FileAccess) {
  EXPECT_TRUE(RunTestInAllSandboxTypes("MacSandboxedFileAccessTestCase", NULL));
}

}  // namespace
