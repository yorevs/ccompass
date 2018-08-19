/*******************************************************************************
 * Project:        C-Compass 1.0.0
 * File:           stringTests.c
 * Purpose:        Implement functions to TODO: Describe purpose here
 * Author:         Hugo Saporetti Junior
 * Creation date:  Oct 8, 2009
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
#include "String.h"
/* ------------------------------------------------------------------------- */

/* Defines ----------------------------------------------------------------- */
/*#define NDEBUG*/
#define stats_string(str) System.out.println( \
                          "%s: \"%s\", cap: %d, len: %d, sizeof: %d", \
                          #str, toString(str), \
                          str->m_capacity, \
                          str->m_length, \
                          sizeOf(str));
/* ------------------------------------------------------------------------- */

void testString() {
	String *str1 = null;
	String *str2 = null;
	String *str3 = null;
	char *string = null;

	str1 = String("Initial Text");
	stats_string( str1 );
	str1->clear( str1 );
	stats_string( str1 );
	str1->set( str1, "123456789" );
	stats_string( str1 );

	str2 = String("Other String");
	str3 = clone( str1 );

	stats_string( str2 );
	stats_string( str3 );

	System.out.println( "str1 %s equals to str2",
	  equals(str1, str2) ? "is" : "is not" );
	System.out.println( "str1 %s equals to str3",
	  equals(str1, str3) ? "is" : "is not" );
	System.out.println( "str2 %s equals to str3",
	  equals(str2, str3) ? "is" : "is not" );

	str2->copy( str2, str3 );

	System.out.println( "str1 %s equals to str2",
	  equals(str1, str2) ? "is" : "is not" );
	System.out.println( "str1 %s equals to str3",
	  equals(str1, str3) ? "is" : "is not" );
	System.out.println( "str2 %s equals to str3",
	  equals(str2, str3) ? "is" : "is not" );

	System.out.println( "Value of 33 = %s", (string = str1->valueOf( 33, "%d" )) );
	FREE( string );
	System.out.println( "Value of 0.5f = %s", (string =
	  str1->valueOf( 0.5f, "%.1f" )) );
	FREE( string );

	str1->set( str1, null );
	str1->set( str2, "c = '%c', s = \"%s\", S = \"%S\", d = %.3d, f = %.2f, x = %x, X = %X, p = %p, o = %o", '@', "a text", str1, 33, 0.5f, 0xffffffff, 0xEEEEEEEE, 0x8072aeB4, 27 );
	str1->set( str3, "this is the 3rd string" );

	System.out.println( "%s", toString(str1) );
	System.out.println( "%s", toString(str2) );
	System.out.println( "%s", toString(str3) );

	str3->setCase( str3, TC_UPPERCASE );
	System.out.println( "%s", toString(str3) );
	str3->setCase( str3, TC_LOWERCASE );
	System.out.println( "%s", toString(str3) );

	delete( str1 );
	str1 = str3->replaceAll( str3, " ", "" );
	stats_string( str1 );
	delete( str2 );
	str2 = str3->replaceAll( str3, " ", "_xx_" );
	stats_string( str2 );
	delete( str3 );
	str3 = str2->replaceAll( str2, "3rd", "third" );
	System.out.println( "%s", toString(str3) );

	System.out.println( "the string str3 %s the substring \"%s\"",
	  str3->contains( str3, "third" ) ? "contains" : "does not contain", "third" );
	System.out.println( "the string str3 %s the substring \"%s\"",
	  str3->contains( str3, "second" ) ? "contains" : "does not contain", "second" );

	str1->set( str1, "bbb" );
	str1->set( str2, "ccc" );
	str1->set( str3, "aaa" );
	System.out.println( "the string %s is %s than the string %s", toString(str1),
	  str1->compareTo( str1, str2 ) > 0 ? "greater than" : "lower or equal than", toString(str2) );
	System.out.println( "the string %s is %s than the string %s", toString(str1),
	  str1->compareTo( str1, str3 ) > 0 ? "greater than" : "lower or equal than", toString(str3) );

	str1->set( str1, "bbb" );
	str2->set( str2, "bBb" );
	str3->set( str3, "BBB" );
	System.out.println( "the string %s is %s than the string %s", toString(str1),
	  str1->compareToIgnoreCase( str1, str2 ) > 0 ? "greater than" :
	    "lower or equal than", toString(str2) );
	System.out.println( "the string %s is %s than the string %s", toString(str1),
	  str1->compareToIgnoreCase( str1, str3 ) > 0 ? "greater than" :
	    "lower or equal than", toString(str3) );

	str1->set( str1, "  this is the middle of the string " );
	stats_string( str1 );
	delete( str2 );
	str2 = str1->substring( str1, 14, 19 );
	System.out.println( "the substring 14, 19 of the str1 is \"%s\"", toString(str2) );

	delete( str2 );
	str2 = str1->trim( str1 );
	System.out.println( "No leading and trailing spaces of str1: \"%s\"", toString(str2) );

	delete( str2 );
	str2 = str1->replace( str1, 10, 13, "THE" );
	System.out.println( "Replacing chars(10-13) of str1 by %s: \"%s\"", "THE", toString(str2) );

	System.out.println( "HC: %d", hashCode(str1) );

	delete( str1 );
	delete( str2 );
	delete( str3 );
}


int main( int argc, char **argv ) {
#if ! defined NDEBUG
	System.mem.setDebug( true, DBG_NONE );
#endif

	__initExceptionSignals();

	testString();

	System.out.println( "Done." );

	return SUCCESS;
}
