// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_METRICS_VARIATIONS_VARIATIONS_REQUEST_SCHEDULER_H_
#define CHROME_BROWSER_METRICS_VARIATIONS_VARIATIONS_REQUEST_SCHEDULER_H_

#include "base/bind.h"
#include "base/gtest_prod_util.h"
#include "base/time/time.h"
#include "base/timer/timer.h"

class PrefService;

namespace chrome_variations {

// A helper class that makes VariationsService requests at the correct times.
class VariationsRequestScheduler {
 public:
  virtual ~VariationsRequestScheduler();

  // Starts the task on a schedule.
  virtual void Start();

  // Resets the scheduler if it is currently on a timer.
  virtual void Reset();

  // Schedules a fetch shortly, for example to re-try the initial request which
  // may have failed.
  void ScheduleFetchShortly();

  // Factory method for this class.
  static VariationsRequestScheduler* Create(const base::Closure& task,
                                            PrefService* local_state);

 protected:
  // |task| is the closure to call when the scheduler deems ready.
  explicit VariationsRequestScheduler(const base::Closure& task);

  // Getter for derived classes.
  base::Closure task() const;

 private:
  FRIEND_TEST_ALL_PREFIXES(VariationsRequestSchedulerTest,
                           ScheduleFetchShortly);

  // The task scheduled by this class.
  base::Closure task_;

  // The timer used to repeatedly ping the server. Keep this as an instance
  // member so if VariationsRequestScheduler goes out of scope, the timer is
  // automatically canceled.
  base::RepeatingTimer<VariationsRequestScheduler> timer_;

  // A one-shot timer used for scheduling out-of-band fetches.
  base::OneShotTimer<VariationsRequestScheduler> one_shot_timer_;

  DISALLOW_COPY_AND_ASSIGN(VariationsRequestScheduler);
};

}  // namespace chrome_variations

#endif  // CHROME_BROWSER_METRICS_VARIATIONS_VARIATIONS_REQUEST_SCHEDULER_H_
