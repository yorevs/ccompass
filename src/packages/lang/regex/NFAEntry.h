/*******************************************************************************
* Project:        C-Compass 1.0.0
* File:           NFAEntry
* Purpose:        Public prototypes for NFAEntry C-ANSI class
* Author:         Hugo Saporetti Junior
* Creation date:  Sep 9, 2009
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
#ifndef NFAENTRY_H_
#define NFAENTRY_H_

/* Includes ----------------------------------------------------------------- */
#include "Compass.h"
/* -------------------------------------------------------------------------- */

/* Usefull Macro to shorcut the constructor method */
#define NFAEntry(isbypass, in, out, chr)	_NEW_(NFAEntry, isbypass, in, out, chr)

/**
 * @purpose Definition of the NFAEntry C-ANSI class
 */
class (NFAEntry,
  boolean m_isBypass;       /* TODO Comentar */
  boolean m_isWildcard;     /* TODO Comentar */
  boolean m_isNegating;     /* TODO Comentar */
  boolean m_isLastSubex;    /* TODO Comentar */
  boolean m_isCloned;       /* TODO Comentar */
	void *m_in;               /* TODO Comentar */
  void *m_out;              /* TODO Comentar */
  uint16 m_groups;          /* TODO Comentar */
  uint16 m_codepointStart;  /* TODO Comentar */
  uint16 m_codepointEnd;    /* TODO Comentar */
  int (*posixFnc)(int c);   /* TODO Comentar */

	/* Public Methods */
  boolean (*match)(const NFAEntry *this, uint16 chr);
);

/* Declare all public prototypes here */

/**
 * TODO Comentar
 */
extern boolean NFAEntry_match(const NFAEntry *this, uint16 codepoint);

#endif /* NFAENTRY_H_ */

