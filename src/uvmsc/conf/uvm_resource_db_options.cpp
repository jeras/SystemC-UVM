//----------------------------------------------------------------------
//   Copyright 2011 Cypress Semiconductor
//   Copyright 2010 Mentor Graphics Corporation
//   Copyright 2012-2014 NXP B.V.
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

#include "uvmsc/conf/uvm_resource_db_options.h"

namespace uvm {

//----------------------------------------------------------------------
// initialize static data members
//----------------------------------------------------------------------

bool uvm_resource_db_options::ready = false;
bool uvm_resource_db_options::tracing = false;

//----------------------------------------------------------------------
// member function: turn_on_tracing
//
//! Turn tracing on for the resource database. This causes all
//! reads and writes to the database to display information about
//! the accesses. Tracing is off by default.
//----------------------------------------------------------------------

void uvm_resource_db_options::turn_on_tracing()
{
  if (!ready) init();
  tracing = true;
}

//----------------------------------------------------------------------
// member function: turn_off_tracing
//
//! Turn tracing off for the resource database.
//----------------------------------------------------------------------

void uvm_resource_db_options::turn_off_tracing()
{
  if (!ready) init();
  tracing = false;
}

//----------------------------------------------------------------------
// member function: is_tracing
//
//! Returns true if the tracing facility is on and false if it is off.
//----------------------------------------------------------------------

bool uvm_resource_db_options::is_tracing()
{
  if (!ready) init();
  return tracing;
}

//----------------------------------------------------------------------
// member function: init
//
//! Read options from command line
//----------------------------------------------------------------------

void uvm_resource_db_options::init()
{
  /* TODO command line interface
  uvm_cmdline_processor clp;
  std::string trace_args[$];

  clp = uvm_cmdline_processor::get_inst();

  if (clp.get_arg_matches("+UVM_RESOURCE_DB_TRACE", trace_args)) begin
    tracing = true;
  end

  ready = true;
  */
}


} // namespace uvm

