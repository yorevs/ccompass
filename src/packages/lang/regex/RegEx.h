/*******************************************************************************
* Project:        CRSwE_Library_V_1_1_beta
* File:           RegEx.h
* Purpose:        Public prototypes for RegEx C-ANSI class
* Author:         Hugo Saporetti Junior
* Creation date:  Sep 3, 2009
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
#ifndef REGEX_H_
#define REGEX_H_

/* Includes ----------------------------------------------------------------- */
#include <ctype.h>

#include "CRSwE.h"
#include "String.h"
#include "NFAEntry.h"
#include "NFAState.h"
#include "RegExGroup.h"
/* -------------------------------------------------------------------------- */

/* Useful Macro to shortcut the constructor method */
#define RegEx()           _NEW_(RegEx, null)

#define REG_MAX_GROUPS    10  /* TODO */


/*
 * EXPR.    | DESCRIPTION
 *------------------------------------------------------------------------------
 *  x       | Matches the character x.
 *------------------------------------------------------------------------------
 *  .       | Matches any single character; line terminators are included.
 *          | For example, a.c matches "abc", "aac", etc.
 *------------------------------------------------------------------------------
 * [  ]     | A bracket expression. Matches a single character that is contained
 *          | within the brackets. For example, [abc] matches "a", "b", or "c".
 *          | [a-z] specifies a range which matches any lowercase letter from
 *          | "a" to "z". These forms can be mixed: [abcx-z] matches "a", "b",
 *          | "c", "x", "y", or "z", as does [a-cx-z]. The - character is
 *          | treated as a literal character if it is escaped with a backslash:
 *          | e.g: [a\-bc].
 *------------------------------------------------------------------------------
 * [^ ]     | Matches a single character that is not contained within the
 *          | brackets. For example, [^abc] matches any character other than
 *          | "a", "b", or "c". [^a-z] matches any single character that is not
 *          | a lowercase letter from "a" to "z". As above, literal characters
 *          | and ranges can be mixed.
 *------------------------------------------------------------------------------
 *  ^       | Matches the starting position within the string.
 * TODO     |
 *------------------------------------------------------------------------------
 *  $       | Matches the ending position of the string or the position just
 * TODO     | before a string-ending newline.
 *------------------------------------------------------------------------------
 * (  )     | Defines a marked subexpression. The string matched within the
 *          | parentheses can be recalled later (see the next entry, \n). A
 *          | marked subexpression is also called a block or capturing group.
 *------------------------------------------------------------------------------
 *  *       | Matches the preceding element ZERO OR MORE times. For example,
 *          | ab*c matches "ac", "abc", "abbbc", etc. [xyz]* matches "", "x",
 *          | "y", "z", "zx", "zyx", "xyzzy", and so on. (ab)* matches "", "ab",
 *          | "abab", "ababab", and so on.
 *------------------------------------------------------------------------------
 *  ?       | Matches the preceding element ZERO OR ONE time. For example, ba?
 *          | matches "b" or "ba".
 *------------------------------------------------------------------------------
 *  +       | Matches the preceding element ONE OR MORE times. For example, ba+
 *          | matches "ba", "baa", "baaa", and so on.
 *------------------------------------------------------------------------------
 *  |       | The choice (aka ALTERNATION or set union) operator matches either
 *          | the expression before or the expression after the operator. For
 *          | example, abc|def matches "abc" or "def".
 *------------------------------------------------------------------------------
 * {m, n}   | Matches the preceding element at least m and not more than n
 *          | times. For example, a{3,5} matches only "aaa", "aaaa" and "aaaaa".
 *------------------------------------------------------------------------------
 *  \n      | XXX Matches what the nth marked subexpression matched, where n is
 *          | a digit from 1 to 9. This construct is theoretically irregular and
 *          | was not adopted in the POSIX ERE syntax.# n Can also be a control
 *          | character, to allow the characters used in the regex to be used
 *          | in the search. E.g \. \( \\ etc.
 *------------------------------------------------------------------------------
 *
 * Character classes
 *------------------------------------------------------------------------------
 * Simple: [abc] a, b or c.
 * Negation: [^abc] Any character except a, b or c.
 * Range: [a-zA-Z] a through z or A through Z, inclusive.
 * TODO Union: [a-d[m-p]] a through d, or m through p; [a-dm-p].
 * TODO Intersection: [a-z&&[def]] d, e or f; [def].
 * TODO Subtraction: [a-z&&[^bc]] a through z, except for b and c; [ad-z].
 * TODO              [a-z&&[^m-p]] a through z, and not m through p; [a-lq-z].
 *------------------------------------------------------------------------------
 *
 * Escape Characters
 *------------------------------------------------------------------------------
 *  \\      | Matches the backslash character.
 *  \xhh    | Matches the character with hexadecimal value 0xhh.
 *  \uhhhh  | TODO Matches the unicode character with hexadecimal value 0xhhhh.
 *  \t      | Matches the tab (0x09) character.
 *  \n      | Matches the newline (line feed) character (0x0A).
 *  \r      | Matches the carriage-return character (0x0D).
 *  \f      | Matches the form-feed character (0x0C).
 *  \a      | TODO Matches the alert (bell) character (0x07).
 *  \e      | Matches the escape character (0x1B).
 *  \cx     | TODO Matches the control character corresponding to x.
 *------------------------------------------------------------------------------
 *
 * Predefined character classes
 *------------------------------------------------------------------------------
 *  \d      | A digit: [0-9].
 *  \D      | A non-digit: [^0-9].
 *  \s      | A whitespace character: [ \t\n\x0B\f\r].
 *  \S      | A non-whitespace character: [^\s].
 *  \w      | A word character: [a-zA-Z_0-9].
 *  \W      | A non-word character: [^\w].
 *------------------------------------------------------------------------------
 *
 * POSIX character classes (US-ASCII only)
 *------------------------------------------------------------------------------
 *  \p{xxx} | Matches the POSIX a character class xxx of one of the following:
 *
 *  Lower   | Matches a lower-case alphabetic character: [a-z].
 *  Upper   | Matches an upper-case alphabetic character: [A-Z].
 *  ASCII   | Matches all ASCII: [\0x00-\0x7F].
 *  Alpha   | Matches an alphabetic character: [a-zA-Z].
 *  Digit   | Matches a decimal digit: [0-9].
 *  Alnum   | Matches an alphanumeric character: [a-zA-Z0-9].
 *  Punct   | Matches a punctuation: One of !"#$%&'()*+,-./:;<=>?@[\]^_`{|}~
 *  Graph   | Matches a visible character: [\p{Alnum}\p{Punct}]
 *  Print   | Matches a printable character: [\p{Graph}\x20].
 *  Blank   | Matches a space of tab ( \t): [\x20\x09].
 *  Cntrl   | Matches a control character: [\x00-\x01\x7F].
 *  XDigit  | Matches a hexadecimal digit: [0-9a-fA-F].
 *  Space   | Matches a whitespace character: [ \t\n\x0B\f\r].
 *------------------------------------------------------------------------------
 *
 * TODO Boundary Matchers
 *------------------------------------------------------------------------------
 *  ^       | TODO The beginning of a line.
 *  $       | TODO The end of a line.
 *  \b      | TODO A word boundary.
 *  \B      | TODO A non-word boundary.
 *  \A      | TODO The beginning of the input.
 *  \G      | TODO The end of the previous match.
 *  \Z      | TODO The end of the input but for the final terminator, if any.
 *  \z      | TODO the end of the input.
 */

/**
 * @purpose RegEx reserved characters
 */
enum RegExSymbols {
  REG_OSBR        = '[',  /** @purpose Open Square Bracket; */
  REG_CSBR        = ']',  /** @purpose Close Square Bracket; */
  REG_OCAP        = '(',  /** @purpose Open Capture; */
  REG_CCAP        = ')',  /** @purpose Close Capture; */
  REG_OBR         = '{',  /** @purpose Open Bracket; */
  REG_CBR         = '}',  /** @purpose Close Bracket; */
  REG_ESC         = '\\', /** @purpose Escape; */
  REG_ALT         = '|',  /** @purpose Alternator; */
  REG_RANGE       = '-',  /** @purpose Range; */
  REG_ATS         = '^',  /** @purpose At Start; */
  REG_ATE         = '$',  /** @purpose At End; */
  REG_ZOO         = '?',  /** @purpose Zero or One; */
  REG_ZOM         = '*',  /** @purpose Zero or More; */
  REG_OOM         = '+',  /** @purpose One or More; */
  REG_WILDCARD    = '.'   /** @purpose Wildcard; */
};


/**
 * @purpose Regex syntax errors
 */
enum RegSyntaxErrors {
  REG_IPOSIX    = 0,    /** @purpose Invalid POSIX class. */
  REG_BADBR     = 1,    /** @purpose Bad Bracket -> {...} . */
  REG_BADPAT    = 2,    /** @purpose Bad Pattern -> Syntax Error .*/
  REG_BADRPT    = 3,    /** @purpose Bad Repetition -> ?, *, +, {n,m} . */
  REG_ECTYPE    = 4,    /** @purpose Bad Escape Type -> \n . */
  REG_EBRACK    = 5,    /** @purpose Unbalanced Square Brackets -> [..] . */
  REG_EPAREN    = 6,    /** @purpose Unbalanced Parentheses -> (..) . */
  REG_EBRACE    = 7,    /** @purpose Unbalanced Braces -> {..} . */
  REG_ERANGE    = 8,    /** @purpose Invalid Range Endpoint -> n-m . */
};


/**
 * @purpose Definition of the RegEx C-ANSI class
 */
class (RegEx,
  String *m_groups[REG_MAX_GROUPS]; /* Positions to capture groups. */
  ArrayList *m_subExprs;            /* Array of sub-expressions of this RegEx.*/
  RegExGroup *m_curExpr;            /* Current expression. */
  int16 m_status;                   /* The status of the last compilation. */

  /* Public Methods */
  int16 (*compile)(RegEx *this, const char *regex);
  void (*cleanup)(RegEx *this);
  boolean (*matches)(RegEx *this, const char *input);
  uint16 (*groupCount)(const RegEx *this);
  String * (*group)(const RegEx *this, uint16 index);
  int16 (*getStatus)(const RegEx *this);
  char * (*getStatusMsg)(const RegEx *this);
);

/* Declare all public prototypes here */

/**
 * Function: RegEx_compile
 * @purpose Compiles the given regular expression into a pattern.
 * @param *this : The target RegEx.
 * @param *regex : The pattern to be compiled
 * @return
 * - int16 : SUCCESS or ERROR if any occurs.
 */
extern int16 RegEx_compile(RegEx *this, const char *regex);

/**
 * Function: RegEx_cleanup
 * @purpose Cleans the compilation of this RegEx.
 * @param *this : The target RegEx.
 * @return none
 */
extern void RegEx_cleanup(RegEx *this);

/**
 * Function: RegEx_matches
 * @purpose Attempts to match the entire region against the pattern.
 * @param *this : The target RegEx.
 * @return
 * - boolean : true if, and only if, the entire region matches the pattern.
 */
extern boolean RegEx_matches(RegEx *this, const char *input);

/**
 * Function: RegEx_groupCount
 * @purpose Returns the number of groups in the last matched pattern
 * @param *this : The target RegEx.
 * @return
 * - uint16 : The number of groups in the last matched pattern
 */
extern uint16 RegEx_groupCount(const RegEx *this);

/**
 * Function: RegEx_group
 * @purpose Returns the input  subsequence captured by the given group during
 * the previous match operation.
 * @param *this : The target RegEx.
 * @return
 * - String : The (possibly empty) subsequence captured by the group during the
 * previous match, or null if the group failed to match part of the input.
 */
extern String * RegEx_group(const RegEx *this, uint16 index);

/**
 * Function: RegEx_getStatus
 * @purpose Returns the status of the last compilation.
 * @param *this : The target RegEx.
 * @return
 * - int16 : the status of the last compilation; -1 if the expression is not
 * compiled. A positive value means an error occurred during the compilation. 0
 * the compilation was successful.
 */
extern int16 RegEx_getStatus(const RegEx *this);


/**
 * Function: RegEx_getStatusMsg
 * @purpose Returns the message associated with the status of the last
 * compilation.
 * @param *this : The target RegEx.
 * @return
 * - char * : the message associated with the current status of the last
 * compilation of this regex.
 */
extern char * RegEx_getStatusMsg(const RegEx *this);


#endif /* REGEX_H_ */

