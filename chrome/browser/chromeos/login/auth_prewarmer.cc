// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/chromeos/login/auth_prewarmer.h"

#include "chrome/browser/chromeos/net/connectivity_state_helper.h"
#include "chrome/browser/chromeos/profiles/profile_helper.h"
#include "chrome/browser/net/chrome_url_request_context.h"
#include "chrome/browser/net/preconnect.h"
#include "chrome/common/chrome_notification_types.h"
#include "content/public/browser/browser_thread.h"
#include "google_apis/gaia/gaia_urls.h"
#include "googleurl/src/gurl.h"

namespace chromeos {

AuthPrewarmer::AuthPrewarmer()
    : doing_prewarm_(false) {
}

AuthPrewarmer::~AuthPrewarmer() {
  if (registrar_.IsRegistered(
          this,
          chrome::NOTIFICATION_PROFILE_URL_REQUEST_CONTEXT_GETTER_INITIALIZED,
          content::Source<Profile>(ProfileHelper::GetSigninProfile()))) {
    registrar_.Remove(
        this,
        chrome::NOTIFICATION_PROFILE_URL_REQUEST_CONTEXT_GETTER_INITIALIZED,
        content::Source<Profile>(ProfileHelper::GetSigninProfile()));
  }
  ConnectivityStateHelper::Get()->RemoveNetworkManagerObserver(this);
}

void AuthPrewarmer::PrewarmAuthentication(
    const base::Closure& completion_callback) {
  if (doing_prewarm_) {
    LOG(ERROR) << "PrewarmAuthentication called twice.";
    return;
  }
  doing_prewarm_ = true;
  completion_callback_ = completion_callback;
  if (GetRequestContext() && IsNetworkConnected()) {
    DoPrewarm();
    return;
  }
  if (!IsNetworkConnected())
    ConnectivityStateHelper::Get()->AddNetworkManagerObserver(this);
  if (!GetRequestContext()) {
    registrar_.Add(
        this,
        chrome::NOTIFICATION_PROFILE_URL_REQUEST_CONTEXT_GETTER_INITIALIZED,
        content::Source<Profile>(ProfileHelper::GetSigninProfile()));
  }
}

void AuthPrewarmer::NetworkManagerChanged() {
  if (IsNetworkConnected()) {
    ConnectivityStateHelper::Get()->RemoveNetworkManagerObserver(this);
    if (GetRequestContext())
      DoPrewarm();
  }
}

void AuthPrewarmer::DefaultNetworkChanged() {
  NetworkManagerChanged();
}

void AuthPrewarmer::Observe(int type,
                            const content::NotificationSource& source,
                            const content::NotificationDetails& details) {
  switch (type) {
    case chrome::NOTIFICATION_PROFILE_URL_REQUEST_CONTEXT_GETTER_INITIALIZED:
      registrar_.Remove(
          this,
          chrome::NOTIFICATION_PROFILE_URL_REQUEST_CONTEXT_GETTER_INITIALIZED,
          content::Source<Profile>(ProfileHelper::GetSigninProfile()));
      if (IsNetworkConnected())
        DoPrewarm();
      break;
    default:
      NOTREACHED();
  }
}

void AuthPrewarmer::DoPrewarm() {
  const int kConnectionsNeeded = 1;

  std::vector<GURL> urls;
  urls.push_back(GURL(GaiaUrls::GetInstance()->client_login_url()));
  urls.push_back(GURL(GaiaUrls::GetInstance()->service_login_url()));

  for (size_t i = 0; i < urls.size(); ++i) {
    chrome_browser_net::PreconnectOnUIThread(
        urls[i],
        urls[i],
        chrome_browser_net::UrlInfo::EARLY_LOAD_MOTIVATED,
        kConnectionsNeeded,
        GetRequestContext());
  }
  if (!completion_callback_.is_null()) {
    content::BrowserThread::PostTask(content::BrowserThread::UI,
                                     FROM_HERE,
                                     completion_callback_);
  }
}

bool AuthPrewarmer::IsNetworkConnected() const {
  return ConnectivityStateHelper::Get()->IsConnected();
}

net::URLRequestContextGetter* AuthPrewarmer::GetRequestContext() const {
  return ProfileHelper::GetSigninProfile()->GetRequestContext();
}

}  // namespace chromeos

