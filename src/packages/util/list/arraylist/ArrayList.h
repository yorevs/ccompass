/*******************************************************************************
* Project:        CRSwE_Library_V_1_1_beta
* File:           ArrayList.h
* Purpose:        Public prototypes for ArrayList C-ANSI class
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
#ifndef ARRAYLIST_H_
#define ARRAYLIST_H_

/* Includes ----------------------------------------------------------------- */
#include "CRSwE.h"
#include "ListEntry.h"
/* -------------------------------------------------------------------------- */

/* Usefull Macro to shorcut the constructor method */
#define ArrayList()		_NEW_(ArrayList, null)


/**
 * @purpose Definition of the ArrayList C-ANSI class
 */
class (ArrayList,
  ListEntry *m_head;  /** The first object in the list */
  ListEntry *m_tail;  /** The last object in the list */
  size_t m_size;      /** The number of elements in this list */

  /* Public Methods */
  int16 (*clear)(ArrayList *this);
  int16 (*indexOf)(ArrayList *this, void *element);
  boolean (*contains)(ArrayList *this, void *element);
  int16 (*addElement)(ArrayList *this, void *element);
  void * (*elementAt)(ArrayList *this, uint16 index);
  int16 (*removeElementAt)(ArrayList *this, uint16 index);
  int16 (*insertElementAt)(ArrayList *this, void *element, uint16 index);
  int16 (*setElementAt)(ArrayList *this, void *element, uint16 index);
  int16 (*size)(ArrayList *this);
);

/* Declare all public prototypes here */

/**
 * Function: ArrayList_clear
 * @purpose Removes all of the elements from this list
 * @param *this : The list to be empty
 * @return
 *  - int16 : Number of elements removed or ERROR if any error occurs
 */
extern int16 ArrayList_clear(ArrayList *this);


/**
 * Function: ArrayList_indexOf
 * @purpose Searches for the first occurence of the given element, testing for
 * equality using a comparison between the two pointer addresses
 * @param *this : The list the elements are stored
 * @param *element : The searching element
 * @return
 *  - int16 : The current index of the element or ERROR if no element was found
 * or any error occurs
 *
 * Notes:
 *
 *  The element must have the same memory address to get a positve m_value
 */
extern int16 ArrayList_indexOf(ArrayList *this, void *element);


/**
 * Function: ArrayList_contains
 * @purpose Returns true if this list contains the specified element
 * @param *this : The list the elements are stored
 * @param *element : The testing element
 * @return
 *  - true if the element is present , false otherwise
 */
extern boolean ArrayList_contains(ArrayList *this, void *element);


/**
 * Function: ArrayList_addElement
 * @purpose Appends the specified element to the end of this list
 * @param *this : The list to have the element appended
 * @param *element : The element to be added into the list
 * @return
 *  - int16 : SUCCESS or ERROR if any occurs
 */
extern int16 ArrayList_addElement(ArrayList *this, void *element);


/**
 * Function: ArrayList_removeElementAt
 * @purpose Removes the element at the specified position in this list
 * @param *this : The list to have the element removed
 * @param index : The index of the removing element
 * @return
 *  - int16 : SUCCESS or ERROR if any occurs
 */
extern int16 ArrayList_removeElementAt(ArrayList *this, uint16 index);

/**
 * Function: ArrayList_insertElementAt
 * @purpose Inserts the specified element at the specified position in this list
 * @param *this : The list to have the element inserted
 * @param *element : The element to be inserted into the list
 * @param index : The index of the inserting element
 * @return
 *  - int16 : SUCCESS or ERROR if any occurs
 */
extern int16 ArrayList_insertElementAt(ArrayList *this, void *element, uint16 index);


/**
 * Function: ArrayList_elementAt
 * @purpose Returns the element at the specified position in this list
 * @param *this : The list to have the element retrieved
 * @param index : The index of the retrieving element
 * @return
 *  - void * : A reference to the retrieved element or null if no corresponding
 * element was found at this index or an error occurs
 */
extern void * ArrayList_elementAt(ArrayList *this, uint16 index);


/**
 * Function: ArrayList_size
 * @purpose Returns the number of elements in this list
 * @param *this : The list the elements were stored
 * @return
 *  - int16 : The current m_size of the list or ERROR if an any occur during the
 * proccess
 */
extern int16 ArrayList_size(ArrayList *this);


/**
 * Function: ArrayList_setElementAt
 * @purpose Replaces the element at the specified position in this list with the
 * specified element
 * @param *this : The list in which the element will be replaced
 * @param *element : The replacement element
 * @param index : The index of the element to be set
 * @return
 *  - int16 : SUCCESS or ERROR if any occurs
 */
extern int16 ArrayList_setElementAt(ArrayList *this, void *element, uint16 index);


#endif /* ARRAYLIST_H_ */

