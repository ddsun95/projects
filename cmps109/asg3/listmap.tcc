// $Id: listmap.tcc,v 1.7 2015-04-28 19:22:02-07 - - $
// David Sun(ddsun@ucsc.edu) and Mithu Athreya(mathreya@ucsc.edu)

#include "listmap.h"
#include "trace.h"
#include <stdexcept>

//
/////////////////////////////////////////////////////////////////
// Operations on listmap::node.
/////////////////////////////////////////////////////////////////
//

//
// listmap::node::node (link*, link*, const value_type&)
//
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::node::node (node* next, node* prev,
                                     const value_type& value):
            link (next, prev), value (value) {
}

//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
//
/* Erases each node from the list while !empty */
template <typename Key, typename Value, class Less>
listmap<Key,Value,Less>::~listmap() {
   TRACE ('l', (void*) this);
   while(begin() != end()) erase(begin());
}


//
// iterator listmap::insert (const value_type&)
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::insert (const value_type& pair) {
   TRACE ('l', &pair << "->" << pair);
   iterator itor;
   Less less;
   for(itor = begin(); itor != end(); ++itor){
      if(!less(itor->first, pair.first) && 
         !less(pair.first, itor->first)){
         itor->second = pair.second;
         return itor;
      }
      if(less(pair.first, itor->first) || 
         itor.where->next == end().where)
         break;
   }
   node* insertNode;
   //Insert in empty list
   if(begin() == end()){
      insertNode = new node(begin().where, end().where, pair);
      begin().where->next = insertNode;   

   //Insert as last item
   } else if(itor.where->next == end().where && 
             !less(pair.first, itor->first)){  
      insertNode = new node(end().where, itor.where, pair);
      itor.where->next = insertNode;

   //Insert in between two nodes
   } else {                               
      insertNode = new node(itor.where, itor.where->prev, pair);
      itor.where->prev->next = insertNode;
      itor.where->prev = insertNode;
   }
   return iterator(insertNode);
}

//
// listmap::find(const key_type&)
//
/* a == b iff !(a > b) and !(b < a)
 * Otherwise return the end() iterator.
 */
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::find (const key_type& that){
   Less less;
   iterator itor;
   for(itor = begin(); itor != end(); ++itor)
      if(!less(itor->first, that) && !less(that, itor->first))
         return itor;
   return end(); 
}

//
// iterator listmap::erase (iterator position)
//
/* Assumes the iterator is a valid iterator 
 * Throws an invalid argument exception when the end() iterator is
 * specified
 */
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator
listmap<Key,Value,Less>::erase (iterator position) {
   TRACE ('l', &*position);
   if(position == end())
      throw invalid_argument(begin() == end() ? "Empty List." 
      : "Unable to find Key.");
   node* del = position.where;
   iterator nextItor = iterator(del->next);
   del->prev->next = del->next;
   del->next->prev = (nextItor == end()) ? end().where : del->prev;
   delete del;
   return nextItor;
}


//
/////////////////////////////////////////////////////////////////
// Operations on listmap::iterator.
/////////////////////////////////////////////////////////////////
//

//
// listmap::value_type& listmap::iterator::operator*()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type&
listmap<Key,Value,Less>::iterator::operator*() {
   TRACE ('l', where);
   return where->value;
}

//
// listmap::value_type* listmap::iterator::operator->()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::value_type*
listmap<Key,Value,Less>::iterator::operator->() {
   TRACE ('l', where);
   return &(where->value);
}

//
// listmap::iterator& listmap::iterator::operator++()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator++() {
   TRACE ('l', where);
   where = where->next;
   return *this;
}

//
// listmap::iterator& listmap::iterator::operator--()
//
template <typename Key, typename Value, class Less>
typename listmap<Key,Value,Less>::iterator&
listmap<Key,Value,Less>::iterator::operator--() {
   TRACE ('l', where);
   where = where->prev;
   return *this;
}


//
// bool listmap::iterator::operator== (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator==
            (const iterator& that) const {
   return this->where == that.where;
}

//
// bool listmap::iterator::operator!= (const iterator&)
//
template <typename Key, typename Value, class Less>
inline bool listmap<Key,Value,Less>::iterator::operator!=
            (const iterator& that) const {
   return this->where != that.where;
}

