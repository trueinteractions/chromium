// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/browser/gpu/gpu_control_list.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace content {

class OsInfoTest : public testing::Test {
 public:
  OsInfoTest() { }
  virtual ~OsInfoTest() { }

  typedef GpuControlList::OsInfo OsInfo;
};

TEST_F(OsInfoTest, ValidOsInfo) {
  const std::string os[] = {
    "win",
    "linux",
    "macosx",
    "chromeos",
    "android",
    "any"
  };
  const GpuControlList::OsType os_type[] = {
    GpuControlList::kOsWin,
    GpuControlList::kOsLinux,
    GpuControlList::kOsMacosx,
    GpuControlList::kOsChromeOS,
    GpuControlList::kOsAndroid,
    GpuControlList::kOsAny
  };
  for (size_t i = 0; i < arraysize(os); ++i) {
    OsInfo info(os[i], "=", "10.6", std::string());
    EXPECT_TRUE(info.IsValid());
    EXPECT_EQ(os_type[i], info.type());
  }
  {
    OsInfo info("any", "any", std::string(), std::string());
    EXPECT_TRUE(info.IsValid());
  }
}

TEST_F(OsInfoTest, InvalidOsInfo) {
  const std::string os[] = {
    "win",
    "linux",
    "macosx",
    "chromeos",
    "android",
    "any"
  };
  for (size_t i = 0; i < arraysize(os); ++i) {
    {
      OsInfo info(os[i], std::string(), std::string(), std::string());
      EXPECT_FALSE(info.IsValid());
    }
    {
      OsInfo info(os[i], "=", std::string(), std::string());
      EXPECT_FALSE(info.IsValid());
    }
    {
      OsInfo info(os[i], std::string(), "10.6", std::string());
      EXPECT_FALSE(info.IsValid());
    }
  }
  const std::string os_cap[] = {
    "Win",
    "Linux",
    "MacOSX",
    "ChromeOS",
    "Android",
  };
  for (size_t i = 0; i < arraysize(os_cap); ++i) {
    OsInfo info(os_cap[i], "=", "10.6", std::string());
    EXPECT_FALSE(info.IsValid());
  }
}

TEST_F(OsInfoTest, OsComparison) {
  {
    OsInfo info("any", "any", std::string(), std::string());
    const GpuControlList::OsType os_type[] = {
      GpuControlList::kOsWin, GpuControlList::kOsLinux,
      GpuControlList::kOsMacosx, GpuControlList::kOsChromeOS,
      GpuControlList::kOsAndroid,
    };
    for (size_t i = 0; i < arraysize(os_type); ++i) {
      EXPECT_TRUE(info.Contains(os_type[i], std::string()));
      EXPECT_TRUE(info.Contains(os_type[i], "7.8"));
    }
  }
  {
    OsInfo info("win", ">=", "6", std::string());
    EXPECT_FALSE(info.Contains(GpuControlList::kOsMacosx, "10.8.3"));
    EXPECT_FALSE(info.Contains(GpuControlList::kOsLinux, "10"));
    EXPECT_FALSE(info.Contains(GpuControlList::kOsChromeOS, "13"));
    EXPECT_FALSE(info.Contains(GpuControlList::kOsAndroid, "7"));
    EXPECT_FALSE(info.Contains(GpuControlList::kOsAny, "7"));
    EXPECT_FALSE(info.Contains(GpuControlList::kOsWin, std::string()));
    EXPECT_TRUE(info.Contains(GpuControlList::kOsWin, "6"));
    EXPECT_TRUE(info.Contains(GpuControlList::kOsWin, "6.1"));
    EXPECT_TRUE(info.Contains(GpuControlList::kOsWin, "7"));
    EXPECT_FALSE(info.Contains(GpuControlList::kOsWin, "5"));
  }
}

}  // namespace content

