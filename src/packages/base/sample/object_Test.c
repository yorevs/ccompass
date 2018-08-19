/*******************************************************************************
* Project:        C-Compass 1.0.0
* File:           objectTests.c
* Purpose:        Implement functions to TODO: Describe purpose here
* Author:         Hugo Saporetti Junior
* Creation date:  Oct 8, 2009
* ANSI:           no
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
#include "CRSwE.h"
#include "Exception.h"
/* ------------------------------------------------------------------------- */

/* Defines ----------------------------------------------------------------- */
/*#define NDEBUG*/
/* ------------------------------------------------------------------------- */

void testObject() {
  Object *obj1 = null;
  Object *obj2 = null;
  Object *obj3 = null;
  obj1 = Object();
  obj2 = clone(obj1);
  obj3 = Object();
  System.out.println("Class Name: %s", (char*) getClass(obj1)->m_name);
  System.out.println("%s@%p", toString(obj1), System_hasInstance(obj1));
  System.out.println("%s@%p", toString(obj2), System_hasInstance(obj2));
  System.out.println("%s@%p", toString(obj3), System_hasInstance(obj3));
  System.out.println("Obj1 %s equals to Obj1", equals(obj1, obj1) ? "is" : "is not");
  System.out.println("Obj1 %s equals to Obj2", equals(obj1, obj2) ? "is" : "is not");
  System.out.println("Obj1 %s equals to Obj3", equals(obj1, obj3) ? "is" : "is not");
  delete(obj1);
  delete(obj2);
  delete(obj3);
}

int main(int argc, char **argv) {
#if ! defined NDEBUG
  System.mem.setDebug(true, DBG_ALL);
#endif

  __initExceptionSignals();

  testObject();

  System.out.println("Done.");

  return SUCCESS;
}


