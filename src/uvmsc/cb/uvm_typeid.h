//----------------------------------------------------------------------
//   Copyright 2013-2014 NXP B.V.
//   Copyright 2007-2011 Mentor Graphics Corporation
//   Copyright 2007-2010 Cadence Design Systems, Inc.
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
//----------------------------------------------------------------------

#ifndef UVM_TYPEID_H_
#define UVM_TYPEID_H_

#include <string>
#include <map>

namespace uvm {


// forward class declarations
class uvm_callback;
class uvm_callbacks_base;
class uvm_object;

//------------------------------------------------------------------------------
// Class: uvm_typeid_base
//
//! Implementation defined
//!
//! Simple typeid interface. Need this to set up the base-super mapping.
//! This is similar to the factory, but much simpler. The idea of this
//! interface is that each object type T has a typeid that can be
//! used for mapping type relationships. This is not a user visible class.
//------------------------------------------------------------------------------


class uvm_typeid_base
{
  typedef std::map<uvm_typeid_base*, uvm_callbacks_base*> typeid_map_t;
  typedef std::map<uvm_callbacks_base*, uvm_typeid_base*> type_map_t;

 public:
    static int set_cb(uvm_typeid_base*, uvm_callbacks_base*);
    static uvm_callbacks_base* get_cb(uvm_typeid_base*);

    static int set_typeid(uvm_callbacks_base*, uvm_typeid_base*);
    static uvm_typeid_base* get_typeid(uvm_callbacks_base*);

  static std::string type_name;

 private:
   static typeid_map_t& m_access_typeid_map()
   {
     static typeid_map_t typeid_map;// = typeid_map_t();
     return typeid_map;
   }

   static type_map_t& m_access_type_map()
   {
     static type_map_t type_map; // = type_map_t();
     return type_map;
   }
}; // class uvm_typeid_base


//------------------------------------------------------------------------------
// Class: uvm_typeid<T>
//
// Implementation defined
//------------------------------------------------------------------------------

template <typename T = uvm_object>
class uvm_typeid : public uvm_typeid_base
{
 public:
  static uvm_typeid<T>* m_b_inst;
  static uvm_typeid<T>* get();
};

template <typename T>
uvm_typeid<T>* uvm_typeid<T>::m_b_inst = NULL;

template <typename T>
inline uvm_typeid<T>* uvm_typeid<T>::get()
{
  if( m_b_inst == NULL )
    m_b_inst = new uvm_typeid<T>();

  return m_b_inst;
}


} // namespace uvm

#endif // UVM_TYPEID_H_
