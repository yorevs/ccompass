/*******************************************************************************
* Project:        C-Compass 1.0.0
* File:           ListEntry.c
* Purpose:        Implementation of C-ANSI class ListEntry
* Author:         Hugo Saporetti Junior
* Creation date:  Aug 28, 2009
* ANSI:           yes
* ------------------------------------------------------------------------------
* Developed by: Hugo Saporetti Junior
* Access: https://github.com/yorevs/ccompass
* Copyright (C) 2018 Hugo Saporetti Junior.
*
* This file is part of C-Compass Library.
*
* C-Compass Library is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* C-Compass Library is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with C-Compass Library. If not, see <http://www.gnu.org/licenses />.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <assert.h>

#include "CRSwE.h"
#include "ListEntry.h"
/* --------------------------------------------------------------------------*/

/**
 * Function: ListEntry_new
 * @purpose: Constructs a ListEntry object
 * @param *_class : A pointer the this class descriptor
 * @param *parameters : The parameter list for this constructor
 * @return void * : A pointer to the instance of ListEntry newly created
 */
void * ListEntry_new(void *_class, va_list *parameters) {
	ListEntry *this = _class;
	void *elementData = va_arg(*parameters, void *);
	assert(this);
  assert(elementData);

  this->m_elementData = elementData;
  this->m_next = null;
  this->m_previous = null;

  return this;
}


/**
 * Function: ListEntry_delete
 * @purpose: Destructs a ListEntry object
 * @param *this : A pointer the object to be destroyed
 * @return void * : A pointer to null if the object was successfully deleted
 */
void ListEntry_delete(void *_this) {
	ListEntry *this = _this;
	assert(this);
	if (this->m_elementData) {
	  delete(this->m_elementData);
	}
}


/**
 * Function: ListEntry_clone
 * @purpose: Creates and returns a copy of a ListEntry object
 * @param *_this : A pointer the object to be cloned
 * @return void * : A pointer to the cloned object
 */
void * ListEntry_clone(const void *_this) {
	const ListEntry *this = _this;
	ListEntry *cloned = ListEntry(clone(this->m_elementData));
	cloned->m_next = this->m_next;
	cloned->m_previous = this->m_previous;

  return cloned;
}


/**
 * Function: ListEntry_equals
 * @purpose: Indicates whether some other object is "equal to" another
 * @param *_this : A pointer the object to be compared
 * @param *_other :The reference object with which to compare
 * @return int : true if this object is the same as the obj argument; false otherwise
 */
boolean ListEntry_equals(const void *_this, const void *_other) {
  const ListEntry *this = _this;
  const ListEntry *other = _other;
  if (this == other) {
    return true;
  }
  if (!other || other->d_class != d_ListEntry) {
    return false;
  }

  return equals(this->m_elementData, other->m_elementData);
}


/**
 * Function: ListEntry_getClass
 * @purpose: TODO Comment
 * @param *_this : A pointer the object to get the class descriptor
 * @return
 * - Class * : the Class descriptor for this class
 */
Class * ListEntry_getClass(const void * _this) {
  const ListEntry *this = _this;
	return this->d_class;
}


/**
 * Function: ListEntry_sizeOf
 * @purpose: Returns the m_size in bytes of the given class
 * @param *_this : The reference object
 * @return return_type: the m_size in bytes of the given class
 *
 * Note:
 * May return a different m_value from sizeof function
 */
size_t ListEntry_sizeOf(const void *_this) {
  return sizeof(ListEntry);
}


/**
 * Function: ListEntry_toString
 * @purpose: Returns a cstring representation of the object
 * @param *_this : The reference object
 * @return char * : A pointer to the string representation of the object
 */
char * ListEntry_toString(const void *_this) {
  const ListEntry *this = _this;

  return toString(this->m_elementData);
}

ClassDescriptor(ListEntry);

/* See ListEntry.h for documentation about the functions below */

/* TODO Implement all class methods from here */
