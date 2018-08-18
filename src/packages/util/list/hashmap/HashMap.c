/*******************************************************************************
 * Project:        CRSwE_Library_V_1_1_beta
 * File:           HashMap.c
 * Purpose:        Implementation of C-ANSI class HashMap
 * Author:         Hugo Saporetti Junior
 * Creation date:  Sep 1, 2009
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
#include <string.h>

#include "CRSwE.h"
#include "HashMap.h"
/* --------------------------------------------------------------------------*/

/**
 * Function: HashMap_new
 * @purpose: Constructs a HashMap object
 * @param *_class : A pointer the this class descriptor
 * @param *parameters : The parameter list for this constructor
 * @return void * : A pointer to the instance of HashMap newly created
 */
void * HashMap_new( void *_class, va_list *parameters ) {
	HashMap *this = _class;
	assert( this );

	this->containsKey = HashMap_containsKey;
	this->clear = HashMap_clear;
	this->put = HashMap_put;
	this->get = HashMap_get;
	this->remove = HashMap_remove;
	this->size = HashMap_size;
	this->isEmpty = HashMap_isEmpty;

	return this;
}

/**
 * Function: HashMap_delete
 * @purpose: Destructs a HashMap object
 * @param *this : A pointer the object to be destroyed
 * @return void * : A pointer to null if the object was successfully deleted
 */
void HashMap_delete( void *_this ) {
	HashMap *this = _this;
	assert( this );
	HashMap_clear( this );
}

/**
 * Function: HashMap_clone
 * @purpose: Creates and returns a copy of a HashMap object
 * @param *_this : A pointer the object to be cloned
 * @return void * : A pointer to the cloned object
 */
void * HashMap_clone( const void *_this ) {
	const HashMap *this = _this;
	HashMap *cloned = HashMap();
	HashMap_cloneNodes(cloned, this->m_root);

	return cloned;
}

/**
 * Function: HashMap_equals
 * @purpose: Indicates whether _this object is "equal to" _other
 * @param *_this : A pointer the object to be compared
 * @param *_other :The reference object with which to compare
 * @return int : true if this object is the same as the obj argument; false otherwise
 */
boolean HashMap_equals( const void *_this, const void *_other ) {
	const HashMap *this = _this;
	const HashMap *other = _other;
	if ( this == other ) {
		return true;
	}
	if ( !other || other->d_class != d_HashMap ) {
		return false;
	}

	return ((this->m_root == other->m_root) && (this->m_size == other->m_size));
}

/**
 * Function: HashMap_getClass
 * @purpose: TODO Comment
 * @param *_this : A pointer the object to get the class descriptor
 * @return
 * - Class * : the Class descriptor for this class
 */
Class * HashMap_getClass( const void * _this ) {
	const HashMap *this = _this;

	return this->d_class;
}

/**
 * Function: HashMap_sizeOf
 * @purpose: Returns the m_size in bytes of the given class
 * @param *_this : The reference object
 * @return return_type: the m_size in bytes of the given class
 *
 * Note:
 * May return a different m_value from sizeof function
 */
size_t HashMap_sizeOf( const void *_this ) {

	return sizeof(HashMap);
}

/**
 * Function: HashMap_toString
 * @purpose: Returns a cstring representation of the object
 * @param *_this : The reference object
 * @return char * : A pointer to the string representation of the object
 */
char * HashMap_toString( const void *_this ) {
	const HashMap *this = _this;

	return toString(this);
}

ClassDescriptor( HashMap );

/* See HashMap.h for documentation about the functions below */

void HashMap_cloneNodes( HashMap *cloned, MapNode *node ) {
	assert(cloned);
	if ( node == null ) {
		return;
	}
	cloned->put( cloned, node->m_key->m_text, clone( node->m_value ) );
	HashMap_cloneNodes( cloned, node->m_left);
	HashMap_cloneNodes( cloned, node->m_right);
}

void HashMap_searchKey( MapNode *node, const char *key, boolean *result ) {
	if ( !node || !key || !result ) {
		return;
	}
	if ( strncmp( node->m_key->m_text, key, strlen( key ) ) == 0 ) {
		*result = true;
	}
	HashMap_searchKey( node->m_left, key, result );
	HashMap_searchKey( node->m_right, key, result );
}

boolean HashMap_containsKey( HashMap *this, const char* key ) {
	MapNode *node = null;
	boolean result = false;
	if ( !this || !key ) {
		return false;
	}
	node = this->m_root;
	HashMap_searchKey( node, key, &result );

	return result;
}

int16 HashMap_clear( HashMap *this ) {
	int16 result = 0;
	if ( this == null ) {
		return ERROR;
	}
	HashMap_clearNodes( this->m_root, &result );

	return result;
}

void HashMap_clearNodes(MapNode * node, int16 *result) {
	if ( !node ) {
		return;
	}
	MapNode *left = node->m_left;
	MapNode *right = node->m_right;
	delete( node );
	(*result)++;
	HashMap_clearNodes( left, result );
	HashMap_clearNodes( right, result );
}

int16 HashMap_put( HashMap *this, const char *key, void *value ) {
	MapNode *newNode = null;
	MapNode *nextNode = null;
	if ( !this || !key || !value ) {
		return ERROR;
	}
	if ( (newNode = HashMap_getNode( this, key )) != null ) {
		delete( newNode->m_value );
		newNode->m_value = value;
		return SUCCESS;
	}
	else {
		newNode = MapNode(key);
		newNode->m_value = value;
	}
	if ( !this->m_root ) {
		this->m_root = newNode;
		this->m_size++;
		return SUCCESS;
	}
	nextNode = this->m_root;
	while( nextNode ) {
		if ( newNode->m_id <= nextNode->m_id ) {
			if ( nextNode->m_left ) {
				nextNode = nextNode->m_left;
				continue;
			}
			break;
		}
		else {
			if ( nextNode->m_right ) {
				nextNode = nextNode->m_right;
				continue;
			}
			break;
		}
	}
	if ( newNode->m_id <= nextNode->m_id ) {
		nextNode->m_left = newNode;
	}
	else {
		nextNode->m_right = newNode;
	}
	newNode->m_parent = nextNode;
	this->m_size++;

	return SUCCESS;
}

MapNode * HashMap_searchNode( MapNode *node, const char *key, uint16 searchMode ) {
	MapNode *retNode = null;
	if ( !node || !key ) {
		return null;
	}
	if ( searchMode == HM_PRE_ORDER ) {
		if ( strncmp( node->m_key->m_text, key, strlen( key ) ) == 0 ) {
			return node;
		}
	}
	if ( (retNode = HashMap_searchNode( node->m_left, key, searchMode )) != null ) {
		return retNode;
	}
	if ( searchMode == HM_IN_ORDER ) {
		if ( strncmp( node->m_key->m_text, key, strlen( key ) ) == 0 ) {
			return node;
		}
	}
	if ( (retNode = HashMap_searchNode( node->m_right, key, searchMode )) != null ) {
		return retNode;
	}
	if ( searchMode == HM_POST_ORDER ) {
		if ( strncmp( node->m_key->m_text, key, strlen( key ) ) == 0 ) {
			return node;
		}
	}

	return null;
}

MapNode * HashMap_getNode( HashMap *this, const char *key ) {
	if ( !this || !key ) {
		return null;
	}

	return HashMap_searchNode( this->m_root, key, HM_IN_ORDER );
}

void * HashMap_get( HashMap *this, const char *key ) {
	MapNode *node = null;
	if ( (node = HashMap_getNode( this, key )) == null ) {
		return null;
	}

	return node->m_value;
}

int16 HashMap_remove( HashMap *this, const char *key ) {
	MapNode *delNode = null;
	MapNode *replNode = null;
	if ( !this || !key ) {
		return ERROR;
	}
	delNode = HashMap_getNode( this, key );
	if ( !delNode ) {
		return ERROR;
	}
	if ( (replNode = delNode->m_left) != null ) {
		if ( replNode->m_right ) {
			while( replNode->m_right ) {
				replNode = replNode->m_right;
			}
			replNode->m_parent->m_right = replNode->m_left;
			replNode->m_left = delNode->m_left;
			delNode->m_left->m_parent = replNode;
		}
		replNode->m_parent = delNode->m_parent;
		replNode->m_right = delNode->m_right;
	}
	else if ( (replNode = delNode->m_right) != null ) {
		if ( replNode->m_left ) {
			while( replNode->m_left ) {
				replNode = replNode->m_left;
			}
			replNode->m_parent->m_left = replNode->m_right;
			replNode->m_right = delNode->m_right;
			delNode->m_right->m_parent = replNode;
		}
		replNode->m_parent = delNode->m_parent;
		replNode->m_left = delNode->m_left;
	}
	if ( !delNode->m_parent ) {
		this->m_root = replNode;
	}
	else {
		if ( delNode->m_parent->m_id < delNode->m_id ) {
			delNode->m_parent->m_right = replNode;
		}
		else {
			delNode->m_parent->m_left = replNode;
		}
	}delete( delNode->m_value );
	delete( delNode );
	this->m_size--;

	return SUCCESS;
}

size_t HashMap_size( HashMap *this ) {
	return this->m_size;
}

boolean HashMap_isEmpty( HashMap *this ) {
	return (this->size > 0 ? false : true);
}

