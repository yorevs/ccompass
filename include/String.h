/*******************************************************************************
* Project:        C-Compass 1.0.0
* File:           String.h
* Purpose:        Public prototypes for String C-ANSI class
* Author:         Hugo Saporetti Junior
* Creation date:  Aug 6, 2009
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
#ifndef STRING_H_
#define STRING_H_

/* Includes ----------------------------------------------------------------- */
#include "CRSwE.h"
/* -------------------------------------------------------------------------- */

/* Usefull Macro to shorcut the constructor method */
#define String(text)	       	_NEW_(String, text)

/** @purpose Defines the growth step of a String */
#define STR_GROWTH            10

/** @purpose Possible format identifiers */
#define STR_ALL_SPECIFIERS    "sScdoxXfp"

/** @purpose Possible number format identifiers */
#define STR_NUM_SPECIFIERS    "cdoxXfp"

/** @purpose Maximum length a string representation of a number could have */
#define MAXLEN_LITERAL_NUM    25


/** @purpose String Format Specifiers
 *
 * Usage: %[Flags]{Width}(.Precision+Number)[Length]<Specifier>
 */
enum FormatSpecifier {
  FS_BEGIN      = '%',  /** indicates, next character is a format specifier */
  FS_CHAR_SEQ   = 's',  /** a formatted character sequence */
  FS_STRING     = 'S',  /** a formatted String */
  FS_CHAR       = 'c',  /** a formatted char */
  FS_DECIMAL    = 'd',  /** a formatted decimal number */
  FS_OCTAL      = 'o',  /** a formatted octal number */
  FS_HEXA_LOW   = 'x',  /** a formatted hexadecimal number (lower-case) */
  FS_HEXA_HIGH  = 'X',  /** a formatted hexadecimal number (upper-case) */
  FS_FLOAT      = 'f',  /** a formatted floating number */
  FS_PTR_ADDR   = 'p',  /** a formatted pointer address */
};


/** @purpose Upper or Lower cases for characters
 */
enum TextCase {
  TC_UPPERCASE  = 'U',  /** [A-Z] Upper-case characters */
  TC_LOWERCASE  = 'L'   /** [a-z] Lower-case characters */
};


/* Definition of the String C-ANSI class */
class (String,
  char *m_text;         /** A pointer to the character sequence representing the
                            string */
  size_t m_capacity;    /** The current capacity of characters */
  size_t m_length;      /** The current length of the string */

  /* Public Methods */
  void (*clear)(String *this);
  int16 (*compareTo)(const String *this, const String *string);
  int16 (*compareToIgnoreCase)(const String *this, const String *string);
  int16 (*concatChar)(String *this, uint16 chr);
  int16 (*concatText)(String *this, const char *text);
  boolean (*contains)(const String *this, const char *text);
  int16 (*copy)(String *dest, const String *source);
  String * (*replace)(const String *this, uint16 startIndex, uint16 endIndex, const char *replacement);
  String * (*replaceAll)(const String *this, const char *text, const char *replacement);
  int16 (*set)(String *this, const char *fmt, ...);
  int16 (*setCase)(String *this, uint16 caseToSet);
  String * (*substring)(const String *this, uint16 startIndex, uint16 endIndex);
  String * (*trim)(const String *this);
  char * (*valueOf)(double number, const char *fmt);
);

/* Declare all public prototypes here */

/**
 * Function: String_clear
 * @purpose Clear the contents of the given string
 * @param *this : The String to be cleared
 * @return none
 */
extern void String_clear(String *this);

/**
 * Function: String_compareTo
 * @purpose Compares this to another; CASE sensitive
 * @param *this : The string to be compared
 * @param *string : The second string to be compared
 * @return
 *  - int16 :
 *    > 0 if this string is greater then the second
 *    < 0 if the second string is greater then the this string
 *    = 0 if both strings are equal
 */
extern int16 String_compareTo(const String *this, const String *string);


/**
 * Function: String_compareToIgnoreCase
 * @purpose Compares this to another; NOT CASE sensitive
 * @param *this : The string to be compared
 * @param *string : The second string to be compared
 * @return
 *  - int16 :
 *    > 0 if this string is greater then the second
 *    < 0 if the second string is greater then the this string
 *    = 0 if both strings are equal
 */
extern int16 String_compareToIgnoreCase(const String *this, const String *string);

/**
 * Function: String_concatChar
 * @purpose Concatenates a character into the end of this string and moves the
 * null terminating character to the m_next position, resizing the string if
 * needed.
 * @param *this : The String to concatenate the char
 * @param chr : The character to be concatenated
 * @return
 *  - int16 : SUCCESS or ERROR if any error occur
 */
extern int16 String_concatChar(String *this, uint16 chr);


/**
 * Function: String_concatString
 * @purpose Concatenates a m_text into the end of this source string and moves
 * the null terminating character to the m_next position, resizing the string if
 * needed.
 * @param *this : The CString to concatenate the char sequence
 * @param *m_text : The string to be concatenated
 * @return
 *  - int16 : SUCCESS or ERROR if any error occur
 */
extern int16 String_concatText(String *this, const char *text);

/**
 * Function: String_contains
 * @purpose Returns a pointer to the first occurrence of m_text in this string, or
 * a null pointer if m_text is not part of this string
 * @param *this : The string to be searched
 * @param *string : The m_text to be found
 * @return
 *  - boolean : True if the string contains the given m_text
 */
extern boolean String_contains(const String *this, const char *text);

/**
 * Function: String_copy
 * @purpose Copies the contents of a string to another
 * @param *dest : The String to receive the copied string content
 * @param *source : The String containing the copying string content
 * @return
 *  - int16 : The number of characters copied or ERROR if any error occurs
 */
extern int16 String_copy(String *dest, const String *source);

/**
 * Function: String_replace
 * @purpose Returns a string, replacing the char sequence between start and
 * end indexes by the replacement string
 * @param *this : The targeted string to have the subsequence replaced
 * @param startIndex : The start index
 * @param endIndex : The end index
 * @param *replacement : The replacement string of the sub sequence
 * @return
 *  - String * : A pointer to the replaced string or null if any ERROR occurs
 */
extern String * String_replace(const String *this, uint16 startIndex, uint16 endIndex, const char *replacement);

/**
 * Function: String_replaceAll
 * @purpose Returns a string, replacing each occurrence of a m_text that
 * matches the literal target sequence with the specified literal replacement
 * sequence
 * @param *this : The string to be searched and the string after the replace
 *  match
 * @param *m_text : The searching string
 * @param *replacement : String to replace any occurence of the string
 * @return
 *  - String * : The resulting string after replacement, null if any ERROR
 *  occurs
 */
extern String * String_replaceAll(const String *this, const char *text, const char *replacement);

/**
 * Function: String_resize
 * @purpose Reallocates the memory needed by string data pointer of the given
 * and increases/decreases this string m_capacity according to the use
 * @param *this : The String to be resized
 * @param len : The length the string should support
 * @return
 *  - int16 : SUCCESS or ERROR if any error occurs
 */
extern int16 String_resize(String *this, size_t len);

/**
 * Function: String_set
 * @purpose Set the contents of the given string to a formatted m_text
 * @param *this : The String to be set
 * @param *fmt : The format used to set the string
 * @return
 *  - int16 : The number of characters set or ERROR if any error occur
 *
 * @Notes
 *
 * Depending on the format string, the function may expect a sequence of
 * additional arguments, each containing one m_value to be inserted instead of
 * each %-tag specified in the format parameter, if any. There should be the
 * same number of these arguments as the number of %-tags that expect a m_value
 *
 * The following formats are recognized
 *
 * c A single character
 * d or i  Signed decimal integer  (Ex:.392)
 * f Decimal floating point  (Ex:.392.65)
 * o Signed octal  (Ex:.610)
 * s String of characters sample
 * S CString containing characters sample
 * u Unsigned decimal integer  (Ex:.7235)
 * x Unsigned hexadecimal integer  (Ex:.7fa)
 * X Unsigned hexadecimal integer (capital letters)  (Ex:.7FA)
 */
extern int16 String_set(String *this, const char *fmt, ...);

/**
 * Function: String_setCase
 * @purpose Convert all of the characters in the String to lower or upper case
 * @param *this : The string to be set
 * @param caseToSet : The case to set, upper or lower
 * @return
 *  - int16 : SUCCESS, or ERROR if any occur
 */
extern int16 String_setCase(String *this, uint16 caseToSet);

/**
 * Function: String_substring
 * @purpose Returns a substring within the specified an index range
 * @param *this : The source string to be subsequenced
 * @param startIndex : The starting index, inclusive
 * @param endIndex : The ending index, exclusive
 * @return
 *  - String * : A pointer to the substring, null if any ERROR occurs
 */
extern String * String_substring(const String *this, uint16 startIndex, uint16 endIndex);

/**
 * Function: String_trim
 * @purpose Returns a string, removing all leading and trailing whitespaces
 * from the string
 * @param *this : The source string to be trimmed
 * @return
 *  - String * : The New string, removing all leading and trailing whitespaces
 * from the string, null if any ERROR occurs
 */
extern String * String_trim(const String *this);

/**
 * Function: String_valueOf
 * @purpose Returns the string representation of a number argument
 * @param number : The number to be converted
 * @param *fmt : The format of the number according to the printf
 * function
 * @return
 * - char * : A pointer to the string instance, representation of the number
 *
 * Notes:
 *
 * This function returns a instance of a character sequence
 */
extern char * String_valueOf(double number, const char *fmt);

#endif /* STRING_H_ */

