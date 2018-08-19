/*******************************************************************************
* Project:        C-Compass 1.0.0
* File:           ListEntry.h
* Purpose:        Public prototypes for ListEntry C-ANSI class
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
#ifndef LISTENTRY_H_
#define LISTENTRY_H_

/* Includes ----------------------------------------------------------------- */
#include "Compass.h"
/* -------------------------------------------------------------------------- */

/* Usefull Macro to shorcut the constructor method */
#define ListEntry(data)		_NEW_(ListEntry, data)

/**
 * @purpose Definition of the ListEntry C-ANSI class
 */
class (ListEntry,
  ListEntry *m_next;      /** the m_next object in the list */
  ListEntry *m_previous;  /** the m_prevState object in the list */
  void *m_elementData;    /** the data associated to this list entry Object */
);

/* Declare all public prototypes here */

#endif /* LISTENTRY_H_ */

