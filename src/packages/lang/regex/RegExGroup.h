/*******************************************************************************
* Project:        C-Compass 1.0.0
* File:           RegExGroup.h
* Purpose:        Public prototypes for RegExGroup C-ANSI class
* Author:         Hugo Saporetti Junior
* Creation date:  Sep 14, 2009
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
#ifndef REGEXGROUP_H_
#define REGEXGROUP_H_

/* Includes ----------------------------------------------------------------- */
#include "Compass.h"
#include "NFAState.h"
/* -------------------------------------------------------------------------- */

/* Usefull Macro to shorcut the constructor method */
#define RegExGroup()		      _NEW_(RegExGroup, null)

/**
 * @purpose Definition of the RegExGroup C-ANSI class
 */
class (RegExGroup,
  NFAState *m_startState; /* Points to the initial state of the compiled
                             pattern. */
  NFAState *m_lastState;  /* Points to the last state of the compiled
                             pattern. */

	/* Public Methods */
);

/* Declare all public prototypes here */

#endif /* REGEXGROUP_H_ */

