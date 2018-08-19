/*******************************************************************************
* Project:        C-Compass 1.0.0
* File:           HashMap.h
* Purpose:        Public prototypes for HashMap C-ANSI class
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
#ifndef HASHMAP_H_
#define HASHMAP_H_

/* Includes ----------------------------------------------------------------- */
#include "Compass.h"
#include "MapNode.h"
/* -------------------------------------------------------------------------- */

/* Usefull Macro to shorcut the constructor method */
#define HashMap()		_NEW_(HashMap, null)

/** @purpose Binary tree searching modes:
 *
 * - PRE ORDER: Visit the node when following the left subtree, going to the
 * right subtree and then visiting the root node
 * - POST ORDER: Visit the root node, follow the left subtree and then the
 * right subtree visiting each node once
 * - IN ORDER: Follow in-order the left subtree, visit the node, following in
 * order the right subtree and then visiting the root node
 *  */
enum HashMapSearchModes {
  HM_PRE_ORDER  = 1,
  HM_IN_ORDER   = 2,
  HM_POST_ORDER = 3
};


/**
 * @purpose Definition of the HashMap C-ANSI class
 */
class (HashMap,
  MapNode *m_root;        /** the first map entry of this HashMap (m_root) */
  size_t m_size;          /** the number of key-m_value mappings in this map */

  /* Public Methods */
  boolean (*containsKey)(HashMap *this, const char *m_key);
  int16 (*clear)(HashMap *this);
  int16 (*put)(HashMap *this, const char *m_key, void *m_value);
  void * (*get)(HashMap *this, const char *m_key);
  int16 (*remove)(HashMap *this, const char *m_key);
  size_t (*size)(HashMap *this);
  boolean (*isEmpty)(HashMap *this);
);


/* Declare all public prototypes here */

/**
 * Function: HashMap_cloneNodes.
 * @purpose Recursively clone a node and it's children, from left to right. The
 * ID is not cloned.
 * @param *cloned : The HashMap to receive the cloned nodes.
 * @param *node : The MapNode to be cloned.
 * @return none
 */
extern void HashMap_cloneNodes( HashMap *cloned, MapNode *node);


/**
 * Function: HashMap_searchKey.
 * @purpose Recursively pre-order searches for a key in the HashMap
 * @param *node : The node to be searched
 * @param *key : The key associated to the node
 * @param *result : The output parameter to assign the return m_value of this
 * function
 * @return none
 */
extern void HashMap_searchKey(MapNode *node, const char *key, boolean *result);


/**
 * Function: HashMap_containsKey.
 * @purpose Returns true if this map contains a mapping for the specified key
 * @param *this : The HashMap to be tested
 * @param *key : The testing key
 * @return
 *  - boolean : true if the key exists is being used by the HashMap, false
 * otherwise
 */
extern boolean HashMap_containsKey(HashMap *this, const char *key);


/**
 * Function: HashMap_clear.
 * @purpose Removes all mappings from this map
 * @param *this : The HashMap to be cleared
 * @return
 * - int16 : SUCCESS or ERROR if any occurs
 */
extern int16 HashMap_clear(HashMap *this);


/**
 * Function: HashMap_clearNodes.
 * @purpose Recursively clear a node and it's children, from left to right.
 * @param *node : The MapNode to be cleared.
 * @param *result : The number of nodes cleared from this one.
 * @return none
 */
extern void HashMap_clearNodes(MapNode *node, int16 *result);


/**
 * Function: HashMap_put.
 * @purpose Associates the specified m_value with the specified key in this map.
 * If the map previously contained a mapping for the key, the old m_value is
 * replaced.
 * @param *this : The HashMap in which the node will be mapped
 * @param *key : A key to associate the node
 * @param *m_value : The m_value of the corresponding node
 * @return
 *  - int16 : SUCCESS or ERROR if an occurs during the add match
 *
 */
extern int16 HashMap_put(HashMap *this, const char *key, void *value);


/**
 * Function: HashMap_searchNode
 * @purpose Recursively pre-order searches for a node in the HashMap
 * @param *node : The node to be searched
 * @param *key : The key representing the node
 * @paran searchMode : TODO
 * @return
 *  - MapNode * : The node if it exists, null otherwise
 */
extern MapNode * HashMap_searchNode(MapNode *node, const char *key, uint16 searchMode);


/**
 * Function: HashMap_getNode
 * @purpose Retrieves a MapNode from the HashMap bound to the given key
 * @param *this : The HashMap in which the element will be retrieved
 * @param *key : The key bound to this node
 * @return
 *  - MapNode * : A pointer to the node relative to the key or null if no
 * node was found or any error occur during the get match
 */
extern MapNode * HashMap_getNode(HashMap *this, const char *key);


/**
 * Function: HashMap_get
 * @purpose Returns the m_value to which the specified key is mapped in this
 * identity hash map, or null if the map contains no mapping for this key
 * @param *this : The HashMap in which the m_value will be searched
 * @param *key : The key associated to this node
 * @return
 *  - void * : A pointer to the m_value relative to a node mapped to the specified
 * key or null if no node was found or any error occur during the get match
 */
extern void * HashMap_get(HashMap *this, const char *key);


/**
 * Function: HashMap_remove
 * @purpose Removes the mapping for this key from this map if present
 * @param *this : The HashMap from where the element will be removed
 * @param *key : The key associated to this element
 * @return
 *  - int16 : SUCCESS if no error occur or ERROR otherwise
 */
extern int16 HashMap_remove(HashMap *this, const char *key);


/**
 * Function: HashMap_size
 * @purpose Returns the number of key-m_value mappings in this map
 * @param *this : The HashMap the elements were mapped
 * @return
 *  - size_t : The current m_size of the HashMap
 */
extern size_t HashMap_size(HashMap *this);


/**
 * Function: HashMap_isEmpty
 * @purpose Returns true if this map contains no key-m_value mappings
 * @param *this : The operating HashMap
 * @return
 *  - boolean : True if the map is empty, false otherwise
 */
extern boolean HashMap_isEmpty(HashMap *this);


#endif /* HASHMAP_H_ */

