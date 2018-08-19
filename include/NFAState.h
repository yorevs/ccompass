/*******************************************************************************
* Project:        C-Compass 1.0.0
* File:           NFAState.h
* Purpose:        Public prototypes for NFAState C-ANSI class
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
#ifndef NFASTATE_H_
#define NFASTATE_H_

/* Includes ----------------------------------------------------------------- */
#include "Compass.h"
#include "ArrayList.h"
#include "NFAEntry.h"
/* -------------------------------------------------------------------------- */

/* Usefull Macro to shorcut the constructor method */
#define NFAState(previous)		_NEW_(NFAState, previous)

/**
 * @purpose Definition of the NFAState C-ANSI class
 */
class (NFAState,
  NFAState *m_prevState;    /* TODO Comentar */
  NFAEntry *m_in;           /* TODO Comentar */
  ArrayList *m_outEntries;  /* TODO Comentar */
  boolean m_isFinal;        /* TODO Comentar */
  boolean m_hasWildcard;    /* TODO Comentar */

	/* Public Methods */
);

/* Declare all public prototypes here */

#endif /* NFASTATE_H_ */

