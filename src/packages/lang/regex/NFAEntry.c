/*******************************************************************************
* Project:        C-Compass 1.0.0
* File:           NFAEntry.c
* Purpose:        Implementation of C-ANSI class NFAEntry
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
#include "RegEx.h"
#include "NFAEntry.h"
/* --------------------------------------------------------------------------*/

/**
 * Function: NFAEntry_new
 * @purpose: Constructs a NFAEntry object
 * @param *_class : A pointer the this class descriptor
 * @param *parameters : The parameter list for this constructor
 * @return void * : A pointer to the instance of NFAEntry newly created
 */
void * NFAEntry_new(void *_class, va_list *parameters) {
	NFAEntry *this = _class;
	assert(this);
	this->m_isBypass = va_arg(*parameters, int);
	this->m_in = va_arg(*parameters, void *);
	this->m_out = va_arg(*parameters, void *);
	this->m_codepointStart = va_arg(*parameters, int);
	this->m_codepointEnd = 0;
	this->m_groups = 0;
	this->m_isWildcard = false;
	this->m_isCloned = false;
	this->m_isNegating = false;
	this->m_isLastSubex = true;

	this->match = NFAEntry_match;
	this->posixFnc = null;

  return this;
}


/**
 * Function: NFAEntry_delete
 * @purpose: Destructs a NFAEntry object
 * @param *this : A pointer the object to be destroyed
 * @return void * : A pointer to null if the object was successfully deleted
 */
void NFAEntry_delete(void *_this) {
	NFAEntry *this = _this;
	assert(this);
}


/**
 * Function: NFAEntry_clone
 * @purpose: Creates and returns a copy of a NFAEntry object
 * @param *_this : A pointer the object to be cloned
 * @return void * : A pointer to the cloned object
 */
void * NFAEntry_clone(const void *_this) {
	const NFAEntry *this = _this;
  NFAEntry *cloned = NFAEntry(this->m_isBypass, this->m_in, this->m_out, this->m_codepointStart);
  cloned->m_groups = this->m_groups;
  cloned->m_codepointEnd = this->m_codepointEnd;
  cloned->posixFnc = this->posixFnc;
  cloned->m_isWildcard = this->m_isWildcard;
  cloned->m_isNegating = this->m_isNegating;
  cloned->m_isLastSubex = this->m_isLastSubex;
  cloned->m_isCloned = true;

  return cloned;
}


/**
 * Function: NFAEntry_equals
 * @purpose: Indicates whether some other object is "equal to" another
 * @param *_this : A pointer the object to be compared
 * @param *_other :The reference object with which to compare
 * @return int : true if this object is the same as the obj argument; false otherwise
 */
boolean NFAEntry_equals(const void *_this, const void *_other) {
  const NFAEntry *this = _this;
  const NFAEntry *other = _other;
  if (this == other) {
    return true;
  }
  if (!other || other->d_class != d_NFAEntry) {
    return false;
  }

  return (this->m_codepointStart == other->m_codepointStart && this->m_in
      == other->m_in && this->m_out == other->m_out && this->m_isBypass
      == other->m_isBypass && this->m_codepointEnd == other->m_codepointEnd &&
      this->posixFnc == other->posixFnc && this->m_isCloned == other->m_isCloned &&
      this->m_isWildcard == other->m_isWildcard);
}


/**
 * Function: NFAEntry_getClass
 * @purpose: TODO Comment
 * @param *_this : A pointer the object to get the class descriptor
 * @return
 * - Class * : the Class descriptor for this class
 */
Class * NFAEntry_getClass(const void * _this) {
  const NFAEntry *this = _this;

	return this->d_class;
}


/**
 * Function: NFAEntry_sizeOf
 * @purpose: Returns the size in bytes of the given class
 * @param *_this : The reference object
 * @return return_type: the size in bytes of the given class
 *
 * Note:
 * May return a different value from sizeof function
 */
size_t NFAEntry_sizeOf(const void *_this) {

  return sizeof(NFAEntry);
}


/**
 * Function: NFAEntry_toString
 * @purpose: Returns a cstring representation of the object
 * @param *_this : The reference object
 * @return char * : A pointer to the string representation of the object
 */
char * NFAEntry_toString(const void *_this) {
  const NFAEntry *this = _this;

  return (char*)this->d_class->m_name/* the string representing the class */;
}

ClassDescriptor(NFAEntry);

/* See NFAEntry.h for documentation about the functions below */


boolean NFAEntry_match(const NFAEntry *this, uint16 codepoint) {
  boolean result = false;

  if (this->posixFnc) {
    result = this->posixFnc(codepoint) > 0 ? true: false;
  }
  else {
    if (!this->m_codepointEnd) {

      System.out.printDebug(DBG_DEBUG, "[RegEx] Matching a %s. %sxpected: '%c', Input: '%c'", (this->m_isWildcard ? "WILDCARD" : "single char"),
          (this->m_isNegating ? "Un" : "E"), this->m_codepointStart, codepoint);
      result = (this->m_isBypass || this->m_isWildcard) ?
          true : this->m_isNegating ^ (this->m_codepointStart == codepoint);
    }
    else {
      System.out.printDebug(DBG_DEBUG, "[RegEx] Matching a range. %sxpected: ['0x%.2X'-'0x%.2X'], Input: '0x%.2X'",
          (this->m_isNegating ? "un" : "e"), this->m_codepointStart, this->m_codepointEnd, codepoint);
      result = (this->m_isBypass || this->m_isWildcard) ?
          true : this->m_isNegating ^ (codepoint >= this->m_codepointStart &&
              codepoint <= this->m_codepointEnd);
    }
  }

  return result;
}

