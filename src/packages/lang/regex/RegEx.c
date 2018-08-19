/*******************************************************************************
 * Project:        C-Compass 1.0.0
 * File:           RegEx.c
 * Purpose:        Implementation of C-ANSI class RegEx
 * Author:         Hugo Saporetti Junior
 * Creation date:  Sep 3, 2009
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
 *
 * TODO
 *
 *   Alterar o modo de sub-expressoes. no momento ela corta em duas, dai nao
 *   funciona isso: ab((cd)|(ef))gh .
 *
 *******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#include "Compass.h"
#include "RegEx.h"
#include "String.h"
/* --------------------------------------------------------------------------*/

/* TODO Comment */
static char *serrMsgs[] =
  {
    "Invalid POSIX class; check documentation for possible options",
    "Invalid '{..}' construct in the regex. A valid construct must contain a \
single number, or two numbers in increasing order separated by a comma.",
    "Syntax error in the regular expression",
    "A repetition operator such as ? or * appeared in a bad position; with no \
preceding sub-expression to act on.",
    "The regex referred to an invalid escape character class name",
    "Unbalanced square bracket '[]' .", "Unbalanced parentheses '()' .",
    "Unbalanced braces '{}' .", "One of the endpoints in a range was invalid.", };

/**
 * Function: RegEx_new
 * @purpose: Constructs a RegEx object
 * @param *_class : A pointer the this class descriptor
 * @param *parameters : The parameter list for this constructor
 * @return void * : A pointer to the instance of RegEx newly created
 */
void * RegEx_new( void *_class, va_list *parameters ) {
	RegEx *this = _class;
	assert( this );
	this->m_subExprs = ArrayList();
	this->m_curExpr = null;
	*this->m_groups = null;
	this->m_status = ERROR;

	this->compile = RegEx_compile;
	this->cleanup = RegEx_cleanup;
	this->matches = RegEx_matches;
	this->groupCount = RegEx_groupCount;
	this->group = RegEx_group;
	this->getStatus = RegEx_getStatus;
	this->getStatusMsg = RegEx_getStatusMsg;

	return this;
}

/**
 * Function: RegEx_delete
 * @purpose: Destructs a RegEx object
 * @param *this : A pointer the object to be destroyed
 * @return void * : A pointer to null if the object was successfully deleted
 */
void RegEx_delete( void *_this ) {
	RegEx *this = _this;
	assert( this );
	RegEx_cleanup( this );
	assert( this->m_subExprs );
	delete( this->m_subExprs );
}

/**
 * Function: RegEx_clone
 * @purpose: Creates and returns a copy of a RegEx object
 * @param *_this : A pointer the object to be cloned
 * @return void * : A pointer to the cloned object
 */
void * RegEx_clone( const void *_this ) {
	/*const RegEx *this = _this;*/
	RegEx *cloned = RegEx();
	/*TODO Implementar corretamente este metodo */

	return cloned;
}

/**
 * Function: RegEx_equals
 * @purpose: Indicates whether some other object is "equal to" another
 * @param *_this : A pointer the object to be compared
 * @param *_other :The reference object with which to compare
 * @return boolean : true if this object is the same as the obj argument; false otherwise
 */
boolean RegEx_equals( const void *_this, const void *_other ) {
	const RegEx *this = _this;
	const RegEx *other = _other;
	if ( this == other ) {
		return true;
	}
	if ( !other || other->d_class != d_RegEx ) {
		return false;
	}

	return (hashCode(this) == hashCode(other));
}

/**
 * Function: RegEx_getClass
 * @purpose: TODO Comment
 * @param *_this : A pointer the object to get the class descriptor
 * @return
 * - Class * : the Class descriptor for this class
 */
Class * RegEx_getClass( const void * _this ) {
	const RegEx *this = _this;

	return this->d_class;
}

/**
 * Function: RegEx_sizeOf
 * @purpose: Returns the size in bytes of the given class
 * @param *_this : The reference object
 * @return return_type: the size in bytes of the given class
 *
 * Note:
 * May return a different value from sizeof function
 */
size_t RegEx_sizeOf( const void *_this ) {

	return sizeof(RegEx);
}

/**
 * Function: RegEx_toString
 * @purpose: Returns a cstring representation of the object
 * @param *_this : The reference object
 * @return char * : A pointer to the string representation of the object
 */
char * RegEx_toString( const void *_this ) {
	const RegEx *this = _this;

	return (char*) this->d_class->m_name;
}

ClassDescriptor( RegEx );

/* See RegEx.h for documentation about the functions below */

void RegEx_allocCaptureGroup( RegEx *this ) {
	uint16 i = 0;
	assert( this && this->m_groups );
	while( this->m_groups[i] && i < REG_MAX_GROUPS ) {
		i++;
	}
	this->m_groups[i] = String("");
}

void RegEx_clearGroups( RegEx *this ) {
	uint16 i = 0;
	assert( this && this->m_groups );
	while( this->m_groups[i] && i < REG_MAX_GROUPS ) {
		this->m_groups[i]->clear( this->m_groups[i] );
		i++;
	}
}

void RegEx_cleanup( RegEx *this ) {
	uint16 i = 0;
	assert( this );
	while( this->m_groups[i] && i < REG_MAX_GROUPS ) {
		delete( this->m_groups[i] );
		i++;
	}
	RegExGroup *rootGroup = RegExGroup();
	this->m_subExprs->clear( this->m_subExprs );
	this->m_subExprs->addElement( this->m_subExprs, rootGroup );
	this->m_curExpr = rootGroup;
	this->m_status = ERROR; /* Does not mean an error. */
}

uint16 RegEx_aToHex( const char *string ) {
	uint16 hexDigit = 0, hexNumber = 0; /* Digits on the form: 0xhh */
	char text[] = { "\x0\x0" };

	while (*string) {
		hexNumber = hexNumber << 4;
		text[0] = *(string++);
		if ( (hexDigit = atoi( text )) == 0 ) {
			switch( toupper( text[0] ) ) {
				case 'A':
					hexDigit = 10;
				break;
				case 'B':
					hexDigit = 11;
				break;
				case 'C':
					hexDigit = 12;
				break;
				case 'D':
					hexDigit = 13;
				break;
				case 'E':
					hexDigit = 14;
				break;
				case 'F':
					hexDigit = 15;
				break;

				default:
					return 0;
			}
		}
		hexNumber |= hexDigit;
	}


	return hexNumber;
}

int RegEx_isBlank( int c ) {
	return (c == '\x20' || c == '\x09');
}

int16 RegEx_splitExpr( RegEx *this ) {
	RegExGroup *newSubex = RegExGroup();
	int16 result = ArrayList_addElement( this->m_subExprs, newSubex );
	this->m_curExpr = newSubex;
	System.out.printDebug( DBG_DEBUG, "[RegEx] Expression split" );

	return result;
}

void RegEx_unmatchPrevStates( NFAState *s ) {
	NFAState *prev = s->m_prevState;
	while( prev ) {
		prev->m_isFinal = false;
		prev = prev->m_prevState;
	}
}

int16 RegEx_patch( NFAState *s, NFAEntry *e, boolean append ) {
	int16 result = 0;
	if ( append ) {
		result = s->m_outEntries->addElement( s->m_outEntries, e );
	}
	else {
		result = s->m_outEntries->insertElementAt( s->m_outEntries, e, 0 );
	}
	System.out.printDebug( DBG_DEBUG, "[RegEx] %s %s matching: '%s%c' for State: (%.3d) >>> (%.3d)", (
	  append ? (e->m_isCloned ? "Repeated" : "Appended") :
	    (e->m_isCloned ? "Repeated" : "Inserted")), (
	  e->m_isBypass ? "a BYPASS" : (e->m_isWildcard ? "a WILDCARD" : "an entry")), (
	  e->m_isNegating ? "^" : ""), e->m_codepointStart, hashCode(s), hashCode(e->m_out) );

	return result;
}

void RegEx_append( RegEx *this, uint16 codepoint, uint16 groups, boolean isNegating, boolean isWildcard ) {
	NFAState *s = NFAState(this->m_curExpr->m_lastState);
	NFAEntry *e = NFAEntry(false, s->m_prevState, s, codepoint);
	e->m_groups = groups;
	e->m_isNegating = isNegating;
	e->m_isWildcard = isWildcard;
	s->m_in = e;
	s->m_isFinal = true;
	RegEx_unmatchPrevStates( s );
	RegEx_patch( s->m_prevState, e, !s->m_prevState->m_hasWildcard );
	s->m_hasWildcard = isWildcard;
	if ( s->m_prevState->m_hasWildcard ) {
		NFAEntry *e2 = clone( e );
		e2->m_in = s->m_prevState->m_prevState;
		e2->m_out = s;
		RegEx_patch( s->m_prevState->m_prevState, e2, false );
	}
	this->m_curExpr->m_lastState = s;
}

void RegEx_insert( RegEx *this, uint16 codepoint, uint16 groups, boolean isNegating ) {
	NFAState *s = this->m_curExpr->m_lastState;
	;
	NFAEntry *e = null;
	ArrayList *paths = paths = (
	  (s && s->m_prevState) ? s->m_prevState->m_outEntries : null);
	if ( !paths ) {
		return RegEx_append( this, codepoint, groups, isNegating, false );
	}
	else {
		((NFAEntry*) paths->elementAt( paths, paths->size( paths ) - 1 ))->m_isLastSubex =
		  false;
	}
	e = NFAEntry(false, s->m_prevState, s, codepoint);
	e->m_groups = groups;
	e->m_isNegating = isNegating;
	RegEx_patch( s->m_prevState, e, true );
}

int16 RegEx_bypass( RegEx *this ) {
	NFAState *s = this->m_curExpr->m_lastState;
	NFAEntry *e = null;
	if ( s->m_hasWildcard ) return SUCCESS;
	if ( !s || !s->m_prevState ) return REG_BADRPT;
	e = NFAEntry(true, s->m_prevState, s, REG_WILDCARD);
	e->m_groups = s->m_in->m_groups;
	e->m_isNegating = s->m_in->m_isNegating;
	RegEx_patch( s->m_prevState, e, true );

	return SUCCESS;
}

int16 RegEx_repeat( RegEx *this ) {
	NFAState *s = this->m_curExpr->m_lastState;
	NFAEntry *e = null;
	ArrayList *paths = null;
	uint16 i = 0;
	if ( !s || !s->m_prevState ) return REG_BADRPT;
	paths = s->m_prevState->m_outEntries;
	for( i = 0; i < paths->size( paths ); i++ ) {
		NFAEntry *next = paths->elementAt( paths, i );
		if ( next->m_isCloned || next->m_isBypass ) continue;
		e = clone( next );
		e->m_in = s;
		RegEx_patch( s, e, true );
	}

	return SUCCESS;
}

void RegEx_patchCodepoint( RegEx *this, uint16 codepoint, uint16 *subex, uint16 groups, boolean isNegating ) {
	if ( !(*subex) ) RegEx_append( this, codepoint, groups, isNegating, false );
	else {
		if ( *subex >= 2 ) {
			RegEx_insert( this, codepoint, groups, isNegating );
		}
		else {
			RegEx_append( this, codepoint, groups, isNegating, false );
			(*subex)++;
		}
	}
}

int16 RegEx_setRange( RegEx * this, char **regex ) {
	NFAState *s = null;
	NFAEntry *e = null;
	ArrayList *paths = null;
	char byte[] = { "\x0\x0" };
	int16 codepoint = *(++(*regex));

	if ( !isalnum(codepoint)
	  && (codepoint != '_' && codepoint != ' ' && codepoint != '\\'
	    && codepoint != ']') ) {
		return REG_ERANGE;
	}
	if ( codepoint == ']' ) return SUCCESS;

	(*regex)++;
	if ( codepoint == REG_ESC ) {
		if ( *(*regex) == 'x' ) {
			uint16 hex = 0;
			byte[0] = *((*regex) + 1);
			byte[1] = *((*regex) + 2);
			hex = RegEx_aToHex( byte );
			codepoint = hex;
			(*regex) += 2;
		}
		else {
			return REG_ERANGE;
		}
	}
	if ( !this->m_curExpr->m_lastState
	  || !this->m_curExpr->m_lastState->m_prevState ) {
		return REG_ERANGE;
	}
	s = this->m_curExpr->m_lastState->m_prevState;
	paths = s->m_outEntries;
	e = paths->elementAt( paths, paths->size( paths ) - 1 );
	if ( codepoint > e->m_codepointStart ) {
		e->m_codepointEnd = codepoint;
	}
	else {
		e->m_codepointEnd = e->m_codepointStart;
		e->m_codepointStart = codepoint;
	}
	System.out.printDebug( DBG_DEBUG, "[RegEx] Setting range [%s0x%.2X..0x%.2X]",
	  e->m_isNegating ? "^" : "", e->m_codepointStart, e->m_codepointEnd );

	return SUCCESS;
}

int16 RegEx_limitVisits( RegEx * this, char **regex ) {
	NFAState *s = null;
	NFAEntry *e = null;
	ArrayList *paths = null;
	char limits[] = { "\x0\x0\x0\x0\x0\x0" };
	uint16 i = 0, j = 0;
	int16 minRpt = -1, maxRpt = -1;
	int16 *curLimit = &minRpt;
	int16 pathsSize = 0;

	(*regex)++;
	if ( !this->m_curExpr->m_lastState
	  || !this->m_curExpr->m_lastState->m_prevState ) {
		return REG_EBRACE;
	}

	curLimit = &minRpt;
	while( *regex && *(*regex) ) {
		uint16 next = *(*regex)++;
		if ( next == ',' ) {
			if ( curLimit == &maxRpt ) return REG_BADBR;
			*curLimit = atoi( limits );
			curLimit = &maxRpt;
			i = 0;
			limits[0] = '\x0';
			continue;
		}
		else if ( next == '}' ) {
			*curLimit = atoi( limits );
			if ( limits[0] != '\x0' && curLimit != &maxRpt ) {
				maxRpt = minRpt;
			}
			else if ( limits[0] == '\x0' ) {
				*curLimit = -1;
			}
			break;
		}
		if ( next && isdigit(next) ) {
			limits[i++] = (uint16) next;
			limits[i] = '\x0';
		}
		else {
			return REG_BADBR;
		}
	}
	if ( *((*regex) - 1) != '}' ) {
		return REG_EBRACE;
	}
	if ( maxRpt > 0 && minRpt > maxRpt ) {
		return REG_BADBR;
	}

	s = this->m_curExpr->m_lastState->m_prevState;
	paths = s->m_outEntries;
	pathsSize = paths->size( paths );

	if ( minRpt == 0 ) {
		RegEx_bypass( this );
	}
	for( i = 1; i < (maxRpt > 0 ? maxRpt : minRpt); i++ ) {
		e = this->m_curExpr->m_lastState->m_in;
		RegEx_append( this, e->m_codepointStart, e->m_groups, e->m_isNegating, false );
		for( j = 0; j < pathsSize; j++ ) {
			NFAEntry *next = paths->elementAt( paths, j );
			if ( next->m_isCloned || next->m_isBypass || equals(e, next) ) continue;
			RegEx_insert( this, next->m_codepointStart, next->m_groups, next->m_isNegating );
		}
		if ( maxRpt > 0 && i >= minRpt ) {
			RegEx_bypass( this );
		}
	}
	if ( maxRpt < 0 ) {
		RegEx_repeat( this );
	}
	System.out.printDebug( DBG_DEBUG, "[RegEx] Setting limits: [min: %d, max: %d]", minRpt, maxRpt );

	return SUCCESS;
}

int16 RegEx_setPosix( RegEx * this, const char *posixExp ) {
	NFAState *s = null;
	NFAEntry *e = null;
	ArrayList *paths = null;

	if ( !posixExp ) {
		return REG_IPOSIX;
	}

	s = this->m_curExpr->m_lastState->m_prevState;
	paths = s->m_outEntries;
	e = paths->elementAt( paths, paths->size( paths ) - 1 );

	if ( strncmp( posixExp, "Lower", 5 ) == 0 ) {
		e->posixFnc = islower;
	}
	else if ( strncmp( posixExp, "Upper", 5 ) == 0 ) {
		e->posixFnc = isupper;
	}
	else if ( strncmp( posixExp, "ASCII", 5 ) == 0 ) {
		e->m_codepointStart = '\x00';
		e->m_codepointEnd = '\x7F';
	}
	else if ( strncmp( posixExp, "Alpha", 5 ) == 0 ) {
		e->posixFnc = isalpha;
	}
	else if ( strncmp( posixExp, "Digit", 5 ) == 0 ) {
		e->posixFnc = isdigit;
	}
	else if ( strncmp( posixExp, "Alnum", 5 ) == 0 ) {
		e->posixFnc = isalnum;
	}
	else if ( strncmp( posixExp, "Punct", 5 ) == 0 ) {
		e->posixFnc = ispunct;
	}
	else if ( strncmp( posixExp, "Graph", 5 ) == 0 ) {
		e->posixFnc = isgraph;
	}
	else if ( strncmp( posixExp, "Print", 5 ) == 0 ) {
		e->posixFnc = isprint;
	}
	else if ( strncmp( posixExp, "Blank", 5 ) == 0 ) {
		e->posixFnc = RegEx_isBlank;
	}
	else if ( strncmp( posixExp, "Cntrl", 5 ) == 0 ) {
		e->posixFnc = iscntrl;
	}
	else if ( strncmp( posixExp, "XDigit", 6 ) == 0 ) {
		e->posixFnc = isxdigit;
	}
	else if ( strncmp( posixExp, "Space", 5 ) == 0 ) {
		e->posixFnc = isspace;
	}
	else {
		return REG_IPOSIX;
	}
	System.out.printDebug( DBG_DEBUG, "[RegEx] Found a POSIX class: '{%s'", posixExp );

	return SUCCESS;
}

void RegEx_preProcessEscape( String **regex ) {
	String **newRegex = regex;
	String *reg = *newRegex;
	String *reg2 = null;
	String *reg3 = null;
	char *pRegex = null;
	char exp[] = { "\x0\x0\x0" };
	char rexp[] = { "\x0\x0\x0\x0\x0\x0\x0" };

	if ( !reg ) {
		return;
	}

	pRegex = strstr( reg->m_text, "\\" );
	reg3 = clone( reg );
	while( pRegex ) {
		switch( *(pRegex + 1) ) {
			case 'd': /* A digit */
				reg2 = reg3->replaceAll( reg3, "\\d", "[0-9]" );
			break;
			case 'D': /* A non-digit */
				reg2 = reg3->replaceAll( reg3, "\\D", "[^0-9]" );
			break;
			case 'w': /* A word */
				reg2 = reg3->replaceAll( reg3, "\\w", "[a-zA-Z_0-9]" );
			break;
			case 'W': /* A non-word */
				reg2 = reg3->replaceAll( reg3, "\\W", "[^a-zA-Z_0-9]" );
			break;
			case 's': /* A whitespace character */
				reg2 = reg3->replaceAll( reg3, "\\s", "[\\x20\t\n\x0B\f\r]" );
			break;
			case 'S': /* A non-whitespace character */
				reg2 = reg3->replaceAll( reg3, "\\S", "[^\\x20\t\n\x0B\f\r]" );
			break;
			case 't': /* TAB */
				reg2 = reg3->replaceAll( reg3, "\\t", "\t" );
			break;
			case 'n': /* New line indicator */
				reg2 = reg3->replaceAll( reg3, "\\n", "\n" );
			break;
			case 'r': /* Carriage return */
				reg2 = reg3->replaceAll( reg3, "\\r", "\r" );
			break;
			case 'f': /* Form-feed */
				reg2 = reg3->replaceAll( reg3, "\\f", "\f" );
			break;
			case 'e': /* Escape (0x1B) */
				reg2 = reg3->replaceAll( reg3, "\\e", "\e" );
			break;
			case 'u': /* UNICODE */
				/* TODO unicode is not implemented yet ! */
				System.out.printDebug( DBG_DEBUG, "[RegEx] Found a UNICODE. Not implemented yet!" );
			break;
			case 'p': /* POSIX class is processed later */
			case 'x': /* Hex-digits are processed later */
				pRegex = strstr( pRegex + 2, "\\" );
				continue;
			break;
			case REG_ESC:
				reg2 = reg3->replaceAll( reg3, "\\\\", "\\" ); /* The Backslash char */
			break;

			default: /* Unknown Escape char is translated into hexadecimal \xhh */
				exp[0] = REG_ESC;
				exp[1] = *(pRegex + 1);
				sprintf( rexp, "\\x%X", *(pRegex + 1) );
				reg2 = reg3->replaceAll( reg3, exp, rexp );
			break;
		}
		pRegex = strstr( pRegex + 2, "\\" );
		if ( reg2 != null ) {
			delete( reg3 );
			reg3 = reg2;
		}
		else {
			reg2 = reg3;
		}
	}
	if ( reg2 != null) {
		delete( reg );
		*newRegex = reg2;
	}
	else {
		delete( reg3 );
	}
	System.out.printDebug( DBG_DEBUG, "[RegEx] Predefined ESCAPE (\\) character classes replaced" );
}

uint16 RegEx_getGroupIndex( uint16 groupNum ) {
	uint16 grpIndex = 0 /* Group Number = 2^x, so, index = gn/2^x */;
	uint16 result = groupNum;

	if ( groupNum == 0 ) return 0;

	while( result ) {
		result /= 2;
		grpIndex++;
	}

	return grpIndex;
}

uint16 RegEx_findNextClosingGroup( uint16 groups, uint16 prevClosingGroup ) {
	uint16 next = prevClosingGroup;

	if ( groups == 0 || next == 1 ) return 0;

	while( next ) {
		/* See if the group on sequence is open, then return it if so. */
		if ( (groups & next) == next ) {
			System.out.printDebug( DBG_DEBUG, "[RegEx] Next CLOSE number is (%d)", next );
			return next;
		}
		next = next >> 1;
	}

	return 0;
}

int16 RegEx_compile( RegEx *this, const char *regex ) {
	String *finalRegex = String(regex);
	char *pRegex = null;
	char text[] = { "\x0\x0\x0\x0\x0\x0\x0" };
	boolean isNegating = false, isEscaping = false;
	uint16 subexFlag = 0, result = SUCCESS;
	uint16 hexCode = 0;
	uint16 curGroupPosition = 0, groups = 0, closingGroup = 0;
	if ( !this || !regex ) {
		return ERROR;
	}
	RegEx_cleanup( this );
	RegEx_allocCaptureGroup( this );
	System.out.printDebug( DBG_DEBUG, "[RegEx] Compiling pattern: \"%s\" ...", regex );
	RegEx_preProcessEscape( &finalRegex );
	pRegex = finalRegex->m_text;
	while( *pRegex && result == SUCCESS ) {
		switch( *pRegex ) {
			if ( !isEscaping ) {
				case REG_OSBR: /* [ */
				System.out.printDebug( DBG_DEBUG, "[RegEx] Found a OPEN SQUARE BRACKET" );
				subexFlag = 1;
				break;
				case REG_CSBR: /* ] */
				System.out.printDebug( DBG_DEBUG, "[RegEx] Found a CLOSE SQUARE BRACKET" );
				subexFlag = 0;
				isNegating = false;
				break;
				case REG_OCAP: /* ( */
				RegEx_allocCaptureGroup( this );
				curGroupPosition = (curGroupPosition ? curGroupPosition << 1 : 1);
				closingGroup = curGroupPosition;
				groups |= curGroupPosition;
				System.out.printDebug( DBG_DEBUG, "[RegEx] Found a OPEN CAPTURE (%d)", curGroupPosition );
				System.out.printDebug( DBG_DEBUG, "[RegEx] Next CLOSE IS (%d)", closingGroup );
				break;
				case REG_CCAP: /* ) */
				groups ^= closingGroup;
				System.out.printDebug( DBG_DEBUG, "[RegEx] Found a CLOSE CAPTURE (%d)", closingGroup );
				closingGroup = RegEx_findNextClosingGroup( groups, closingGroup );
				break;
				case REG_ALT: /* | */
				System.out.printDebug( DBG_DEBUG, "[RegEx] Found a ALTERNATOR" );
				if ( !subexFlag ) {
					RegEx_splitExpr( this );
					curGroupPosition = 0;
				}
				else {
					RegEx_patchCodepoint( this, *pRegex, &subexFlag, groups, isNegating );
				}
				break;
				case REG_RANGE: /* - */
				System.out.printDebug( DBG_DEBUG, "[RegEx] Found a RANGE" );
				result |= RegEx_setRange( this, &pRegex );
				break;
				case REG_ATS: /* ^ */
				System.out.printDebug( DBG_DEBUG, "[RegEx] Found a AT START" );
				isNegating = true;
				break;
				case REG_ATE: /* & */
				System.out.printDebug( DBG_DEBUG, "[RegEx] Found a At END. Not Implemented Yet!" );
				break;
				case REG_ZOO: /* ? */
				System.out.printDebug( DBG_DEBUG, "[RegEx] Found a ZERO OR ONE" );
				result |= RegEx_bypass( this );
				break;
				case REG_ZOM: /* * */
				System.out.printDebug( DBG_DEBUG, "[RegEx] Found a ZERO OR MORE" );
				result |= RegEx_repeat( this );
				result |= RegEx_bypass( this );
				break;
				case REG_OOM: /* + */
				System.out.printDebug( DBG_DEBUG, "[RegEx] Found a ONE OR MORE" );
				RegEx_repeat( this );
				break;
				case REG_OBR: /* { */
				System.out.printDebug( DBG_DEBUG, "[RegEx] Found a OPEN BRACKET" );
				result |= RegEx_limitVisits( this, &pRegex );
				continue;
				break;
				case REG_CBR: /* } */
				System.out.printDebug( DBG_DEBUG, "[RegEx] Found a CLOSE BRACKET" );
				result |= REG_EBRACE;
				break;
				case REG_WILDCARD: /* . */
				System.out.printDebug( DBG_DEBUG, "[RegEx] Found a WILDCARD" );
				RegEx_append( this, REG_WILDCARD, groups, isNegating, true );
				break;
				case REG_ESC: /* \xhh */
				System.out.printDebug( DBG_DEBUG, "[RegEx] Found a ESCAPE" );
				isEscaping = true;
				break;
			}

		default:
			if ( isEscaping && *pRegex == 'x' ) {
				text[0] = *(pRegex + 1);
				text[1] = *(pRegex + 2);
				hexCode = RegEx_aToHex( text );
				pRegex += 2;
				System.out.printDebug( DBG_DEBUG, "[RegEx] Found a hexadecimal character: '0x%.2X'", hexCode );
			}
			else if ( isEscaping && *pRegex == 'p' ) {
				RegEx_patchCodepoint( this, '\x00', &subexFlag, groups, isNegating );
				strncpy( text, pRegex + 2, 6 );
				RegEx_setPosix( this, text );
				pRegex = strstr( pRegex + 2, "}" );
				if ( !pRegex ) {
					result = REG_BADPAT;
				}
				pRegex++;
				continue;
			}
			else {
				hexCode = *pRegex;
				System.out.printDebug( DBG_DEBUG, "[RegEx] Found a regular character: '%c'", hexCode );
			}
			RegEx_patchCodepoint( this, hexCode, &subexFlag, groups, isNegating );
			isEscaping = false;
		break;
		}
		pRegex++;
	}
	if (subexFlag > 0) {
	 result = REG_EBRACK;
	 }
	else if (groups > 0) {
	 result = REG_EPAREN;
	 }
	delete( finalRegex );
	if ( result == SUCCESS ) {
		System.out.printDebug( DBG_DEBUG, "[RegEx] Expression %s compiled with success", regex );
	}
	else {
		System.out.printDebug( DBG_ERROR, "[RegEx] Expression compiled with errors" );
		RegEx_cleanup( this );
		this->m_status = result;
	}

	return result;
}

void RegEx_record( RegEx *this, uint16 codepoint, uint16 groups ) {
	uint16 i = 0;
	int16 maxGrps = 1 << (REG_MAX_GROUPS - 1);
	System.out.printDebug( DBG_DEBUG, "[RegEx] Capture Groups: %d", groups );
	for( ; i <= maxGrps; ) {
		int16 n = 1;
		String *nextGroup = this->m_groups[RegEx_getGroupIndex( i )];
		if ( nextGroup ) {
			if ( i == 0 ) {
				nextGroup->concatChar( nextGroup, codepoint );
				System.out.printDebug( DBG_DEBUG, "[RegEx] Captured '%c' for default Group: (%d)", codepoint, RegEx_getGroupIndex( i ) );
				if ( groups == 0 ) break;
			}
			else if ( groups & (n << RegEx_getGroupIndex( i - 1 )) ) {
				nextGroup->concatChar( nextGroup, codepoint );
				System.out.printDebug( DBG_DEBUG, "[RegEx] Captured '%c' for Group: (%d)", codepoint, RegEx_getGroupIndex( i ) );
			}
		}
		i = (i == 0 ? 1 : i << 1);
	}
}

NFAState * RegEx_lastBypassState( NFAState *state ) {
	NFAEntry *e = null;
	NFAState *s = state;
	ArrayList *paths = s->m_outEntries;
	uint16 i = 0;
	for( i = 0; i < paths->size( paths ); i++ ) {
		e = (NFAEntry *) paths->elementAt( paths, i );
		if ( e->m_isBypass ) {
			s = RegEx_lastBypassState( e->m_out );
			break;
		}
	}
	System.out.printDebug( DBG_DEBUG, "[RegEx] Last bypass state: %d", hashCode(s) );

	return s;
}

void RegEx_visitNFAStates( RegEx *this, NFAState *state, const char *input, boolean *result ) {
	ArrayList *paths = state->m_outEntries;
	NFAEntry *e = null;
	boolean resultSum = true, match = false;
	int16 i = 0;

	for( i = 0; i < paths->m_size; i++ ) {
		System.out.printDebug( DBG_DEBUG, "[RegEx] Iterating next path for state: %d", hashCode(state) );
		e = (NFAEntry *) paths->elementAt( paths, i );
		match = e->match( e, *input );
		resultSum &= match;
		if ( match ) {
			System.out.printDebug( DBG_DEBUG, "[RegEx] PASSED: '%c'", *input );
			if ( !e->m_isNegating ) {
				if ( !e->m_isBypass ) {
					RegEx_record( this, *input, e->m_groups );
					input++;
				}
				if ( *input ) {
					System.out.printDebug( DBG_DEBUG, "[RegEx] Visiting next state: %d", hashCode(e->m_out) );
					RegEx_visitNFAStates( this, e->m_out, input, result );
				}
				else {
					NFAState *lastMatchState = RegEx_lastBypassState( e->m_out );
					*result = lastMatchState->m_isFinal;
				}
				return;
			}
			else {
				if ( (e->m_isLastSubex && resultSum) || e->m_isBypass ) {
					if ( !e->m_isBypass ) {
						System.out.printDebug( DBG_DEBUG, "[RegEx] PASSED all entries: '%c'", *input );
						RegEx_record( this, *input, e->m_groups );
						if ( resultSum ) input++;
					}
					if ( *input ) {
						System.out.printDebug( DBG_DEBUG, "[RegEx] Visiting next state: %d", hashCode(e->m_out) );
						RegEx_visitNFAStates( this, (NFAState *) e->m_out, input, result );
					}
					else {
						NFAState *lastMatchState = RegEx_lastBypassState( e->m_out );
						*result = lastMatchState->m_isFinal;
					}
					return;
				}
				System.out.printDebug( DBG_DEBUG, "[RegEx] Matching next entry" );
				continue;
			}
			return;
		}
		else {
			System.out.printDebug( DBG_DEBUG, "[RegEx] FAILED: '%c' , trying next entry", *input );
			continue;
		}
	}
}

boolean RegEx_matches( RegEx *this, const char *input ) {
	RegExGroup *curExpr = null;
	ArrayList *subExprs = this->m_subExprs;
	uint16 i = 0;
	boolean result = false;

	if ( !this || !input ) {
		return false;
	}
	RegEx_clearGroups( this );
	for( i = 0; i < subExprs->size( subExprs ); i++ ) {
		System.out.printDebug( DBG_DEBUG, "[RegEx] Processing next expression" );
		curExpr = subExprs->elementAt( subExprs, i );
		if ( !curExpr || !curExpr->m_startState ) break;
		RegEx_visitNFAStates( this, curExpr->m_startState, input, &result );
		if ( result ) {
			System.out.printDebug( DBG_DEBUG, "[RegEx] Match state achieved" );
			return true;
		}
	}
	if ( !result ) {
		System.out.printDebug( DBG_DEBUG, "[RegEx] No more paths available" );
		RegEx_clearGroups( this );
	}

	return false;
}

uint16 RegEx_groupCount( const RegEx *this ) {
	uint16 i = 1;
	String *nextGroup = this->m_groups[i];
	while( nextGroup && i < REG_MAX_GROUPS ) {
		i++;
		nextGroup = this->m_groups[i];
	}

	return (i > 1 ? i - 1 : 0);
}

String * RegEx_group( const RegEx *this, uint16 index ) {
	String *group =
	  (index >= 0 && index <= this->groupCount( this ) && index < REG_MAX_GROUPS) ?
	    this->m_groups[index] : null;

	return group;
}

int16 RegEx_getStatus( const RegEx *this ) {
	return this->m_status;
}

char * RegEx_getStatusMsg( const RegEx *this ) {
	return serrMsgs[this->m_status];
}
