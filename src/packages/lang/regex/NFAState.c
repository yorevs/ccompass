/*******************************************************************************
* Project:        C-Compass 1.0.0
* File:           NFAState.c
* Purpose:        Implementation of C-ANSI class NFAState
* Author:         Hugo Saporetti Junior
* Creation date:  Sep 9, 2009
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

#include "Compass.h"
#include "NFAState.h"
/* --------------------------------------------------------------------------*/

/**
 * Function: NFAState_new
 * @purpose: Constructs a NFAState object
 * @param *_class : A pointer the this class descriptor
 * @param *parameters : The parameter list for this constructor
 * @return void * : A pointer to the instance of NFAState newly created
 */
void * NFAState_new(void *_class, va_list *parameters) {
	NFAState *this = _class;
	assert(this);
	this->m_prevState = (NFAState*) va_arg(*parameters, NFAState *);
  this->m_outEntries = ArrayList();
  this->m_isFinal = false;
  this->m_hasWildcard = false;
  assert(this->m_outEntries);
  System.out.printDebug(DBG_DEBUG, "[NFAState] Created a STATE: %d", hashCode(this));

  return this;
}


/**
 * Function: NFAState_delete
 * @purpose: Destructs a NFAState object
 * @param *this : A pointer the object to be destroyed
 * @return void * : A pointer to null if the object was successfully deleted
 */
void NFAState_delete(void *_this) {
	NFAState *this = _this;
	assert(this);
	delete(this->m_outEntries);
}


/**
 * Function: NFAState_clone
 * @purpose: Creates and returns a copy of a NFAState object
 * @param *_this : A pointer the object to be cloned
 * @return void * : A pointer to the cloned object
 */
void * NFAState_clone(const void *_this) {
	const NFAState *this = _this;
  NFAState *cloned = NFAState(this->m_in);
  cloned->m_outEntries = this->m_outEntries;
  cloned->m_prevState = this->m_prevState;
  cloned->m_isFinal = this->m_isFinal;
  cloned->m_hasWildcard = this->m_hasWildcard;

  return cloned;
}


/**
 * Function: NFAState_equals
 * @purpose: Indicates whether some other object is "equal to" another
 * @param *_this : A pointer the object to be compared
 * @param *_other :The reference object with which to compare
 * @return int : true if this object is the same as the obj argument; false otherwise
 */
boolean NFAState_equals(const void *_this, const void *_other) {
  const NFAState *this = _this;
  const NFAState *other = _other;
  if (this == other) {
    return true;
  }
  if (!other || other->d_class != d_NFAState) {
    return false;
  }

  return (this->m_outEntries == other->m_outEntries && this->m_in
      == other->m_in && this->m_prevState == other->m_prevState &&
      this->m_hasWildcard == other->m_hasWildcard);
}


/**
 * Function: NFAState_getClass
 * @purpose: TODO Comment
 * @param *_this : A pointer the object to get the class descriptor
 * @return
 * - Class * : the Class descriptor for this class
 */
Class * NFAState_getClass(const void * _this) {
  const NFAState *this = _this;

	return this->d_class;
}


/**
 * Function: NFAState_sizeOf
 * @purpose: Returns the size in bytes of the given class
 * @param *_this : The reference object
 * @return return_type: the size in bytes of the given class
 *
 * Note:
 * May return a different value from sizeof function
 */
size_t NFAState_sizeOf(const void *_this) {

  return sizeof(NFAState);
}


/**
 * Function: NFAState_toString
 * @purpose: Returns a cstring representation of the object
 * @param *_this : The reference object
 * @return char * : A pointer to the string representation of the object
 */
char * NFAState_toString(const void *_this) {
  const NFAState *this = _this;

  return (char*)this->d_class->m_name;
}

ClassDescriptor(NFAState);

/* See NFAState.h for documentation about the functions below */

