//------------------------------------------------------------------------------
//   Copyright 2013 NXP B.V.
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

#ifndef UVM_REPORT_HANDLER_H_
#define UVM_REPORT_HANDLER_H_

#include <string>
#include <map>

#include "uvmsc/conf/uvm_pool.h"
#include "uvmsc/base/uvm_object_globals.h"

namespace uvm {

// forward class declaration
class uvm_report_object;
class uvm_report_server;

//------------------------------------------------------------------------------
// CLASS: uvm_report_handler
//
//! The uvm_report_handler is the class to which most methods in
//! #uvm_report_object delegate. It stores the maximum verbosity, actions,
//! and files that affect the way reports are handled.
//!
//! The report handler is not intended for direct use. See #uvm_report_object
//! for information on the UVM reporting mechanism.
//!
//! The relationship between #uvm_report_object (a base class for #uvm_component)
//! and #uvm_report_handler is typically one to one, but it can be many to one
//! if several #uvm_report_objects are configured to use the same
//! #uvm_report_handler_object. See uvm_report_object::set_report_handler.
//!
//! The relationship between #uvm_report_handler and #uvm_report_server is many
//! to one.
//------------------------------------------------------------------------------

typedef uvm_pool<std::string, uvm_action> uvm_id_actions_array;
typedef uvm_pool<std::string, UVM_FILE> uvm_id_file_array;
typedef uvm_pool<std::string, int> uvm_id_verbosities_array;
typedef uvm_pool<uvm_severity, uvm_severity> uvm_sev_override_array;

class uvm_report_handler
{
 public:
  friend class uvm_report_object;
  friend class uvm_report_catcher;

  //--------------------------------------------------------------------------
  // UVM Standard LRM API below
  //--------------------------------------------------------------------------

  uvm_report_handler();

  int get_verbosity_level( uvm_severity severity = UVM_INFO,
                           const std::string& id = "" );

  uvm_action get_action( uvm_severity severity,
                         const std::string& id );

  UVM_FILE get_file_handle( uvm_severity severity,
                            const std::string& id );

  virtual void report( uvm_severity severity,
                       const std::string& name,
                       const std::string& id,
                       const std::string& message,
                       int verbosity_level = UVM_MEDIUM,
                       const std::string& filename = "",
                       int line = 0,
                       uvm_report_object* client = NULL );

  std::string format_action( uvm_action action );

  /////////////////////////////////////////////////////
  /////////////////////////////////////////////////////
  // Implementation-defined member functions below,
  // not part of UVM Class reference / LRM
  /////////////////////////////////////////////////////
  /////////////////////////////////////////////////////

  virtual ~uvm_report_handler();

  // deprecated in 1.2 - will be removed soon
  virtual bool run_hooks( const uvm_report_object* client,
                          uvm_severity severity,
                          const std::string& id,
                          const std::string& message,
                          int verbosity,
                          const std::string& filename,
                          int line ) const;

 private:
  uvm_report_server* get_server();

  void set_max_quit_count( int max_count );

  void summarize( UVM_FILE file = 0 );

  void report_relnotes_banner( UVM_FILE file = 0 );

  void report_header( UVM_FILE file = 0 );

  void m_initialize();

  UVM_FILE get_severity_id_file( uvm_severity severity,
                                 const std::string& id );

  void set_verbosity_level( int verbosity_level );

  void set_defaults();

  void set_severity_action( uvm_severity severity,
                            uvm_action action );

  void set_id_action( const std::string& id,
                      uvm_action action );

  void set_severity_id_action( uvm_severity severity,
                               const std::string& id,
                               uvm_action action );

  void set_id_verbosity( const std::string& id,
                         int verbosity );

  void set_severity_id_verbosity( uvm_severity severity,
                                  const std::string& id,
                                  int verbosity );

  void set_default_file( UVM_FILE file );

  void set_severity_file( uvm_severity severity, UVM_FILE file );

  void set_id_file( const std::string& id, UVM_FILE file );

  void set_severity_id_file( uvm_severity severity,
                             const std::string& id,
                             UVM_FILE file );

  void set_severity_override( uvm_severity cur_severity,
                              uvm_severity new_severity );

  void set_severity_id_override( uvm_severity cur_severity,
                                 const std::string& id,
                                 uvm_severity new_severity );

  void dump_state();


  // internal variables

  static bool m_relnotes_done;

  int m_max_verbosity_level;

  typedef std::map<uvm_severity, uvm_action> severity_actions_mapt;
  severity_actions_mapt severity_actions;
  typedef severity_actions_mapt::iterator severity_actions_mapitt;

  uvm_id_actions_array id_actions;
  typedef std::map<uvm_severity, uvm_id_actions_array> severity_id_actions_mapt;
  typedef severity_id_actions_mapt::iterator severity_id_actions_mapitt;
  severity_id_actions_mapt severity_id_actions;

  // id verbosity settings : default and severity
  uvm_id_verbosities_array id_verbosities;
  typedef std::map<uvm_severity, uvm_id_verbosities_array> severity_id_verbosities_mapt;
  typedef severity_id_verbosities_mapt::iterator severity_id_verbosities_mapitt;
  severity_id_verbosities_mapt severity_id_verbosities;

  // severity overrides
  uvm_sev_override_array sev_overrides;
  std::map<std::string, uvm_sev_override_array> sev_id_overrides;

  // file handles : default, severity, action, (severity,id)
  UVM_FILE default_file_handle;
  typedef std::map<uvm_severity, UVM_FILE> severity_file_handles_mapt;
  typedef severity_file_handles_mapt::iterator severity_file_handles_mapitt;
  severity_file_handles_mapt severity_file_handles;

  uvm_id_file_array id_file_handles;

  typedef std::map<uvm_severity, uvm_id_file_array> severity_id_file_handles_mapt;
  typedef severity_id_file_handles_mapt::iterator severity_id_file_handles_mapitt;
  severity_id_file_handles_mapt severity_id_file_handles;

}; // class uvm_report_handler

} // namespace uvm


#endif // UVM_REPORT_HANDLER_H_

