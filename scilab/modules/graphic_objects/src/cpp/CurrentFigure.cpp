/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ScilabView.hxx"

extern "C"
{
#include "BOOL.h"
#include "CurrentFigure.h"
}

char* getCurrentFigure(void)
{
    return ScilabView::getCurrentFigure();
}

void setCurrentFigure(char *UID)
{
    ScilabView::setCurrentFigure(UID);
}

BOOL isCurrentFigure(char* UID)
{
    if (strcmp(UID, ScilabView::getCurrentFigure()) == 0)
    {
        return TRUE;
    }
    return FALSE;
}