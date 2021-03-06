// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_QUIC_QUIC_IN_MEMORY_CACHE_H_
#define NET_TOOLS_QUIC_QUIC_IN_MEMORY_CACHE_H_

#include <string>

#include "base/containers/hash_tables.h"
#include "base/memory/singleton.h"
#include "base/strings/string_piece.h"
#include "net/tools/flip_server/balsa_frame.h"
#include "net/tools/flip_server/balsa_headers.h"

template <typename T> struct DefaultSingletonTraits;

namespace net {
namespace tools {

extern std::string FLAGS_quic_in_memory_cache_dir;

class QuicServer;

// In-memory cache for HTTP responses.
// Reads from disk cache generated by:
// `wget -p --save_headers <url>`
class QuicInMemoryCache {
 public:
  // Container for response header/body pairs.
  class Response {
   public:
    Response() {}
    ~Response() {}

    const BalsaHeaders& headers() const { return headers_; }
    const base::StringPiece body() const { return base::StringPiece(body_); }

   private:
    friend class QuicInMemoryCache;

    void set_headers(const BalsaHeaders& headers) {
      headers_.CopyFrom(headers);
    }
    void set_body(base::StringPiece body) {
      body.CopyToString(&body_);
    }

    BalsaHeaders headers_;
    std::string body_;

    DISALLOW_COPY_AND_ASSIGN(Response);
  };
  static QuicInMemoryCache* GetInstance();

  // Retrieve a response from this cache for a given request.
  // If no appropriate response exists, NULL is returned.
  // Currently, responses are selected based on request URI only.
  const Response* GetResponse(const BalsaHeaders& request_headers) const;

  // Add a response to the cache.
  void AddResponse(const BalsaHeaders& request_headers,
                   const BalsaHeaders& response_headers,
                   base::StringPiece response_body);

  void ResetForTests();

 private:
  typedef base::hash_map<std::string, Response*> ResponseMap;


  QuicInMemoryCache();
  friend struct DefaultSingletonTraits<QuicInMemoryCache>;
  ~QuicInMemoryCache();

  void Initialize();
  std::string GetKey(const BalsaHeaders& response_headers) const;

  // Cached responses.
  ResponseMap responses_;

  DISALLOW_COPY_AND_ASSIGN(QuicInMemoryCache);
};

}  // namespace tools
}  // namespace net

#endif  // NET_TOOLS_QUIC_QUIC_IN_MEMORY_CACHE_H_
