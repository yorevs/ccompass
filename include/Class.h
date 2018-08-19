/*******************************************************************************
 * Project:        CRSwE_Library_V_1_0_Beta
 * File:           Class.h
 * Purpose:        Prototypes for Class.c functions
 * Author:         Hugo Saporetti Junior
 * Creation date:  Jul 4, 2008
 * ANSI:           yes
 * -----------------------------------------------------------------------------
 * Developed by: Hugo Saporetti Junior
 * Access: https://github.com/yorevs/ccompass
 * Copyright (C) 2018 Hugo Saporetti Junior
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
#ifndef CLASS_H_
#define CLASS_H_

/* Includes ----------------------------------------------------------------- */
#include <stdarg.h>

#include "DefTypes.h"
#include "System.h"
/* -------------------------------------------------------------------------- */

/*******************************************************************************
 * DEFINED MACROS
 ******************************************************************************/

/* Useful Macro to shortcut the Object constructor method */
#define Object()                    _NEW_(Object, null)

/** @purpose Defines the maximum length a class name could have */
#define MAX_CLASS_NAME              60
/** @purpose Makes x a string */
#define MakeStr(x)                  # x
/** @purpose Make the class name according to the Struct name */
#define ClassName(x,y)              "Class: " MakeStr(x) " at " y

/** @purpose TODO Comment */
#ifdef _NEW_
#undef _NEW_
#endif
#define _NEW_(className, ...)       (className *) __new(d_ ## className ,__LINE__, (uint16*)ClassName(className, __FILE__), __VA_ARGS__)

/** @purpose TODO Comment */
#ifdef delete
#undef delete
#endif
#define delete(obj)                 __delete(obj, __LINE__, (uint16*)ClassName(obj, __FILE__));obj = null

/** @purpose TODO Comment */
#ifdef clone
#undef clone
#endif
#define clone                       __clone

/** @purpose TODO Comment */
#ifdef equals
#undef equals
#endif
#define equals(this, other)         __equals(this, other)

/** @purpose TODO Comment */
#ifdef finalize
#undef finalize
#endif
#define finalize(obj)               __finalize(obj)

/** @purpose TODO Comment */
#ifdef getClass
#undef getClass
#endif
#define getClass(obj)               __getClass(obj)

/** @purpose TODO Comment */
#ifdef sizeOf
#undef sizeOf
#endif
#define sizeOf                      __sizeOf

/** @purpose TODO Comment */
#ifdef toString
#undef toString
#endif
#define toString(value)             __toString(value)

/** @purpose TODO Comment */
#ifdef extends
#undef extends
#endif
#define extends(superClass)         superClass *super; /* Inherited class */

/** @purpose TODO Comment */
#ifdef super
#undef super
#endif
#define super(this)                 this->super /* Access the super class */

/** @purpose TODO Comment */
/* class MACRO definition { */
#ifdef class
#undef class
#endif
#define class(className, classScope)                                          \
/* Defined Type */                                                            \
typedef struct className className;                                           \
                                                                              \
struct className {                                                            \
  /* Required object. Keeps information about this class type. */             \
  Class * d_class;                                                            \
  /* Here goes the scope of the class */                                      \
  classScope                                                                  \
};                                                                            \
                                                                              \
/* A declaration for this class descriptor */                                 \
extern const Class * d_##className;
/* } */


/* ClassDescriptor MACRO definition { */
#ifdef ClassDescriptor
#undef ClassDescriptor
#endif
#define ClassDescriptor(className)                                                 \
/* This is the Class descriptor. Keeps information about this class type. */  \
static const Class __##className = {                                          \
  sizeof(className),                                                          \
  MakeStr(className),                                                         \
  className##_new,className##_delete,                                         \
  className##_clone,className##_equals,                                       \
  className##_getClass,                                                       \
  className##_sizeOf,className##_toString                                     \
};                                                                            \
/* Public class descriptor object */                                          \
const Class * d_##className = (const Class*)& __##className
/* } */


/** @purpose TODO Comment */
#ifdef hashCode
#undef hash
#endif
#define hashCode(obj)               Object_hashCode(obj)


/*****************************************************************************
 * Class definitions
 ****************************************************************************/

/* Defined m_type Class */
typedef struct Class Class;

/* Defined m_type Object */
typedef struct Object Object;


/* A declaration for this class descriptor */
extern const Class * d_Object;

/**
 * @purpose Definition of the className C-ANSI class
 */
struct Object {
  /* Required object. Keeps information about this class m_type. */
  Class * d_class;

  /* Public Methods */
  uint32 (*hashCode)(const void *this);
};


/**
 *  @purpose TODO
 */
struct Class {
  /* Attributes */
  size_t m_size;
  uchar m_name[MAX_CLASS_NAME];

  /* Methods */
  void* (*__ctor)(void *, va_list *);             /** @purpose Constructor */
  void (*__dtor)(void *);                         /** @purpose Destructor */
  void* (*__clone)(const void *);                 /** @purpose Clone method */
  boolean (*__equals)(const void *, const void *);/** @purpose Equals method */
  Class* (*__getClass)(const void *);             /* TODO REMOVER ??? */
  size_t (*__sizeOf)(const void *);               /** @purpose sizeOf method */
  char* (*__toString)(const void *);              /** @purpose toString method*/
};

/**
 * Function: __new
 * @purpose: TODO: Function's purpose here
 * @param d_class : TODO Param description
 * @param m_line : The invoking m_line number
 * @param *file : The invoking file name and path
 * @return return_type: TODO Describe return m_type here
 */
extern void * __new(const void *d_class, uint16 line, const uint16 *file, ...);

/**
 * Function: __delete
 * @purpose: TODO: Function's purpose here
 * @param this : TODO Param description
 * @param m_line : The invoking m_line number
 * @param *file : The invoking file name and path
 * @return return_type: TODO Describe return m_type here
 */
extern void __delete(void *this, uint16 line, const uint16 *file);

/**
 * Function: __clone
 * @purpose: Creates and returns a copy of a Object object
 * @param *_this : A pointer the object to be cloned
 * @return void * : A pointer to the cloned object
 */
extern void * __clone(const void *_this);

/**
 * Function: __equals
 * @purpose: Indicates whether some other object is "equal to" another
 * @param *_this : A pointer the object to be compared
 * @param *_other :The reference object with which to compare
 * @return boolean : true if this object is the same as the obj argument; false otherwise
 */
extern boolean __equals(const void *_this, const void *other);

/**
 * Function: __finalize
 * @purpose: TODO Comment
 * @param *_this : A pointer the object to get the class descriptor
 * @return
 * - void * : TODO Comment
 */
extern Class * __getClass(const void * _this);

/**
 * Function: __sizeOf
 * @purpose: Returns the m_size of the given class
 * @param this : TODO Param description
 * @return return_type: TODO Describe return m_type here
 */
extern size_t __sizeOf(const void *_this);

/**
 * Function: __toString
 * @purpose: Returns a cstring representation of the object
 * @param *_this : The reference object
 * @return uint16 * : A pointer to the string representation of the object
 */
extern char * __toString(const void *_this);

/*****************************************************************************
 * Class Object
 ****************************************************************************/

/* Declare all public prototypes here */

/**
 * Function: Object_scrambleHash
 * @purpose: Scrambles the hash code to avoid repetition, according to a prime
 * based algorithm
 * @param hash : A pointer the object to calculate the hash code
 * @return
 * - uint32 : the scrambled hash code
 */
extern uint32 Object_scrambleHash(uint32 hash);

/**
 * Function: Object_hashCode
 * @purpose: Returns a hash code m_value for the object. This method is supported
 * for the benefit of hash-tables such as those provided by HashMap.
 * @param *this : A pointer to the object to calculate the hash code
 * @return
 * - uint32 : the generated hash code
 */
extern uint32 Object_hashCode(const void *this);

#endif /* CLASS_H_ */

