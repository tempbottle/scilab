/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Factory for DrawableTextBridge
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_TEXT_BRIDGE_FACTORY_H_
#define _DRAWABLE_TEXT_BRIDGE_FACTORY_H_

#include "DrawableText.h"
#include "DrawableTextBridge.hxx"

namespace sciGraphics
{

class DrawableTextBridgeFactory
{
public:

  DrawableTextBridgeFactory( void ) { m_pDrawable = NULL ; }

  virtual ~DrawableTextBridgeFactory( void ) {}

  /**
  * Create a new drawer from the drawed object.
  */
  virtual DrawableTextBridge * create( void ) ;

  /**
   * Set the DrawableText which needs algorithms
   */
  void setDrawedText( DrawableText * poly ) ;

protected:

  DrawableText * m_pDrawable;

};

}  

#endif /* _DRAWABLE_TEXT_BRIDGE_FACTORY_H_ */
