/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Factory for DrawableGrayplotBridge
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "DrawableGrayplotBridgeFactory.hxx"
#include "DrawableGrayplotJoGL.hxx"
#include "DrawableGrayplotJavaMapper.hxx"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableGrayplotBridge * DrawableGrayplotBridgeFactory::create( void )
{
  DrawableGrayplotJoGL * imp = new DrawableGrayplotJoGL( m_pDrawable ) ;
  imp->setJavaMapper(new DrawableGrayplotJavaMapper()) ;
  return imp ;
}
/*---------------------------------------------------------------------------------*/
void DrawableGrayplotBridgeFactory::setDrawedGrayplot( DrawableGrayplot * poly )
{
  m_pDrawable = poly ;
}
/*---------------------------------------------------------------------------------*/

}