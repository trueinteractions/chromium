// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

function runTests() {
  var getURL = chrome.extension.getURL;
  chrome.tabs.create({"url": "about:blank"}, function(tab) {
    var tabId = tab.id;

    chrome.test.runTests([
      // Opens a new tab from javascript.
      function openTab() {
        expect([
          [ "onBeforeNavigate",
            { frameId: 0,
              requestId: "0",
              tabId: 0,
              timeStamp: 0,
              url: getURL('openTab/a.html') }],
          [ "onCommitted",
            { frameId: 0,
              tabId: 0,
              timeStamp: 0,
              transitionQualifiers: [],
              transitionType: "link",
              url: getURL('openTab/a.html') }],
          [ "onDOMContentLoaded",
            { frameId: 0,
              tabId: 0,
              timeStamp: 0,
              url: getURL('openTab/a.html') }],
          [ "onCompleted",
            { frameId: 0,
              tabId: 0,
              timeStamp: 0,
              url: getURL('openTab/a.html') }],
          [ "onBeforeRetarget",
            { sourceTabId: 0,
              sourceUrl: getURL('openTab/a.html'),
              timeStamp: 0,
              url: getURL('openTab/b.html') }],
          [ "onBeforeNavigate",
            { frameId: 0,
              requestId: "0",
              tabId: 1,
              timeStamp: 0,
              url: getURL('openTab/b.html') }],
          [ "onCommitted",
            { frameId: 0,
              tabId: 1,
              timeStamp: 0,
              transitionQualifiers: [],
              transitionType: "link",
              url: getURL('openTab/b.html') }],
          [ "onDOMContentLoaded",
            { frameId: 0,
              tabId: 1,
              timeStamp: 0,
              url: getURL('openTab/b.html') }],
          [ "onCompleted",
            { frameId: 0,
              tabId: 1,
              timeStamp: 0,
              url: getURL('openTab/b.html') }]]);
        chrome.tabs.update(tabId, { url: getURL('openTab/a.html') });
      },

      // Opens a new tab from javascript within an iframe.
      function openTabFrame() {
        expect([
          [ "onBeforeNavigate",
            { frameId: 0,
              requestId: "0",
              tabId: 0,
              timeStamp: 0,
              url: getURL('openTab/c.html') }],
          [ "onCommitted",
            { frameId: 0,
              tabId: 0,
              timeStamp: 0,
              transitionQualifiers: [],
              transitionType: "link",
              url: getURL('openTab/c.html') }],
          [ "onBeforeNavigate",
            { frameId: 1,
              requestId: "0",
              tabId: 0,
              timeStamp: 0,
              url: getURL('openTab/a.html') }],
          [ "onDOMContentLoaded",
            { frameId: 0,
              tabId: 0,
              timeStamp: 0,
              url: getURL('openTab/c.html') }],
          [ "onCommitted",
            { frameId: 1,
              tabId: 0,
              timeStamp: 0,
              transitionQualifiers: [],
              transitionType: "auto_subframe",
              url: getURL('openTab/a.html') }],
          [ "onDOMContentLoaded",
            { frameId: 1,
              tabId: 0,
              timeStamp: 0,
              url: getURL('openTab/a.html') }],
          [ "onCompleted",
            { frameId: 1,
              tabId: 0,
              timeStamp: 0,
              url: getURL('openTab/a.html') }],
          [ "onCompleted",
            { frameId: 0,
              tabId: 0,
              timeStamp: 0,
              url: getURL('openTab/c.html') }],
          [ "onBeforeRetarget",
            { sourceTabId: 0,
              sourceUrl: getURL('openTab/a.html'),
              timeStamp: 0,
              url: getURL('openTab/b.html') }],
          [ "onBeforeNavigate",
            { frameId: 0,
              requestId: "0",
              tabId: 1,
              timeStamp: 0,
              url: getURL('openTab/b.html') }],
          [ "onCommitted",
            { frameId: 0,
              tabId: 1,
              timeStamp: 0,
              transitionQualifiers: [],
              transitionType: "link",
              url: getURL('openTab/b.html') }],
          [ "onDOMContentLoaded",
            { frameId: 0,
              tabId: 1,
              timeStamp: 0,
              url: getURL('openTab/b.html') }],
          [ "onCompleted",
            { frameId: 0,
              tabId: 1,
              timeStamp: 0,
              url: getURL('openTab/b.html') }]]);
        chrome.tabs.update(tabId, { url: getURL('openTab/c.html') });
      },
    ]);
  });
}
