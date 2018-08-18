/*******************************************************************************
* Project:        CRSwE_Library_V_1_1_beta
* File:           String.c
* Purpose:        Implementation of C-ANSI class String
* Author:         Hugo Saporetti Junior
* Creation date:  Aug 6, 2009
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
#include <ctype.h>

#include "CRSwE.h"
#include "String.h"
/* --------------------------------------------------------------------------*/

/**
 * Function: String_new
 * @purpose: Constructs a String object
 * @param *_class : A pointer the this class descriptor
 * @param *parameters : The parameter list for this constructor
 * @return void * : A pointer to the instance of String newly created
 */
void * String_new(void *_class, va_list *parameters) {
  String *this = _class;
  const char *text = va_arg(*parameters, const char*);
  assert(this);
  assert(text);
  ALLOC(this->m_text, strlen(text) + 1);
  assert(this->m_text);
  String_set(this, text);

  this->clear = String_clear;
  this->compareTo = String_compareTo;
  this->compareToIgnoreCase = String_compareToIgnoreCase;
  this->concatChar = String_concatChar;
  this->concatText = String_concatText;
  this->contains = String_contains;
  this->copy = String_copy;
  this->replace = String_replace;
  this->replaceAll = String_replaceAll;
  this->set = String_set;
  this->setCase = String_setCase;
  this->substring = String_substring;
  this->trim = String_trim;
  this->valueOf = String_valueOf;

  return this;
}


/**
 * Function: String_delete
 * @purpose: Destructs a String object
 * @param *this : A pointer the object to be destroyed
 * @return void * : A pointer to null if the object was successfully deleted
 */
void String_delete(void *_this) {
  String *this = _this;
  assert(this);
  FREE(this->m_text);
}


/**
 * Function: String_clone
 * @purpose: Creates and returns a copy of a String object
 * @param *_this : A pointer the object to be cloned
 * @return void * : A pointer to the cloned object
 */
void * String_clone(const void *_this) {
  const String *this = _this;

  return String(this->m_text);
}


/**
 * Function: String_equals
 * @purpose: Indicates whether some other object is "equal to" another
 * @param *_this : A pointer the object to be compared
 * @param *_other :The reference object with which to compare
 * @return boolean : true if this object is the same as the obj argument; false
 * otherwise
 */
boolean String_equals(const void *_this, const void *_other) {
  const String *this = _this;
  const String *other = _other;
  if (this == other) {
    return true;
  }
  if (!other || other->d_class != d_String) {
    return false;
  }

  return (strcmp(this->m_text, other->m_text) == 0 ? true : false);
}


/**
 * Function: String_getClass
 * @purpose: TODO Comment
 * @param *_this : A pointer the object to get the class descriptor
 * @return
 * - Class * : TODO Comment
 */
Class * String_getClass(const void * _this) {
  const String *this = _this;

	return this->d_class;
}


/**
 * Function: String_sizeOf
 * @purpose: Returns the m_size in bytes of the given class
 * @param this : TODO Param description
 * @return return_type: TODO Describe return m_type here
 */
size_t String_sizeOf(const void *_this) {
  const String *this = _this;

  return sizeof(String) + this->m_capacity;
}


/**
 * Function: String_toString
 * @purpose: Returns a cstring representation of the object
 * @param *_this : The reference object
 * @return char * : A pointer to the string representation of the object
 */
char * String_toString(const void *_this) {
  const String *this = _this;

  return this->m_text;
}

ClassDescriptor(String);

/* See String.h for documentation about the functions below */

void String_clear(String *this) {
  if (!this) {
    return;
  }
  String_resize(this, 0);
}

int16 String_compareTo(const String *this, const String *string) {
  if (!this || !string) {
    return ERROR;
  }

  return strcmp(this->m_text, string->m_text);
}


int16 String_compareToIgnoreCase(const String *this, const String *string) {
  int16 result = ERROR;
  String *cmpString1 = null;
  String *cmpString2 = null;
  if (!this || !string) {
    return ERROR;
  }
  cmpString1 = clone(this);
  cmpString2 = clone(string);
  String_setCase(cmpString1, TC_LOWERCASE);
  String_setCase(cmpString2, TC_LOWERCASE);
  result = strcmp(cmpString1->m_text, cmpString2->m_text);
  delete(cmpString1);
  delete(cmpString2);

  return result;
}

int16 String_concatChar(String *this, uint16 chr) {
  if (!this || chr == '\x0') {
    return ERROR;
  }
  if (this->m_length + 1 >= this->m_capacity) {
    String_resize(this, this->m_length + 1);
  }
  this->m_text[this->m_length] = chr;
  this->m_text[this->m_length+1] = '\x0';
  this->m_length += 1;

  return SUCCESS;
}

int16 String_concatText(String *this, const char *text) {
  uint16 addLength = 0;
  if (!this || !text || *(text) == '\x0') {
    return ERROR;
  }
  addLength = strlen(text);
  if (this->m_length + addLength >= this->m_capacity) {
    String_resize(this, this->m_length + addLength + 1);
  }
  strncat(this->m_text, text, addLength);
  this->m_length += addLength;

  return SUCCESS;
}

boolean String_contains(const String *this, const char *text) {
  char *pStr = null;
  if (!this || !text || *(text) == '\x0') {
    return false;
  }
  if (!(pStr = strstr(this->m_text, text)) || this->m_length <= 0 || strlen(text) <=0 ) {
    return false;
  }
  else {
    return true;
  }
}

int16 String_copy(String *dest, const String *source) {
  if (!source || !dest) {
    return ERROR;
  }
  String_set(dest, source->m_text);

  return SUCCESS;
}

String * String_replace(const String *this, uint16 startIndex, uint16 endIndex, const char *replacement) {
  char *pSrc = null;
  char *pThis = null;
  String *replStr = null;
  if (!this || !replacement || startIndex < 0 || startIndex > endIndex || endIndex > this->m_length) {
    return null;
  }
  pThis = this->m_text;
  pSrc = this->m_text + startIndex;
  replStr = String("");
  while (pThis && pThis != pSrc) {
    String_concatChar(replStr, *pThis);
    pThis++;
  }
  String_concatText(replStr, replacement);
  pThis = this->m_text + endIndex;
  String_concatText(replStr, this->m_text + endIndex);

  return replStr;
}

String * String_replaceAll(const String *this, const char *text, const char *replacement) {
  String *replStr = null;
  char *pText = null;
  char *pSrc = this->m_text;
  uint16 textLen = 0;
  uint16 replLen = 0;
  int16 addLen = 0;

  textLen = strlen(text);
  replLen = strlen(replacement);
  pText = strstr(pSrc, text);
  if (pText == null) {
    return null;
  }
  replStr = String("");

  while (pText) {
    addLen = pText-pSrc;
    String_resize(replStr, replStr->m_length + replLen + addLen);
    strncat(replStr->m_text, pSrc, addLen);
    strncat(replStr->m_text, replacement, replLen);
    pSrc = pText + textLen;
    replStr->m_length += replLen + addLen;
    pText = strstr(pText + textLen, text);
  }
  if (*pSrc) {
    addLen = strlen(pSrc);
    String_resize(replStr, replStr->m_length + addLen);
    strncat(replStr->m_text, pSrc, addLen);
    replStr->m_length += addLen;
  }

  return replStr;
}


int16 String_resize(String *this, size_t len) {
  uint16 reqCapacity = 0;
  uint16 estLength = 0;
  if (!this) {
    return ERROR;
  }
  else if (len == 0) {
    memset(this->m_text, '\x0', this->m_capacity);
    REALLOC(this->m_text, STR_GROWTH + 1);
    this->m_capacity = STR_GROWTH;
    this->m_length = 0;
    return SUCCESS;
  }
  else {
    estLength = (len <= 1 ? 1 : len);
    reqCapacity = ((estLength / STR_GROWTH) + 1) * STR_GROWTH;
    if (reqCapacity == this->m_capacity) {
      return SUCCESS;
    }
    REALLOC(this->m_text, reqCapacity + 1);
    if (this->m_capacity < reqCapacity) {
      memset(this->m_text + this->m_length, '\x0', (reqCapacity - this->m_length));
    }
    else {
      this->m_text[len] = '\x0';
    }
    this->m_capacity = reqCapacity;
  }

  return SUCCESS;
}

int16 String_set(String *this, const char *fmt, ...) {
  char numFmt[] = { "%\x0\x0\x0\x0\x0\x0" };
  char *pStr = null;
  String *pCStr = null;
  int16 iNum = 0;
  float fNum = 0.0f;
  uint16 result = 0, fmtOffset = 0, cBase = 0;
  va_list args;
  if (!this || !fmt || *(fmt) == '\x0') {
    return ERROR;
  }
  String_clear(this);
  va_start(args, fmt);
  for (; *fmt; ++fmt) {
    switch (*fmt) {
      case FS_BEGIN:
        switch (*++fmt) {
          case FS_BEGIN:
            result += String_concatChar(this, *fmt);
          break;
          case FS_STRING:
            pCStr = va_arg(args, String*);
            if (pCStr->m_text) {
              result += String_concatText(this, pCStr->m_text);
            }
          break;
          case FS_CHAR_SEQ:
            pStr = va_arg(args, char*);
            if (pStr) {
              result += String_concatText(this, pStr);
            }
          break;

          default:
            fmtOffset = strcspn(fmt, STR_NUM_SPECIFIERS);
            if (fmtOffset >= 7) {
              System.err.println("String_setFormatted: Invalid fmt \'%%%c\'", *fmt);
              return ERROR;
            }
            strncpy(numFmt, "%\x0\x0\x0\x0\x0\x0", 7);
            strncat(numFmt, fmt, fmtOffset + 1);
            cBase = *(fmt + fmtOffset);
            if (cBase == FS_DECIMAL || cBase == FS_HEXA_LOW || cBase == FS_HEXA_HIGH ||
                cBase == FS_OCTAL || cBase == FS_PTR_ADDR || cBase == FS_CHAR) {
              iNum = va_arg(args, int);
              pStr = String_valueOf(iNum, numFmt);
            } else if (cBase == FS_FLOAT) {
              fNum = (float) va_arg(args, double);
              pStr = String_valueOf(fNum, numFmt);
            } else {
              ALLOC(pStr, 7);
              sprintf(pStr, "<null>");
            }
            if (pStr) {
              result += String_concatText(this, pStr);
              FREE(pStr);
            }
            fmt += fmtOffset;
          break;
        }
        continue;
      break;

      default:
        result += String_concatChar(this, *fmt);
      break;
    }
  }
  va_end(args);

  return result;
}

int16 String_setCase(String *this, uint16 caseToSet) {
  int (*caseFunc)(int) = null;
  uint16 i = 0;
  uint16 chr = 0;
  if (!this) {
    return ERROR;
  }
  switch(caseToSet) {
    case TC_UPPERCASE:
      caseFunc = toupper;
    break;
    case TC_LOWERCASE:
      caseFunc = tolower;
    break;

    default:
      return ERROR;
  }
  while (this->m_text[i]) {
    chr = this->m_text[i];
    this->m_text[i] = (*caseFunc)(chr);
    i++;
  }

  return SUCCESS;
}

String * String_substring(const String *this, uint16 startIndex, uint16 endIndex) {
  String *subStr = null;
  char *pThis = null, *pStr = null;
  if (startIndex < 0 || startIndex > this->m_length || endIndex < startIndex) {
    return null;
  }
  pThis = this->m_text + startIndex;
  pStr = this->m_text + endIndex;
  subStr = String("");
  while (pThis <= pStr) {
    String_concatChar(subStr, *(pThis++));
  }

  return subStr;
}

String * String_trim(const String *this) {
  char *pSrcStart = null;
  char *pSrcEnd = null;
  String *resultStr = null;
  if (!this) {
    return resultStr;
  }
  pSrcStart = this->m_text;
  pSrcEnd = this->m_text + (this->m_length - 1);
  resultStr = String("");
  while (pSrcStart && *(pSrcStart) == ' ') {
    pSrcStart++;
  }
  while (pSrcEnd && *(pSrcEnd) == ' ') {
    pSrcEnd--;
  }
  while (pSrcStart != pSrcEnd) {
   String_concatChar(resultStr, *(pSrcStart++));
  }
  if (*(pSrcEnd) != '\x0' && *(pSrcEnd) != ' ') {
    String_concatChar(resultStr, *pSrcEnd);
  };

  return resultStr;
}

char * String_valueOf(double number, const char *fmt) {
  char *strValue = null;
  char numFmt[] = {"%\x0\x0\x0\x0\x0\x0"};
  size_t fmtOffset = 0;
  size_t len = 0;
  fmtOffset = strcspn (fmt, STR_ALL_SPECIFIERS);
  if (fmtOffset >= 7 || fmtOffset < 1) {
    return null;
  }
  strncpy(numFmt, fmt, fmtOffset + 1);
  ALLOC(strValue, MAXLEN_LITERAL_NUM);
  if ( *(fmt + fmtOffset) == FS_FLOAT) {
    len = sprintf(strValue, numFmt, number);
  }
  else {
    len = sprintf(strValue, numFmt, (int16)number);
  }
  if (len <= 0) {
    FREE(strValue);
    return null;
  }

  return strValue;
}


