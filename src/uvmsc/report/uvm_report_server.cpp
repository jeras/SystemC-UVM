//----------------------------------------------------------------------------
//   Copyright 2014 Fraunhofer-Gesellschaft zur Foerderung
//					der angewandten Forschung e.V.
//   Copyright 2007-2010 Mentor Graphics Corporation
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

#include <ostream>
#include <iostream>
#include <iomanip>  // setw

#include "uvmsc/report/uvm_report_server.h"
#include "uvmsc/report/uvm_report_handler.h"
#include "uvmsc/report/uvm_report_catcher.h"
#include "uvmsc/base/uvm_object_globals.h"
#include "uvmsc/base/uvm_globals.h"

namespace uvm {


//----------------------------------------------------------------------------
// Initialize static data members
//----------------------------------------------------------------------------

uvm_report_server* uvm_report_server::m_global_report_server = NULL;

//----------------------------------------------------------------------------
// Constructor
//
//! Creates an instance of the class.
//----------------------------------------------------------------------------

uvm_report_server::uvm_report_server() : uvm_object("uvm_report_server")
{
  m_max_quit_overridable = true;
  enable_report_id_count_summary = true;

  set_max_quit_count(0);
  reset_quit_count();
  reset_severity_counts();
}


//----------------------------------------------------------------------------
// member function: set_server (static)
//
//! Sets the global report server to use for reporting. The report
//! server is responsible for formatting messages.
//----------------------------------------------------------------------------

void uvm_report_server::set_server( uvm_report_server* server )
{
  if(m_global_report_server != NULL)
  {
    server->set_max_quit_count(m_global_report_server->get_max_quit_count());
    server->set_quit_count(m_global_report_server->get_quit_count());
    m_global_report_server->copy_severity_counts(server);
    m_global_report_server->copy_id_counts(server);
  }

  m_global_report_server = server;
}


//----------------------------------------------------------------------------
// member function: get_server (static)
//
//! Gets the global report server. The method will always return
//! a valid handle to a report server.
//----------------------------------------------------------------------------

uvm_report_server* uvm_report_server::get_server()
{
  if (m_global_report_server == NULL)
    m_global_report_server = new uvm_report_server();

  return m_global_report_server;
}

//----------------------------------------------------------------------------
// member function: set_max_quit_count
//----------------------------------------------------------------------------

void uvm_report_server::set_max_quit_count( int count, bool overridable )
{
  if (!m_max_quit_overridable)
  {
    std::ostringstream str;
    str << "The max quit count setting of "
        << max_quit_count
        << " is not overridable to "
        << count
        << " due to a previous setting.";
    uvm_report_info("NOMAXQUITOVR", str.str(), UVM_NONE);
    return;
  }

  m_max_quit_overridable = overridable;
  max_quit_count = (count < 0) ? 0 : count;
}
//----------------------------------------------------------------------------
// member function: get_max_quit_count
//
// Get or set the maximum number of COUNT actions that can be tolerated
// before an UVM_EXIT action is taken. The default is 0, which specifies
// no maximum.
//----------------------------------------------------------------------------

int uvm_report_server::get_max_quit_count() const
{
  return max_quit_count;
}

//----------------------------------------------------------------------------
// member function: set_quit_count
//----------------------------------------------------------------------------

void uvm_report_server::set_quit_count(int quit_count_)
{
  quit_count = (quit_count_ < 0) ? 0 : quit_count;
}

//----------------------------------------------------------------------------
// member function: get_quit_count
//----------------------------------------------------------------------------

int uvm_report_server::get_quit_count() const
{
  return quit_count;
}

//----------------------------------------------------------------------------
// member function: incr_quit_count
//----------------------------------------------------------------------------

void uvm_report_server::incr_quit_count()
{
  quit_count++;
}

//----------------------------------------------------------------------------
// member function: reset_quit_count
//
// Set, get, increment, or reset to 0 the quit count, i.e., the number of
// COUNT actions issued.
//----------------------------------------------------------------------------

void uvm_report_server::reset_quit_count()
{
  quit_count = 0;
}

//----------------------------------------------------------------------------
// member function: is_quit_count_reached
//
// If is_quit_count_reached returns 1, then the quit counter has reached
// the maximum.
//----------------------------------------------------------------------------

bool uvm_report_server::is_quit_count_reached()
{
  return (quit_count >= max_quit_count);
}

//----------------------------------------------------------------------------
// member function: set_severity_count
//----------------------------------------------------------------------------

void uvm_report_server::set_severity_count( uvm_severity severity, int count )
{
  severity_count[severity] = (count < 0) ? 0 : count;
}

//----------------------------------------------------------------------------
// member function: get_severity_count
//----------------------------------------------------------------------------

int uvm_report_server::get_severity_count( uvm_severity severity ) const
{
  return severity_count.find(severity)->second;
}

//----------------------------------------------------------------------------
// member function: incr_severity_count
//----------------------------------------------------------------------------

void uvm_report_server::incr_severity_count( uvm_severity severity )
{
  severity_count[severity]++;
}

//----------------------------------------------------------------------------
// member function: reset_severity_counts
//
// Set, get, or increment the counter for the given severity, or reset
// all severity counters to 0.
//----------------------------------------------------------------------------

void uvm_report_server::reset_severity_counts()
{
  for (int i = 0; i < 4; i++) // cycle through all four states
    severity_count[(uvm_severity)i] = 0; // TODO check
}

//----------------------------------------------------------------------------
// member function: set_id_count
//----------------------------------------------------------------------------

void uvm_report_server::set_id_count( const std::string& id, int count )
{
  id_count[id] = (count < 0) ? 0 : count;
}

//----------------------------------------------------------------------------
// member function: get_id_count
//----------------------------------------------------------------------------

int uvm_report_server::get_id_count( const std::string& id ) const
{
  if(id_count.find(id) != id_count.end()) // exists
    return id_count.find(id)->second;

  return 0;
}

//----------------------------------------------------------------------------
// member function: incr_id_count
//
// Set, get, or increment the counter for reports with the given id.
//----------------------------------------------------------------------------

void uvm_report_server::incr_id_count( const std::string& id )
{
  if( id_count.find(id) != id_count.end() ) // exists
    id_count[id]++;
  else
    id_count[id] = 1;
}

//----------------------------------------------------------------------------
// member function: f_display
//
// This method sends string severity to the command line if file is 0 and to
// the file(s) specified by file if it is not 0.
//----------------------------------------------------------------------------

void uvm_report_server::f_display( UVM_FILE file, const std::string& str ) const
{
  if (file == 0) // NULL pointer
    std::cout << str << std::endl;
  else
    *file << str << std::endl;
}

//----------------------------------------------------------------------------
// member function: report (virtual)
//
//
//----------------------------------------------------------------------------

void uvm_report_server::report( uvm_severity severity,
                                const std::string& name,
                                const std::string& id,
                                const std::string& message,
                                int verbosity_level,
                                const std::string& filename,
                                int line,
                                uvm_report_object* client )
{
  std::string m;
  uvm_action a;
  UVM_FILE f;
  bool report_ok;
  uvm_report_handler* rh;

  std::string loc_id = id;
  std::string loc_message = message;

  rh = client->get_report_handler();

  // filter based on verbosity level

  if(!client->uvm_report_enabled(verbosity_level, severity, id))
     return;

  // determine file to send report and actions to execute

  a = rh->get_action(severity, id);
  if( a == UVM_NO_ACTION )
    return;

  f = rh->get_file_handle(severity, id);

  // The hooks can do additional filtering.  If the hook function
  // return 1 then continue processing the report.  If the hook
  // returns 0 then skip processing the report.

  if(a & UVM_CALL_HOOK)
    report_ok = rh->run_hooks( client, severity, loc_id,
                               loc_message, verbosity_level, filename, line);
  else
    report_ok = true;

  if(report_ok)
    report_ok = uvm_report_catcher::process_all_report_catchers(
                   this, client, severity, name, loc_id, loc_message,
                   verbosity_level, a, filename, line);

  if(report_ok)
  {
    m = compose_message(severity, name, id, message, filename, line);

    process_report(severity, name, id, message, a, f, filename,
                   line, m, verbosity_level, client);
  }
}


//----------------------------------------------------------------------------
// member function: process_report (virtual)
//
// Calls <compose_message> to construct the actual message to be
// output. It then takes the appropriate action according to the value of
// action and file.
//
// This method can be overloaded by expert users to customize the way the
// reporting system processes reports and the actions enabled for them.
//----------------------------------------------------------------------------

void uvm_report_server::process_report( uvm_severity severity,
                                        const std::string& name,
                                        const std::string& id,
                                        const std::string& message,
                                        uvm_action action,
                                        UVM_FILE file,
                                        const std::string& filename,
                                        int line,
                                        const std::string& composed_message,
                                        int verbosity_level,
                                        uvm_report_object* client )
{
  // update counts
  incr_severity_count(severity);
  incr_id_count(id);

  if(action & UVM_DISPLAY)
    std::cout << composed_message << std::endl;

  // if log is set we need to send to the file but not resend to the
  // display. So, we need to mask off stdout for an mcd or we need
  // to ignore the stdout file handle for a file handle.

  // TODO check
  if(action & UVM_LOG)
  {
    if( (file == 0) || (file != &std::cout )) //ignore stdout handle
    {
      UVM_FILE tmp_file = file;
      //if( (file&32'h8000_0000) == 0) //is an mcd so mask off stdout
      //   tmp_file = file & 32'hffff_fffe;
      f_display(tmp_file,composed_message);
    }
  }

  if(action & UVM_EXIT)
  {
    uvm_root* root = uvm_root::get();
    root->die();
  }

  if(action & UVM_COUNT)
  {
    if(get_max_quit_count() != 0)
    {
      incr_quit_count();

      if(is_quit_count_reached())
      {
        uvm_root* root = uvm_root::get();
        root->die();
      }
    }
  }

  if (action & UVM_STOP)
    sc_core::sc_stop(); // TODO different stop mechanism?
}


//----------------------------------------------------------------------------
// member function: compose_message (virtual)
//
// Constructs the actual string sent to the file or command line
// from the severity, component name, report id, and the message itself.
//
// Expert users can overload this method to customize report formatting.
//----------------------------------------------------------------------------

std::string uvm_report_server::compose_message( uvm_severity severity,
                                                const std::string& name,
                                                const std::string& id,
                                                const std::string& message,
                                                const std::string& filename,
                                                int line ) const
{
  std::string line_str;

  if(name.empty() && filename.empty())
  {
    std::ostringstream str;
    str << uvm_severity_name[severity]
        << " @ "
        << sc_core::sc_time_stamp()
        << " ["
        << id
        << "] "
        << message;
    return str.str();
  }

  if(!name.empty() && filename.empty())
  {
    std::ostringstream str;
    str << uvm_severity_name[severity]
        << " @ "
        << sc_core::sc_time_stamp()
        << ": "
        << name
        << " ["
        << id
        << "] "
        << message;
    return str.str();
  }

  if(name.empty() && !filename.empty())
  {
    std::ostringstream str;
    str << uvm_severity_name[severity]
        << " "
        << filename
        << "("
        << line
        << ")"
        << " @ "
        << sc_core::sc_time_stamp()
        << " ["
        << id
        << "] "
        << message;
    return str.str();
  }

  if(!name.empty() && !filename.empty())
  {
    std::ostringstream str;
    str << uvm_severity_name[severity]
        << " "
        << filename
        << "("
        << line
        << ")"
        << " @ "
        << sc_core::sc_time_stamp()
        << ": "
        << name
        << " ["
        << id
        << "] "
        << message;
    return str.str();
  }

  return "";
}

//----------------------------------------------------------------------------
// member function: report_summarize (virtual)
//
//----------------------------------------------------------------------------

void uvm_report_server::summarize( UVM_FILE file ) const
{
  std::string id;
  std::string name;
  std::string space= "                 ";

  uvm_report_catcher::summarize_report_catcher(file);

  f_display(file, "");
  f_display(file, "--- UVM Report Summary ---");
  f_display(file, "");

  if(max_quit_count != 0)
  {
    if ( quit_count >= max_quit_count )
      f_display(file, "Quit count reached!");

    std::ostringstream output_str;
    output_str << "Quit count : "
               << std::setw(5)
               << quit_count
               << " of "
               << std::setw(5)
               << max_quit_count;
    f_display(file, output_str.str());
  }

  f_display(file, "** Report counts by severity");

  for( int i = 0; i < 4 ; i++) // all 4 severity levels
  {
    if(severity_count.find((uvm_severity)i) != severity_count.end() )
    {
      std::ostringstream output_str;
      output_str << uvm_severity_name[i]
                 << space.substr(1, 13-std::string(uvm_severity_name[i]).length()) // TODO set max length
                 << " :"
                 << std::setw(4)
                 << severity_count.find((uvm_severity)i)->second;
      f_display(file, output_str.str());
    }
  }

  if (enable_report_id_count_summary)
  {
    f_display(file, "** Report counts by id");
    for( id_count_citt it = id_count.begin();
         it != id_count.end();
         it++)
    {
      std::ostringstream output_str;
      output_str << "["
                 << it->first // id
                 << "] "
                 << space.substr(1, 20-(it->first).length()) // TODO set max length
                 << std::setw(2)
                 << it->second; // id_count[id];
      f_display(file, output_str.str());
    }
  }
}

//----------------------------------------------------------------------------
// member function: dump_server_state
//
// Dumps server state information.
//----------------------------------------------------------------------------

void uvm_report_server::dump_server_state() const
{
  std::ostringstream s;

  std::string space= "                 ";

  f_display(0, "report server state");
  f_display(0, "");
  f_display(0, "+-------------+");
  f_display(0, "|   counts    |");
  f_display(0, "+-------------+");
  f_display(0, "");

  s << "max quit count = " << std::setw(5) << max_quit_count;
  f_display(0, s.str());

  s.clear();

  s << "quit count = " << std::setw(5) << quit_count;
  f_display(0, s.str());

  for(int i = 0; i < 4; i++) // all 4 severity states
  {
    std::ostringstream str;
    str << uvm_severity_name[i]
        //<< space.substr(1, 10-sizeof(uvm_severity_name[i]))
        << " :"
        << std::setw(5)
        << severity_count.find((uvm_severity)i)->second;
    f_display(0, str.str());
  }

  for( id_count_citt it = id_count.begin();
       it != id_count.end();
       it++)
  {
    std::ostringstream str;
    str << it->first // id
        << " :"
        << std::setw(5)
        << it->second; // id_count[id];
    f_display(0, str.str());
  }
}

//----------------------------------------------------------------------------
// member function: copy_severity_counts
//
// Internal member function.
//----------------------------------------------------------------------------

void uvm_report_server::copy_severity_counts( uvm_report_server* dst )
{
  for( severity_count_itt it = severity_count.begin();
       it != severity_count.end();
       it++)
    dst->set_severity_count(it->first, it->second);

}

//----------------------------------------------------------------------------
// member function: copy_severity_counts
//
// Internal member function.
//----------------------------------------------------------------------------

void uvm_report_server::copy_id_counts( uvm_report_server* dst )
{
  for( id_count_itt it = id_count.begin();
       it != id_count.end();
       it++)
    dst->set_id_count(it->first, it->second);
}


//----------------------------------------------------------------------------
// member function: get_type_name
//
// Internal member function. Needed for callbacks
//----------------------------------------------------------------------------

std::string uvm_report_server::get_type_name()
{
  return "uvm_report_server";
}


} // namespace uvm
