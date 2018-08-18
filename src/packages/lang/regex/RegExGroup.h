/*******************************************************************************
* Project:        CRSwE_Library_V_1_1_beta
* File:           RegExGroup.h
* Purpose:        Public prototypes for RegExGroup C-ANSI class
* Author:         Hugo Saporetti Junior
* Creation date:  Sep 14, 2009
* ANSI:           yes
* ------------------------------------------------------------------------------
* Developed by: ICC-SWE
* Access: http://crswe.inatel.br/iccforum
* Copyright (C) 2009 Inatel Competence Center - Software Development/CRSwE.
*
* This file is part of CRSwE Library.
*
* CRSwE Library is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* CRSwE Library is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with CRSwE Library. If not, see <http://www.gnu.org/licenses />.
*******************************************************************************/
#ifndef REGEXGROUP_H_
#define REGEXGROUP_H_

/* Includes ----------------------------------------------------------------- */
#include "CRSwE.h"
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

