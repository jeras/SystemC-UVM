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

//----------------------------------------------------------------------
// This example will illustrate the usage of uvm_factory methods.
//----------------------------------------------------------------------

#include <systemc>
#include <uvm>

#include "top.h"
#include "dut.h"

int sc_main(int, char*[]) 
{  
  // instantiate the DUT
  dut dut_top("dut"); 
  top* test_top = new top("top");

  uvm::run_test();

  return 0;
}
