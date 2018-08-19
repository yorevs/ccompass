/*******************************************************************************
* Project:        C-Compass 1.0.0
* File:           System.h
* Purpose:        Prototypes for System.c functions
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
*******************************************************************************/
#ifndef SYSTEM_H_
#define SYSTEM_H_

/* Includes ----------------------------------------------------------------- */
#include <stdio.h>

#include "DefTypes.h"
/* -------------------------------------------------------------------------- */

/*******************************************************************************
 * DEFINED MACROS
 ******************************************************************************/

/* Uncomment this MACRO to disable debugging */
/*#define NDEBUG*/

/**
 * The macros below expands in simple functions when memory management is set to
 * be avoided, when defined NDEBUG
 */

#if ! defined NDEBUG
/** @purpose Allocate bytes of memory with management resources */
#define ALLOC(addr, nbytes)   addr = System_alloc(nbytes, __LINE__, (uint16*)__FILE__)
/** @purpose Free previously allocated memory with management resources */
#define FREE(addr)            System_free(addr, __LINE__, (uint16*)__FILE__);addr = null
/** @purpose Reallocate bytes memory with management resources */
#define REALLOC(addr, nbytes) addr = System_realloc(addr, nbytes, __LINE__, (uint16*)__FILE__)
#else
/** @purpose Allocate bytes of memory */
#define ALLOC(addr, nbytes)   addr = malloc(nbytes);addr = null
/** @purpose Free previously allocated memory */
#define FREE(addr)            free(addr)
/** @purpose Reallocate bytes memory */
#define REALLOC(addr, nbytes) addr = realloc(addr, nbytes)
#endif


/**
 * TODO Comment
 **/
enum DBG_LEVELS {
  DBG_NONE    = 0,    /* 0x00000000 */
  DBG_FATAL   = 1,    /* 0x00000001 */
  DBG_ERROR   = 2,    /* 0x00000010 */
  DBG_MEMORY  = 4,    /* 0x00000100 */
  DBG_WARN    = 8,    /* 0x00001000 */
  DBG_INFO    = 16,   /* 0x00010000 */
  DBG_DEBUG   = 32,   /* 0x00100000 */
  DBG_ALL     = 127   /* 0x01111111 */
};


#if ! defined NDEBUG
/** @purpose Defined m_type MemoryBlock */
typedef struct __MemoryBlock __MemoryBlock;

/**
 *  @purpose The struct memoryBlock and its members. represents a block of
 *  allocated memory in the memory stack
 */
struct __MemoryBlock {
  size_t nbytes;        /** The number of bytes allocated for this block */
  void *address;        /** The address allocated */
  uint16 line;          /** The m_line number where this block was allocated */
  const uint16 *file;   /** The file responsible for this block */
  __MemoryBlock *prev;  /** The m_prevState allocated memory block in stack */
  __MemoryBlock *next;  /** The m_next allocated memory block in stack */
};
#endif


/** @purpose The struct __System and its members */
struct __System {
  int16 dbgLevels;
  /* __Out methods and attributes for stdout */
  struct __Out {
      int (*print) (const char *fmt, ...);
      int (*println) (const char *fmt, ...);
      int (*printDebug) (uint16 dbgLevel, const char *fmt, ...);
  } out;
  /* __Err methods and attributes for stderr */
  struct __Err {
      int (*print) (const char *fmt, ...);
      int (*println) (const char *fmt, ...);
  } err;
#if ! defined NDEBUG
  /* Methods and attributes for memory management purposes */
  struct __Memory {
      /**
       * @purpose The struct memoryStack and its members. Global memory stack
       * which contains all allocated memory. This can be avoided by defining
       * NDEBUG, and if so, management functions will not be available.
       * Instead, simple ansi functions will take place.
       */
      struct __MemoryStack {
        __MemoryBlock *tailBlock; /** The lastException allocated block in the
                                      stack */
        size_t size;              /** The number of bytes allocated for this
                                      block */
        size_t usedMemory;        /** The amount of memory used in bytes */
        boolean isDebugging;      /** If true, enables debug messages */
        boolean isGCFlag;         /** Indicates the garbage collector must be
                                      called at the end of the program */
      } stack;
      void (*printStackTrace)(void);
      void (*setDebug) (boolean, uint16 dbgLevels);
      boolean (*isDebugging)(void);
  } mem;
#endif
};

/* TODO Comentar */
extern struct __System System;

/* Prototype declarations */

/**
 * Function: System_outPrint
 * @purpose Write formated string to stdout stream.
 * @param *fmt : The format used to write the data
 * @return
 *  - int : On success, the total number of characters written is returned.
 * Otherwise a negative number is returned.
 *
 * @Notes
 * Depending on the format string, the function may expect a sequence of
 * additional arguments, each containing one m_value to be inserted instead of
 * each %-tag specified in the format parameter, if any. There should be the
 * same number of these arguments as the number of %-tags that expect a m_value
 */
extern int System_outPrint(const char *fmt, ...);

/**
 * Function: System_outPrintln
 * @purpose Write formated string to stdout stream and appends a \n at the end.
 * @param *fmt : The format used to write the data
 * @return
 *  - int : On success, the total number of characters written is returned.
 * Otherwise a negative number is returned.
 *
 * @Notes
 * Depending on the format string, the function may expect a sequence of
 * additional arguments, each containing one m_value to be inserted instead of
 * each %-tag specified in the format parameter, if any. There should be the
 * same number of these arguments as the number of %-tags that expect a m_value
 */
extern int System_outPrintln(const char *fmt, ...);

/**
 * Function: System_outPrintDebug
 * @purpose Write formated string to stdout stream if the given debug level is
 * enabled.
 * @param dbgLevel : The debug level of the message
 * @param *fmt : The format used to write the data
 * @return
 *  - int : On success, the total number of characters written is returned.
 * Otherwise a negative number is returned.
 *
 * @Notes
 * Depending on the format string, the function may expect a sequence of
 * additional arguments, each containing one m_value to be inserted instead of
 * each %-tag specified in the format parameter, if any. There should be the
 * same number of these arguments as the number of %-tags that expect a m_value
 */
extern int System_outPrintDebug(uint16 dbgLevel, const char *fmt, ...);

/**
 * Function: System_errPrint
 * @purpose Write formated string to stderr stream.
 * @param *fmt : The format used to write the data
 * @return
 *  - int : On success, the total number of characters written is returned.
 * Otherwise a negative number is returned.
 *
 * @Notes
 * Depending on the format string, the function may expect a sequence of
 * additional arguments, each containing one m_value to be inserted instead of
 * each %-tag specified in the format parameter, if any. There should be the
 * same number of these arguments as the number of %-tags that expect a m_value
 */
extern int System_errPrint(const char *fmt, ...);

/**
 * Function: System_errPrintln
 * @purpose Write formated string to stderr stream and appends a \n at the end.
 * @param *fmt : The format used to write the data
 * @return
 *  - int : On success, the total number of characters written is returned.
 * Otherwise a negative number is returned.
 *
 * @Notes
 * Depending on the format string, the function may expect a sequence of
 * additional arguments, each containing one m_value to be inserted instead of
 * each %-tag specified in the format parameter, if any. There should be the
 * same number of these arguments as the number of %-tags that expect a m_value
 */
extern int System_errPrintln(const char *fmt, ...);

#if ! defined NDEBUG

/**
 * Function: System_memPrintStackTrace
 * @purpose Exhibits the status of used memory by the program
 * @return none
 */
extern void System_memPrintStackTrace(void);

/**
 * Function: System_setDebug
 * @purpose Enables/Disables memory debug tracing
 * @param enable : true to enable or false to disable debugging tracing
 * @param dbgLevels : enabled debug levels
 * @return none
 */
extern void System_setDebug(boolean enable, uint16 dbgLevels);

/**
 * Function: System_isDebugging
 * @purpose Check weather memory debug is set or not
 * @return
 * - boolean : true if enabled, false otherwise
 */
extern boolean System_isDebugging(void);

/**
 * Function: System_allocMemoryBlock
 * @purpose Allocates a block of memory management
 * @param addr : TODO Comment
 * @param m_prev : TODO Comment
 * @return
 * - __MemoryBlock : A pointer to the newly allocated memory block
 */
extern __MemoryBlock * System_allocMemoryBlock(void *addr, __MemoryBlock *prev);

/**
 * Function: System_findMemoryBlock
 * @purpose Searches for an allocate management block with specified address
 * @param addr : TODO Comment
 * @return
 * - __MemoryBlock : A pointer to the matching block found. null otherwise
 */
extern __MemoryBlock * System_findMemoryBlock(const void *addr);

/**
 * Function: System_freeMemoryBlock
 * @purpose Deallocates a block of memory management
 * @param block : TODO Comment
 * @return none
 */
extern void System_freeMemoryBlock(__MemoryBlock *block);

/**
 * Function: delete
 * @purpose A block of memory previously allocated using a call to is
 * deallocated, making it availbale again for further allocations.
 * @param * addr : Pointer to a memory block previously allocated with new
 * @param lineNumber : The caller m_line number
 * @param *filename : The caller file name and path to be deallocated.
 * @return none
 */
extern void System_free(void * addr, uint16 lineNumber, const uint16* filename);

/**
 * Function: System_gc
 * @purpose Checks for allocations stuck in memory and dealloctes them
 * @return none
 *
 * @Notes
 * This function can ONLY be called at the end of the program because it is
 * going to free ALL allocated memory.
 * TODO Find a way to find all references for allocated pointers and upgrade
 * this function to be able to be called in the middle of the program execution.
 */
extern void System_gc(void);

/**
 * Function: System_alloc
 * @purpose Allocates a block of nbytes of memory, returning a pointer to the
 * beginning of the block
 * @param nbytes : The Size of the memory block, in bytes
 * @param lineNumber : The caller m_line number
 * @param *filename : The caller filename
 * @return
 *  - void* : A pointer to the beginning of the newly allocated memory block
 */
extern void * System_alloc(size_t nbytes, uint16 lineNumber, const uint16* filename);

/**
 * Function: System_realloc
 * @purpose The m_size of the memory block pointed to by the ptr parameter is
 * changed to the m_size nbytes, expanding or reducing the amount of memory
 * available in the block.
 * @param *addr : Pointer to a memory block previously allocated using the
 * "System_alloc" function, to be reallocated.
 * @param nbytes : The m_size of the memory block, in bytes
 * @param lineNumber : TODO Comment
 * @param filename : TODO Comment
 * @return
 *  - void* : A pointer to the beginning of the reallocated block
 */
extern void * System_realloc(void *addr, size_t nbytes, uint16 lineNumber, const uint16 *filename);

/**
 * Function: System_hasInstance
 * @purpose Checks the given address to find if the address has allocation in
 * memory
 * @param *addr : The checking address
 * @return
 *  - void * : The address of the allocated instance or null if no allocation found
 */
extern void * System_hasInstance(const void *addr);

#endif

#endif /* SYSTEM_H_ */

