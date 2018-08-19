/*******************************************************************************
* Project:        C-Compass 1.0.0
* File:           DefTypes.h
* Purpose:        This file defines all safe types to be used in all embedded
*                 systems. TODO Requires further study to correctly have this
*                 types in ANSI-C and preferebly following MISRA-C guidelines.
* Author:         Hugo Saporetti Junior
* Creation date:  Aug 4, 2009
* ANSI:           yes
* -----------------------------------------------------------------------------
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
******************************************************************************/
#ifndef DEFTYPES_H_
#define DEFTYPES_H_

/* Includes ---------------------------------------------------------------- */
#include <stddef.h>
/* ------------------------------------------------------------------------- */

#if defined __x86_64__
#define __HAS_LONG_LONG
#endif

#ifdef null
#undef null
#endif
#define null NULL                   /* boolean m_value true */

#ifdef true
#undef true
#endif
#define true 1                      /* boolean m_value true */

#ifdef false
#undef false
#endif
#define false 0                     /* boolean m_value false */

#ifdef SUCCESS
#undef SUCCESS
#endif
#define SUCCESS 0                   /* Successful return status */

#ifdef ERROR
#undef ERROR
#endif
#define ERROR -1                    /* Failing return status */

#ifndef EXIT_FAILURE
#define EXIT_FAILURE  1             /* Failing exit status.  */
#endif

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS  0             /* Successful exit status.  */
#endif

#ifdef boolean
#undef boolean
#endif
typedef unsigned char boolean;      /* Boolean type */


/* Safe types */

#ifndef schar
typedef signed char schar;          /* 8 bit, signed char type */
#endif

#ifndef uchar
typedef unsigned char uchar;        /* 8 bit, unsigned char type */
#endif

#ifndef int8
typedef signed char int8;           /* 8 bit, signed integer type */
#endif

#ifndef uint8
typedef unsigned char uint8;        /* 8 bit, unsigned integer type */
#endif

#ifndef int16
typedef signed short int int16;     /* 16 bit, signed integer type */
#endif

#ifndef uint16
typedef unsigned short int uint16;  /* 16 bit, unsigned integer type */
#endif

#ifndef int32
typedef signed long int int32;      /* 32 bit, signed integer type */
#endif

#ifndef uint32
typedef unsigned long int uint32;   /* 32 bit, unsigned integer type */
#endif

#ifndef word
typedef uint32 word;                /* 32 bit Word */
#endif

/* If the processor supports long long */

#if defined __HAS_LONG_LONG
#ifndef int64
typedef signed long long int64;     /* 64 bit, signed integer type */
#endif
#ifndef uint64
typedef unsigned long long uint64;  /* 64 bit, unsigned integer type */
#endif
typedef int64 dword;                /* Double 32 bit Word */
#endif

#endif /* DEFTYPES_H_ */

