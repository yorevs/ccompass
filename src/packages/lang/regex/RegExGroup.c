/*******************************************************************************
* Project:        CRSwE_Library_V_1_1_beta
* File:           RegExGroup.c
* Purpose:        Implementation of C-ANSI class RegExGroup
* Author:         Hugo Saporetti Junior
* Creation date:  Sep 14, 2009
* ANSI:           yes
* ------------------------------------------------------------------------------
* Developed by: ICC-SWE
* Access: http://crswe.inatel.br/iccforum
* Copyright (C) 2009 Inatel Competence Center - Software Development/CRSwE.
*
* This file is part of CRSwE Library.
*
* CRSwE Library is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* CRSwE Library is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with CRSwE Library. If not, see <http://www.gnu.org/licenses />.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <assert.h>

#include "CRSwE.h"
#include "RegExGroup.h"
/* --------------------------------------------------------------------------*/

/**
 * Function: RegExGroup_new
 * @purpose: Constructs a RegExGroup object
 * @param *_class : A pointer the this class descriptor
 * @param *parameters : The parameter list for this constructor
 * @return void * : A pointer to the instance of RegExGroup newly created
 */
void * RegExGroup_new(void *_class, va_list *parameters) {
	RegExGroup *this = _class;
	assert(this);
	this->m_startState = NFAState(null);
	System.out.printDebug(DBG_DEBUG, "[RegExGroup] Created root state: %d", hashCode(this->m_startState));
	assert(this->m_startState);
	this->m_lastState = this->m_startState;

  return this;
}


/**
 * Function: RegExGroup_delete
 * @purpose: Destructs a RegExGroup object
 * @param *this : A pointer the object to be destroyed
 * @return void * : A pointer to null if the object was successfully deleted
 */
void RegExGroup_delete(void *_this) {
	RegExGroup *this = _this;
	NFAState *nextState = this->m_lastState;
	NFAState *prevState = nextState->m_prevState;
	assert(this);
	while(nextState) {
	  delete(nextState);
	  nextState = prevState;
	  prevState = (nextState ? nextState->m_prevState : null);
	}
}


/**
 * Function: RegExGroup_clone
 * @purpose: Creates and returns a copy of a RegExGroup object
 * @param *_this : A pointer the object to be cloned
 * @return void * : A pointer to the cloned object
 */
void * RegExGroup_clone(const void *_this) {
	const RegExGroup *this = _this;
  RegExGroup *cloned = RegExGroup();
  cloned->m_startState = this->m_startState;
  cloned->m_lastState = this->m_lastState;

  return cloned;
}


/**
 * Function: RegExGroup_equals
 * @purpose: Indicates whether some other object is "equal to" another
 * @param *_this : A pointer the object to be compared
 * @param *_other :The reference object with which to compare
 * @return int : true if this object is the same as the obj argument; false otherwise
 */
boolean RegExGroup_equals(const void *_this, const void *_other) {
  const RegExGroup *this = _this;
  const RegExGroup *other = _other;
  if (this == other) {
    return true;
  }
  if (!other || other->d_class != d_RegExGroup) {
    return false;
  }

  return ((this->m_startState == other->m_startState) && (this->m_lastState
      == other->m_lastState));
}


/**
 * Function: RegExGroup_getClass
 * @purpose: TODO Comment
 * @param *_this : A pointer the object to get the class descriptor
 * @return
 * - Class * : the Class descriptor for this class
 */
Class * RegExGroup_getClass(const void * _this) {
  const RegExGroup *this = _this;

	return this->d_class;
}


/**
 * Function: RegExGroup_sizeOf
 * @purpose: Returns the size in bytes of the given class
 * @param *_this : The reference object
 * @return return_type: the size in bytes of the given class
 *
 * Note:
 * May return a different value from sizeof function
 */
size_t RegExGroup_sizeOf(const void *_this) {

  return sizeof(RegExGroup);
}


/**
 * Function: RegExGroup_toString
 * @purpose: Returns a cstring representation of the object
 * @param *_this : The reference object
 * @return char * : A pointer to the string representation of the object
 */
char * RegExGroup_toString(const void *_this) {
  const RegExGroup *this = _this;
	/*TODO Implement how is the string representing this class*/

  return (char*)this->d_class->m_name/* the string representing the class */;
}

ClassDescriptor(RegExGroup);

/* See RegExGroup.h for documentation about the functions below */

/*TODO Implement all class methods from here*/
