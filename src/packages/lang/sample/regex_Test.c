/*******************************************************************************
* Project:        C-Compass 1.0.0
* File:           langTests.c
* Purpose:        Implement functions to TODO: Describe purpose here
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
*******************************************************************************/

/* Includes ---------------------------------------------------------------- */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "CRSwE.h"
#include "Exception.h"
#include "RegEx.h"
/* ------------------------------------------------------------------------- */

/* Defines ----------------------------------------------------------------- */
/*#define NDEBUG*/
/* ------------------------------------------------------------------------- */

void testRegEx(void) {
  int16 grp = 0;
  RegEx *reg = RegEx();

  System.out.println("Starting test-cases ...");

  reg->compile(reg, "ab");
  System.out.print("TestCase  #1 ... ");
  assert(!reg->matches(reg, "a"));      /* False */
  assert(!reg->matches(reg, "b"));      /* False */
  assert(reg->matches(reg, "ab"));      /* True */
  assert(!reg->matches(reg, "abx"));    /* False */
  System.out.println("OK");

  reg->compile(reg, "ab*c");
  System.out.print("TestCase  #2 ... ");
  assert(!reg->matches(reg, "a"));      /* False */
  assert(!reg->matches(reg, "b"));      /* False */
  assert(!reg->matches(reg, "c"));      /* False */
  assert(!reg->matches(reg, "ab"));     /* False */
  assert(reg->matches(reg, "ac"));      /* True */
  assert(reg->matches(reg, "abc"));     /* True */
  assert(reg->matches(reg, "abbbc"));   /* True */
  System.out.println("OK");

  reg->compile(reg, "ab+c");
  System.out.print("TestCase  #3 ... ");
  assert(!reg->matches(reg, "a"));      /* False */
  assert(!reg->matches(reg, "b"));      /* False */
  assert(!reg->matches(reg, "c"));      /* False */
  assert(!reg->matches(reg, "ab"));     /* False */
  assert(!reg->matches(reg, "ac"));     /* False */
  assert(reg->matches(reg, "abc"));     /* True */
  assert(reg->matches(reg, "abbbc"));   /* True */
  System.out.println("OK");

  reg->compile(reg, "ab?c");
  System.out.print("TestCase  #4 ... ");
  assert(!reg->matches(reg, "a"));      /* False */
  assert(!reg->matches(reg, "b"));      /* False */
  assert(!reg->matches(reg, "c"));      /* False */
  assert(!reg->matches(reg, "ab"));     /* False */
  assert(reg->matches(reg, "ac"));      /* True */
  assert(reg->matches(reg, "abc"));     /* True */
  assert(!reg->matches(reg, "abbbc"));  /* False */
  System.out.println("OK");

  reg->compile(reg, "[ab]c");
  System.out.print("TestCase  #5 ... ");
  assert(!reg->matches(reg, "a"));      /* False */
  assert(!reg->matches(reg, "b"));      /* False */
  assert(!reg->matches(reg, "c"));      /* False */
  assert(!reg->matches(reg, "ab"));     /* False */
  assert(reg->matches(reg, "ac"));      /* True */
  assert(reg->matches(reg, "bc"));      /* True */
  assert(!reg->matches(reg, "abc"));    /* False */
  System.out.println("OK");

  reg->compile(reg, "[ab]*c");
  System.out.print("TestCase  #6 ... ");
  assert(!reg->matches(reg, "a"));      /* False */
  assert(!reg->matches(reg, "b"));      /* False */
  assert(reg->matches(reg, "c"));       /* True */
  assert(!reg->matches(reg, "ab"));     /* False */
  assert(reg->matches(reg, "ac"));      /* True */
  assert(reg->matches(reg, "bc"));      /* True */
  assert(reg->matches(reg, "abc"));     /* True */
  assert(reg->matches(reg, "abbbbc"));  /* True */
  System.out.println("OK");

  reg->compile(reg, "[ab]+c");
  System.out.print("TestCase  #7 ... ");
  assert(!reg->matches(reg, "a"));      /* False */
  assert(!reg->matches(reg, "b"));      /* False */
  assert(!reg->matches(reg, "c"));      /* False */
  assert(!reg->matches(reg, "ab"));     /* False */
  assert(reg->matches(reg, "ac"));      /* True */
  assert(reg->matches(reg, "bc"));      /* True */
  assert(reg->matches(reg, "abc"));     /* True */
  assert(reg->matches(reg, "abaabc"));  /* True */
  System.out.println("OK");

  reg->compile(reg, "[ab]?c");
  System.out.print("TestCase  #8 ... ");
  assert(!reg->matches(reg, "a"));      /* False */
  assert(!reg->matches(reg, "b"));      /* False */
  assert(reg->matches(reg, "c"));       /* True */
  assert(!reg->matches(reg, "ab"));     /* False */
  assert(reg->matches(reg, "ac"));      /* True */
  assert(reg->matches(reg, "bc"));      /* True */
  assert(!reg->matches(reg, "abc"));    /* False */
  assert(!reg->matches(reg, "abaabc")); /* False */
  System.out.println("OK");

  reg->compile(reg, "[^ab]ac");
  System.out.print("TestCase  #9 ... ");
  assert(!reg->matches(reg, "a"));      /* False */
  assert(!reg->matches(reg, "b"));      /* False */
  assert(!reg->matches(reg, "ac"));     /* False */
  assert(!reg->matches(reg, "aac"));    /* False */
  assert(!reg->matches(reg, "bac"));    /* False */
  assert(reg->matches(reg, "cac"));     /* True */
  assert(!reg->matches(reg, "cccac"));  /* False */
  System.out.println("OK");

  reg->compile(reg, "[^ab]*ac");
  System.out.print("TestCase #10 ... ");
  assert(!reg->matches(reg, "a"));      /* False */
  assert(!reg->matches(reg, "b"));      /* False */
  assert(reg->matches(reg, "ac"));      /* True */
  assert(!reg->matches(reg, "aac"));    /* False */
  assert(!reg->matches(reg, "bac"));    /* False */
  assert(reg->matches(reg, "cac"));     /* True */
  assert(reg->matches(reg, "cccac"));   /* True */
  System.out.println("OK");

  reg->compile(reg, "[^ab]+ac");
  System.out.print("TestCase #11 ... ");
  assert(!reg->matches(reg, "a"));      /* False */
  assert(!reg->matches(reg, "b"));      /* False */
  assert(!reg->matches(reg, "ac"));     /* False */
  assert(!reg->matches(reg, "aac"));    /* False */
  assert(!reg->matches(reg, "bac"));    /* False */
  assert(reg->matches(reg, "cac"));     /* True */
  assert(reg->matches(reg, "cccac"));   /* True */
  System.out.println("OK");

  reg->compile(reg, "[^ab]?ac");
  System.out.print("TestCase #12 ... ");
  assert(!reg->matches(reg, "a"));      /* False */
  assert(!reg->matches(reg, "b"));      /* False */
  assert(reg->matches(reg, "ac"));      /* True */
  assert(!reg->matches(reg, "aac"));    /* False */
  assert(!reg->matches(reg, "bac"));    /* False */
  assert(reg->matches(reg, "cac"));     /* True */
  assert(!reg->matches(reg, "cccac"));  /* False */
  System.out.println("OK");

  reg->compile(reg, "ab{2}c");
  System.out.print("TestCase #13 ... ");
  assert(!reg->matches(reg, "a"));      /* False */
  assert(!reg->matches(reg, "b"));      /* False */
  assert(!reg->matches(reg, "ac"));     /* False */
  assert(!reg->matches(reg, "abc"));    /* False */
  assert(reg->matches(reg, "abbc"));    /* True */
  assert(!reg->matches(reg, "abbbc"));  /* False */
  System.out.println("OK");

  reg->compile(reg, "ab{0,2}c");
  System.out.print("TestCase #14 ... ");
  assert(!reg->matches(reg, "a"));      /* False */
  assert(!reg->matches(reg, "b"));      /* False */
  assert(reg->matches(reg, "ac"));      /* True */
  assert(reg->matches(reg, "abc"));     /* True */
  assert(reg->matches(reg, "abbc"));    /* True */
  assert(!reg->matches(reg, "abbbc"));  /* False */
  System.out.println("OK");

  reg->compile(reg, "ab{1,2}c");
  System.out.print("TestCase #15 ... ");
  assert(!reg->matches(reg, "a"));      /* False */
  assert(!reg->matches(reg, "b"));      /* False */
  assert(!reg->matches(reg, "ac"));     /* False */
  assert(reg->matches(reg, "abc"));     /* True */
  assert(reg->matches(reg, "abbc"));    /* True */
  assert(!reg->matches(reg, "abbbc"));  /* False */
  System.out.println("OK");

  reg->compile(reg, "ab{0,}c");
  System.out.print("TestCase #16 ... ");
  assert(!reg->matches(reg, "a"));      /* False */
  assert(!reg->matches(reg, "b"));      /* False */
  assert(reg->matches(reg, "ac"));      /* True */
  assert(reg->matches(reg, "abc"));     /* True */
  assert(reg->matches(reg, "abbbc"));   /* True */
  System.out.println("OK");

  reg->compile(reg, "ab{1,}c");
  System.out.print("TestCase #17 ... ");
  assert(!reg->matches(reg, "a"));      /* False */
  assert(!reg->matches(reg, "b"));      /* False */
  assert(!reg->matches(reg, "ac"));     /* False */
  assert(reg->matches(reg, "abc"));     /* True */
  assert(reg->matches(reg, "abbbc"));   /* True */
  System.out.println("OK");

  reg->compile(reg, "(.*)\\.(.*)\\.(.*)\\.(.*)");
  System.out.print("TestCase #18 ... ");
  assert(!reg->matches(reg, "192.168.20"));   /* False */
  assert(reg->matches(reg, "192.168..51"));   /* True */
  assert(reg->matches(reg, "192.168.20.51")); /* True */
  assert((grp = reg->groupCount(reg)) == 4);
  System.out.println("OK");
  static char *tc18Results[] = { "192.168.20.51", "192", "168", "20", "51" };
  while(grp >= 0) {
    char *results = toString(reg->group(reg, grp));
    System.out.print("Group (%d); Expected: %s, Result: %s ... ", grp, tc18Results[grp], results);
		assert(strcmp(results, tc18Results[grp]) == 0);
		System.out.println("OK");
    grp--;
  }

  reg->compile(reg, "((ab)([cd]*))([ef]*)(g{0,2})");
  System.out.print("TestCase #19 ... ");
  assert(reg->matches(reg, "ab"));             /* True */
  assert(reg->matches(reg, "abc"));            /* True */
  assert(reg->matches(reg, "abe"));            /* True */
  assert(reg->matches(reg, "abg"));            /* True */
  assert(!reg->matches(reg, "abh"));           /* False*/
  assert(!reg->matches(reg, "abcdefgggg"));    /* False*/
  assert(reg->matches(reg, "abcdefgg"));       /* True */
  assert((grp = reg->groupCount(reg)) == 5);
  static char *tc19Results[] = { "abcdefgg", "abcd", "ab", "cd", "ef", "gg" };
  while(grp >= 0) {
    char *results = toString(reg->group(reg, grp));
    System.out.print("Group (%d); Expected: %s, Result: %s ... ", grp, tc19Results[grp], results);
    assert(strcmp(results, tc19Results[grp]) == 0);
    System.out.println("OK");
    grp--;
  }

  delete(reg);
}


int16 getExpression(char *buffer, const char *textEx) {
  System.out.print("\nType %s (Max. 80 characters): \n>> ", textEx);
  fgets(buffer, 80, stdin);
  if (!buffer || *buffer == '\x0') {
    return ERROR;
  }
  buffer[strlen(buffer)-1] = '\x0';

  return SUCCESS;
}


void testRegEx2(void) {
  RegEx *reg = RegEx();
  char regex[80] = { "\0" };
  char input[80] = { "a" };
  int16 grp = 0;

  if (getExpression(regex, "a Regular Expression ") != SUCCESS) {
    System.err.println("Failed to acquire a pattern !!!");
    exit(1);
  }
  if (*regex != 0) {
    if (reg->compile(reg, regex) != SUCCESS) {
      System.err.println("# Error # Compiling regex: \"%s\"", reg->getStatusMsg(reg));
      exit(1);
    }
    System.out.println("Using pattern: %s", regex);
  }

  while(*input != '\n' && *input != 0 && *regex != '\n' && *regex != 0) {
    if (getExpression(input, "an Input String") != SUCCESS) {
      System.err.println("Failed to acquire expression !!!");
      exit(1);
    }
    if (*input == 0) break;
    grp = 0;
    System.out.println(">> %s", reg->matches(reg, input) ? "TRUE": "FALSE");
    System.out.println("Matching %s with %s", regex, input);
    grp = reg->groupCount(reg);

    System.out.println("Groups: %d", grp);

    while(grp >= 0) {
      System.out.println("Group (%d): %s", grp, toString(reg->group(reg, grp)));
      grp--;
    }

  };

  delete(reg);
}




int main(int argc, char **argv) {
#if ! defined NDEBUG
  System.mem.setDebug(true, DBG_NONE /*| DBG_ERROR | DBG_DEBUG*/);
#endif

  __initExceptionSignals();

 /* testRegEx();*/
  testRegEx2();

  System.out.println("Done");

  return EXIT_SUCCESS;
}

