/*  Scicos
*
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/
/**
   \file scoMisc.c
   \author Benoit Bayol
   \version 1.0
   \date September 2006 - January 2007
   \brief Source Code for Misc function (not only for the sco lib)
*/
/*--------------------------------------------------------------------------*/
#include "scoBase.h"
#include "scoMisc.h"
#include "scoGetProperty.h"
#include "scoSetProperty.h"
#include "sciprint.h"
#include "HandleManagement.h"
#include "scicos.h"
#include "scicos_block4.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
void scoScopeError(ScopeMemory * pScopeMemory, int code_error)
{

	scoGraphicalObject pScopeWindow;
	scoInteger win_id;

	switch(code_error)
	{
	case 0:
		sciprint(_("SCOPE ERROR : sciDrawObj cannot be performed, pShortDraw is NULL for unknown reason !\n Maybe you have destroyed the windows or any parent of curves\n"));
		break;
	case 1:
		sciprint(_("SCOPE ERROR : Error during malloc - Check Memory\n"));
		break;
	default:
		sciprint(_("SCOPE ERROR : Unknown code error !\n"));
		break;
	}

	pScopeWindow = scoGetPointerScopeWindow(pScopeMemory);
	win_id = scoGetWindowID(pScopeMemory);
	if (pScopeWindow != NULL )
	{
		sciDeleteWindow(win_id );
	}

	set_block_error(-16);
}
/*--------------------------------------------------------------------------*/