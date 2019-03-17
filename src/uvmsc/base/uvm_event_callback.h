//------------------------------------------------------------------------------
//   Copyright 2013-2014 NXP B.V.
//   Copyright 2007-2010 Mentor Graphics Corporation
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
//------------------------------------------------------------------------------

#ifndef UVM_EVENT_CALLBACK_H_
#define UVM_EVENT_CALLBACK_H_

#include <systemc>
#include <string>

#include "uvmsc/base/uvm_object.h"

namespace uvm {


// forward class declarations
class uvm_event;

//------------------------------------------------------------------------------
// CLASS: uvm_event_callback
//
//! The uvm_event_callback class is an abstract class that is used to create
//! callback objects which may be attached to <uvm_events>. To use, you
//! derive a new class and override any or both <pre_trigger> and <post_trigger>.
//!
//! Callbacks are an alternative to using processes that wait on events. When a
//! callback is attached to an event, that callback object's callback function
//! is called each time the event is triggered.
//------------------------------------------------------------------------------

class uvm_event_callback : public uvm_object
{
public:

  // Constructor
  // Creates a new callback object.
  uvm_event_callback( const std::string& name = "" ) : uvm_object(name)
  {}

  // Function: pre_trigger
  //
  // This callback is called just before triggering the associated event.
  // In a derived class, override this method to implement any pre-trigger
  // functionality.
  //
  // If your callback returns 1, then the event will not trigger and the
  // post-trigger callback is not called. This provides a way for a callback
  // to prevent the event from triggering.
  //
  // In the function, ~e~ is the <uvm_event> that is being triggered, and ~data~
  // is the optional data associated with the event trigger.

  virtual bool pre_trigger( const uvm_event* e, const uvm_object* data = NULL ) const
  {
    return false;
  }

  // Function: post_trigger
  //
  // This callback is called after triggering the associated event.
  // In a derived class, override this method to implement any post-trigger
  // functionality.
  //
  //
  // In the function, \p e is the #uvm_event that is being triggered, and \p data
  // is the optional data associated with the event trigger.

  virtual void post_trigger( const uvm_event* e, const uvm_object* data = NULL) const
  {
    return;
  }

  virtual uvm_object* create( const std::string& name = "" )
  {
    return NULL;
  }

}; // class uvm_event_callback

} // namespace uvm

#endif
