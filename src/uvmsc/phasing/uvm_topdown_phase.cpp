//----------------------------------------------------------------------
//   Copyright 2014 Fraunhofer-Gesellschaft zur Foerderung
//					der angewandten Forschung e.V.
//   Copyright 2012-2014 NXP B.V.
//   Copyright 2007-2011 Mentor Graphics Corporation
//   Copyright 2007-2010 Cadence Design Systems, Inc.
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

#include <sstream>

#include "uvmsc/base/uvm_component.h"
#include "uvmsc/base/uvm_globals.h"
#include "uvmsc/phasing/uvm_phase.h"
#include "uvmsc/phasing/uvm_topdown_phase.h"
#include "uvmsc/base/uvm_object_globals.h"
#include "uvmsc/macros/uvm_message_defines.h"

//////////////

namespace uvm {

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------

uvm_topdown_phase::uvm_topdown_phase( const std::string& name )
  : uvm_phase(name, UVM_PHASE_IMP)
{}

//----------------------------------------------------------------------
// member function: traverse
//
//! Traverses the component tree in top-down order, calling #execute for
//! each component.
//----------------------------------------------------------------------

void uvm_topdown_phase::traverse( uvm_component* comp,
                                  uvm_phase* phase,
                                  uvm_phase_state state )
{
  std::string name;
  uvm_domain* phase_domain = phase->get_domain();
  uvm_domain* comp_domain = comp->get_domain();

  if (m_phase_trace)
  {
    std::ostringstream str;
    str << "topdown-phase phase = " << phase->get_name() << std::endl
        << " state = " << uvm_phase_state_name[state] << std::endl
        << " component name = " << comp->get_full_name()  << std::endl
        << " component domain = " << comp_domain->get_name() << std::endl
        << " phase domain = " << phase_domain->get_name();
    UVM_INFO("PH_TRACE", str.str(), UVM_DEBUG);
  }

  if (phase_domain == uvm_domain::get_common_domain() ||
      phase_domain == comp_domain)
  {
      switch (state)
      {
        case UVM_PHASE_STARTED:
        {
          comp->m_current_phase = phase;
          comp->m_apply_verbosity_settings(phase);
          comp->phase_started(*phase); //TODO make ref?
          break;
        }

        case UVM_PHASE_EXECUTING:
        {

          if (!(phase->get_name() == "build" && comp->m_build_done))
          {


            uvm_phase* ph = this;
            comp->m_phasing_active++;
            if (comp->m_phase_imps.find(this) != comp->m_phase_imps.end() ) // if exists
              ph = comp->m_phase_imps[this];

            ph->execute(comp, phase);
            comp->m_phasing_active--;
          }
          break;
        }

        case UVM_PHASE_READY_TO_END:
        {
          comp->phase_ready_to_end(*phase); //TODO make ref?
          break;
        }

        case UVM_PHASE_ENDED:
        {
          comp->phase_ended(*phase);
          comp->m_current_phase = NULL;
          break;
        }

        default:
          uvm_report_fatal("PH_BADEXEC","topdown phase traverse internal error");
          break;
      }
  }

  if(comp->get_first_child(name))
    do
      traverse(comp->get_child(name), phase, state);
    while(comp->get_next_child(name));
}


//----------------------------------------------------------------------
// member function: execute
//
//! Executes the top-down phase \p phase for the component \p comp.
//----------------------------------------------------------------------

void uvm_topdown_phase::execute( uvm_component* comp,
                                 uvm_phase* phase)
{
  // TODO
  // reseed this process for random stability
  //process proc = process::self();
  //proc.srandom(uvm_create_random_seed(phase.get_type_name(), comp.get_full_name()));

  comp->m_current_phase = phase;
  exec_func(comp,phase);
}

} // namespace uvm
