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

#ifndef UVM_REPORT_CATCHER_H_
#define UVM_REPORT_CATCHER_H_

#include <string>

#include "uvmsc/macros/uvm_callback_defines.h"
#include "uvmsc/cb/uvm_callbacks.h"
#include "uvmsc/cb/uvm_callback_iter.h"

namespace uvm {

// forward class declarations
class uvm_report_object;
class uvm_report_handler;
class uvm_report_server;
class uvm_report_catcher;


typedef uvm_callbacks<uvm_report_object, uvm_report_catcher> uvm_report_cb;
typedef uvm_callback_iter<uvm_report_object, uvm_report_catcher> uvm_report_cb_iter;

struct sev_id_struct
{
  bool sev_specified;
  bool id_specified;
  uvm_severity sev;
  std::string id;
  bool is_on;
};

//----------------------------------------------------------------------------
// CLASS: uvm_report_catcher
//
//! The uvm_report_catcher is used to catch messages issued by the uvm report
//! server. Catchers are
//! #uvm_callbacks<uvm_report_object, uvm_report_catcher> objects,
//! so all factilities in the #uvm_callback and #uvm_callbacks<T,CB>
//! classes are available for registering catchers and controlling catcher
//! state.
//! The #uvm_callbacks<uvm_report_object, uvm_report_catcher> class is
//! aliased to #uvm_report_cb to make it easier to use.
//! Multiple report catchers can be
//! registered with a report object. The catchers can be registered as default
//! catchers which catch all reports on all #uvm_report_object reporters,
//! or catchers can be attached to specific report objects (i.e. components).
//!
//! User extensions of #uvm_report_catcher must implement the #do_catch method in
//! which the action to be taken on catching the report is specified. The catch
//! method can return CAUGHT, in which case further processing of the report is
//! immediately stopped, or return THROW in which case the (possibly modified) report
//! is passed on to other registered catchers. The catchers are processed in the order
//! in which they are registered.
//!
//! On catching a report, the #do_catch method can modify the severity, id, action,
//! verbosity or the report string itself before the report is finally issued by
//! the report server. The report can be immediately issued from within the catcher
//! class by calling the #issue method.
//!
//! The catcher maintains a count of all reports with FATAL,ERROR or WARNING severity
//! and a count of all reports with FATAL, ERROR or WARNING severity whose severity
//! was lowered. These statistics are reported in the summary of the <uvm_report_server>.
//----------------------------------------------------------------------------

class uvm_report_catcher : public uvm_callback
{
 public:
  friend class uvm_report_server;
  friend class uvm_root;

  typedef enum { UNKNOWN_ACTION, THROW, CAUGHT} action_e;

  //--------------------------------------------------------------------------
  // UVM Standard LRM API below
  //--------------------------------------------------------------------------

  uvm_report_catcher( const std::string& name = "uvm_report_catcher" );

  //--------------------------------------------------------------------------
  // Group: Current Message State
  //--------------------------------------------------------------------------

  uvm_report_object* get_client() const;

  uvm_severity get_severity() const;

  int get_verbosity() const;
  
  std::string get_id() const;
  
  std::string get_message() const;
  
  uvm_action get_action() const;

  std::string get_fname() const;
  
  int get_line() const;
  
  //--------------------------------------------------------------------------
  // Group: Change Message State
  //--------------------------------------------------------------------------

 protected:

  void set_severity( uvm_severity severity );

  void set_verbosity( int verbosity );

  void set_id( const std::string& id );

  void set_message( const std::string& message );
  
  void set_action( uvm_action action );

  //--------------------------------------------------------------------------
  // Group: Debug
  //--------------------------------------------------------------------------

 public:
  static uvm_report_catcher* get_report_catcher( const std::string& name );

  static void print_catcher( UVM_FILE file = 0 );

  //--------------------------------------------------------------------------
  // Group: Callback interface
  //--------------------------------------------------------------------------

  virtual action_e do_catch() = 0; // was catch, but is reserved keyword in C++

  //--------------------------------------------------------------------------
  // Group: Reporting
  //--------------------------------------------------------------------------

 protected:
  void uvm_report_fatal( const std::string& id,
                         const std::string& message,
                         int verbosity,
                         const std::string& fname = "",
                         int line = 0 );

  void uvm_report_error( const std::string& id,
                         const std::string& message,
                         int verbosity,
                         const std::string& fname = "",
                         int line = 0 );
   
  void uvm_report_warning( const std::string& id,
                           const std::string& message,
                           int verbosity,
                           const std::string& fname = "",
                           int line = 0 );
   
  void uvm_report_info( const std::string& id,
                        const std::string& message,
                        int verbosity,
                        const std::string& fname = "",
                        int line = 0 );

  void issue();

 public:
  static void summarize_report_catcher( UVM_FILE file = 0 ); //TODO added default, not in SV

  /////////////////////////////////////////////////////
  /////////////////////////////////////////////////////
  // Implementation-defined member functions below,
  // not part of UVM Class reference / LRM
  /////////////////////////////////////////////////////
  /////////////////////////////////////////////////////

 private:

  // Group: Current Message State

  std::string get_context() const;

  // Group: Debug

  static void debug_report_catcher( int what = 0 );

  // Group: Reporting

  void uvm_report( uvm_severity severity,
                   const std::string& id,
                   const std::string& message,
                   int verbosity,
                   const std::string& fname="",
                   int line = 0);

  int process_report_catcher();

  static int process_all_report_catchers( uvm_report_server* server, //input
                                          uvm_report_object*& client, //input - TODO check
                                          uvm_severity& severity, //ref
                                          const std::string& name, //input
                                          std::string& id, //ref
                                          std::string& message, //ref
                                          int& verbosity_level, //ref
                                          uvm_action& action, //ref
                                          const std::string& filename, //input
                                          const int& line ); //input

  // Other methods

  static void f_display( UVM_FILE file, const std::string& str );

  UVM_REGISTER_CB(uvm_report_object,uvm_report_catcher)

}; // class uvm_report_catcher

} // namespace uvm

#endif // UVM_REPORT_CATCHER_H_
