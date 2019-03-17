//----------------------------------------------------------------------------
//   Copyright 2012-2014 NXP B.V.
//   Copyright 2007-2011 Mentor Graphics Corporation
//   Copyright 2007-2011 Cadence Design Systems, Inc.
//   Copyright 2010-2011 Synopsys, Inc.
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
//----------------------------------------------------------------------------

#ifndef UVM_FACTORY_H_
#define UVM_FACTORY_H_

#include <map>
#include <list>
#include <string>
#include <iostream>

#include "uvmsc/base/uvm_object.h"
#include "uvmsc/factory/uvm_object_wrapper.h"
#include "uvmsc/report/uvm_report_object.h"


//////////////////////

namespace uvm {


// forward declarations
class uvm_object;
class uvm_component;


//----------------------------------------------------------------------------
// CLASS: uvm_factory_override
//
// Internal class
//----------------------------------------------------------------------------

class uvm_factory_override: public uvm_report_object
{
 public:
  uvm_factory_override( const std::string& full_inst_path_ = "",
		  std::string orig_type_name_ = "",
                        uvm_object_wrapper* orig_type_ = NULL,
                        uvm_object_wrapper* ovrd_type_ = NULL )
  {
    if (ovrd_type_ == NULL)
      uvm_report_fatal( "NULLWR",
                        "Attempting to register a NULL override object with the factory",
                        UVM_NONE);

    full_inst_path = full_inst_path_;
    orig_type_name = (orig_type_ == NULL) ? orig_type_name_ : (orig_type_->get_type_name() );
    orig_type      = orig_type_;
    ovrd_type_name = ovrd_type_->get_type_name();
    ovrd_type      = ovrd_type_;
  }

 public: //data members
  std::string full_inst_path;
  std::string orig_type_name;
  std::string ovrd_type_name;
  bool selected;
  uvm_object_wrapper* orig_type;
  uvm_object_wrapper* ovrd_type;
};


//----------------------------------------------------------------------------
// CLASS: uvm_factory
//
//! As the name implies, uvm_factory is used to manufacture (create) UVM objects
//! and components. Only one instance of the factory is present in a given
//! simulation (termed a singleton). Object and component types are registered
//! with the factory using lightweight proxies to the actual objects and
//! components being created. The #uvm_object_registry<T> and
//! #uvm_component_registry<T> class are used to proxy uvm_objects
//! and uvm_components.
//!
//! The factory provides both name-based and type-based interfaces.
//!
//! type-based - The type-based interface is far less prone to errors in usage.
//!   When errors do occur, they are caught at compile-time.
//!
//! name-based - The name-based interface is dominated
//!   by string arguments that can be misspelled and provided in the wrong order.
//!   Errors in name-based requests might only be caught at the time of the call,
//!   if at all. Further, the name-based interface is not portable across
//!   simulators when used with parameterized classes.
//----------------------------------------------------------------------------

class uvm_factory : public uvm_report_object
{
  friend class uvm_root;
  friend class uvm_object;
  friend class uvm_object_wrapper;
  friend class uvm_default_coreservice_t;

public:

  static uvm_factory* get();

  //--------------------------------------------------------------------------
  // Group: Registering Types
  //--------------------------------------------------------------------------

  void do_register( uvm_object_wrapper* obj );

  //--------------------------------------------------------------------------
  // Group: Type & Instance Overrides
  //--------------------------------------------------------------------------

  void set_inst_override_by_type( uvm_object_wrapper* original_type,
                                  uvm_object_wrapper* override_type,
                                  const std::string& full_inst_path );

  void set_inst_override_by_name( const std::string& original_type_name,
                                  const std::string& override_type_name,
                                  const std::string& full_inst_path );

  void set_type_override_by_type( uvm_object_wrapper* original_type,
                                  uvm_object_wrapper* override_type,
                                  bool replace = true );

  void set_type_override_by_name( const std::string& original_type_name,
                                  const std::string& override_type_name,
                                  bool replace = true );

  //--------------------------------------------------------------------------
  // Group: Creation
  //--------------------------------------------------------------------------

  uvm_object* create_object_by_type( uvm_object_wrapper* requested_type,
                                     const std::string& parent_inst_path = "",
                                     const std::string& name = "" );

  uvm_component* create_component_by_type( uvm_object_wrapper* requested_type,
                                           const std::string& parent_inst_path = "",
                                           const std::string& name = "",
                                           uvm_component* parent = NULL );

  uvm_object* create_object_by_name( const std::string& requested_type_name,
                                     const std::string& parent_inst_path = "",
                                     const std::string& name = "" );

  uvm_component* create_component_by_name( const std::string& requested_type_name,
                                           const std::string& parent_inst_path = "",
                                           const std::string& name = "",
                                           uvm_component* parent = NULL );

  //--------------------------------------------------------------------------
  // Group: Debug
  //--------------------------------------------------------------------------

  void debug_create_by_type( uvm_object_wrapper* requested_type,
                             const std::string& parent_inst_path = "",
                             const std::string& name = "" );

  void debug_create_by_name( const std::string& requested_type_name,
                             const std::string& parent_inst_path = "",
                             const std::string& name = "" );

  uvm_object_wrapper* find_override_by_type( uvm_object_wrapper* requested_type,
                                             const std::string& full_inst_path );

  uvm_object_wrapper* find_override_by_name( const std::string& requested_type_name,
                                             const std::string& full_inst_path );

  void print ( int all_types = 1 );

  /////////////////////////////////////////////////////
  // Implementation-defined member functions below,
  // not part of UVM Class reference / LRM
  /////////////////////////////////////////////////////

 protected:
  uvm_factory();

  virtual ~uvm_factory();

  bool check_inst_override_exists( uvm_object_wrapper* original_type,
                                   uvm_object_wrapper* override_type,
                                   const std::string& full_inst_path );

  bool m_has_wildcard( const std::string& nm );

  void m_debug_create( const std::string& requested_type_name,
                       uvm_object_wrapper* requested_type,
                       const std::string& parent_inst_path,
                       const std::string& name );

  void m_debug_display( const std::string& requested_type_name,
                        uvm_object_wrapper* result,
                        const std::string& full_inst_path );

  uvm_object_wrapper* find_by_name( const std::string& type_name );

 private:

  static void cleanup();

 protected:

  // data members

  static uvm_factory* m_inst;

  typedef std::map<uvm_object_wrapper*, bool> m_types_mapT;
  typedef m_types_mapT::iterator m_types_mapItT;

  typedef std::map<std::string, uvm_object_wrapper*> m_type_names_mapT;
  typedef m_type_names_mapT::iterator m_type_names_mapItT;

  typedef std::list<uvm_factory_override*> m_overrides_listT;
  typedef m_overrides_listT::iterator m_overrides_listItT;
  typedef std::map< std::string, bool > m_lookup_strs_mapT;

  struct uvm_factory_queue_class
  {
    m_overrides_listT queue;
  };

  m_types_mapT       m_types;
  m_type_names_mapT  m_type_names;
  m_lookup_strs_mapT m_lookup_strs;
  m_overrides_listT  m_type_overrides;
  m_overrides_listT  m_wildcard_inst_overrides;

  typedef std::map<uvm_object_wrapper*, uvm_factory_queue_class*>
            m_inst_override_queues_mapT;
  typedef m_inst_override_queues_mapT::iterator
            m_inst_override_queues_mapItT;
  typedef std::map<std::string, uvm_factory_queue_class*>
            m_inst_override_name_queues_mapT;
  typedef m_inst_override_name_queues_mapT::iterator
            m_inst_override_name_queues_mapItT;

  m_inst_override_queues_mapT  m_inst_override_queues;
  m_inst_override_name_queues_mapT m_inst_override_name_queues;

 private:
  m_overrides_listT m_override_info;

  static bool m_debug_pass;
};

//-----------------------------------------------------------------------------
// singleton factory; it is statically initialized
//
// global entry point to get a handle to the factory
//-----------------------------------------------------------------------------

inline
uvm_factory* get_factory()
{
  return uvm_factory::get();
}

} // namespace uvm

//////////////////////

#endif /* UVM_FACTORY_H_ */
