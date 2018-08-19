/*******************************************************************************
* Project:        C-Compass 1.0.0
* File:           Exception.c
* Purpose:        Implementation of C-ANSI class Exception
* Author:         Hugo Saporetti Junior
* Creation date:  Aug 20, 2009
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

/* Includes ------------------------------------------------------------------*/
#include <stdlib.h>
#include <assert.h>

#include "Compass.h"
#include "Exception.h"
/* --------------------------------------------------------------------------*/

/**
 * @purpose Pre-defined runtime error messages according to DefaultExceptions in
 *  Exceptions.h
 */
const char * runtimeErrMsgs[] = {
  "Segmentation fault",
  "Uncaught exception",
  "Program abnormally terminated",
  "Program aborted by the user",
  "Illegal instruction",
  "Floating point exception"
};

/* TODO */
struct __RuntimeEnv Runtime = {
 {
   null,  /* Last Exception */
   true,  /* Release Stack Flag */
 },
 0 /* Result of a Restore call */
};

/**
 * Function: __uncaught
 * @purpose: TODO Comentar
 * @param sigNum : TODO Comentar
 * @return none
 */
void __uncaught (int sigNum) {
  if (signal(SIGINT, __uncaught) == SIG_ERR) {
   System.err.println("Unable to setup signal handler: SIGINT");
   exit(-1);
  }
  RETRY(2);
}

/**
 * Function: __segFault
 * @purpose: TODO Comentar
 * @param sigNum : TODO Comentar
 * @return none
 */
void __segFault (int sigNum) {
  System.err.println("Runtime Exception: %s", runtimeErrMsgs[SEGMENTATION_FAULT]);
  if (signal(SIGSEGV, __segFault) == SIG_ERR) {
   System.err.println("Unable to setup signal handler: SIGSEGV");
  }
  exit(-1);
}

/**
 * Function: __terminate
 * @purpose: TODO Comentar
 * @param sigNum : TODO Comentar
 * @return none
 */
void __terminate (int sigNum) {
  System.err.println("Runtime Exception: %s", runtimeErrMsgs[PROGRAM_TERMINATED]);
  if (signal(SIGTERM, __terminate) == SIG_ERR) {
   System.err.println("Unable to setup signal handler: SIGTERM");
  }
  exit(-1);
}

/**
 * Function: __abort
 * @purpose: TODO Comentar
 * @param sigNum : TODO Comentar
 * @return none
 */
void __abort (int sigNum) {
  System.err.println("Runtime Exception: %s", runtimeErrMsgs[PROGRAM_ABORTED]);
  if (signal(SIGABRT, __abort) == SIG_ERR) {
   System.err.println("Unable to setup signal handler: SIGABRT");
  }
  exit(-1);
}

/**
 * Function: __fpe
 * @purpose: TODO Comentar
 * @param sigNum : TODO Comentar
 * @return none
 */
void __fpe (int sigNum) {
  System.err.println("Runtime Exception: %s", runtimeErrMsgs[FLOAT_POINT_EXCEPTION]);
  if (signal(SIGFPE, __fpe) == SIG_ERR) {
   System.err.println("Unable to setup signal handler: SIGFPE");
  }
  exit(-1);
}

/**
 * Function: __illegal
 * @purpose: TODO Comentar
 * @param sigNum : TODO Comentar
 * @return none
 */
void __illegal (int sigNum) {
  System.err.println("Runtime Exception: %s", runtimeErrMsgs[ILLEGAL_INSTRUCTION]);
  if (signal(SIGILL, __illegal) == SIG_ERR) {
   System.err.println("Unable to setup signal handler: SIGILL");
  }
  exit(-1);
}

/**
 * Function: __initExceptionSignals
 * @purpose: TODO Comentar
 * @return none
 */
void __initExceptionSignals(void) {
  /* set up the first restore point */
  SAVE_PROG_STATE;
  /* set up the handler for the abort signal */
  if (signal(SIGABRT, __abort) == SIG_ERR) {
    System.err.println("Unable to setup signal handler: SIGIARBT");
    exit(-1);
  }
  /* set up the handler for the abort signal */
  if (signal(SIGFPE, __fpe) == SIG_ERR) {
    System.err.println("Unable to setup signal handler: SIGFPE");
    exit(-1);
  }
  /* set up the handler for the illegal instruction signal */
  if (signal(SIGILL, __illegal) == SIG_ERR) {
   System.err.println("Unable to setup signal handler: SIGILL");
   exit(-1);
  }
  /* set up the handler for Throw events */
  if (signal(SIGINT, __uncaught) == SIG_ERR) {
    System.err.println("Unable to setup signal handler: SIGINT");
    exit(-1);
  }
  /* set up the handler for segmentation faults */
  if (signal(SIGSEGV, __segFault) == SIG_ERR) {
    System.err.println("Unable to setup signal handler: SIGSEGV");
    exit(-1);
  }
  /* set up the handler for the terminate signal */
  if (signal(SIGTERM, __terminate) == SIG_ERR) {
    System.err.println("Unable to setup signal handler: SIGTERM");
    exit(-1);
  }
  System.out.printDebug(DBG_INFO, "[EXCEPTION] Exception Signals Initialized!");
}


/**
 * Function: Exception_new
 * @purpose: Constructs a Exception object
 * @param *_class : A pointer the this class descriptor
 * @param *parameters : The parameter list for this constructor
 * @return void * : A pointer to the instance of Exception newly created
 */
void * Exception_new(void *_class, va_list *parameters) {
	Exception *this = _class;
	assert(this);
	char *msg = (char*) va_arg(*parameters, char *);
	this->m_cause = (Exception *) va_arg(*parameters, Exception *);
	this->m_type = (const uint16*) va_arg(*parameters, uint16 *);
	this->m_line = (const uint16*) va_arg(*parameters, uint16 *);
	this->m_file = (const uint16*) va_arg(*parameters, uint16 *);
	assert(msg);
	assert(this->m_type);
	assert(this->m_line);
	assert(this->m_file);
	this->m_msg = String(msg);
	assert(this->m_msg);
	this->m_wasCaught = false;
	this->printStackTrace = Exception_printStackTrace;
	this->getMessage = Exception_getMessage;

	return this;
}


/**
 * Function: Exception_delete
 * @purpose: Destructs a Exception object
 * @param *this : A pointer the object to be destroyed
 * @return void * : A pointer to null if the object was successfully deleted
 */
void Exception_delete(void *_this) {
	Exception *this = _this;
	assert(this);
	delete(this->m_msg);
}


/**
 * Function: Exception_clone
 * @purpose: Creates and returns a copy of a Exception object
 * @param *_this : A pointer the object to be cloned
 * @return void * : A pointer to the cloned object
 */
void * Exception_clone(const void *_this) {
	const Exception *this = _this;

  return Exception(this->m_msg, this->m_cause, this->m_type);
}


/**
 * Function: Exception_equals
 * @purpose: Indicates whether some other object is "equal to" another
 * @param *_this : A pointer the object to be compared
 * @param *_other :The reference object with which to compare
 * @return int : true if this object is the same as the obj argument; false otherwise
 */
boolean Exception_equals(const void *_this, const void *_other) {
  const Exception *this = _this;
  const Exception *other = _other;
  if (this == other) {
    return true;
  }
  if (!other || other->d_class != d_Exception) {
    return false;
  }

  return ((this->m_type == other->m_type) && equals(this->m_msg, other->m_msg));
}


/**
 * Function: Exception_getClass
 * @purpose: TODO Comment
 * @param *_this : A pointer the object to get the class descriptor
 * @return
 * - Class * : the Class descriptor for this class
 */
Class * Exception_getClass(const void * _this) {
  const Exception *this = _this;

	return this->d_class;
}


/**
 * Function: Exception_sizeOf
 * @purpose: Returns the m_size in bytes of the given class
 * @param *_this : The reference object
 * @return return_type: the m_size in bytes of the given class
 *
 * Note:
 * May return a different m_value from sizeof function
 */
size_t Exception_sizeOf(const void *_this) {
  const Exception *this = _this;

  return sizeof(Exception) + sizeOf(this->m_msg);
}


/**
 * Function: Exception_toString
 * @purpose: Returns a cstring representation of the object
 * @param *_this : The reference object
 * @return char * : A pointer to the string representation of the object
 */
char * Exception_toString(const void *_this) {
  const Exception *this = _this;

  return toString(this->m_msg);
}

ClassDescriptor(Exception);

/* See Exception.h for documentation about the functions below */

/* TODO Implement all class methods from here */

/** @purpose TODO Comentar */
void Exception_push(Exception * this) {
  assert(this);
  if (Runtime.ex.releaseStackFlag) {
    Runtime.ex.releaseStackFlag = false;
    System.out.printDebug(DBG_INFO, "[Exception] Requested to clean Exception Stack at program exit");
    atexit(Exception_releaseStack);
  }
  if (Runtime.ex.lastException != null) {
    this->m_prev = Runtime.ex.lastException;
  }
  Runtime.ex.lastException = this;
}

/** @purpose TODO Comentar */
Exception * Exception_pop(void) {
  Exception *ex = Runtime.ex.lastException;
  if (ex != null) {
    Runtime.ex.lastException = ex->m_prev;
  }

  return ex;
}

/** @purpose TODO Comentar */
void Exception_releaseStack(void) {
  Exception_catch();
  System.out.printDebug(DBG_INFO, "[Exception] Releasing exception Stack");
  while (Runtime.ex.lastException != null) {
    Exception *ex = Exception_pop();
    Exception *cause = ex->m_cause;
    while (ex != null) {
      if (System_hasInstance(ex)) {
        /* XXX Funciona, porém se tiver em NDEBUG não vai funcionar */
        delete(ex);
      }
      if (cause == null) {
        break;
      }
      ex = cause;
      cause = ex->m_cause;
    }
  }
}

/** @purpose TODO Comentar */
void Exception_catch(void) {
  Exception *exp = Runtime.ex.lastException;
  if (exp != null && !exp->m_wasCaught) {
    System.err.println(runtimeErrMsgs[UNCAUGHT_EXCEPTION]);
    Exception_printStackTrace();
  }
}

/** @purpose TODO Comentar */
const String * Exception_getMessage(const Exception *this) {

  return this->m_msg;
}

/** @purpose TODO Comentar */
void Exception_printStackTrace(void) {
  const Exception *ex = Runtime.ex.lastException;
  if (ex == null) {
    return;
  }
  const Exception *cause = ex->m_cause;
  System.err.println("\nException thrown: %s", toString(ex));
  System.err.println("\t at %s:(line %d)", ex->m_file, ex->m_line);
  while (cause != null) {
    System.err.println(" Caused by: %s", toString(cause));
    System.err.println("\t at %s:(line %d)", cause->m_file, cause->m_line);
    cause = cause->m_cause;
  }
}

