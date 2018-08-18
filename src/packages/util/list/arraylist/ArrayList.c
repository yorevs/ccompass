/*******************************************************************************
* Project:        CRSwE_Library_V_1_1_beta
* File:           ArrayList.c
* Purpose:        Implementation of C-ANSI class ArrayList
* Author:         Hugo Saporetti Junior
* Creation date:  Aug 27, 2009
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
#include "ArrayList.h"
/* --------------------------------------------------------------------------*/

/**
 * Function: ArrayList_new
 * @purpose: Constructs a ArrayList object
 * @param *_class : A pointer the this class descriptor
 * @param *parameters : The parameter list for this constructor
 * @return void * : A pointer to the instance of ArrayList newly created
 */
void * ArrayList_new(void *_class, va_list *parameters) {
	ArrayList *this = _class;
	assert(this);

  this->m_size = 0;
  this->m_head = null;
  this->m_tail = null;
  this->clear = ArrayList_clear;
  this->indexOf = ArrayList_indexOf;
  this->contains = ArrayList_contains;
  this->addElement = ArrayList_addElement;
  this->elementAt = ArrayList_elementAt;
  this->removeElementAt = ArrayList_removeElementAt;
  this->insertElementAt = ArrayList_insertElementAt;
  this->setElementAt = ArrayList_setElementAt;
  this->size = ArrayList_size;

  return this;
}


/**
 * Function: ArrayList_delete
 * @purpose: Destructs a ArrayList object
 * @param *this : A pointer the object to be destroyed
 * @return void * : A pointer to null if the object was successfully deleted
 */
void ArrayList_delete(void *_this) {
	ArrayList *this = _this;
	assert(this);
	ArrayList_clear(this);
}


/**
 * Function: ArrayList_clone
 * @purpose: Creates and returns a copy of a ArrayList object
 * @param *_this : A pointer the object to be cloned
 * @return void * : A pointer to the cloned object
 */
void * ArrayList_clone(const void *_this) {
	const ArrayList *this = _this;
	ArrayList *cloned = ArrayList();
	ListEntry *entry = this->m_head;
  while(entry != null) {
    cloned->addElement(cloned, clone(entry->m_elementData));
    entry = entry->m_next;
  }

  return cloned;
}


/**
 * Function: ArrayList_equals
 * @purpose: Indicates whether some other object is "equal to" another
 * @param *_this : A pointer the object to be compared
 * @param *_other :The reference object with which to compare
 * @return int : true if this object is the same as the obj argument; false otherwise
 */
boolean ArrayList_equals(const void *_this, const void *_other) {
  const ArrayList *this = _this;
  const ArrayList *other = _other;
  if (this == other) {
    return true;
  }
  if (!other || other->d_class != d_ArrayList) {
    return false;
  }

  return (this->m_head == other->m_head && this->m_tail == this->m_tail
      && this->m_size == other->m_size);
}


/**
 * Function: ArrayList_getClass
 * @purpose: TODO Comment
 * @param *_this : A pointer the object to get the class descriptor
 * @return
 * - Class * : the Class descriptor for this class
 */
Class * ArrayList_getClass(const void * _this) {
  const ArrayList *this = _this;

	return this->d_class;
}


/**
 * Function: ArrayList_sizeOf
 * @purpose: Returns the m_size in bytes of the given class
 * @param *_this : The reference object
 * @return return_type: the m_size in bytes of the given class
 *
 * Note:
 * May return a different m_value from sizeof function
 */
size_t ArrayList_sizeOf(const void *_this) {

  return sizeof(ArrayList);
}


/**
 * Function: ArrayList_toString
 * @purpose: Returns a cstring representation of the object
 * @param *_this : The reference object
 * @return char * : A pointer to the string representation of the object
 */
char * ArrayList_toString(const void *_this) {
  const ArrayList *this = _this;

  return (char*)this->d_class->m_name/* the string representing the class */;
}

ClassDescriptor(ArrayList);

/**
 * Function: ArrayList_seekEntry
 * @purpose Recursively searhes for a pointer of a list entry at a specified
 * position in this list
 * @param *entry : The seeking vector entry
 * @param index : The index to search for
 * @param *curIndex : The currrent searching index
 * @return
 *  - ListEntry * : A reference to the retrieved entry or null if no entry was
 * found at this index or an error occurs
 */
ListEntry * ArrayList_seekEntry(ListEntry *entry, uint16 index, uint16 *curIndex) {
  ListEntry *foundEntry = null;
  ListEntry *nextInSearch = null;
  if (!entry) {
    return null;
  }
  if (*curIndex == index) {
    return entry;
  }
  if (*curIndex < index) {
    (*curIndex)++;
    nextInSearch = entry->m_next;
  }
  else {
    (*curIndex)--;
    nextInSearch = entry->m_previous;
  }
  if ((foundEntry = ArrayList_seekEntry(nextInSearch, index, curIndex)) != null) {
    return foundEntry;
  }

  return null;
}

/* See ArrayList.h for documentation about the functions below */

int16 ArrayList_clear(ArrayList *this) {
  ListEntry *entry = null;
  uint16 count = 0;
  if (!this) {
    return 0;
  }
  entry = (this->m_tail != null ? this->m_tail->m_previous : null);
  while(entry) {
    delete(this->m_tail);
    this->m_tail = entry;
    entry = entry->m_previous;
    count++;
  }
  if (this->m_tail != null) {
    delete(this->m_tail);
  }
  this->m_head = null;
  this->m_tail = null;
  this->m_size = 0;

  return count;
}


int16 ArrayList_indexOf(ArrayList *this, void *element) {
  uint16 index = 0;
  ListEntry *nextEntry = null;
  if (this == null || element == null) {
    return ERROR;
  }
  nextEntry = this->m_head;
  while(nextEntry != null) {
    if (nextEntry->m_elementData == element) {
      return index;
    }
    index++;
    nextEntry = nextEntry->m_next;
  }

  return ERROR;
}


boolean ArrayList_contains(ArrayList *this, void *element) {
  if (!this || !element) {
    return false;
  }

  return (ArrayList_indexOf(this, element) < 0 ? false : true);
}


int16 ArrayList_addElement(ArrayList *this, void *element) {
  ListEntry *entry = null;
  if (!this || !element ||
        ArrayList_contains(this, element) ||
        (entry = ListEntry(element)) == null
     ) {
    return ERROR;
  }
  this->m_size++;
  if (this->m_head == null) {
    this->m_head = entry;
    this->m_tail = entry;
    return SUCCESS;
  }
  entry->m_previous = this->m_tail;
  this->m_tail->m_next = entry;
  this->m_tail = entry;

  return SUCCESS;
}


int16 ArrayList_removeElementAt(ArrayList *this, uint16 index) {
  ListEntry *entry = null;
  uint16 curIndex = 0;
  if (!this || index > this->m_size ||
      (entry = ArrayList_seekEntry(this->m_head, index, &curIndex)) == null) {
    return ERROR;
  }
  if (entry->m_previous) {
    entry->m_previous->m_next = entry->m_next;
  }
  else {
    this->m_head = entry->m_next;
  }
  if (entry->m_next) {
    entry->m_next->m_previous = entry->m_previous;
  }
  else {
    this->m_tail = entry->m_previous;
  }
  delete(entry);
  this->m_size--;

  return SUCCESS;
}


int16 ArrayList_insertElementAt(ArrayList *this, void *element, uint16 index) {
  uint16 curIndex = 0;
  ListEntry *entry = null;
  ListEntry *curEntry = null;
  if (!this || index > this->m_size || ArrayList_contains(this, element)) {
    return ERROR;
  }
  curEntry = ArrayList_seekEntry(this->m_head, index, &curIndex);
  if (curEntry) {
    if ((entry = ListEntry(element)) == null) {
      return ERROR;
    }
    if (curEntry->m_previous) {
      curEntry->m_previous->m_next = entry;
    }
    else {
      this->m_head = entry;
    }
    entry->m_previous = curEntry->m_previous;
    entry->m_next = curEntry;
    curEntry->m_previous = entry;
    this->m_size++;
  }
  else {
    ArrayList_addElement(this, element);
  }

  return SUCCESS;
}


void * ArrayList_elementAt(ArrayList *this, uint16 index) {
  ListEntry *element = null;
  uint16 curIndex = 0;
  if (!this || index >= this->m_size) {
    return null;
  }
  if (index >= this->m_size/2) {
    curIndex = this->m_size-1;
    element = ArrayList_seekEntry(this->m_tail, index, &curIndex);
  }
  else{
    element = ArrayList_seekEntry(this->m_head, index, &curIndex);
  }

  return (element != null ? element->m_elementData : null);
}


int16 ArrayList_size(ArrayList *this) {
  if (this == null) {
    return ERROR;
  }

  return this->m_size;
}


int16 ArrayList_setElementAt(ArrayList *this, void *element, uint16 index) {
  ListEntry *entry = null;
  uint16 count = 0;
  if (this == null || element == null || this->m_size == 0 || (index+1) > this->m_size ||
     (entry = ArrayList_seekEntry(this->m_head, index, &count)) == null) {
    return ERROR;
  }
  delete(entry->m_elementData);
  entry->m_elementData = element;

  return SUCCESS;
}


