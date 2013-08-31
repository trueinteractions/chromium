// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/translate/translate_browser_metrics.h"

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "base/metrics/histogram.h"
#include "base/metrics/histogram_samples.h"
#include "base/metrics/statistics_recorder.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "testing/platform_test.h"

using base::HistogramBase;
using base::HistogramSamples;
using base::StatisticsRecorder;

namespace {

class MetricsRecorder {
 public:
  explicit MetricsRecorder(const char* key) : key_(key) {
    StatisticsRecorder::Initialize();

    HistogramBase* histogram = StatisticsRecorder::FindHistogram(key_);
    if (histogram)
      base_samples_ = histogram->SnapshotSamples();
  }

  void CheckInitiationStatus(int expected_disabled_by_prefs,
                             int expected_disabled_by_switch,
                             int expected_disabled_by_config,
                             int expected_language_is_not_supported,
                             int expected_url_is_not_supported,
                             int expected_similar_languages,
                             int expected_accept_languages,
                             int expected_auto_by_config,
                             int expected_auto_by_link,
                             int expected_show_infobar) {
    Snapshot();

    EXPECT_EQ(expected_disabled_by_prefs, GetCountWithoutSnapshot(
        TranslateBrowserMetrics::INITIATION_STATUS_DISABLED_BY_PREFS));
    EXPECT_EQ(expected_disabled_by_switch, GetCountWithoutSnapshot(
        TranslateBrowserMetrics::INITIATION_STATUS_DISABLED_BY_SWITCH));
    EXPECT_EQ(expected_disabled_by_config, GetCountWithoutSnapshot(
        TranslateBrowserMetrics::INITIATION_STATUS_DISABLED_BY_CONFIG));
    EXPECT_EQ(expected_language_is_not_supported, GetCountWithoutSnapshot(
        TranslateBrowserMetrics::INITIATION_STATUS_LANGUAGE_IS_NOT_SUPPORTED));
    EXPECT_EQ(expected_url_is_not_supported, GetCountWithoutSnapshot(
        TranslateBrowserMetrics::INITIATION_STATUS_URL_IS_NOT_SUPPORTED));
    EXPECT_EQ(expected_similar_languages, GetCountWithoutSnapshot(
        TranslateBrowserMetrics::INITIATION_STATUS_SIMILAR_LANGUAGES));
    EXPECT_EQ(expected_accept_languages, GetCountWithoutSnapshot(
        TranslateBrowserMetrics::INITIATION_STATUS_ACCEPT_LANGUAGES));
    EXPECT_EQ(expected_auto_by_config, GetCountWithoutSnapshot(
        TranslateBrowserMetrics::INITIATION_STATUS_AUTO_BY_CONFIG));
    EXPECT_EQ(expected_auto_by_link, GetCountWithoutSnapshot(
        TranslateBrowserMetrics::INITIATION_STATUS_AUTO_BY_LINK));
    EXPECT_EQ(expected_show_infobar, GetCountWithoutSnapshot(
        TranslateBrowserMetrics::INITIATION_STATUS_SHOW_INFOBAR));
  }

  HistogramBase::Count GetTotalCount() {
    Snapshot();
    if (!samples_.get())
      return 0;
    HistogramBase::Count count = samples_->TotalCount();
    if (!base_samples_.get())
      return count;
    return count - base_samples_->TotalCount();
  }

  HistogramBase::Count GetCount(HistogramBase::Sample value) {
    Snapshot();
    return GetCountWithoutSnapshot(value);
  }

 private:
  void Snapshot() {
    HistogramBase* histogram = StatisticsRecorder::FindHistogram(key_);
    if (!histogram)
      return;
    samples_ = histogram->SnapshotSamples();
  }

  HistogramBase::Count GetCountWithoutSnapshot(HistogramBase::Sample value) {
    if (!samples_.get())
      return 0;
    HistogramBase::Count count = samples_->GetCount(value);
    if (!base_samples_.get())
      return count;
    return count - base_samples_->GetCount(value);
  }

  std::string key_;
  scoped_ptr<HistogramSamples> base_samples_;
  scoped_ptr<HistogramSamples> samples_;

  DISALLOW_COPY_AND_ASSIGN(MetricsRecorder);
};

}  // namespace

TEST(TranslateBrowserMetricsTest, ReportInitiationStatus) {
  MetricsRecorder recorder(TranslateBrowserMetrics::GetMetricsName(
      TranslateBrowserMetrics::UMA_INITIATION_STATUS));

  recorder.CheckInitiationStatus(0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  TranslateBrowserMetrics::ReportInitiationStatus(
      TranslateBrowserMetrics::INITIATION_STATUS_DISABLED_BY_PREFS);
  recorder.CheckInitiationStatus(1, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  TranslateBrowserMetrics::ReportInitiationStatus(
      TranslateBrowserMetrics::INITIATION_STATUS_DISABLED_BY_SWITCH);
  recorder.CheckInitiationStatus(1, 1, 0, 0, 0, 0, 0, 0, 0, 0);
  TranslateBrowserMetrics::ReportInitiationStatus(
      TranslateBrowserMetrics::INITIATION_STATUS_DISABLED_BY_CONFIG);
  recorder.CheckInitiationStatus(1, 1, 1, 0, 0, 0, 0, 0, 0, 0);
  TranslateBrowserMetrics::ReportInitiationStatus(
      TranslateBrowserMetrics::INITIATION_STATUS_LANGUAGE_IS_NOT_SUPPORTED);
  recorder.CheckInitiationStatus(1, 1, 1, 1, 0, 0, 0, 0, 0, 0);
  TranslateBrowserMetrics::ReportInitiationStatus(
      TranslateBrowserMetrics::INITIATION_STATUS_URL_IS_NOT_SUPPORTED);
  recorder.CheckInitiationStatus(1, 1, 1, 1, 1, 0, 0, 0, 0, 0);
  TranslateBrowserMetrics::ReportInitiationStatus(
      TranslateBrowserMetrics::INITIATION_STATUS_SIMILAR_LANGUAGES);
  recorder.CheckInitiationStatus(1, 1, 1, 1, 1, 1, 0, 0, 0, 0);
  TranslateBrowserMetrics::ReportInitiationStatus(
      TranslateBrowserMetrics::INITIATION_STATUS_ACCEPT_LANGUAGES);
  recorder.CheckInitiationStatus(1, 1, 1, 1, 1, 1, 1, 0, 0, 0);
  TranslateBrowserMetrics::ReportInitiationStatus(
      TranslateBrowserMetrics::INITIATION_STATUS_AUTO_BY_CONFIG);
  recorder.CheckInitiationStatus(1, 1, 1, 1, 1, 1, 1, 1, 0, 0);
  TranslateBrowserMetrics::ReportInitiationStatus(
      TranslateBrowserMetrics::INITIATION_STATUS_AUTO_BY_LINK);
  recorder.CheckInitiationStatus(1, 1, 1, 1, 1, 1, 1, 1, 1, 0);
  TranslateBrowserMetrics::ReportInitiationStatus(
      TranslateBrowserMetrics::INITIATION_STATUS_SHOW_INFOBAR);
  recorder.CheckInitiationStatus(1, 1, 1, 1, 1, 1, 1, 1, 1, 1);
}

TEST(TranslateBrowserMetricsTest, ReportLanguageDetectionError) {
  MetricsRecorder recorder(TranslateBrowserMetrics::GetMetricsName(
      TranslateBrowserMetrics::UMA_LANGUAGE_DETECTION_ERROR));
  EXPECT_EQ(0, recorder.GetTotalCount());
  TranslateBrowserMetrics::ReportLanguageDetectionError();
  EXPECT_EQ(1, recorder.GetTotalCount());

}


TEST(TranslateBrowserMetricsTest, ReportedLocalesOnDisabledByPrefs) {
  const int ENGLISH = 25966;

  MetricsRecorder recorder(TranslateBrowserMetrics::GetMetricsName(
      TranslateBrowserMetrics::UMA_LOCALES_ON_DISABLED_BY_PREFS));
  EXPECT_EQ(0, recorder.GetTotalCount());
  TranslateBrowserMetrics::ReportLocalesOnDisabledByPrefs("en");
  EXPECT_EQ(1, recorder.GetCount(ENGLISH));
}

TEST(TranslateBrowserMetricsTest, ReportedUndisplayableLanguage) {
  const int ENGLISH = 25966;

  MetricsRecorder recorder(TranslateBrowserMetrics::GetMetricsName(
      TranslateBrowserMetrics::UMA_UNDISPLAYABLE_LANGUAGE));
  EXPECT_EQ(0, recorder.GetTotalCount());
  TranslateBrowserMetrics::ReportUndisplayableLanguage("en");
  EXPECT_EQ(1, recorder.GetCount(ENGLISH));
}

TEST(TranslateBrowserMetricsTest, ReportedUnsupportedLanguageAtInitiation) {
  const int ENGLISH = 25966;

  MetricsRecorder recorder(TranslateBrowserMetrics::GetMetricsName(
      TranslateBrowserMetrics::UMA_UNSUPPORTED_LANGUAGE_AT_INITIATION));
  EXPECT_EQ(0, recorder.GetTotalCount());
  TranslateBrowserMetrics::ReportUnsupportedLanguageAtInitiation("en");
  EXPECT_EQ(1, recorder.GetCount(ENGLISH));
}
