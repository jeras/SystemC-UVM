//----------------------------------------------------------------------
//   Copyright 2012 NXP B.V.
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
//----------------------------------------------------------------------

#ifndef UVM_REPORT_OBJECT_H_
#define UVM_REPORT_OBJECT_H_

#include <map>
#include <string>
#include <systemc>

#include "uvmsc/base/uvm_object.h"
#include "uvmsc/base/uvm_object_globals.h"

namespace uvm {

// forward class declarations
class uvm_report_handler;
class uvm_report_server;

//----------------------------------------------------------------------------
// CLASS: uvm_report_object
//
//! The class #uvm_report_object provides an interface to the UVM reporting
//! facility. Through this interface, components issue the various messages
//! that occur during simulation. An application can configure what actions
//! are taken and what file(s) are output for individual messages from a
//! particular component or for all messages from all components in the
//! environment. Defaults are applied where there is no explicit configuration.
//----------------------------------------------------------------------------

class uvm_report_object : public uvm_object
{
  //--------------------------------------------------------------------------
  // UVM Standard LRM API below
  //--------------------------------------------------------------------------

 protected:

  uvm_report_object();

  explicit uvm_report_object( const std::string& name );

 public:

  //--------------------------------------------------------------------------
  // Group: Reporting
  //--------------------------------------------------------------------------

  int uvm_report_enabled( int verbosity,
                          uvm_severity severity = UVM_INFO,
                          const std::string& id = "" ) const;

  virtual void uvm_report_info( const std::string& id,
                                const std::string& message,
                                int verbosity = UVM_MEDIUM,
                                const std::string& filename = "",
                                int line = 0 ) const;

  virtual void uvm_report_warning( const std::string& id,
                                   const std::string& message,
                                   int verbosity = UVM_MEDIUM,
                                   const std::string& filename = "",
                                   int line = 0 ) const;

  virtual void uvm_report_error( const std::string& id,
                                 const std::string& message,
                                 int verbosity = UVM_LOW,
                                 const std::string& filename = "",
                                 int line = 0 ) const;

  virtual void uvm_report_fatal( const std::string& id,
                                 const std::string& message,
                                 int verbosity = UVM_NONE,
                                 const std::string& filename = "",
                                 int line = 0 ) const;

  //--------------------------------------------------------------------------
  // Group: Verbosity Configuration
  //--------------------------------------------------------------------------

  int get_report_verbosity_level( uvm_severity severity = UVM_INFO,
                                  const std::string& id = "" ) const;

  // TODO
  // int get_report_max_verbosity_level() const;

  void set_report_verbosity_level( int verbosity_level );

  void set_report_id_verbosity( const std::string& id, int verbosity );

  void set_report_severity_id_verbosity( uvm_severity severity,
                                         const std::string& id,
                                         int verbosity );

  //--------------------------------------------------------------------------
  // Group: Action Configuration
  //--------------------------------------------------------------------------

  int get_report_action( uvm_severity severity, const std::string& id ) const;

  void set_report_severity_action( uvm_severity severity,
                                   uvm_action action );

  void set_report_id_action( const std::string& id, uvm_action action );

  void set_report_severity_id_action( uvm_severity severity,
                                      const std::string& id,
                                      uvm_action action );

  //--------------------------------------------------------------------------
  // Group: File Configuration
  //--------------------------------------------------------------------------

  UVM_FILE get_report_file_handle( uvm_severity severity, const std::string& id ) const;

  void set_report_default_file( UVM_FILE file);

  void set_report_id_file( const std::string& id, UVM_FILE file );

  void set_report_severity_file( uvm_severity severity, UVM_FILE file );

  void set_report_severity_id_file( uvm_severity severity,
                                    const std::string& id,
                                    UVM_FILE file);

  //--------------------------------------------------------------------------
  // Group: Override Configuration
  //--------------------------------------------------------------------------

  void set_report_severity_override( uvm_severity cur_severity,
                                     uvm_severity new_severity );

  void set_report_severity_id_override( uvm_severity cur_severity,
                                        const std::string& id,
                                        uvm_severity new_severity );


  //----------------------------------------------------------------------------
  // Group: Setup
  //----------------------------------------------------------------------------

  void set_report_handler( uvm_report_handler* handler );

  uvm_report_handler* get_report_handler() const;

  void reset_report_handler();

  /////////////////////////////////////////////////////
  /////////////////////////////////////////////////////
  // Implementation-defined member functions below,
  // not part of UVM Class reference / LRM
  /////////////////////////////////////////////////////
  /////////////////////////////////////////////////////

  // Callbacks deprecated in UVM 1.2 - will be removed soon

  virtual bool report_info_hook( const std::string& id,
                                 const std::string& message,
                                 int verbosity,
                                 const std::string& filename,
                                 int line ) const;

  virtual bool report_error_hook( const std::string& id,
                                  const std::string& message,
                                  int verbosity,
                                  const std::string& filename,
                                  int line ) const;

  virtual bool report_warning_hook( const std::string& id,
                                    const std::string& message,
                                    int verbosity,
                                    const std::string& filename,
                                    int line ) const;

  virtual bool report_fatal_hook( const std::string& id,
                                  const std::string& message,
                                  int verbosity,
                                  const std::string& filename,
                                  int line ) const;

  virtual bool report_hook( const std::string& id,
                            const std::string& message,
                            int verbosity,
                            const std::string& filename,
                            int line ) const;

  int uvm_get_max_verbosity() const;

  void m_init();

  static uvm_report_object* get();

  virtual ~uvm_report_object();

 private:
  static uvm_report_object* m_inst;

  uvm_report_handler* m_rh;
};


// global entry point to get a handle to the report object
inline
uvm_report_object* get_report_object()
{
  return uvm_report_object::get();
}

} // namespace uvm

#endif /* UVM_REPORT_OBJECT_H_ */
