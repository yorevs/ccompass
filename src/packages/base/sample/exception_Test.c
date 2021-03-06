/*******************************************************************************
* Project:        C-Compass 1.0.0
* File:           baseTests.c
* Purpose:        Implement functions to TODO: Describe purpose here
* Author:         Hugo Saporetti Junior
* Creation date:  Sep 3, 2009
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
#include "Compass.h"
#include "Exception.h"
/* ------------------------------------------------------------------------- */

/* Defines ----------------------------------------------------------------- */
//#define NDEBUG
/* ------------------------------------------------------------------------- */

void testException(void) {

  typedef enum {
    EXCEPTION_TYPE1 = 1,  /***/
    EXCEPTION_TYPE2 = 2,  /***/
    EXCEPTION_TYPE3 = 3,  /***/
    EXCEPTION_TYPE4 = 4,  /***/
  } RuntimeExceptions;

  Exception *ex = Exception("My Exception", null, EXCEPTION_TYPE1);
  Exception *ex2 = Exception("2nd Exception", ex, EXCEPTION_TYPE2);
  Exception *ex3 = Exception("3nd Exception", ex2, EXCEPTION_TYPE3);

  TRY (
      System.out.println("<Try Block # 1>");
      THROW(ex3);
      System.out.println("<Should not execute # 1>");
  ) CATCH (EXCEPTION_TYPE3, Exception *e,
      System.out.println("<Catch Block # 1>");
      Exception *ex4 = Exception("4nd Exception", e, EXCEPTION_TYPE4);
      TRY (
          System.out.println("<Try Block # 2>");
          THROW(ex4);
          System.out.println("<Should not execute # 2>");
      ) CATCH (EXCEPTION_TYPE4, Exception *e2,
          System.out.println("<Catch Block # 2>");
          e2->printStackTrace();
      )
  )

}


int main(int argc, char **argv) {
#if ! defined NDEBUG
  System.mem.setDebug(true, DBG_ERROR | DBG_FATAL | DBG_WARN);
#endif

  __initExceptionSignals();

  testException();

  System.out.println("Done.");

  return SUCCESS;
}

