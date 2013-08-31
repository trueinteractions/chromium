// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_INDEXED_DB_LEVELDB_LEVELDB_WRITE_BATCH_H_
#define CONTENT_BROWSER_INDEXED_DB_LEVELDB_LEVELDB_WRITE_BATCH_H_

#include "base/memory/scoped_ptr.h"

namespace leveldb {
class WriteBatch;
}

namespace content {

class LevelDBSlice;

// Wrapper around leveldb::WriteBatch.
// This class holds a collection of updates to apply atomically to a database.
class LevelDBWriteBatch {
 public:
  static scoped_ptr<LevelDBWriteBatch> Create();
  ~LevelDBWriteBatch();

  void Put(const LevelDBSlice& key, const LevelDBSlice& value);
  void Remove(const LevelDBSlice& key);  // Add remove operation to the batch.
  void Clear();

 private:
  friend class LevelDBDatabase;
  LevelDBWriteBatch();

  scoped_ptr<leveldb::WriteBatch> write_batch_;
};

}  // namespace content

#endif  // CONTENT_BROWSER_INDEXED_DB_LEVELDB_LEVELDB_WRITE_BATCH_H_
