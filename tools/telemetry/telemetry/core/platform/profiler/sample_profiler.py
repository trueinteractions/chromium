# Copyright (c) 2013 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

import signal
import subprocess
import sys
import tempfile

from telemetry.core import util
from telemetry.core.platform import profiler


class _SingleProcessSampleProfiler(object):
  """An internal class for using iprofiler for a given process."""
  def __init__(self, pid, output_path):
    self._output_path = output_path
    self._tmp_output_file = tempfile.NamedTemporaryFile('w', 0)
    self._proc = subprocess.Popen(
        ['sample', str(pid), '-mayDie', '-file', self._output_path],
        stdout=self._tmp_output_file, stderr=subprocess.STDOUT)
    def IsStarted():
      return 'Sampling process' in self._GetStdOut()
    util.WaitFor(IsStarted, 120)

  def CollectProfile(self):
    self._proc.send_signal(signal.SIGINT)
    exit_code = self._proc.wait()
    try:
      if exit_code:
        raise Exception(
            'sample failed with exit code %d. Output:\n%s' % (
            exit_code, self._GetStdOut()))
    finally:
      self._proc = None
      self._tmp_output_file.close()

    print 'To view the profile, run:'
    print '  open -a TextEdit %s' % self._output_path

  def _GetStdOut(self):
    self._tmp_output_file.flush()
    try:
      with open(self._tmp_output_file.name) as f:
        return f.read()
    except IOError:
      return ''


class SampleProfiler(profiler.Profiler):

  def __init__(self, browser_backend, platform_backend, output_path):
    super(SampleProfiler, self).__init__(
        browser_backend, platform_backend, output_path)
    process_output_file_map = self._GetProcessOutputFileMap()
    self._process_profilers = []
    for pid, output_file in process_output_file_map.iteritems():
      self._process_profilers.append(
          _SingleProcessSampleProfiler(pid, output_file))

  @classmethod
  def name(cls):
    return 'sample'

  @classmethod
  def is_supported(cls, options):
    return (sys.platform == 'darwin'
            and not options.browser_type.startswith('android')
            and not options.browser_type.startswith('cros'))

  def CollectProfile(self):
    for single_process in self._process_profilers:
      single_process.CollectProfile()
