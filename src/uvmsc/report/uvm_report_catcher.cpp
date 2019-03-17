//------------------------------------------------------------------------------
//   Copyright 2013-2014 NXP B.V.
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
//------------------------------------------------------------------------------

#include <iostream>
#include <iomanip>

#include "uvmsc/report/uvm_report_object.h"
#include "uvmsc/report/uvm_report_catcher.h"
#include "uvmsc/report/uvm_report_catcher_data.h"
#include "uvmsc/report/uvm_report_handler.h"
#include "uvmsc/report/uvm_report_server.h"
#include "uvmsc/macros/uvm_callback_defines.h"

namespace uvm {

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------

uvm_report_catcher::uvm_report_catcher( const std::string& name )
  : uvm_callback(name)
{
	uvm_report_catcher_data::get().do_report = true;

  // register the catcher callback
  // this differs from UVMSV since we cannot register using the macro
  // because the static method cannot be initialize within the class declaration
  m_register_cb();
}

//------------------------------------------------------------------------------
// Group: Current Message State
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// member function: get_client
//
//! Returns the #uvm_report_object that has generated the message that
//! is currently being processes.
//------------------------------------------------------------------------------

uvm_report_object* uvm_report_catcher::get_client() const
{
  return uvm_report_catcher_data::get().m_client;
}

//------------------------------------------------------------------------------
// member function: get_severity
//
//! Returns the #uvm_severity of the message that is currently being
//! processed. If the severity was modified by a previously executed
//! catcher object (which re-threw the message), then the returned
//! severity is the modified value.
//------------------------------------------------------------------------------

uvm_severity uvm_report_catcher::get_severity() const
{
  return uvm_report_catcher_data::get().m_modified_severity;
}

//------------------------------------------------------------------------------
// member function: get_context
//
//! Returns the context (source) of the message that is currently being
//! processed. This is typically the full hierarchical name of the component
//! that issued the message. However, when the message comes via a report
//! handler that is not associated with a component, the context is
//! user-defined.
//------------------------------------------------------------------------------

std::string uvm_report_catcher::get_context() const
{
  return uvm_report_catcher_data::get().m_name;
}

//------------------------------------------------------------------------------
// member function: get_verbosity
//
//! Returns the verbosity of the message that is currently being
//! processed. If the verbosity was modified by a previously executed
//! catcher (which re-threw the message), then the returned
//! verbosity is the modified value.
//------------------------------------------------------------------------------

int uvm_report_catcher::get_verbosity() const
{
  return uvm_report_catcher_data::get().m_modified_verbosity;
}

//------------------------------------------------------------------------------
// member function: get_id
//
//! Returns the string id of the message that is currently being
//! processed. If the id was modified by a previously executed
//! catcher (which re-threw the message), then the returned
//! id is the modified value.
//------------------------------------------------------------------------------

std::string uvm_report_catcher::get_id() const
{
  return uvm_report_catcher_data::get().m_modified_id;
}

//------------------------------------------------------------------------------
// member function: get_message
//
//! Returns the string message of the message that is currently being
//! processed. If the message was modified by a previously executed
//! catcher (which re-threw the message), then the returned
//! message is the modified value.
//------------------------------------------------------------------------------

std::string uvm_report_catcher::get_message() const
{
  return uvm_report_catcher_data::get().m_modified_message;
}

//------------------------------------------------------------------------------
// member function: get_action
//
// Returns the <uvm_action> of the message that is currently being
// processed. If the action was modified by a previously executed
// catcher (which re-threw the message), then the returned
// action is the modified value.
//------------------------------------------------------------------------------

uvm_action uvm_report_catcher::get_action() const
{
  return uvm_report_catcher_data::get().m_modified_action;
}

//------------------------------------------------------------------------------
// member function: get_fname
//
//! Returns the file name of the message.
//------------------------------------------------------------------------------

std::string uvm_report_catcher::get_fname() const
{
  return uvm_report_catcher_data::get().m_file_name;
}

//------------------------------------------------------------------------------
// member function: get_line
//
//! Returns the line number of the message.
//------------------------------------------------------------------------------

int uvm_report_catcher::get_line() const
{
  return uvm_report_catcher_data::get().m_line_number;
}

//------------------------------------------------------------------------------
// Group: Change Message State
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// member function: set_severity
//
//! Change the severity of the message to \p severity. Any other
//! report catchers will see the modified value.
//------------------------------------------------------------------------------

void uvm_report_catcher::set_severity( uvm_severity severity )
{
  uvm_report_catcher_data::get().m_modified_severity = severity;
}

//------------------------------------------------------------------------------
// member function: set_verbosity
//
//! Change the verbosity of the message to \p verbosity. Any other
//! report catchers will see the modified value.
//------------------------------------------------------------------------------

void uvm_report_catcher::set_verbosity( int verbosity )
{
  uvm_report_catcher_data::get().m_modified_verbosity = verbosity;
}

//------------------------------------------------------------------------------
// member function: set_id
//
//! Change the id of the message to \p id. Any other
//! report catchers will see the modified value.
//------------------------------------------------------------------------------

void uvm_report_catcher::set_id( const std::string& id )
{
  uvm_report_catcher_data::get().m_modified_id = id;
}

//------------------------------------------------------------------------------
// member function: set_message
//
//! Change the text of the message to \p message. Any other
//! report catchers will see the modified value.
//------------------------------------------------------------------------------

void uvm_report_catcher::set_message( const std::string& message )
{
  uvm_report_catcher_data::get().m_modified_message = message;
}

//------------------------------------------------------------------------------
// member function: set_action
//
//! Change the action of the message to \p action. Any other
//! report catchers will see the modified value.
//------------------------------------------------------------------------------

void uvm_report_catcher::set_action( uvm_action action )
{
  uvm_report_catcher_data::get().m_modified_action = action;
  uvm_report_catcher_data::get().m_set_action_called = true;
}

//------------------------------------------------------------------------------
// Group: Debug
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// member function: get_report_catcher (static)
//
//! Returns the first report catcher that has \p name.
//------------------------------------------------------------------------------

uvm_report_catcher* uvm_report_catcher::get_report_catcher( const std::string& name )
{
  static uvm_report_cb_iter* iter = new uvm_report_cb_iter(NULL);

  uvm_report_catcher* report_catcher = iter->first();

  while(report_catcher != NULL)
  {
    if(report_catcher->get_name() == name)
      return report_catcher;
    report_catcher = iter->next();
  }

  return NULL;
}

//------------------------------------------------------------------------------
// member function: print_catcher (static)
//
//! Prints information about all of the report catchers that are
//! registered. For finer grained detail, the uvm_callbacks<T,CB>::display
//! method can be used by calling uvm_report_cb::display(#uvm_report_object).
//------------------------------------------------------------------------------

void uvm_report_catcher::print_catcher( UVM_FILE file )
{
  std::ostringstream msg;
  std::string enabled;
  uvm_report_catcher* catcher;

  static uvm_report_cb_iter* iter = new uvm_report_cb_iter(NULL);

  f_display(file, "-------------UVM REPORT CATCHERS----------------------------");

  catcher = iter->first();

  while(catcher != NULL)
  {
    if(catcher->callback_mode())
      enabled = "ON";
    else
      enabled = "OFF";

    msg << std::setw(20)
        << catcher->get_name()
        << " : "
        << enabled;
    f_display(file, msg.str());
    catcher = iter->next();
  }

  f_display(file, "--------------------------------------------------------------");
}

//------------------------------------------------------------------------------
// member function: debug_report_catcher (static)
//
//! Turn on report catching debug information. \p what is a bitwise and of
//! * DO_NOT_CATCH  -- forces catch to be ignored so that all catchers see the
//!   the reports.
//! * DO_NOT_MODIFY -- forces the message to remain unchanged
//------------------------------------------------------------------------------

void uvm_report_catcher::debug_report_catcher( int what )
{
	uvm_report_catcher_data::get().m_debug_flags = what;
}

//------------------------------------------------------------------------------
// Group: Callback Interface
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// member function: catch
//
// This is the method that is called for each registered report catcher.
// There are no arguments to this function. The <Current Message State>
// interface methods can be used to access information about the
// current message being processed.
//------------------------------------------------------------------------------

//action_e catch();


//------------------------------------------------------------------------------
// Group: Reporting
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// member function: uvm_report_fatal
//
//! Issues a fatal message using the current message's report object.
//! This message will bypass any message catching callbacks.
//------------------------------------------------------------------------------
 
void uvm_report_catcher::uvm_report_fatal( const std::string& id,
                                           const std::string& message,
                                           int verbosity,
                                           const std::string& fname,
                                           int line )
{
  uvm_report_catcher_data& urcd = uvm_report_catcher_data::get();
  std::string m;
  uvm_action a;
  UVM_FILE f;
  uvm_report_handler* rh;

  rh   = urcd.m_client->get_report_handler();
  a    = rh->get_action(UVM_FATAL,id);
  f    = rh->get_file_handle(UVM_FATAL,id);

  m    = urcd.m_server->compose_message( UVM_FATAL, urcd.m_name, id, message, fname, line );

  urcd.m_server->process_report( UVM_FATAL, urcd.m_name, id, message, a, f, fname, line,
                            m, verbosity, urcd.m_client );
}


//------------------------------------------------------------------------------
// member function: uvm_report_error
//
//! Issues a error message using the current message's report object.
//! This message will bypass any message catching callbacks.
//------------------------------------------------------------------------------

void uvm_report_catcher::uvm_report_error( const std::string& id,
                                           const std::string& message,
                                           int verbosity,
                                           const std::string& fname,
                                           int line )
{
  uvm_report_catcher_data& urcd = uvm_report_catcher_data::get();
  std::string m;
  uvm_action a;
  UVM_FILE f;
  uvm_report_handler* rh;

  rh   = urcd.m_client->get_report_handler();
  a    = rh->get_action(UVM_ERROR,id);
  f    = rh->get_file_handle(UVM_ERROR,id);

  m    = urcd.m_server->compose_message(UVM_ERROR, urcd.m_name, id, message, fname, line);

  urcd.m_server->process_report( UVM_ERROR, urcd.m_name, id, message, a, f, fname, line,
                            m, verbosity, urcd.m_client);
}


//------------------------------------------------------------------------------
// member function: uvm_report_warning
//
//! Issues a warning message using the current message's report object.
//! This message will bypass any message catching callbacks.
//------------------------------------------------------------------------------

void uvm_report_catcher::uvm_report_warning( const std::string& id,
                                             const std::string& message,
                                             int verbosity,
                                             const std::string& fname,
                                             int line )
{
  uvm_report_catcher_data& urcd = uvm_report_catcher_data::get();
  std::string m;
  uvm_action a;
  UVM_FILE f;
  uvm_report_handler* rh;

  rh   = urcd.m_client->get_report_handler();
  a    = rh->get_action(UVM_WARNING,id);
  f    = rh->get_file_handle(UVM_WARNING,id);

  m    = urcd.m_server->compose_message( UVM_WARNING, urcd.m_name, id, message, fname, line );

  urcd.m_server->process_report( UVM_WARNING, urcd.m_name, id, message, a, f, fname, line,
                            m, verbosity, urcd.m_client);
}


//------------------------------------------------------------------------------
// member function: uvm_report_info
//
//! Issues a info message using the current message's report object.
//! This message will bypass any message catching callbacks.
//------------------------------------------------------------------------------

void uvm_report_catcher::uvm_report_info( const std::string& id,
                                          const std::string& message,
                                          int verbosity,
                                          const std::string& fname,
                                          int line )
{
  uvm_report_catcher_data& urcd = uvm_report_catcher_data::get();
  std::string m;
  uvm_action a;
  UVM_FILE f;
  uvm_report_handler* rh;

  rh   = urcd.m_client->get_report_handler();
  a    = rh->get_action(UVM_INFO,id);
  f    = rh->get_file_handle(UVM_INFO,id);

  m     = urcd.m_server->compose_message( UVM_INFO, urcd.m_name, id, message, fname, line);

  urcd.m_server->process_report( UVM_INFO, urcd.m_name, id, message, a, f, fname, line,
                            m, verbosity, urcd.m_client);

}

//------------------------------------------------------------------------------
// member function: uvm_report
//
//! Issues a message using the current message's report object.
//! This message will bypass any message catching callbacks.
//------------------------------------------------------------------------------

void uvm_report_catcher::uvm_report( uvm_severity severity,
                                     const std::string& id,
                                     const std::string& message,
                                     int verbosity,
                                     const std::string& fname,
                                     int line )
{
  uvm_report_catcher_data& urcd = uvm_report_catcher_data::get();
  std::string m;
  uvm_action a;
  UVM_FILE f;
  uvm_report_handler* rh;

  rh = urcd.m_client->get_report_handler();
  a = rh->get_action(severity, id);
  f = rh->get_file_handle(severity, id);

  m = urcd.m_server->compose_message(severity, urcd.m_name, id, message, fname, line);

  urcd.m_server->process_report( severity, urcd.m_name, id, message, a , f, fname, line,
                            m, verbosity, urcd.m_client);
}

//------------------------------------------------------------------------------
// member function: issue
//
//! Immediately issues the message which is currently being processed. This
//! is useful if the message is being CAUGHT but should still be emitted.
//!
//! Issuing a message will update the report_server stats, possibly multiple
//! times if the message is not CAUGHT.
//------------------------------------------------------------------------------

void uvm_report_catcher::issue()
{
  uvm_report_catcher_data& urcd = uvm_report_catcher_data::get();
  std::string m;
  uvm_action a;
  UVM_FILE f;
  uvm_report_handler* rh;

  rh = urcd.m_client->get_report_handler();
  a  =  urcd.m_modified_action;

  f  = rh->get_file_handle(urcd.m_modified_severity, urcd.m_modified_id);

  m  = urcd.m_server->compose_message( urcd.m_modified_severity,
                                  urcd.m_name,
                                  urcd.m_modified_id,
                                  urcd.m_modified_message,
                                  urcd.m_file_name,
                                  urcd.m_line_number );

  urcd.m_server->process_report( urcd.m_modified_severity,
                            urcd.m_name,
                            urcd.m_modified_id,
                            urcd.m_modified_message,
                            a, f,
                            urcd.m_file_name,
                            urcd.m_line_number,
                            m, urcd.m_modified_verbosity,
                            urcd.m_client );
}


//------------------------------------------------------------------------------
// member function: process_all_report_catchers (static)
//
//! Method called by report_server.report to process catchers
//------------------------------------------------------------------------------

int uvm_report_catcher::process_all_report_catchers( uvm_report_server* server,
                                                     uvm_report_object*& client,
                                                     uvm_severity& severity,
                                                     const std::string& name,
                                                     std::string& id,
                                                     std::string& message,
                                                     int& verbosity_level,
                                                     uvm_action& action,
                                                     const std::string& filename,
                                                     const int& line )
{
  uvm_report_catcher_data& urcd = uvm_report_catcher_data::get();
  int iter;
  uvm_report_catcher* catcher;
  int thrown = 1;
  uvm_severity orig_severity;
  static bool in_catcher;


  if(in_catcher)
  {
      return 1;
  }
  in_catcher = true;
  uvm_callbacks_base::m_tracing = false;  //turn off cb tracing so catcher stuff doesn't print

  urcd.m_server             = server;
  urcd.m_client             = client;
  orig_severity        = severity;
  urcd.m_name               = name;
  urcd.m_file_name          = filename;
  urcd.m_line_number        = line;
  urcd.m_modified_id        = id;
  urcd.m_modified_severity  = severity;
  urcd.m_modified_message   = message;
  urcd.m_modified_verbosity = verbosity_level;
  urcd.m_modified_action    = action;

  urcd.m_orig_severity  = severity;
  urcd.m_orig_id        = id;
  urcd.m_orig_verbosity = verbosity_level;
  urcd.m_orig_action    = action;
  urcd.m_orig_message   = message;

  catcher = uvm_report_cb::get_first(iter,client);
  while(catcher != NULL)
  {
    uvm_severity prev_sev;

    if (!catcher->callback_mode())
    {
      catcher = uvm_report_cb::get_next(iter,client);
      continue;
    }

    prev_sev = urcd.m_modified_severity;
    urcd.m_set_action_called = 0;
    thrown = catcher->process_report_catcher();

    // Set the action to the default action for the new severity
    // if it is still at the default for the previous severity,
    // unless it was explicitly set.
    if (!urcd.m_set_action_called &&
        urcd.m_modified_severity != prev_sev &&
        urcd.m_modified_action == urcd.m_client->get_report_action(prev_sev, "*@&*^*^*#"))
    {
       urcd.m_modified_action = urcd.m_client->get_report_action(urcd.m_modified_severity, "*@&*^*^*#");
    }

    if(thrown == 0)
    {
      switch(orig_severity)
      {
        case UVM_FATAL:   urcd.m_caught_fatal++; break;
        case UVM_ERROR:   urcd.m_caught_error++; break;
        case UVM_WARNING: urcd.m_caught_warning++; break;
        case UVM_INFO: break; // skip UVM_INFO
        default: break; // do nothing
      }
      break;
    } // if
    catcher = uvm_report_cb::get_next(iter,client);
  } //while

  //update counters if message was returned with demoted severity
  switch(orig_severity)
  {
    case UVM_FATAL: if(urcd.m_modified_severity < orig_severity) urcd.m_demoted_fatal++; break;
    case UVM_ERROR: if(urcd.m_modified_severity < orig_severity) urcd.m_demoted_error++; break;
    case UVM_WARNING: if(urcd.m_modified_severity < orig_severity) urcd.m_demoted_warning++; break;
    case UVM_INFO: break; // nothing to do for UVM_INFO
    default: break;
  }

  in_catcher = false;
  uvm_callbacks_base::m_tracing = true;  // turn tracing stuff back on

  severity        = urcd.m_modified_severity;
  id              = urcd.m_modified_id;
  message         = urcd.m_modified_message;
  verbosity_level = urcd.m_modified_verbosity;
  action          = urcd.m_modified_action;

  return thrown;
}


//------------------------------------------------------------------------------
// member function: process_report_catcher
//
//! Internal method to call user do_catch() method
//------------------------------------------------------------------------------

int uvm_report_catcher::process_report_catcher()
{
  uvm_report_catcher_data& urcd = uvm_report_catcher_data::get();
  action_e act;

  act = do_catch(); // was catch in UVM-SV, but is reserved keyword in C++

  if(act == UNKNOWN_ACTION)
  {
    uvm_report_error("RPTCTHR",
                     "uvm_report_this.catch() in catcher instance " + get_name() +
                     " must return THROW or CAUGHT",
                     UVM_NONE, UVM_FILE_M, UVM_LINE_M );
  }

  if(urcd.m_debug_flags & urcd.DO_NOT_MODIFY)
  {
    urcd.m_modified_severity    = urcd.m_orig_severity;
    urcd.m_modified_id          = urcd.m_orig_id;
    urcd.m_modified_verbosity   = urcd.m_orig_verbosity;
    urcd.m_modified_action      = urcd.m_orig_action;
    urcd.m_modified_message     = urcd.m_orig_message;
  }

  if(act == CAUGHT  && !(urcd.m_debug_flags & urcd.DO_NOT_CATCH))
    return 0;

  return 1;
}

//------------------------------------------------------------------------------
// member function: f_display (static)
//
//! Internal method to check if file is open
//------------------------------------------------------------------------------

void uvm_report_catcher::f_display( UVM_FILE file, const std::string& str )
{
  if (file == 0)
    std::cout << str << std::endl;
  else
    *file << str << std::endl;
}

//------------------------------------------------------------------------------
// member function: summarize_report_catcher (static)
//
//! This function is called automatically by uvm_report_server::summarize().
//! It prints the statistics for the active catchers.
//------------------------------------------------------------------------------

void uvm_report_catcher::summarize_report_catcher( UVM_FILE file )
{
  std::ostringstream s;
  uvm_report_catcher_data& urcd = uvm_report_catcher_data::get();

  if(urcd.do_report)
  {
    f_display(file, "");
    f_display(file, "--- UVM Report catcher Summary ---");
    f_display(file, "");

    s.clear(); s.str("");
    s << "Number of demoted UVM_FATAL reports  :"
      << std::setw(5)
      << urcd.m_demoted_fatal;
    f_display(file,s.str());

    s.clear(); s.str("");
    s << "Number of demoted UVM_ERROR reports  :"
      << std::setw(5)
      << urcd.m_demoted_error;
    f_display(file,s.str());

    s.clear(); s.str("");
    s << "Number of demoted UVM_WARNING reports:"
      << std::setw(5)
      << urcd.m_demoted_warning;
    f_display(file,s.str());

    s.clear(); s.str("");
    s << "Number of caught UVM_FATAL reports   :"
      << std::setw(5)
      << urcd.m_caught_fatal;
    f_display(file,s.str());

    s.clear(); s.str("");
    s << "Number of caught UVM_ERROR reports   :"
      << std::setw(5)
      << urcd.m_caught_error;
    f_display(file,s.str());

    s.clear(); s.str("");
    s << "Number of caught UVM_WARNING reports :"
      << std::setw(5)
      << urcd.m_caught_warning;
    f_display(file,s.str());
  }
}


} // namespace uvm

