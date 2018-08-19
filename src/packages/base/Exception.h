/*******************************************************************************
* Project:        C-Compass 1.0.0
* File:           Exception
* Purpose:        Public prototypes for Exception C-ANSI class
* Author:         Hugo Saporetti Junior
* Creation date:  Aug 19, 2009
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
#ifndef EXCEPTION_
#define EXCEPTION_

/* Includes ----------------------------------------------------------------- */
#include <signal.h>
#include <errno.h>
#include <setjmp.h>

#include "CRSwE.h"
#include "String.h"
/* -------------------------------------------------------------------------- */

/* Usefull Macro to shorcut the constructor method */
#define Exception(msg, cause, type)   _NEW_(Exception, msg, cause, type, __LINE__, __FILE__)

/** @purpose MACRO that implements a return for a CATCH statement. If this macro
 * is called, the code after the Exception was thrown will not be executed and
 * the finally statement will be called, if it exist in the code. It a simple
 * return is assigned, the remaining code after the m_line where the Exception was
 * thrown will be executed normally and the finally block will not be called. */
#define RETRY(type)         longjmp(Runtime.restorePoint, type)

/* TODO Comment */
#define SAVE_PROG_STATE     (Runtime.resultRestore = setjmp(Runtime.restorePoint))

/**
 * @purpose MACRO that implements a TRY statement. The TRY/CATCH statement
 * encloses some code and is used to handle errors and exceptions that might
 * occur in that code.
 */
#define TRY(code)           if (SAVE_PROG_STATE == 0) { \
                              void _tryBlock_(void) { \
                                code \
                              } _tryBlock_(); \
                            } \

/**
 * @purpose MACRO that implements a CATCH statement. The TRY/CATCH statement
 * encloses some code and is used to handle errors and exceptions that might
 * occur in that code.
 */
#define CATCH(tp, e, code)  if (Runtime.resultRestore == tp) { \
                              void _catchBlock_(void) { \
                                System.out.printDebug(DBG_ERROR, "[Exception] Exception caught (line:%d)", __LINE__); \
                                e = (Exception*)Runtime.ex.lastException; \
                                Runtime.ex.lastException->m_wasCaught = true; \
                                code \
                              } _catchBlock_(); \
                            }

/** TODO Comment */
#define THROW(e)            System.out.printDebug(DBG_ERROR, "[Exception] Thrown exception (line:%d)", __LINE__); \
                            e->m_line = (uint16*)__LINE__; \
                            e->m_file = (uint16*)__FILE__; \
                            Exception_push(e); \
                            RETRY((int)e->m_type);

/** @purpose Pre-defined error messages according to the DefaultExceptions in Excptions.h */
extern const char *runtimeErrMsgs[];

/** @purpose Defined m_type SignalFnc, represents a signaling function */
typedef void (*SignalFnc)(int);

/* Definition of the Exception C-ANSI class */
class (Exception,
  String *m_msg;          /** @purpose TODO */
  Exception *m_prev;      /** @purpose TODO */
  Exception *m_cause;     /** @purpose TODO */
  const uint16 *m_type;   /** @purpose TODO */
  const uint16 *m_line;   /** @purpose TODO */
  const uint16 *m_file;   /** @purpose TODO */
  boolean m_wasCaught;    /** @purpose TODO */

  /* Public Methods */
  void (*printStackTrace)(void);
  const String * (*getMessage)(const Exception *);
);

/* TODO Comentar */
struct __RuntimeEnv {
  struct __ExceptionStack {
    Exception *lastException;
    boolean releaseStackFlag;
  } ex;
  int16 resultRestore;
  jmp_buf restorePoint;
};


/* TODO Comentar */
extern struct __RuntimeEnv Runtime;


/** @purpose Default Exception types */
typedef enum {
  SEGMENTATION_FAULT    = 0,  /***/
  UNCAUGHT_EXCEPTION    = 1,  /***/
  PROGRAM_TERMINATED    = 2,  /***/
  PROGRAM_ABORTED       = 3,  /***/
  ILLEGAL_INSTRUCTION   = 4,  /***/
  FLOAT_POINT_EXCEPTION = 5,  /***/
} RuntimeExceptions;


/* Declare all public prototypes here */

extern void __initExceptionSignals(void);

/** @purpose TODO Comentar */
extern void Exception_push(Exception * this);

/** @purpose TODO Comentar */
extern Exception * Exception_pop(void);

/** @purpose TODO Comentar */
extern void Exception_releaseStack(void);

/** @purpose TODO Comentar */
extern void Exception_catch(void);

/** @purpose TODO Comentar */
extern const String * Exception_getMessage(const Exception *this);

/** @purpose TODO Comentar */
extern void Exception_printStackTrace(void);

#endif /* EXCEPTION_ */

