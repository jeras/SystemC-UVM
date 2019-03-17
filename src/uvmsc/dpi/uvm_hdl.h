//----------------------------------------------------------------------
//   Copyright 2007-2010 Mentor Graphics Corporation
//   Copyright 2013 NXP B.V.
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

#ifndef UVM_HDL_H_
#define UVM_HDL_H_

#include <systemc>

namespace uvm {


#ifndef UVM_HDL_MAX_WIDTH
#define UVM_HDL_MAX_WIDTH 1024
#endif

/* 
 * VARIABLE: uvm_hdl_max_width
 *
 * Sets the maximum size bit vector for backdoor access. 
 * This parameter will be looked up by the 
 * DPI-C code using:
 *   vpi_handle_by_name(
 *     "uvm_pkg::UVM_HDL_MAX_WIDTH", 0);
 *
 * TODO - do we define a variable for UVM-SC?
 * parameter int uvm_hdl_max_width = `UVM_HDL_MAX_WIDTH;
 */

typedef sc_dt::sc_lv<UVM_HDL_MAX_WIDTH> uvm_hdl_data_t;


bool uvm_hdl_check_path(const std::string& path );
bool uvm_hdl_deposit(const std::string& path, uvm_hdl_data_t value);
bool uvm_hdl_force(const std::string& path, uvm_hdl_data_t value);
void uvm_hdl_force_time(const std::string& path, uvm_hdl_data_t value, sc_core::sc_time force_time = sc_core::SC_ZERO_TIME);
bool uvm_hdl_release_and_read(const std::string& path, uvm_hdl_data_t& value);
bool uvm_hdl_release(const std::string& path);
bool uvm_hdl_read(const std::string& path, uvm_hdl_data_t& value);

} // namespace uvm

#endif // UVM_HDL_H_
