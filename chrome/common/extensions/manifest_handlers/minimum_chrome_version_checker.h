// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_EXTENSIONS_MANIFEST_HANDLERS_MINIMUM_CHROME_VERSION_CHECKER_H_
#define CHROME_COMMON_EXTENSIONS_MANIFEST_HANDLERS_MINIMUM_CHROME_VERSION_CHECKER_H_

#include "chrome/common/extensions/manifest_handler.h"

namespace extensions {

// Checks that the "minimum_chrome_version" requirement is met.
class MinimumChromeVersionChecker : public ManifestHandler {
 public:
  MinimumChromeVersionChecker();
  virtual ~MinimumChromeVersionChecker();

  // Validate minimum Chrome version. We don't need to store this, since the
  // extension is not valid if it is incorrect.
  virtual bool Parse(Extension* extension, string16* error) OVERRIDE;

 private:
  virtual const std::vector<std::string> Keys() const OVERRIDE;

  DISALLOW_COPY_AND_ASSIGN(MinimumChromeVersionChecker);
};

}  // namespace extensions

#endif  // CHROME_COMMON_EXTENSIONS_MANIFEST_HANDLERS_MINIMUM_CHROME_VERSION_CHECKER_H_
