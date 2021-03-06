/*******************************************************************************
* Project:        C-Compass 1.0.0
* File:           MapNode.h
* Purpose:        Public prototypes for MapNode C-ANSI class
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
#ifndef MAPNODE_H_
#define MAPNODE_H_

/* Includes ----------------------------------------------------------------- */
#include "Compass.h"
#include "String.h"
/* -------------------------------------------------------------------------- */

/* Usefull Macro to shorcut the constructor method */
#define MapNode(key)		_NEW_(MapNode, key)

/**
 * @purpose Definition of the MapNode C-ANSI class
 */
class (MapNode,
  String *m_key;      /** the string which will identify this map node */
  void *m_value;      /** the value associated with the stored object */
  MapNode *m_parent;  /** the parent of this map node; null if top node */
  MapNode *m_left;    /** the map entry with ID <= this entry's ID (left side)*/
  MapNode *m_right;   /** the map entry with ID > this entry's ID (right side)*/
  uint32 m_id;        /** this map node hashCode (ID) */
);

/* Declare all public prototypes here */

#endif /* MAPNODE_H_ */

