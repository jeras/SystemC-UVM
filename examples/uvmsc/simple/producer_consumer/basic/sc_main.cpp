//----------------------------------------------------------------------
//   Copyright 2009 Cadence Design Systems, Inc.
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

//----------------------------------------------------------------------
// producer-consumer example based using a TLM1 fifo
// This example uses the UVM port definition. These ports have a get and
// put implemented to to get access to the interface which implements the
// get and put respectively.
//----------------------------------------------------------------------

#include <systemc>
#include <uvm>

#include "top.h"

int sc_main(int, char*[])
{  
  top* topenv = new top("topenv");

  uvm::run_test();

  delete topenv;

  return 0;
}
