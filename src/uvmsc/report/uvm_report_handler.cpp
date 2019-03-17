//----------------------------------------------------------------------------
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
//----------------------------------------------------------------------------

#include "uvmsc/report/uvm_report_handler.h"
#include "uvmsc/report/uvm_report_server.h"
#include "uvmsc/base/uvm_version.h"

namespace uvm {

//------------------------------------------------------------------------------
// Initialization static data members
//------------------------------------------------------------------------------

bool uvm_report_handler::m_relnotes_done = false;

//----------------------------------------------------------------------------
// Constructor
//
//! Creates and initializes a new #uvm_report_handler object.
//----------------------------------------------------------------------------

uvm_report_handler::uvm_report_handler()
{
  //id_actions=new();
  //id_verbosities=new();
  //sev_overrides=new();
  m_initialize();
}

//----------------------------------------------------------------------------
// member function: get_verbosity_level
//
//! Returns the verbosity associated with the given \p severity and \p id.
//!
//! First, if there is a verbosity associated with the \p severity, \p id pair,
//! return that. Else, if there is an verbosity associated with the \p id, return
//! that. Else, return the max verbosity setting.
//----------------------------------------------------------------------------

int uvm_report_handler::get_verbosity_level( uvm_severity severity,
                                             const std::string& id )
{
  uvm_id_verbosities_array array;
  std::string loc_id = id;

  if( severity_id_verbosities.find(severity) != severity_id_verbosities.end() ) //exists
  {
    array = severity_id_verbosities[severity];
    if(array.exists(loc_id))
      return array.get(loc_id);
  }

  if(id_verbosities.exists(loc_id))
    return id_verbosities.get(loc_id);

  return m_max_verbosity_level;
}

//----------------------------------------------------------------------------
// member function: get_action
//
//! Returns the action associated with the given \p severity and \p id.
//!
//! First, if there is an action associated with the \p (severity,id) pair,
//! return that. Else, if there is an action associated with the \p id, return
//! that. Else, if there is an action associated with the \p severity, return
//! that. Else, return the default action associated with the \p severity.
//----------------------------------------------------------------------------

uvm_action uvm_report_handler::get_action( uvm_severity severity,
                                           const std::string& id )
{
  uvm_id_actions_array array;
  std::string loc_id = id;

  if(severity_id_actions.find(severity) != severity_id_actions.end())
  {
    array = severity_id_actions[severity];
    if(array.exists(loc_id))
      return array.get(loc_id);
  }

  if(id_actions.exists(loc_id))
    return id_actions.get(loc_id);

  return severity_actions[severity];
}

//----------------------------------------------------------------------------
// member function: get_file_handle
//
//! Returns the file descriptor associated with the given \p severity and id.
//!
//! First, if there is a file handle associated with the \p (severity,id) pair,
//! return that. Else, if there is a file handle associated with the \p id , return
//! that. Else, if there is an file handle associated with the \p severity, return
//! that. Else, return the default file handle.
//----------------------------------------------------------------------------

UVM_FILE uvm_report_handler::get_file_handle( uvm_severity severity,
                                              const std::string& id )
{
  UVM_FILE file;
  std::string loc_id = id;

  file = get_severity_id_file(severity, id);
  if (file != 0)
    return file;

  if (id_file_handles.exists(loc_id))
  {
    file = id_file_handles.get(loc_id);
    if (file != 0)
      return file;
  }

  if (severity_file_handles.find(severity) != severity_file_handles.end()) //exists
  {
    file = severity_file_handles[severity];
    if(file != 0)
      return file;
  }

  return default_file_handle;
}

//----------------------------------------------------------------------------
// member function: report
//
//! This is the common handler method used by the four core reporting methods
//! (e.g., #uvm_report_error) in #uvm_report_object.
//----------------------------------------------------------------------------

void uvm_report_handler::report( uvm_severity severity,
                                 const std::string& name,
                                 const std::string& id,
                                 const std::string& message,
                                 int verbosity_level,
                                 const std::string& filename,
                                 int line,
                                 uvm_report_object* client )
{
  uvm_report_server* srvr;
  srvr = uvm_report_server::get_server();

  if (client == NULL)
    client = uvm_root::get();

  // Check for severity overrides and apply them before calling the server.
  // An id specific override has precedence over a generic severity override.
  if(sev_id_overrides.find(id) != sev_id_overrides.end()) //exists
  {
    if(sev_id_overrides.find(id)->second.exists(severity))
      severity = sev_id_overrides.find(id)->second.get(severity);
  }
  else
  {
    if(sev_overrides.exists(severity))
       severity = sev_overrides.get(severity);
  }

  srvr->report( severity, name, id, message, verbosity_level,
                filename, line, client);
}

//----------------------------------------------------------------------------
// member function: format_action
//
//! Returns a string representation of the \p action, e.g., "DISPLAY".
//----------------------------------------------------------------------------

std::string uvm_report_handler::format_action( uvm_action action )
{
  std::string s;

  if(uvm_action_type(action) == UVM_NO_ACTION)
    s = "NO ACTION";
  else
  {
    s = "";
    if(action & UVM_DISPLAY)   s = s + "DISPLAY ";
    if(action & UVM_LOG)       s = s + "LOG ";
    if(action & UVM_COUNT)     s = s + "COUNT ";
    if(action & UVM_EXIT)      s = s + "EXIT ";
    if(action & UVM_CALL_HOOK) s = s + "CALL_HOOK ";
    if(action & UVM_STOP)      s = s + "STOP ";
  }

  return s;
}



////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
//////// Implementation-defined member functions start here ////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// destructor
//----------------------------------------------------------------------------

uvm_report_handler::~uvm_report_handler()
{
}

//----------------------------------------------------------------------------
// member function: run_hooks - deprecated in UVM1.2  - will be removed soon

//
//! The member function #run_hooks is called if the #UVM_CALL_HOOK action is set for a
//! report. It first calls the client's uvm_report_object::report_hook method,
//! followed by the appropriate severity-specific hook method. If either
//! returns false, then the report is not processed.
//----------------------------------------------------------------------------

bool uvm_report_handler::run_hooks( const uvm_report_object* client,
                                    uvm_severity severity,
                                    const std::string& id,
                                    const std::string& message,
                                    int verbosity,
                                    const std::string& filename,
                                    int line ) const
{
  bool ok;

  ok = client->report_hook(id, message, verbosity, filename, line);

  switch(severity)
  {
    case UVM_INFO:
      ok &= client->report_info_hook(id, message, verbosity, filename, line);
      break;
    case UVM_WARNING:
      ok &= client->report_warning_hook(id, message, verbosity, filename, line);
      break;
    case UVM_ERROR:
      ok &= client->report_error_hook(id, message, verbosity, filename, line);
      break;
    case UVM_FATAL:
      ok &= client->report_fatal_hook(id, message, verbosity, filename, line);
      break;
  }

  return ok;
}

//----------------------------------------------------------------------------
// member function- get_server
//
//! Internal method called by uvm_report_object::get_report_server.
//----------------------------------------------------------------------------

uvm_report_server* uvm_report_handler::get_server()
{
  return uvm_report_server::get_server();
}


//----------------------------------------------------------------------------
// member function- set_max_quit_count
//
//! Internal method called by uvm_report_object::set_report_max_quit_count.
//----------------------------------------------------------------------------

void uvm_report_handler::set_max_quit_count( int max_count )
{
  uvm_report_server* srvr;
  srvr = uvm_report_server::get_server();
  srvr->set_max_quit_count(max_count);
}


//----------------------------------------------------------------------------
// member function- summarize
//
//! Internal method called by uvm_report_object::report_summarize.
//----------------------------------------------------------------------------

void uvm_report_handler::summarize( UVM_FILE file )
{
  uvm_report_server* srvr;
  srvr = uvm_report_server::get_server();
  srvr->summarize(file);
}


//----------------------------------------------------------------------------
// member function- report_relnotes_banner
//
//! Internal method called by uvm_report_object::report_header.
//----------------------------------------------------------------------------

void uvm_report_handler::report_relnotes_banner( UVM_FILE file )
{
  uvm_report_server* srvr;

  if (m_relnotes_done) return;

  srvr = uvm_report_server::get_server();

  srvr->f_display(file,
      "\n  ***********       IMPORTANT RELEASE NOTES         ************");

  m_relnotes_done = 1;
}

//----------------------------------------------------------------------------
// member function- report_header
//
//! Internal method called by <uvm_report_object::report_header.
//----------------------------------------------------------------------------

void uvm_report_handler::report_header( UVM_FILE file )
{
  uvm_report_server* srvr;

  srvr = uvm_report_server::get_server();
  srvr->f_display(file,
    "----------------------------------------------------------------");
  srvr->f_display(file, uvm_revision_string());
  srvr->f_display(file, uvm_fhg_copyright);
  srvr->f_display(file, uvm_nxp_copyright);
  srvr->f_display(file, uvm_mgc_copyright);
  srvr->f_display(file, uvm_cdn_copyright);
  srvr->f_display(file, uvm_snps_copyright);
  srvr->f_display(file, uvm_cy_copyright);
  srvr->f_display(file,
    "----------------------------------------------------------------");

  {
     //uvm_cmdline_processor clp;
     //vector<std::string> args;

     //clp = uvm_cmdline_processor::get_inst();

     //if (clp.get_arg_matches("+UVM_NO_RELNOTES", args)) return;

#ifndef UVM_NO_DEPRECATED
     report_relnotes_banner(file);
     srvr->f_display(file, "\n  You are using a version of the UVM library that has been compiled");
     srvr->f_display(file, "  with `UVM_NO_DEPRECATED undefined.");
     srvr->f_display(file, "  See http://www.eda.org/svdb/view.php?id=3313 for more details.");
#endif

#ifndef UVM_OBJECT_MUST_HAVE_CONSTRUCTOR
     report_relnotes_banner(file);
     srvr->f_display(file, "\n  You are using a version of the UVM library that has been compiled");
     srvr->f_display(file, "  with `UVM_OBJECT_MUST_HAVE_CONSTRUCTOR undefined.");
     srvr->f_display(file, "  See http://www.eda.org/svdb/view.php?id=3770 for more details.");
#endif

     if (m_relnotes_done)
        srvr->f_display(file, "\n      (Specify +UVM_NO_RELNOTES to turn off this notice)\n");
  }
}

//----------------------------------------------------------------------------
// member function: m_initialize
//
//! This method is called by the constructor to initialize the arrays and
//! other variables described above to their default values.
//----------------------------------------------------------------------------

void uvm_report_handler::m_initialize()
{
  set_default_file(0);
  m_max_verbosity_level = UVM_MEDIUM;
  set_defaults();
}

//----------------------------------------------------------------------------
// member function: get_severity_id_file
//
//! Return the file id based on the \p severity and the \p id.
//----------------------------------------------------------------------------

UVM_FILE uvm_report_handler::get_severity_id_file( uvm_severity severity,
                                                   const std::string& id )
{
  uvm_id_file_array array;
  std::string loc_id = id;

  if(severity_id_file_handles.find(severity) != severity_id_file_handles.end() ) // exists
  {
    array = severity_id_file_handles[severity];
    if(array.exists(loc_id))
      return array.get(loc_id);
  }

  if(id_file_handles.exists(loc_id))
    return id_file_handles.get(loc_id);

  if(severity_file_handles.find(severity) != severity_file_handles.end()) // exists
    return severity_file_handles[severity];

  return default_file_handle;
}


//----------------------------------------------------------------------------
// member function: set_verbosity_level
//
//! Internal method called by uvm_report_object.
//----------------------------------------------------------------------------

void uvm_report_handler::set_verbosity_level(int verbosity_level)
{
  m_max_verbosity_level = verbosity_level;
}


//----------------------------------------------------------------------------
// member function: set_defaults
//
//! Internal method for initializing report handler.
//----------------------------------------------------------------------------

void uvm_report_handler::set_defaults()
{
  set_severity_action(UVM_INFO,    UVM_DISPLAY);
  set_severity_action(UVM_WARNING, UVM_DISPLAY);
  set_severity_action(UVM_ERROR,   UVM_DISPLAY | UVM_COUNT);
  set_severity_action(UVM_FATAL,   UVM_DISPLAY | UVM_EXIT);

  set_severity_file(UVM_INFO, default_file_handle);
  set_severity_file(UVM_WARNING, default_file_handle);
  set_severity_file(UVM_ERROR,   default_file_handle);
  set_severity_file(UVM_FATAL,   default_file_handle);
}

//----------------------------------------------------------------------------
// member function: set_severity_action
//
//! Internal method called by #uvm_report_object.
//----------------------------------------------------------------------------

void uvm_report_handler::set_severity_action( uvm_severity severity,
                          uvm_action action )
{
  severity_actions[severity] = action;
}

//----------------------------------------------------------------------------
// member function: set_id_action
//
//! Internal method called by #uvm_report_object.
//----------------------------------------------------------------------------

void uvm_report_handler::set_id_action( const std::string& id,
                                        uvm_action action )
{
  id_actions.add(id, action);
}

//----------------------------------------------------------------------------
// member function: set_severity_id_action
//
//! Internal method called by #uvm_report_object.
//----------------------------------------------------------------------------

void uvm_report_handler::set_severity_id_action( uvm_severity severity,
                                                 const std::string& id,
                                                 uvm_action action )
{
  //if(severity_id_actions.find(severity) == severity_id_actions.end() ) // not exists
  //  severity_id_actions[severity] = new;

  // TODO check - no need to new item?
  severity_id_actions[severity].add(id,action);
}

//----------------------------------------------------------------------------
// member function: set_id_verbosity
//
//! Internal method called by #uvm_report_object.
//----------------------------------------------------------------------------

void uvm_report_handler::set_id_verbosity( const std::string& id,
                                           int verbosity )
{
  id_verbosities.add(id, verbosity);
}

//----------------------------------------------------------------------------
// member function: set_severity_id_verbosity
//
//! Internal method called by #uvm_report_object.
//----------------------------------------------------------------------------

void uvm_report_handler::set_severity_id_verbosity( uvm_severity severity,
                                                    const std::string& id,
                                                    int verbosity )
{
  //if(!severity_id_verbosities.exists(severity))
  //  severity_id_verbosities[severity] = new;

  // TODO check - no need to new item?
  severity_id_verbosities[severity].add(id, verbosity);
}

//----------------------------------------------------------------------------
// member function: set_default_file
//
//! Internal methods called by #uvm_report_object.
//----------------------------------------------------------------------------

void uvm_report_handler::set_default_file( UVM_FILE file )
{
  default_file_handle = file;
}

//----------------------------------------------------------------------------
// member function: set_severity_file
//
//! Internal methods called by #uvm_report_object.
//----------------------------------------------------------------------------

void uvm_report_handler::set_severity_file( uvm_severity severity,
                                            UVM_FILE file )
{
  severity_file_handles[severity] = file;
}

//----------------------------------------------------------------------------
// member function: set_id_file
//
//! Internal methods called by #uvm_report_object.
//----------------------------------------------------------------------------

void uvm_report_handler::set_id_file( const std::string& id,
                                      UVM_FILE file )
{
  id_file_handles.add(id, file);
}

//----------------------------------------------------------------------------
// member function: set_severity_id_file
//
//! Internal methods called by #uvm_report_object.
//----------------------------------------------------------------------------

void uvm_report_handler::set_severity_id_file( uvm_severity severity,
                                               const std::string& id,
                                               UVM_FILE file)
{
//  if(severity_id_file_handles.find(severity) == severity_id_file_handles.end()) // not exists
//    severity_id_file_handles[severity] = new;

  // TODO check - no need to new item?
  severity_id_file_handles[severity].add(id, file);
}

//----------------------------------------------------------------------------
// member function: set_severity_override
//
//! Internal methods called by #uvm_report_object.
//----------------------------------------------------------------------------

void uvm_report_handler::set_severity_override( uvm_severity cur_severity,
                                                uvm_severity new_severity)
{
  sev_overrides.add(cur_severity, new_severity);
}

//----------------------------------------------------------------------------
// member function: set_severity_id_override
//
//! Internal methods called by #uvm_report_object.
//----------------------------------------------------------------------------

void uvm_report_handler::set_severity_id_override( uvm_severity cur_severity,
                                                   const std::string& id,
                                                   uvm_severity new_severity)
{
  // has precedence over set_severity_override
  // silently override previous setting
  uvm_sev_override_array arr;

  //if(!sev_id_overrides.exists(id))
  //  sev_id_overrides[id] = new;

  // TODO check - no need to new item?
  sev_id_overrides[id].add(cur_severity, new_severity);
}


//----------------------------------------------------------------------------
// member function: dump_state
//
//! Internal method for debug.
//----------------------------------------------------------------------------

void uvm_report_handler::dump_state()
{
  std::string s;
  std::string idx;
  UVM_FILE file;
  uvm_report_server* srvr;

  uvm_id_actions_array id_a_ary;
  uvm_id_verbosities_array id_v_ary;
  uvm_id_file_array id_f_ary;

  srvr = uvm_report_server::get_server();

  srvr->f_display(0,
    "----------------------------------------------------------------------");
  srvr->f_display(0, "report handler state dump");
  srvr->f_display(0, "");

  // verbosities

  srvr->f_display(0, "");
  srvr->f_display(0, "+-----------------+");
  srvr->f_display(0, "|   Verbosities   |");
  srvr->f_display(0, "+-----------------+");
  srvr->f_display(0, "");

  std::ostringstream str;
  str << "max verbosity level = "
      << m_max_verbosity_level;
  srvr->f_display(0, str.str());

  srvr->f_display(0, "*** verbosities by id");

  if(id_verbosities.first(idx))
  do
  {
    uvm_verbosity v = (uvm_verbosity)id_verbosities.get(idx);
    std::ostringstream s;
    s << "["
      << idx
      << "] --> "
      << uvm_verbosity_name[v/100]; //TODO check
    srvr->f_display(0, s.str());
  }
  while(id_verbosities.next(idx));

  // verbosities by id

  srvr->f_display(0, "");
  srvr->f_display(0, "*** verbosities by id and severity");

  for ( severity_id_verbosities_mapitt it = severity_id_verbosities.begin();
        it != severity_id_verbosities.end();
        it++)
  {
    uvm_severity sev = it->first; //severity;
    id_v_ary = it->second; // severity_id_verbosities[severity];
    if(id_v_ary.first(idx))
    do
    {
      uvm_verbosity v = (uvm_verbosity)id_v_ary.get(idx);
      std::ostringstream s;
      s << uvm_severity_name[sev]
        << ":"
        << idx
        << " --> "
        << uvm_verbosity_name[v/100]; // TODO check
      srvr->f_display(0, s.str());
    }
    while(id_v_ary.next(idx));
  }

  // actions

  srvr->f_display(0, "");
  srvr->f_display(0, "+-------------+");
  srvr->f_display(0, "|   actions   |");
  srvr->f_display(0, "+-------------+");
  srvr->f_display(0, "");

  srvr->f_display(0, "*** actions by severity");

  for ( severity_actions_mapitt it = severity_actions.begin();
        it != severity_actions.end();
        it++)
  {
    uvm_severity sev = it->first; // = severity
    std::ostringstream s;
    s << uvm_severity_name[sev]
      << " = "
      << format_action(it->second); // = severity_actions[severity]
    srvr->f_display(0, s.str());
  }

  srvr->f_display(0, "");
  srvr->f_display(0, "*** actions by id");

  if(id_actions.first(idx))
  do
  {
    std::ostringstream s;
    s << "["
      << idx
      << "] --> "
      << format_action(id_actions.get(idx));
    srvr->f_display(0, s.str());
  }
  while(id_actions.next(idx));

  // actions by id

  srvr->f_display(0, "");
  srvr->f_display(0, "*** actions by id and severity");

  for ( severity_id_verbosities_mapitt it = severity_id_actions.begin();
        it != severity_id_actions.end();
        it++)
  {
    uvm_severity sev = it->first; // = severity;
    id_a_ary = it->second; // = severity_id_actions[severity];
    if(id_a_ary.first(idx))
    do
    {
      std::ostringstream s;
      s << uvm_severity_name[sev]
        << ":"
        << idx
        << " --> "
        << format_action(id_a_ary.get(idx));
      srvr->f_display(0, s.str());
    }
    while(id_a_ary.next(idx));
  }

  // Files

  srvr->f_display(0, "");
  srvr->f_display(0, "+-------------+");
  srvr->f_display(0, "|    files    |");
  srvr->f_display(0, "+-------------+");
  srvr->f_display(0, "");

  str.clear();
  str << "default file handle = "
      << default_file_handle;
  srvr->f_display(0, str.str());

  srvr->f_display(0, "");
  srvr->f_display(0, "*** files by severity");

  for ( severity_file_handles_mapitt it = severity_file_handles.begin();
        it != severity_file_handles.end();
        it++)
  {
    uvm_severity sev = it->first; // = severity;
    file = it->second; //severity_file_handles[severity];
    std::ostringstream s;
    s << uvm_severity_name[sev]
      << " = "
      << file;
    srvr->f_display(0, s.str());
  }

  srvr->f_display(0, "");
  srvr->f_display(0, "*** files by id");

  if(id_file_handles.first(idx))
  do
  {
    file = id_file_handles.get(idx);
    std::ostringstream s;
    s << "id "
      << idx
      << " --> "
      << file;
    srvr->f_display(0, s.str());
  }
  while (id_file_handles.next(idx));

  srvr->f_display(0, "");
  srvr->f_display(0, "*** files by id and severity");

  for ( severity_id_file_handles_mapitt it = severity_id_file_handles.begin();
        it != severity_id_file_handles.end();
        it++)
  {
    uvm_severity sev = it->first; // = severity;
    id_f_ary = it->second; // = severity_id_file_handles[severity];
    if(id_f_ary.first(idx))
    do
    {
      std::ostringstream s;
      s << uvm_severity_name[sev]
        << ":"
        << idx
        << " --> "
        << id_f_ary.get(idx);
      srvr->f_display(0, s.str());
    }
    while(id_f_ary.next(idx));
  }

  srvr->dump_server_state();

  srvr->f_display(0,
    "----------------------------------------------------------------------");
}

} // namespace uvm
