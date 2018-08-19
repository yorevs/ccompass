/*******************************************************************************
* Project:        C-Compass
* File:           Class.c
* Purpose:        Implement functions to provide a "Class" like struct
* Author:         Hugo Saporetti Junior
* Creation date:  Jul 29, 2008
* ANSI:           yes
* ------------------------------------------------------------------------------
* Developed by: Hugo Saporetti Junior
* Access: https://github.com/yorevs/ccompass
* Copyright (C) 2018 Hugo Saporetti Junior
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

/* Includes ----------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Class.h"
/* -------------------------------------------------------------------------- */

/* See Class.h for documentation about the functions below */

/** @See Class.h:__*/
void * __new(const void *d_class, uint16 line, const uint16 *file, ...) {
  const Class *class = d_class;
#if ! defined NDEBUG
  void *pClassObj = System_alloc(class->m_size, line, file);
#else
  void *pClassObj = malloc(class->m_size);
#endif
  *(const Class **) pClassObj = class;
  if (class->__ctor) {
    va_list args;
    va_start(args, file);
    class->__ctor(pClassObj, & args);
    va_end(args);
  }
  else {
    d_Object->__ctor(pClassObj, null);
  }

  return pClassObj;
}

/** @See Class.h:__delete */
void __delete(void *this, uint16 line, const uint16 *file) {
  const Class **d_this = this;
  if (this && *d_this) {
    if ((*d_this)->__dtor) {
      (*d_this)->__dtor(this);
    }
    else {
      d_Object->__dtor(this);
    }
  }
#if ! defined NDEBUG
  System_free(this, line, file);
#else
  free(this);
#endif
  this = null;
}

/** @See Class.h:__clone */
void * __clone(const void *_this) {
  const Class * const *this = _this;
  void *result = null;
  if (this && *this) {
    if ((*this)->__clone) {
      result = (*this)->__clone(_this);
    }
    else {
      result = d_Object->__clone(_this);
    }
  }

  return result;
}

/** @See Class.h:__equals */
boolean __equals(const void *_this, const void *_other) {
  const Class * const *this = _this;
  const Class * const *other = _other;
  boolean result = false;
  if (this && *this && other && *other) {
    if ((*this)->__equals && (*other)->__equals) {
      result = (*this)->__equals(_this, _other);
    }
  }

  return result;
}

/** @See Class.h:__getClass */
Class * __getClass(const void * _this) {
  const Class * const *this = _this;
  Class *result = null;
  if (this && *this) {
    if ((*this)->__getClass) {
      result = (*this)->__getClass(_this);
    }
    else {
      result = d_Object->__getClass(_this);
    }
  }

  return result;
}

/** @See Class.h:__sizeOf */
size_t __sizeOf(const void *_this) {
  const Class *const *this = _this;
  size_t result = 0;
  if (this && *this) {
    if ((*this)->__sizeOf) {
      result = (*this)->__sizeOf(_this);
    }
    else {
      result = d_Object->__sizeOf(_this);
    }
  }

  return result;
}

/** @See Class.h:__toString */
char * __toString(const void *_this) {
  const Class * const *this = _this;
  char *result = null;
  if (this && *this) {
    if ((*this)->__toString) {
      result = (*this)->__toString(_this);
    }
    else {
      result = d_Object->__toString(_this);
    }
  }

  return result;
}


/*****************************************************************************
 * Class Object
 ****************************************************************************/


/**
 * Function: Object_new
 * @purpose: Constructs a Object object
 * @param *_class : A pointer the this class descriptor
 * @param *parameters : The parameter list for this constructor
 * @return void * : A pointer to the instance of Object newly created
 */
void * Object_new(void *_class, va_list *parameters) {
  Object *class = _class;
  assert(class);

  return class;
}


/**
 * Function: Object_delete
 * @purpose: Destructs a Object object
 * @param *this : A pointer the object to be destroyed
 * @return void * : A pointer to null if the object was successfully deleted
 */
void Object_delete(void *_this) {
  Object *this = _this;
  assert(this);
}


/**
 * Function: Object_clone
 * @purpose: Creates and returns a copy of a Object object
 * @param *_this : A pointer the object to be cloned
 * @return void * : A pointer to the cloned object
 */
void * Object_clone(const void *_this) {

  return Object();
}


/**
 * Function: Object_equals
 * @purpose: Indicates whether some other object is "equal to" another
 * @param *_this : A pointer the object to be compared
 * @param *_other :The reference object with which to compare
 * @return boolean : true if this object is the same as the obj argument; false
 * otherwise
 */
boolean Object_equals(const void *_this, const void *_other) {
  const Object *this = _this;
  const Object *other = _other;
  if (this == other) {
    return true;
  }
  if (!other || other->d_class != d_Object) {
    return false;
  }

  return (hashCode(this) == hashCode(other));
}


/**
 * Function: Object_getClass
 * @purpose: TODO Comment
 * @param *_this : A pointer the object to get the class descriptor
 * @return
 * - Class * : TODO Comment
 */
Class * Object_getClass(const void * _this) {
  const Object *this = _this;

  return this->d_class;
}


/**
 * Function: Object_sizeOf
 * @purpose: Returns the m_size in bytes of the given class
 * @param this : TODO Param description
 * @return return_type: TODO Describe return m_type here
 */
size_t Object_sizeOf(const void *_this) {

  return sizeof(Object);
}


char * Object_toString(const void *_this) {
  const Object *this = _this;

  return (char*)this->d_class->m_name/* the string representing the class */;
}

ClassDescriptor(Object);

/* See Object.h for documentation about the functions below */


/** @See Class.h:Object_scrambleHash */
uint32 Object_scrambleHash(uint32 hash) {
#ifdef __HAS_LONG_LONG
  hash = (~hash) + (hash << 21);
  hash = hash ^ (hash >> 24);
  hash = (hash + (hash << 3)) + (hash << 8);
  hash = hash ^ (hash >> 14);
  hash = (hash + (hash << 2)) + (hash << 4);
  hash = hash ^ (hash >> 28);
  hash = hash + (hash << 31);
#else
  hash = (hash + 0x7ed55d16) + (hash << 12);
  hash = (hash ^ 0xc761c23c) ^ (hash >> 19);
  hash = (hash + 0x165667b1) + (hash << 5);
  hash = (hash + 0xd3a2646c) ^ (hash << 9);
  hash = (hash + 0xfd7046c5) + (hash << 3);
  hash = (hash ^ 0xb55a4f09) ^ (hash >> 16);
#endif

  return hash;
}

/** @See Class.h:Object_hashCode */
uint32 Object_hashCode(const void *this) {
  uint32 uniqueId = 2654435761LU;
  char key[11] = {};
  uint16 keyLen = 0;
  uint16 i = 0;
  sprintf(key, "%p", this);
  char pKey[sizeof(uint32) + 1];
  keyLen = strlen(key);
  while(i < keyLen) {
    strncpy(pKey, key+i, sizeof(uint32));
    pKey[sizeof(uint32)] = '\x0';
    memset(pKey+strlen(pKey), 0, sizeof(uint32)-strlen(pKey));
    uniqueId ^= *((uint32*)pKey);
    uniqueId = Object_scrambleHash(uniqueId);
    i += sizeof(uint32);
  }

  return uniqueId & 0x7FFFFFFF;
}

