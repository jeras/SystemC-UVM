//----------------------------------------------------------------------
//   Copyright 2007-2010 Mentor Graphics Corporation
//   Copyright 2013 NXP B.V.
//   Copyright 2014 Fraunhofer-Gesellschaft zur Foerderung
//					der angewandten Forschung e.V.
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

//----------------------------------------------------------------------
// TITLE: UVM HDL Backdoor Access support routines.
//
// These routines provide an interface to the DPI/PLI
// implementation of backdoor access used by registers.
//
// TODO implementation not done yet
//----------------------------------------------------------------------

#include "uvmsc/dpi/uvm_hdl.h"

namespace uvm {

//----------------------------------------------------------------------
// Function: uvm_hdl_check_path
//
// Checks that the given HDL ~path~ exists. Returns 0 if NOT found, 1 otherwise.
//----------------------------------------------------------------------

bool uvm_hdl_check_path( const std::string& path )
{
  // TODO
  return false;
}



//----------------------------------------------------------------------
// Function: uvm_hdl_deposit
//
// Sets the given HDL ~path~ to the specified ~value~.
// Returns 1 if the call succeeded, 0 otherwise.
//----------------------------------------------------------------------

bool uvm_hdl_deposit( const std::string& path, uvm_hdl_data_t value )
{
  // TODO
  return false;
}


//----------------------------------------------------------------------
// Function: uvm_hdl_force
//
// Forces the ~value~ on the given ~path~. Returns 1 if the call succeeded, 0 otherwise.
//----------------------------------------------------------------------

bool uvm_hdl_force( const std::string& path, uvm_hdl_data_t value )
{
  // TODO
  return false;
}



//----------------------------------------------------------------------
// Function: uvm_hdl_force_time
//
// Forces the ~value~ on the given ~path~ for the specified amount of ~force_time~.
// If ~force_time~ is 0, <uvm_hdl_deposit> is called.
// Returns 1 if the call succeeded, 0 otherwise.
//----------------------------------------------------------------------

void uvm_hdl_force_time( const std::string& path, uvm_hdl_data_t value, sc_core::sc_time force_time )
{
  // TODO
}


//----------------------------------------------------------------------
// Function: uvm_hdl_release_and_read
//
// Releases a value previously set with <uvm_hdl_force>.
// Returns 1 if the call succeeded, 0 otherwise. ~value~ is set to
// the HDL value after the release. For 'reg', the value will still be
// the forced value until it has bee procedurally reassigned. For 'wire',
// the value will change immediately to the resolved value of its
// continuous drivers, if any. If none, its value remains as forced until
// the next direct assignment.
//----------------------------------------------------------------------

bool uvm_hdl_release_and_read( const std::string& path, uvm_hdl_data_t& value )
{
  // TODO
  return false;
}


//----------------------------------------------------------------------
// Function: uvm_hdl_release
//
// Releases a value previously set with <uvm_hdl_force>.
// Returns 1 if the call succeeded, 0 otherwise.
//----------------------------------------------------------------------

bool uvm_hdl_release( const std::string& path )
{
  // TODO
  return false;
}



//----------------------------------------------------------------------
// Function: uvm_hdl_read()
//
// Gets the value at the given ~path~.
// Returns 1 if the call succeeded, 0 otherwise.
//----------------------------------------------------------------------
bool uvm_hdl_read( const std::string& path, uvm_hdl_data_t& value )
{
  // TODO
  return false;
}

} // namespace uvm
