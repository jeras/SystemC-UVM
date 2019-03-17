//----------------------------------------------------------------------
//   Copyright 2011 Cypress Semiconductor
//   Copyright 2010 Mentor Graphics Corporation
//   Copyright 2011 Cadence Design Systems, Inc.
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

#include "uvmsc/conf/uvm_resource_options.h"

namespace uvm {

//----------------------------------------------------------------------
// Class: uvm_resource_options implementation
//----------------------------------------------------------------------

bool uvm_resource_options::auditing = true;

//----------------------------------------------------------------------
// member function: turn_on_auditing
//
//! Turn auditing on for the resource database. This causes all
//! reads and writes to the database to store information about
//! the accesses. Auditing is turned on by default.
//----------------------------------------------------------------------

void uvm_resource_options::turn_on_auditing()
{
  auditing = true;
}

//----------------------------------------------------------------------
// member function: turn_off_auditing
//
//! Turn auditing off for the resource database. If auditing is turned off,
//! it is not possible to get extra information about resource
//! database accesses.
//----------------------------------------------------------------------

void uvm_resource_options::turn_off_auditing()
{
  auditing = false;
}

//----------------------------------------------------------------------
// member function: is_auditing
//
//! Returns true if the auditing facility is on and false if it is off.
//----------------------------------------------------------------------

bool uvm_resource_options::is_auditing()
{
  return auditing;
}


} // namespace uvm

