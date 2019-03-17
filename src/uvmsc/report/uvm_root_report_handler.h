//------------------------------------------------------------------------------
//   Copyright 2014 NXP B.V.
//   Copyright 2007-2011 Mentor Graphics Corporation
//   Copyright 2007-2011 Cadence Design Systems, Inc. 
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
//------------------------------------------------------------------------------

#ifndef UVM_ROOT_REPORT_HANDLER_H_
#define UVM_ROOT_REPORT_HANDLER_H_

#include <string>
#include <map>

#include "uvmsc/base/uvm_object_globals.h"
#include "uvmsc/report/uvm_report_handler.h"

namespace uvm {

// forward class declaration
//none

//----------------------------------------------------------------------
// Class: uvm_root_report_handler
//
//! Root report has name "reporter"
//----------------------------------------------------------------------

class uvm_root_report_handler : public uvm_report_handler
{
 public:
  virtual void report( uvm_severity severity,
                       const std::string& name,
                       const std::string& id,
                       const std::string& message,
                       int verbosity_level = UVM_MEDIUM,
                       const std::string& filename = "",
                       int line = 0,
                       uvm_report_object* client = NULL )
  {
    std::string name_l;

    if(name.empty() || name == "uvm_top")
      name_l = "reporter";
    else
      name_l = name;

    uvm_report_handler::report(severity, name_l, id, message, verbosity_level, filename, line, client);
  }

}; // class uvm_root_report_handler

} // namespace uvm


#endif // UVM_ROOT_REPORT_HANDLER_H_

