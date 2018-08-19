/*******************************************************************************
* Project:        C-Compass 1.0.0
* File:           MapNode.c
* Purpose:        Implementation of C-ANSI class MapNode
* Author:         Hugo Saporetti Junior
* Creation date:  Sep 1, 2009
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
#include "String.h"
#include "MapNode.h"
/* --------------------------------------------------------------------------*/

/**
 * Function: MapNode_new
 * @purpose: Constructs a MapNode object
 * @param *_class : A pointer the this class descriptor
 * @param *parameters : The parameter list for this constructor
 * @return void * : A pointer to the instance of MapNode newly created
 */
void * MapNode_new(void *_class, va_list *parameters) {
	MapNode *this = _class;
	assert(this);
	char *key = va_arg(*parameters, char *);
	assert(key);

	this->m_key = String(key);
  this->m_id = hashCode(this->m_key);
  this->m_left = null;
  this->m_right = null;
  this->m_value = null;

  return this;
}


/**
 * Function: MapNode_delete
 * @purpose: Destructs a MapNode object
 * @param *this : A pointer the object to be destroyed
 * @return void * : A pointer to null if the object was successfully deleted
 */
void MapNode_delete(void *_this) {
	MapNode *this = _this;
	assert(this);
	delete(this->m_key);
	if (this->m_value) {
	  delete(this->m_value);
	}
}


/**
 * Function: MapNode_clone
 * @purpose: Creates and returns a copy of a MapNode object
 * @param *_this : A pointer the object to be cloned
 * @return void * : A pointer to the cloned object
 */
void * MapNode_clone(const void *_this) {
	const MapNode *this = _this;
	MapNode *cloned = MapNode(this->m_key);
	cloned->m_left = this->m_left;
	cloned->m_parent = this->m_parent;
	cloned->m_right = this->m_right;
	cloned->m_value = clone(this->m_value);

  return cloned;
}


/**
 * Function: MapNode_equals
 * @purpose: Indicates whether some other object is "equal to" another
 * @param *_this : A pointer the object to be compared
 * @param *_other :The reference object with which to compare
 * @return int : true if this object is the same as the obj argument; false otherwise
 */
boolean MapNode_equals(const void *_this, const void *_other) {
  const MapNode *this = _this;
  const MapNode *other = _other;
  if (this == other) {
    return true;
  }
  if (!other || other->d_class != d_MapNode) {
    return false;
  }

  return equals(this->m_key, other->m_key);
}


/**
 * Function: MapNode_getClass
 * @purpose: TODO Comment
 * @param *_this : A pointer the object to get the class descriptor
 * @return
 * - Class * : the Class descriptor for this class
 */
Class * MapNode_getClass(const void * _this) {
  const MapNode *this = _this;

	return this->d_class;
}


/**
 * Function: MapNode_sizeOf
 * @purpose: Returns the m_size in bytes of the given class
 * @param *_this : The reference object
 * @return return_type: the m_size in bytes of the given class
 *
 * Note:
 * May return a different m_value from sizeof function
 */
size_t MapNode_sizeOf(const void *_this) {

  return sizeof(MapNode);
}


/**
 * Function: MapNode_toString
 * @purpose: Returns a cstring representation of the object
 * @param *_this : The reference object
 * @return char * : A pointer to the string representation of the object
 */
char * MapNode_toString(const void *_this) {
  const MapNode *this = _this;

  return toString(this->m_key);
}

ClassDescriptor(MapNode);

/* See MapNode.h for documentation about the functions below */

/* TODO Implement all class methods from here */
