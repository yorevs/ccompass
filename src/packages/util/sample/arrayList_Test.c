/*******************************************************************************
* Project:        C-Compass 1.0.0
* File:           arrayListTests.c
* Purpose:        Implement functions to TODO: Describe purpose here
* Author:         Hugo Saporetti Junior
* Creation date:  Oct 8, 2009
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
#include "ArrayList.h"
/* ------------------------------------------------------------------------- */

/* Defines ----------------------------------------------------------------- */
/*#define NDEBUG*/

#define stats_array(list)     if (true) { \
                              void statsArray() { \
                                ListEntry *entry = list->m_head; \
                                while(entry != null) { \
                                  System.out.println("%s", toString(entry)); \
                                  entry = entry->m_next; \
                                } \
                              }statsArray();}
/* ------------------------------------------------------------------------- */

void testArrayList(void) {
  ArrayList *list = ArrayList();
  ArrayList *myclone = null;

  list->addElement(list, String("String 1"));
  list->addElement(list, String("String 2"));
  list->addElement(list, String("String 3"));
  list->addElement(list, String("String 4"));
  list->addElement(list, String("String 5"));

  list->removeElementAt(list, 1);
  list->removeElementAt(list, 2);

  list->insertElementAt(list, String("String A"), 1);
  list->insertElementAt(list, String("String B"), 3);

  list->setElementAt(list, String("Replacement for 3"), 2);

  System.out.println("Listing %d entries from list ...\n", ArrayList_size(list));
  System.out.println("Element [1] = %s, [3] = %s", toString(ArrayList_elementAt(list, 1)), toString(ArrayList_elementAt(list, 3)));

  myclone = clone(list);

  stats_array(myclone);

  stats_array(list);
  list->clear(list);
  stats_array(list);

  delete(myclone);
  delete(list);
}

int main(int argc, char **argv) {
#if ! defined NDEBUG
  System.mem.setDebug(true, DBG_ALL);
#endif

  __initExceptionSignals();

  testArrayList();

  System.out.println("Done.");

  return SUCCESS;
}
