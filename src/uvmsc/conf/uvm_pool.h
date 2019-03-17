//-----------------------------------------------------------------------------
//   Copyright 2007-2011 Mentor Graphics Corporation
//   Copyright 2007-2010 Cadence Design Systems, Inc. 
//   Copyright 2010 Synopsys, Inc.
//   Copyright 2012-2015 NXP B.V.
//   Copyright 2013 Fraunhofer-Gesellschaft zur Foerderung
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
//-----------------------------------------------------------------------------

#ifndef UVM_POOL_H_
#define UVM_POOL_H_

#include <map>

#include "uvmsc/base/uvm_void.h"
#include "uvmsc/base/uvm_object.h"
#include "uvmsc/base/uvm_root.h"

namespace uvm {

// forward class references
//class uvm_barrier; // TODO
//class uvm_event;

//typedef uvm_object_string_pool<uvm_barrier> uvm_barrier_pool;
//typedef uvm_object_string_pool<uvm_event> uvm_event_pool;


//-----------------------------------------------------------------------------
// CLASS: uvm_pool<KEY,T>
//
//! Implements a class-based dynamic associative array. Allows sparse arrays to
//! be allocated on demand, and passed and stored by reference.
//-----------------------------------------------------------------------------

template <typename KEY = int, typename T = uvm_void >
class uvm_pool : public uvm_object
{
 public:
  typedef uvm_pool<KEY,T> this_type;

  //--------------------------------------------------------------------------
  // UVM Standard LRM API below
  //--------------------------------------------------------------------------

  uvm_pool();
  explicit uvm_pool( const std::string& name );
  virtual ~uvm_pool();

  static uvm_pool<KEY,T>* get_global_pool();
  static T get_global( const KEY& key );
  virtual T get( const KEY& key );
  virtual void add( const KEY& key, const T& item );
  virtual int num() const;
  virtual void do_delete( const KEY& key );
  virtual bool exists( const KEY& key ) const;
  virtual bool first( KEY& key );
  virtual bool last( KEY& key );
  virtual bool next( KEY& key );
  virtual bool prev( KEY& key );

  /////////////////////////////////////////////////////
  // Implementation-defined member functions below,
  // not part of UVM Class reference / LRM
  /////////////////////////////////////////////////////

 private:
  virtual uvm_object* create( const std::string& name = "" );
  virtual const std::string get_type_name() const;
  virtual void do_copy(const uvm_object& rhs );
  virtual void do_print( const uvm_printer& printer ) const;

  // data members
 protected:
  static const std::string type_name;
  static uvm_pool<KEY,T>* m_global_pool;
  typedef std::map<KEY,T> poolT;
  typename std::map<KEY,T>::iterator pool_it;
  poolT pool;

}; // class uvm_pool


/////////////////////////////////////////
// Class implementation starts here
/////////////////////////////////////////


//-----------------------------------------------------------------------------
// Initialization of uvm_pool static data members
//-----------------------------------------------------------------------------

template <typename KEY, typename T>
const std::string uvm_pool<KEY,T>::type_name = "uvm::uvm_pool";

template <typename KEY, typename T>
uvm_pool<KEY,T>* uvm_pool<KEY,T>::m_global_pool = NULL;

//-----------------------------------------------------------------------------
// Constructors
//
// Creates a new pool with the given \p name.
//-----------------------------------------------------------------------------

template <typename KEY, typename T>
uvm_pool<KEY,T>::uvm_pool()
  : uvm_object()
{
  pool.clear();
}

template <typename KEY, typename T>
uvm_pool<KEY,T>::uvm_pool( const std::string& name )
  : uvm_object(name)
{
  pool.clear();
}

//-----------------------------------------------------------------------------
// Destructor
//-----------------------------------------------------------------------------

template <typename KEY, typename T>
uvm_pool<KEY,T>::~uvm_pool()
{
  if( m_global_pool != NULL )
  {
    delete m_global_pool;
    m_global_pool = NULL;
  }
}

//-----------------------------------------------------------------------------
// member function: get_global_pool (static)
//
//! Returns the singleton global pool for the item type, T.
//! This allows items to be shared amongst components throughout the
//! verification environment.
//-----------------------------------------------------------------------------

template <typename KEY, typename T>
uvm_pool<KEY,T>* uvm_pool<KEY,T>::get_global_pool()
{
  if( m_global_pool == NULL )
    m_global_pool = new uvm_pool("pool");
  return m_global_pool;
}

//-----------------------------------------------------------------------------
// member function: get_global (static)
//
//! Returns the specified item instance from the global item pool.
//-----------------------------------------------------------------------------

template <typename KEY, typename T>
T uvm_pool<KEY,T>::get_global( const KEY& key )
{
  uvm_pool<KEY,T>* gpool;
  gpool = get_global_pool();
  return gpool->get(key);
}

//-----------------------------------------------------------------------------
// member function: get
//
//! Returns the item with the given \p key.
//! If no item exists by that key, a new item is created with that key
//! and returned.
//-----------------------------------------------------------------------------

template <typename KEY, typename T>
T uvm_pool<KEY,T>::get( const KEY& key )
{
  if (pool.find(key) == pool.end()) // if not exist
  {
    T t = T(); // TODO check validity for all relevant types
    pool[key] = t;
  }
  return pool[key];
}

//-----------------------------------------------------------------------------
// member function: add
//
//! Adds the given (\p key, \p item) pair to the pool. If an item already
//! exists at the given \p key it is overwritten with the new \p item.
//-----------------------------------------------------------------------------

template <typename KEY, typename T>
void uvm_pool<KEY,T>::add( const KEY& key, const T& item )
{
  pool[key] = item;
}


//-----------------------------------------------------------------------------
// member function: num
//
//! Returns the number of uniquely keyed items stored in the pool.
//-----------------------------------------------------------------------------

template <typename KEY, typename T>
int uvm_pool<KEY,T>::num() const
{
  return pool.size();
}


//-----------------------------------------------------------------------------
// member function: do_delete
//
//! Removes the item with the given \p key from the pool.
//! NOTE: UVM-SV name delete, but reserved keyword in C++
//-----------------------------------------------------------------------------

template <typename KEY, typename T>
void uvm_pool<KEY,T>::do_delete( const KEY& key )
{
  if ( pool.find(key) == pool.end() )
  {
    uvm_root* top;
    top = uvm_root::get();
    top->uvm_report_warning("POOLDEL",
        "do_delete: pool key doesn't exist. Ignoring delete request");
    return;
  }
  pool.erase(key);
}


//-----------------------------------------------------------------------------
// Function: exists
//
//! Returns true if a item with the given \p key exists in the pool,
//! false otherwise.
//-----------------------------------------------------------------------------

template <typename KEY, typename T>
bool uvm_pool<KEY,T>::exists( const KEY& key ) const
{
  return (pool.find(key) != pool.end());
}


//-----------------------------------------------------------------------------
// member function: first
//
//! Returns the key of the first item stored in the pool.
//!
//! If the pool is empty, then \p key is unchanged and false is returned.
//!
//! If the pool is not empty, then \p key is key of the first item
//! and true is returned.
//-----------------------------------------------------------------------------

template <typename KEY, typename T>
bool uvm_pool<KEY,T>::first( KEY& key )
{
  if( pool.size() == 0 )
    return false;
  else
  {
    pool_it = pool.begin();
    key = (*pool_it).first;
    return true;
  }
}


//-----------------------------------------------------------------------------
// member function: last
//
//! Returns the key of the last item stored in the pool.
//!
//! If the pool is empty, then false is returned and \p key is unchanged.
//!
//! If the pool is not empty, then \p key is set to the last key in
//! the pool and true is returned.
//-----------------------------------------------------------------------------

template <typename KEY, typename T>
bool uvm_pool<KEY,T>::last( KEY& key )
{
  if( pool.size() == 0 )
    return false;
  else
  {
    pool_it = pool.end();
    key = (*pool_it).first;
    return true;
  }
}

//-----------------------------------------------------------------------------
// member function: next
//
//! Returns the \p key of the next item in the pool.
//! If the input \p key is the last key in the pool, then \p key is
//! left unchanged and false is returned.
//! If a next key is found, then \p key is updated with that key
//! and true is returned.
//-----------------------------------------------------------------------------

template <typename KEY, typename T>
bool uvm_pool<KEY,T>::next( KEY& key )
{
  typename std::map<KEY,T>::iterator it;
  it = pool.end();
  it--; // last element?

  if ( key == (*it).first )
    return false;
  else
  {
    pool_it++;
    key = (*pool_it).first;
    return true;
  }
}


//-----------------------------------------------------------------------------
// member function: prev
//
//! Returns the \p key of the previous item in the pool.
//!
//! If the input \p key is the first key in the pool, then \p key is
//! left unchanged and false is returned.
//!
//! If a previous key is found, then \p key is updated with that key
//! and true is returned.
//-----------------------------------------------------------------------------

template <typename KEY, typename T>
bool uvm_pool<KEY,T>::prev( KEY& key )
{
  if ( key == (*pool.begin()).first )
    return false;
  else
  {
    pool_it--;
    key = (*pool_it).first;
    return true;
  }
}

//-----------------------------------------------------------------------------
// member function: create
//
//! Returns the newly created key-value pair.
//-----------------------------------------------------------------------------

template <typename KEY, typename T>
uvm_object* uvm_pool<KEY,T>::create(const std::string& name )
{
  uvm_pool<KEY,T>* v;
  v = new uvm_pool<KEY,T>(name);
  return v;
}

//-----------------------------------------------------------------------------
// member function: get_type_name
//
//! Returns the type name of the pool
//-----------------------------------------------------------------------------

template <typename KEY, typename T>
const std::string uvm_pool<KEY,T>::get_type_name() const
{
  return type_name;
}

//-----------------------------------------------------------------------------
// member function: do_copy
//
//! Copy the value given as argument
//-----------------------------------------------------------------------------

template <typename KEY, typename T>
void uvm_pool<KEY,T>::do_copy(const uvm_object& rhs )
{
  // TODO does this copy work?
  const uvm_pool<KEY,T>* p;
  uvm_object::do_copy(rhs);
  p = dynamic_cast<const uvm_pool<KEY,T>*>(&rhs);
  if (p == NULL)
    return;
  pool = p->pool;
}

//-----------------------------------------------------------------------------
// member function: do_print
//
//! Print the content of the pool to the stream given as argument
//-----------------------------------------------------------------------------

template <typename KEY, typename T>
void uvm_pool<KEY,T>::do_print( const uvm_printer& printer ) const
{
  int cnt = 0;
  std::ostringstream item;
  std::ostringstream v;

  static char separator[] = "[";

  typename std::map<KEY,T>::const_iterator pit;

  printer.print_array_header("pool", pool.size(), "aa_object_string");

  for( pit = pool.begin(); pit != pool.end(); pit++)
  {
      item << "[-key #" << cnt++ << " " << pit->first << "--]";
      v << pit->second;
      printer.print_generic(item.str(),"",-1,v.str(),separator);
  }
  printer.print_array_footer();
}

//TODO is key set ? shouldn't pit be used ?
} // namespace uvm

#endif // UVM_POOL_H_

