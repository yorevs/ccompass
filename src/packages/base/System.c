/*******************************************************************************
 * Project:        C-Compass 1.0.0
 * File:           System.c
 * Purpose:        Implement functions to TODO: Describe purpose here
 * Author:         Hugo Saporetti Junior
 * Creation date:  Aug 4, 2009
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
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>

#include "DefTypes.h"
#include "System.h"
/* ------------------------------------------------------------------------- */

/* TODO Comentar */
struct __System System = {
  /* Default debug levels; all enabled */
  DBG_FATAL | DBG_ERROR | DBG_MEMORY | DBG_WARN | DBG_INFO | DBG_DEBUG,
  { /* out */
    System_outPrint,
    System_outPrintln,
    System_outPrintDebug
  }
  , { /* err */
    System_errPrint,
    System_errPrintln
  }
#if ! defined NDEBUG
  , {
    /* mem */
    {
      null, 0, 0, false, true
    },
    System_memPrintStackTrace,
    System_setDebug,
    System_isDebugging
  }
#endif
};


/*
 * If NDEBUG is defined all function related to memory management will not be
 * available for use. new, ALLOC will turn into simple malloc() calls, delete
 * will be a free() and RENEW a realloc call. All debug messages will not be
 * shown as well.
 */
#if ! defined NDEBUG

void System_memPrintStackTrace(void) {
  __MemoryBlock *block = null;
  if (System.mem.stack.size == 0) {
    System.out.println("\n[Info] printStackTable: No allocation found!");
    return;
  }
  System.out.println("\n### Printing contents of the global memory stack ###");
  System.out.println("----------------------------------------------------------------------------");
  block = System.mem.stack.tailBlock;
  while (block != null) {
    System.out.println("From:[%p], To:[%p], %.3ld %cbyte(s), line: [%.4ud]\n |-> at %s", block->address, (char*) block->address
        + block->nbytes, (block->nbytes > 1024 ? block->nbytes / (block->nbytes
        > 1048576L ? 1048576L : 1024L) : block->nbytes), (block->nbytes > 1024 ? (block->nbytes
        > 1048576L ? 'M' : 'K') : ' '), block->line, block->file);
    block = block->prev;
  }
  System.out.println("\n>> Total memory used: (%.3ld) %cbytes.", (System.mem.stack.usedMemory
      > 1024 ? System.mem.stack.usedMemory / (System.mem.stack.usedMemory
      > 1048576L ? 1048576L : 1024L) : System.mem.stack.usedMemory), (System.mem.stack.usedMemory
      > 1024 ? (System.mem.stack.usedMemory > 1048576L ? 'M' : 'K') : ' '));
  System.out.println(">> Additional (%d) bytes used for management purposes.", System.mem.stack.size
      * sizeof(__MemoryBlock ));
  System.out.println("##########################################################################\n");
}

void System_setDebug(boolean enable, uint16 dbgLevels) {
  System.mem.stack.isDebugging = enable;
  if (enable) {
    System.dbgLevels = dbgLevels;
    System.out.printDebug(DBG_INFO, "[System] DEBUG MODE IS ON");
  }
  else {
    System.dbgLevels = DBG_NONE;
  }
}

boolean System_isDebugging(void) {
  return System.mem.stack.isDebugging;
}

__MemoryBlock * System_allocMemoryBlock(void *addr, __MemoryBlock *prev) {
  __MemoryBlock *newBlock = null;
  size_t blockSize = 0;
  assert(addr);
  blockSize = sizeof(__MemoryBlock );
  newBlock = (__MemoryBlock *) malloc(blockSize);
  assert (newBlock);
  newBlock->prev = prev;
  newBlock->next = null;
  newBlock->address = addr;
  if (System.mem.stack.tailBlock != null) {
    System.mem.stack.tailBlock->next = newBlock;
  }
  System.mem.stack.tailBlock = newBlock;
  System.mem.stack.size++;

  return newBlock;
}

__MemoryBlock * System_findMemoryBlock(const void *addr) {
  __MemoryBlock *searchBlock = null;
  assert(addr);
  searchBlock = System.mem.stack.tailBlock;
  while (searchBlock) {
    if (searchBlock->address == addr) {
      return searchBlock;
    }
    searchBlock = searchBlock->prev;
  }

  return searchBlock;
}

void System_freeMemoryBlock(__MemoryBlock *block) {
  assert(block);
  if (!block || System.mem.stack.size == 0) {
    return;
  }
  if (System.mem.stack.tailBlock == block) {
    System.mem.stack.tailBlock = block->prev;
    if (System.mem.stack.tailBlock != null) {
      System.mem.stack.tailBlock->next = null;
    }
  } else {
    if (block->prev) {
      block->prev->next = block->next;
    }
    if (block->next) {
      block->next->prev = block->prev;
    }
  }
  free(block);
  System.mem.stack.size--;
}

void System_free(void * addr, uint16 lineNumber, const uint16* filename) {
  __MemoryBlock *oldBlock = null;
  assert(filename);
  assert(addr);
  oldBlock = System_findMemoryBlock(addr);
  assert(oldBlock);
  free(addr);
  System.mem.stack.usedMemory -= oldBlock->nbytes;
  System.out.printDebug(DBG_MEMORY, "[System] FREE (%s-line:%d), (%d) byte(s) deallocated at address [%p]", oldBlock->file, oldBlock->line, oldBlock->nbytes, oldBlock->address);
  System_freeMemoryBlock(oldBlock);
}

void System_gc(void) {
  uint16 objStuck = 0;
  if (System.mem.stack.size == 0) {
    System.out.printDebug(DBG_INFO, "\n[GC] Garbage Collector executed successfully!", objStuck);
    return;
  }
  if (System.mem.stack.isDebugging) {
    System.mem.stack.isGCFlag = false;
    System.out.printDebug(DBG_INFO, "\n[GC] Executing Garbage Collector ...");
    System.mem.printStackTrace();
  }
  while (System.mem.stack.tailBlock) {
    if (System.mem.stack.isDebugging) {
      System.out.printDebug(DBG_MEMORY, "[GC] Found allocation at [%p]. (%d) %cbyte(s) deallocated", System.mem.stack.tailBlock->address, (System.mem.stack.tailBlock->nbytes
          > 1024 ? System.mem.stack.tailBlock->nbytes
          / (System.mem.stack.tailBlock->nbytes > 1048576L ? 1048576L : 1024L) : System.mem.stack.tailBlock->nbytes), (System.mem.stack.tailBlock->nbytes
          > 1024 ? (System.mem.stack.tailBlock->nbytes > 1048576L ? 'M' : 'K') : ' '));
    }
    objStuck++;
    System_free(System.mem.stack.tailBlock->address, __LINE__, (uint16*)__FILE__);
  }
  if (objStuck > 0) {
    System.out.println("\n[GC] Garbage Collector has cleaned (%d) memory leaks!", objStuck);
  } else {
    System.out.printDebug(DBG_INFO, "\n[GC] Garbage Collector executed successfully!", objStuck);
  }
  System.mem.stack.isGCFlag = false;
}

void * System_alloc(size_t nbytes, uint16 lineNumber, const uint16* filename) {
  void *address = null;
  __MemoryBlock *newBlock = null;
  assert(filename);
  address = malloc(nbytes);
  assert(address);
  if (System.mem.stack.isGCFlag) {
    System.mem.stack.isGCFlag = false;
    System.out.printDebug(DBG_INFO, "[System] GC Requested to run at program exit", filename, lineNumber);
    atexit(System_gc);
  }
  newBlock = System_allocMemoryBlock(address, System.mem.stack.tailBlock);
  assert(newBlock);
  memset(address, '\x0', nbytes);
  newBlock->nbytes = nbytes;
  newBlock->file = filename;
  newBlock->line = lineNumber;
  System.mem.stack.usedMemory += nbytes;
  System.out.printDebug(DBG_MEMORY, "[System] MALLOC (%s-line:%d), (%d) byte(s) allocated at address [%p]", newBlock->file, newBlock->line, newBlock->nbytes, newBlock->address);

  return address;
}

void * System_realloc(void *addr, size_t nbytes, uint16 lineNumber, const uint16 *filename) {
  __MemoryBlock *renewBlock = null;
  assert(addr);
  assert(filename);
  renewBlock = System_findMemoryBlock(addr);
  assert(renewBlock);
  addr = realloc(addr, nbytes);
  assert(addr);
  System.out.printDebug(DBG_MEMORY, "[System] REALLOC (%s-line:%d), from [%p] : (%d) bytes, to [%p] : (%d) bytes", renewBlock->file, renewBlock->line, renewBlock->address, renewBlock->nbytes, addr, nbytes);
  renewBlock->address = addr;
  System.mem.stack.usedMemory += (nbytes - renewBlock->nbytes);
  renewBlock->nbytes = nbytes;
  renewBlock->file = filename;
  renewBlock->line = lineNumber;

  return renewBlock->address;
}

void * System_hasInstance(const void *addr) {
  __MemoryBlock *instanceBlock = null;
  if (!addr) {
    return null;
  }
  instanceBlock = System_findMemoryBlock(addr);
  if (instanceBlock) {
    return instanceBlock->address;
  }

  return false;
}

#else

#endif /* NDEBUG */

int System_outPrint(const char *fmt, ...) {
  int retval = 0;
  va_list args;
  va_start(args, fmt);
  retval = vfprintf(stdout, fmt, args) + 1;
  fflush(stdout);
  va_end(args);

  return retval;
}

int System_outPrintln(const char *fmt, ...) {
  int retval = 0;
  va_list args;
  va_start(args, fmt);
  retval = vfprintf(stdout, fmt, args) + 1;
  fprintf(stdout, "\n");
  fflush(stdout);
  va_end(args);

  return retval;
}

int System_outPrintDebug(uint16 dbgLevel, const char *fmt, ...) {
  int retval = 0;
#if ! defined NDEBUG
  if (System.mem.stack.isDebugging && ((dbgLevel & System.dbgLevels) == dbgLevel)) {
    va_list args;
    va_start(args, fmt);
    retval = vfprintf(stdout, fmt, args) + 1;
    fprintf(stdout, "\n");
    fflush(stdout);
    va_end(args);
  }
#endif

  return retval;
}

int System_errPrint(const char *fmt, ...) {
  int retval = 0;
  va_list args;
  va_start(args, fmt);
  retval = vfprintf(stderr, fmt, args) + 1;
  fflush(stderr);
  va_end(args);

  return retval;
}

int System_errPrintln(const char *fmt, ...) {
  int retval = 0;
  va_list args;
  va_start(args, fmt);
  retval = vfprintf(stderr, fmt, args) + 1;
  fprintf(stderr, "\n");
  fflush(stderr);
  va_end(args);

  return retval;
}

