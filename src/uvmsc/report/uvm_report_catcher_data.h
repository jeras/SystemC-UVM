//----------------------------------------------------------------------------
//   Copyright 2013 NXP B.V.
//   Copyright 2007-2010 Mentor Graphics Corporation
//   Copyright 2007-2009 Cadence Design Systems, Inc.
//   Copyright 2010 Synopsys, Inc.
//   All Rights Reserved Worldwide
//
//   Licensed under the Apache License, Version 2.0 (the
//   "License"); you may not use this file except in
//   compliance with the License.  You may obtain a copy of
//   the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in
//   writing, software distributed under the License is
//   distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
//   CONDITIONS OF ANY KIND, either express or implied.  See
//   the License for the specific language governing
//   permissions and limitations under the License.
//----------------------------------------------------------------------------

#ifndef UVM_REPORT_CATCHER_DATA_H_
#define UVM_REPORT_CATCHER_DATA_H_

#include <string>

#include "uvmsc/base/uvm_object_globals.h"
#include "uvmsc/report/uvm_report_object.h"

namespace uvm {

//----------------------------------------------------------------------------
// CLASS: uvm_report_catcher_data
//
//! TODO: add description
//----------------------------------------------------------------------------

class uvm_report_catcher_data
{
 private:
  uvm_report_catcher_data();

  // not to be implemented
  uvm_report_catcher_data(uvm_report_catcher_data const&);
  void operator=(uvm_report_catcher_data const&);

 public:

  static uvm_report_catcher_data& get() {
	  static uvm_report_catcher_data instance;
	  return instance;
  }

  uvm_severity m_modified_severity;
  int m_modified_verbosity;
  std::string m_modified_id;
  std::string m_modified_message;
  std::string m_file_name;
  int m_line_number;
  uvm_report_object* m_client;
  uvm_action m_modified_action;
  bool m_set_action_called;
  uvm_report_server* m_server;
  std::string m_name;

  int m_demoted_fatal;
  int m_demoted_error;
  int m_demoted_warning;
  int m_caught_fatal;
  int m_caught_error;
  int m_caught_warning;

  const int DO_NOT_CATCH;
  const int DO_NOT_MODIFY
  ;
  int m_debug_flags;

  uvm_severity m_orig_severity;
  uvm_action   m_orig_action;
  std::string  m_orig_id;
  int          m_orig_verbosity;
  std::string  m_orig_message;

  bool do_report;

}; // class uvm_report_catcher_data

} // namespace uvm

#endif // UVM_REPORT_CATCHER_DATA_H_
