//----------------------------------------------------------------------
//   Copyright 2013-2014 NXP B.V.
//   Copyright 2004-2009 Synopsys, Inc.
//   Copyright 2010 Mentor Graphics Corporation
//   Copyright 2010 Cadence Design Systems, Inc.
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

#include "uvmsc/reg/uvm_reg_write_only_cbs.h"
#include "uvmsc/reg/uvm_reg_item.h"
#include "uvmsc/reg/uvm_reg_field.h"

namespace uvm {

//----------------------------------------------------------------------
// static data member initialization
//----------------------------------------------------------------------

uvm_reg_write_only_cbs* uvm_reg_write_only_cbs::m_me = get();

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------

uvm_reg_write_only_cbs::uvm_reg_write_only_cbs( const std::string& name ) : uvm_reg_cbs(name)
{}

//----------------------------------------------------------------------
// member function: pre_read (virtual)
//
//! Produces an error message and sets status to UVM_NOT_OK.
//----------------------------------------------------------------------

void uvm_reg_write_only_cbs::pre_read( uvm_reg_item* rw )
{
  std::string name = rw->element->get_full_name();

  if (rw->status != UVM_IS_OK)
    return;

  if (rw->element_kind == UVM_FIELD)
  {
    uvm_reg_field* fld;
    uvm_reg* rg;
    fld = dynamic_cast<uvm_reg_field*>(rw->element);
    rg = fld->get_parent();
    name = rg->get_full_name();
  }

  UVM_ERROR("UVM/REG/WRTEONLY",
      name + " is write-only. Cannot call read() method.");

  rw->status = UVM_NOT_OK;
}

//----------------------------------------------------------------------
// member function: add (static)
//
//! Add this callback to the specified register and its contained fields.
//----------------------------------------------------------------------

void uvm_reg_write_only_cbs::add( uvm_reg* rg )
{
  std::vector<uvm_reg_field*> flds;

  uvm_reg_cb::add(rg, get());
  rg->get_fields(flds);
  for( unsigned int i = 0; i < flds.size(); i++ )
    uvm_reg_field_cb::add(flds[i], get());
}


//----------------------------------------------------------------------
// member function: remove (static)
//
//! Remove this callback from the specified register and its contained fields.
//----------------------------------------------------------------------

void uvm_reg_write_only_cbs::remove( uvm_reg* rg )
{
  uvm_reg_cb_iter* cbs = new uvm_reg_cb_iter(rg);
  std::vector<uvm_reg_field*> flds;

  cbs->first();
  while (cbs->get_cb() != get())
  {
    if (cbs->get_cb() == NULL)
      return;
    cbs->next();
  }
  uvm_reg_cb::do_delete(rg, get());
  rg->get_fields(flds);
  for( unsigned int i = 0; i < flds.size(); i++ )
    uvm_reg_field_cb::do_delete(flds[i], get());
}

//----------------------------------------------------------------------
// member function: get (static)
//
//! Implementation defined
//! get singleton
//----------------------------------------------------------------------

uvm_reg_write_only_cbs* uvm_reg_write_only_cbs::get()
{
  if (m_me == NULL)
    m_me = new uvm_reg_write_only_cbs();
  return m_me;
}

} // namespace uvm
