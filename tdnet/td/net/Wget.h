//
// Copyright Aliaksei Levin (levlam@telegram.org), Arseny Smirnov (arseny30@gmail.com) 2014-2018
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "td/net/HttpOutboundConnection.h"
#include "td/net/HttpQuery.h"
#include "td/net/SslFd.h"

#include "td/actor/PromiseFuture.h"

#include "td/utils/common.h"
#include "td/utils/Status.h"

#include <utility>

namespace td {

class Wget : public HttpOutboundConnection::Callback {
 public:
  explicit Wget(Promise<HttpQueryPtr> promise, string url, std::vector<std::pair<string, string>> headers = {},
                int32 timeout_in = 10, int32 ttl = 3, SslFd::VerifyPeer verify_peer = SslFd::VerifyPeer::On);

 private:
  Status try_init();
  void loop() override;
  void handle(HttpQueryPtr result) override;
  void on_connection_error(Status error) override;
  void on_ok(HttpQueryPtr http_query_ptr);
  void on_error(Status error);

  void tear_down() override;
  void start_up() override;
  void timeout_expired() override;

  Promise<HttpQueryPtr> promise_;
  ActorOwn<HttpOutboundConnection> connection_;
  string input_url_;
  std::vector<std::pair<string, string>> headers_;
  int32 timeout_in_;
  int32 ttl_;
  SslFd::VerifyPeer verify_peer_;
};

}  // namespace td
