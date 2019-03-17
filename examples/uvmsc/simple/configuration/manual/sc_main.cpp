//----------------------------------------------------------------------
//   Copyright 2012-2014 NXP B.V.
//   Copyright 2009 Cadence Design Systems, Inc.
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

#include <systemc>
#include <uvm>

#include "my_env.h"

int sc_main(int, char*[]) 
{  
  my_env* topenv;

  uvm::uvm_default_printer = uvm::uvm_default_table_printer;
  uvm::uvm_root::get()->enable_print_topology();

  // Tracing of configuration
  //uvm::uvm_config_db_options::turn_on_tracing();

  // set configuration prior to creating the environment
  uvm::uvm_config_db<int>::set(NULL, "topenv.*.u1", "v", 30);
  uvm::uvm_config_db<int>::set(NULL, "topenv.inst2.u1", "v", 10);
  uvm::uvm_config_db<int>::set(NULL, "topenv.*", "debug", 1);
  uvm::uvm_config_db<std::string>::set(NULL, "*", "myaa[foo]", "hi");
  uvm::uvm_config_db<std::string>::set(NULL, "*", "myaa[bar]", "bye");
  uvm::uvm_config_db<std::string>::set(NULL, "*", "myaa[foobar]", "howdy");
  uvm::uvm_config_db<std::string>::set(NULL, "topenv.inst1.u1", "myaa[foo]", "boo");
  uvm::uvm_config_db<std::string>::set(NULL, "topenv.inst1.u1", "myaa[foobar]", "boobah");

  topenv = new my_env("topenv");

  uvm::run_test();

  return 0;
}
