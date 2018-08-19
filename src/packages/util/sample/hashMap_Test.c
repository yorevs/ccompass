/*******************************************************************************
* Project:        C-Compass 1.0.0
* File:           utilTests.c
* Purpose:        Implement functions to TODO: Describe purpose here
* Author:         Hugo Saporetti Junior
* Creation date:  Sep 3, 2009
* ANSI:           yes/no
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

/* Includes ---------------------------------------------------------------- */
#include "Compass.h"
#include "Exception.h"
#include "HashMap.h"
/* ------------------------------------------------------------------------- */

/* Defines ----------------------------------------------------------------- */
/*#define NDEBUG*/

#define stats_HashMap(map)    if (true) { \
                              void _stats_HashMap_(HashMap *map) { \
                                if (!map) { \
                                  return; \
                                } \
                                void _hm_stats_(MapNode *node) { \
                                  if (!node) { \
                                    return; \
                                  } \
                                  printf("Key: \"%s\", Value: \"%s\", ID: %lu\n", \
                                    node->m_key->m_text, toString(node->m_value), node->m_id \
                                  ); \
                                  _hm_stats_(node->m_left); \
                                  _hm_stats_(node->m_right); \
                                } \
                                _hm_stats_(map->m_root); \
                              } \
                              _stats_HashMap_(map);}

/* ------------------------------------------------------------------------- */

void testHashMap( void ) {
	HashMap *hmap = HashMap();
	HashMap *myClone = null;

	hmap->put( hmap, "Chave 1", String("String 1") );
	hmap->put( hmap, "Chave 2", String("String 2") );
	hmap->put( hmap, "Chave 3", String("String 3") );
	hmap->put( hmap, "Chave 4", String("String 4") );
	hmap->put( hmap, "Chave 5", String("String 5") );

	hmap->put( hmap, "Chave 1", String("String A") );
	hmap->put( hmap, "Chave B", String("String B") );
	hmap->put( hmap, "Chave 5", String("String C") );

	hmap->remove( hmap, "Chave 4" );

	myClone = clone( hmap );

	System.out.println( "Listing %d elements from original HashMap ...\n", hmap->size( hmap ) );
	stats_HashMap( hmap );
	System.out.println("\n");

	System.out.println("Listing %d elements from cloned HashMap ...\n", myClone->size( myClone ) );
	stats_HashMap( myClone );
	System.out.println("\n");

	delete( myClone );
	delete( hmap );
}


int main( int argc, char **argv ) {
#if ! defined NDEBUG
	System.mem.setDebug( true, DBG_NONE );
#endif

	__initExceptionSignals();

	testHashMap();

	System.out.println( "Done" );

	return SUCCESS;
}

