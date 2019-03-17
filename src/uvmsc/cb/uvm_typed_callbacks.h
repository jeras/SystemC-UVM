//----------------------------------------------------------------------
//   Copyright 2014 Fraunhofer-Gesellschaft zur Foerderung
//					der angewandten Forschung e.V.
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

#ifndef UVM_TYPED_CALLBACKS_H_
#define UVM_TYPED_CALLBACKS_H_

#include <vector>
#include <string>
#include <iostream>

#include "uvmsc/macros/uvm_message_defines.h"
#include "uvmsc/conf/uvm_queue.h"
#include "uvmsc/conf/uvm_pool.h"
#include "uvmsc/cb/uvm_callbacks_base.h"
#include "uvmsc/cb/uvm_callback.h"

namespace uvm {


// forward class declarations
class uvm_typeid_base;

//----------------------------------------------------------------------
// Class: uvm_typed_callbacks<T>
//
//! daggered / implementation specific class
//!
//! This class contains the queue of typewide
//! callbacks. It also contains some of the public interface methods,
//! but those methods are accessed via the uvm_callbacks#() class
//! so they are documented in that class even though the implementation
//! is in this class.
//!
//! The add, delete, and display methods are implemented in this class.
//----------------------------------------------------------------------


template <typename T = uvm_object>
class uvm_typed_callbacks: public uvm_callbacks_base
{
 public:
  typedef uvm_callbacks_base base_type;

  static uvm_typed_callbacks<T>* m_initialize();

  virtual bool m_am_i_a( uvm_object* obj );

  virtual uvm_queue<uvm_callback*>* m_get_tw_cb_q( uvm_object* obj );

  static int m_cb_find( uvm_queue<uvm_callback*>* q, uvm_callback* cb );

  static int m_cb_find_name( uvm_queue<uvm_callback*>* q, const std::string& name, const std::string& where );
  
  virtual void m_add_tw_cbs( uvm_callback* cb, uvm_apprepend ordering );

  virtual bool m_delete_tw_cbs( uvm_callback* cb );

  static void display( T* obj = NULL);

  // data members

  static uvm_queue<uvm_callback*>* m_tw_cb_q;
  static std::string m_typename;

  //The actual global object from the derivative class. Note that this is
  //just a reference to the object that is generated in the derived class.
  static uvm_typed_callbacks<T>* m_t_inst;

}; // class uvm_typed_callbacks


//------------------------------------------------------------------------------
// Implementation
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// static data member initialization
//------------------------------------------------------------------------------

template <typename T>
std::string uvm_typed_callbacks<T>::m_typename = "";

template <typename T>
uvm_queue<uvm_callback*>* uvm_typed_callbacks<T>::m_tw_cb_q = NULL;

template <typename T>
uvm_typed_callbacks<T>* uvm_typed_callbacks<T>::m_t_inst = NULL;

//----------------------------------------------------------------------------
// member function: m_initialize (static)
//
//! Implementation defined
//----------------------------------------------------------------------------

template <typename T>
uvm_typed_callbacks<T>* uvm_typed_callbacks<T>::m_initialize()
{
  if(m_t_inst == NULL)
  {
    uvm_callbacks_base::m_initialize();
    m_t_inst = new uvm_typed_callbacks<T>();
    m_t_inst->m_tw_cb_q = new uvm_queue<uvm_callback*>("typewide_queue");
  }
  return m_t_inst;
}

//----------------------------------------------------------------------------
// member function: m_am_i_a (virtual)
//
//! Implementation defined
//! Type checking interface: is given ~obj~ of type T?
//----------------------------------------------------------------------------

template <typename T>
bool uvm_typed_callbacks<T>::m_am_i_a( uvm_object* obj )
{
  T* check_type = NULL;

  if (obj == NULL)
    return true;
  check_type = dynamic_cast<T*>(obj);

  return (check_type != NULL);
}

//----------------------------------------------------------------------------
// member function: m_get_tw_cb_q (virtual)
//
//! Implementation defined
//! Getting the typewide queue
//----------------------------------------------------------------------------

template <typename T>
uvm_queue<uvm_callback*>* uvm_typed_callbacks<T>::m_get_tw_cb_q( uvm_object* obj )
{
  uvm_queue<uvm_callback*>* q = NULL;

  if( m_am_i_a(obj) )
  {
    for( unsigned int i = 0; i < this->m_derived_types.size(); i++ )
    {
      base_type* dt = uvm_typeid_base::get_cb(m_derived_types[i]);

      if(dt != NULL && dt != this)
      {
        q = dt->m_get_tw_cb_q(obj);
        if(q != NULL)
          return q;
      }
    }
    return m_t_inst->m_tw_cb_q;
  }
  else
    return NULL;
}

//----------------------------------------------------------------------------
// member function: m_cb_find (static)
//
//! Implementation defined
//----------------------------------------------------------------------------

template <typename T>
int uvm_typed_callbacks<T>::m_cb_find( uvm_queue<uvm_callback*>* q, uvm_callback* cb )
{
  for( int i = 0; i < q->size(); ++i )
    if( q->get(i) == cb )
      return i;
  return -1;
}

//----------------------------------------------------------------------------
// member function: m_cb_find_name (static)
//
//! Implementation defined
//----------------------------------------------------------------------------

template <typename T>
int uvm_typed_callbacks<T>::m_cb_find_name( uvm_queue<uvm_callback*>* q, const std::string& name, const std::string& where)
{
  uvm_callback* cb = NULL;

  for(int i = 0; i < q->size(); ++i)
  {
    cb = q->get(i);
    if(cb->get_name() == name)
    {
       UVM_WARNING("UVM/CB/NAM/SAM", "A callback named '" + name +
         "' is already registered with " + where );
       return 1;
    }
  }
  return 0;
}

//----------------------------------------------------------------------------
// member function: m_add_tw_cbs (virtual)
//
//! Implementation defined
//!
//! For a typewide callback, need to add to derivative types as well.
//----------------------------------------------------------------------------

template <typename T>
void uvm_typed_callbacks<T>::m_add_tw_cbs( uvm_callback* cb, uvm_apprepend ordering )
{
  base_type* cb_pair = NULL;
  uvm_object* obj = NULL;
  T* me = NULL;
  bool warned = false;

  uvm_queue<uvm_callback*>* q = NULL;

  if( m_cb_find(m_t_inst->m_tw_cb_q,cb) == -1)
  {
     warned = m_cb_find_name(m_t_inst->m_tw_cb_q, cb->get_name(), "type");
     if(ordering == UVM_APPEND)
        m_t_inst->m_tw_cb_q->push_back(cb);
     else
        m_t_inst->m_tw_cb_q->push_front(cb);
  }
  if( m_t_inst->m_pool->first(obj) )
  {
    do
    {
      me = dynamic_cast<T*>(obj);
      if( me != NULL )
      {
        q = m_t_inst->m_pool->get(obj);
        if(q == NULL)
        {
          q = new uvm_queue<uvm_callback*>();
          m_t_inst->m_pool->add(obj,q);
        }
        if( m_cb_find(q, cb) == -1 )
        {
          if (!warned)
             m_cb_find_name(q, cb->get_name(), "object instance " + me->get_full_name());

          if(ordering == UVM_APPEND)
            q->push_back(cb);
          else
            q->push_front(cb);
        }
      }
    }
    while( m_t_inst->m_pool->next(obj) );
  }

  for( unsigned int i = 0; i < m_derived_types.size(); i++)
  {
    cb_pair = uvm_typeid_base::get_cb(m_derived_types[i]);

    if(cb_pair != this)
      cb_pair->m_add_tw_cbs(cb, ordering);
  }
}

//----------------------------------------------------------------------------
// member function: m_delete_tw_cbs (virtual)
//
//! Implementation defined
//!
//! For a typewide callback, need to remove from derivative types as well.
//----------------------------------------------------------------------------

template <typename T>
bool uvm_typed_callbacks<T>::m_delete_tw_cbs( uvm_callback* cb )
{
  base_type* cb_pair = NULL;
  uvm_object* obj = NULL;
  uvm_queue<uvm_callback*>* q = NULL;

  bool del_tw_cbs = false;

  int pos = m_cb_find(m_t_inst->m_tw_cb_q,cb);

  if(pos != -1)
  {
    m_t_inst->m_tw_cb_q->do_delete(pos);
    del_tw_cbs = true;
  }

  if( m_t_inst->m_pool->first(obj) )
  {
    do
    {
      q = m_t_inst->m_pool->get(obj);
      if( q == NULL )
      {
        q = new uvm_queue<uvm_callback*>(); // TODO pass name as argument?
        m_t_inst->m_pool->add(obj,q);
      }
      pos = m_cb_find(q,cb);
      if(pos != -1)
      {
        q->do_delete(pos);
        del_tw_cbs = true;
      }
    }
    while(m_t_inst->m_pool->next(obj));
  }

  for( unsigned int i = 0; i < m_derived_types.size(); i++ )
  {
    cb_pair = uvm_typeid_base::get_cb(m_derived_types[i]);

    if(cb_pair != this)
      del_tw_cbs |= cb_pair->m_delete_tw_cbs(cb);
  }

  return del_tw_cbs;
}


//----------------------------------------------------------------------------
// member function: display (static)
//
//! Implementation defined
//----------------------------------------------------------------------------

template <typename T>
void uvm_typed_callbacks<T>::display( T* obj )
{
  T* me = NULL;
  //base_type* ib = m_t_inst; // unused
  std::vector<std::string> cbq;
  std::vector<std::string> inst_q;
  std::vector<std::string> mode_q;

  uvm_callback* cb = NULL;
  std::string blanks = "               ";
  uvm_object* bobj = obj;

  uvm_queue<uvm_callback*>* q = NULL;

  std::string tname, str;

  int max_cb_name = 0;
  int max_inst_name = 0;

  m_tracing = false; // don't allow tracing during display

  if(!m_typename.empty()) tname = m_typename;
  else
    if(obj != NULL) tname = obj->get_type_name();
    else tname = "*";

  q = m_t_inst->m_tw_cb_q;

  for( int i = 0; i < q->size(); ++i )
  {
    cb = q->get(i);
    cbq.push_back(cb->get_name());
    inst_q.push_back("(*)");
    if(cb->is_enabled())
      mode_q.push_back("ON");
    else
      mode_q.push_back("OFF");

    str = cb->get_name();
    max_cb_name = max_cb_name > str.length() ? max_cb_name : str.length();
    str = "(*)";
    max_inst_name = max_inst_name > str.length() ? max_inst_name : str.length();
  }

  if( obj == NULL )
  {
    if(m_t_inst->m_pool->first(bobj))
    {
      do
      {
        me = dynamic_cast<T*>(bobj);
        if(me != NULL) break;
      }
      while(m_t_inst->m_pool->next(bobj));
    }

    if(me != NULL || m_t_inst->m_tw_cb_q->size())
    {
      std::cout << "Registered callbacks for all instances of " <<  tname << std::endl;
      std::cout << "---------------------------------------------------------------" << std::endl;
    }

    if( me != NULL )
    {
      do
      {
        me = dynamic_cast<T*>(bobj);
        if(me != NULL)
        {
          q = m_t_inst->m_pool->get(bobj);
          if (q == NULL)
          {
            q = new uvm_queue<uvm_callback*>(); // TODO pass name as argument?
            m_t_inst->m_pool->add(bobj,q);
          }
          for(int i = 0; i < q->size(); ++i)
          {
            cb = q->get(i);
            cbq.push_back(cb->get_name());
            inst_q.push_back(bobj->get_full_name());
            if(cb->is_enabled()) mode_q.push_back("ON");
            else mode_q.push_back("OFF");

            str = cb->get_name();
            max_cb_name = max_cb_name > str.length() ? max_cb_name : str.length();
            str = bobj->get_full_name();
            max_inst_name = max_inst_name > str.length() ? max_inst_name : str.length();
          }
        }
      }
      while (m_t_inst->m_pool->next(bobj));
    }
    else
      std::cout << "No callbacks registered for any instances of type " << tname << "." << std::endl;
  }
  else
  {
    if(m_t_inst->m_pool->exists(bobj) || m_t_inst->m_tw_cb_q->size())
    {
      std::cout << "Registered callbacks for instance " << obj->get_full_name()
                << " of " << tname << std::endl;
      std::cout << "---------------------------------------------------------------" << std::endl;
    }
    if(m_t_inst->m_pool->exists(bobj))
    {
      q = m_t_inst->m_pool->get(bobj);
      if(q == NULL)
      {
        q = new uvm_queue<uvm_callback*>(); // TODO pass name as argument?
        m_t_inst->m_pool->add(bobj,q);
      }

      for( int i = 0; i < q->size(); ++i )
      {
        cb = q->get(i);
        cbq.push_back(cb->get_name());
        inst_q.push_back(bobj->get_full_name());
        if(cb->is_enabled()) mode_q.push_back("ON");
        else mode_q.push_back("OFF");

        str = cb->get_name();
        max_cb_name = max_cb_name > str.length() ? max_cb_name : str.length();
        str = bobj->get_full_name();
        max_inst_name = max_inst_name > str.length() ? max_inst_name : str.length();
      }
    }
  }

  if(!cbq.size())
  {
    if(obj == NULL) str = "*";
    else str = obj->get_full_name();
    std::cout << "No callbacks registered for instance " << str << " of type " << tname << std::endl;
  }

  for( unsigned int i = 0; i < cbq.size(); i++)
  {
    std::cout << cbq[i]
              << "  "
              << blanks.substr(0, max_cb_name-cbq[i].length()-1)
              << " on "
              << inst_q[i]
              << "  "
              << blanks.substr(0, max_inst_name - inst_q[i].length()-1)
              << "  "
              << mode_q[i] // TODO is this also displayed?
              << std::endl;
  }

  m_tracing = true; //allow tracing to be resumed
}

} // namespace uvm

#endif // UVM_TYPED_CALLBACKS_H_
