//----------------------------------------------------------------------
//   Copyright 2014 Fraunhofer-Gesellschaft zur Foerderung
//					der angewandten Forschung e.V.
//   Copyright 2012-2014 NXP B.V.
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
//----------------------------------------------------------------------

#ifndef UVM_COMPONENT_REGISTRY_H_
#define UVM_COMPONENT_REGISTRY_H_

#include <systemc>
#include <string>
#include <sstream>

#include "uvmsc/base/uvm_root.h"
#include "uvmsc/base/uvm_component.h"
#include "uvmsc/base/uvm_coreservice_t.h"
#include "uvmsc/base/uvm_default_coreservice_t.h"
#include "uvmsc/report/uvm_report_object.h"
#include "uvmsc/factory/uvm_factory.h"
#include "uvmsc/factory/uvm_object_wrapper.h"


namespace uvm {

// forward declarations of necessary classes
class uvm_object_wrapper;

//----------------------------------------------------------------------
// Title: Factory Component Wrapper
//
// Topic: Intro
//
// This section defines the proxy component class used by the
// factory. To avoid the overhead of creating an instance of every component
// and object that get registered, the factory holds lightweight wrappers,
// or proxies. When a request for a new object is made, the factory calls upon
// the proxy to create the object it represents. 
//
//----------------------------------------------------------------------
//
// CLASS: uvm_component_registry<T,Tname>
//
//! The #uvm_component_registry serves as a lightweight proxy for a component of
//! type T and type name Tname, a string. The proxy enables efficient
//! registration with the uvm_factory. Without it, registration would
//! require an instance of the component itself.
//----------------------------------------------------------------------

template <typename T = uvm_component>
class uvm_component_registry : public uvm_object_wrapper
{
 public:

  //--------------------------------------------------------------------------
  // UVM Standard LRM API below
  //--------------------------------------------------------------------------

  virtual uvm_component* create_component( const std::string& name,
                                           uvm_component* parent);

  virtual const std::string get_type_name() const;

  static uvm_component_registry<T>* get();

  static T* create( const std::string& name = "",
                    uvm_component* parent = NULL,
                    const std::string& contxt = "" );

  static void set_type_override( uvm_object_wrapper* override_type,
                                 bool replace = true );

  static void set_inst_override( uvm_object_wrapper* override_type,
                                 const std::string& inst_path,
                                 uvm_component* parent = NULL );

  /////////////////////////////////////////////////////
  // Implementation-defined member functions below,
  // not part of UVM Class reference / LRM
  /////////////////////////////////////////////////////

  virtual ~uvm_component_registry();

 private:
  explicit uvm_component_registry( const std::string& name = "" );

  static const std::string m_type_name_prop();

  // data members

  static const std::string type_name;

  static uvm_component_registry<T>* me;

  std::vector<T* > m_comp_t_list;

}; // class uvm_component_registry


//----------------------------------------------------------------------
// definition of static members outside class definition
//----------------------------------------------------------------------

template <typename T>
const std::string uvm_component_registry<T>::type_name = m_type_name_prop();

template <typename T>
uvm_component_registry<T>* uvm_component_registry<T>::me = get();

//----------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------

template <typename T>
uvm_component_registry<T>::uvm_component_registry( const std::string& name )
{
}

//----------------------------------------------------------------------
// member function: create_component (virtual)
//
//! Creates a component of type T having the provided name and parent.
//! This is an override of the method in uvm_object_wrapper. It is
//! called by the factory after determining the type of object to create.
//! An application shall not call this method directly. Call create instead.
//----------------------------------------------------------------------

template <typename T>
uvm_component* uvm_component_registry<T>::create_component( const std::string& name,
                                                            uvm_component* parent )
{
  T* comp = NULL;
  comp = new T( name.c_str() );
  m_comp_t_list.push_back(comp); // remember object to delete it later
  return comp;
}

//----------------------------------------------------------------------
// member function: get_type_name (virtual)
//
//! Returns the value given by the string parameter, Tname. This method
//! overrides the method in uvm_object_wrapper.
//----------------------------------------------------------------------

template <typename T>
const std::string uvm_component_registry<T>::get_type_name() const
{
  return uvm_component_registry<T>::type_name;
}

//----------------------------------------------------------------------
// member function: get (static)
//
//! Returns the singleton instance of this type. Type-based factory operation
//! depends on there being a single proxy instance for each registered type.
//----------------------------------------------------------------------

template <typename T>
uvm_component_registry<T>* uvm_component_registry<T>::get()
{
  if (type_name.empty())
    m_type_name_prop();

  if (me == NULL)
  {
    uvm_coreservice_t* cs = uvm_coreservice_t::get();
    uvm_factory* f = cs->get_factory();
    me = new uvm_component_registry<T>("comprgy_" + type_name);
    f->do_register(me);
  }
  return me;
}

//----------------------------------------------------------------------
// member function: create (static)
//
//! Returns an instance of the component type, T, represented by this proxy,
//! subject to any factory overrides based on the context provided by the
//! parent's full name. The contxt argument, if supplied, supercedes the
//! parent's context. The new instance will have the given leaf name
//! and parent.
//----------------------------------------------------------------------

template <typename T>
T* uvm_component_registry<T>::create( const std::string& name,
                                      uvm_component* parent,
                                      const std::string& contxt )
{
  std::string l_contxt;
  uvm_component* obj = NULL;
  uvm_coreservice_t* cs = uvm_coreservice_t::get();
  uvm_factory* f = cs->get_factory();

  if (l_contxt.empty() && parent != NULL)
    l_contxt = parent->get_full_name();
  obj = f->create_component_by_type( get(), l_contxt, name, parent );

  T* robj = dynamic_cast<T*>(obj);
  if (robj == NULL)
  {
    std::ostringstream msg;
    msg << "Factory did not return a component of type '" << type_name << "'."
        << " A component of type '" << ((obj == NULL) ? "NULL" : obj->get_type_name() )
        << "' was returned instead. Name=" << name << " Parent="
        << ((parent == NULL) ? "NULL" : parent->get_type_name()) << " contxt=" << l_contxt;

    uvm_report_fatal("FCTTYP", msg.str(), UVM_NONE);
  }
  return robj;
}

//----------------------------------------------------------------------
// member function: set_type_override (static)
//
//! Configures the factory to create an object of the type represented by
//! override_type whenever a request is made to create an object of the type,
//! T, represented by this proxy, provided no instance override applies. The
//! original type, T, is typically a super class of the override type.
//----------------------------------------------------------------------

template <typename T>
void uvm_component_registry<T>::set_type_override( uvm_object_wrapper* override_type,
                                                   bool replace )
{
  uvm_coreservice_t* cs = uvm_coreservice_t::get();
  uvm_factory* factory = cs->get_factory();
  factory->set_type_override_by_type(get(), override_type, replace);
}


//----------------------------------------------------------------------
// member function: set_inst_override (static)
//
//! Configures the factory to create a component of the type represented by
//! override_type whenever a request is made to create an object of the type,
//! T, represented by this proxy,  with matching instance paths. The original
//! type, T, is typically a super class of the override type.
//!
//! If parent is not specified, inst_path is interpreted as an absolute
//! instance path, which enables instance overrides to be set from outside
//! component classes. If parent is specified, inst_path is interpreted
//! as being relative to the parent's hierarchical instance path.
//! The inst_path may contain wildcards for matching against multiple contexts.
//----------------------------------------------------------------------

template <typename T>
void uvm_component_registry<T>::set_inst_override( uvm_object_wrapper* override_type,
                                                   const std::string& inst_path,
                                                   uvm_component* parent )
{
  std::ostringstream loc_inst_path;
  if (parent != NULL)
  {
    if (inst_path.empty())
      loc_inst_path << parent->get_full_name();
    else
      loc_inst_path << parent->get_full_name() << "." << inst_path;
  }
  uvm_coreservice_t* cs = uvm_coreservice_t::get();
  uvm_factory* factory = cs->get_factory();

  factory->set_inst_override_by_type( get(), override_type,loc_inst_path.str());
}

//----------------------------------------------------------------------
// member function: m_type_name_prop (static)
//
//! Implementation defined method
//! Type name propagation to retrieve the string from the original
//! class via the macro M_UVM_COMPONENT_REGISTRY_INTERNAL which
//! defines the string as part of method m_register_type_name()
//
// TODO: other ways to pass string to this template class?
//----------------------------------------------------------------------

template <typename T>
const std::string uvm_component_registry<T>::m_type_name_prop()
{
  return  T::m_register_type_name();
}

//----------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------

template <typename T>
uvm_component_registry<T>::~uvm_component_registry()
{
  // clean memory
  if (me != NULL)
  {
    delete me;
    me = NULL;
  }

  while(!m_comp_t_list.empty())
    delete m_comp_t_list.back(), m_comp_t_list.pop_back();
}


//////////////

} // namespace uvm

#endif // UVM_COMPONENT_REGISTRY_H_

