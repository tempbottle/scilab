/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Contains mapping of java method used by SurfaceFacetDrawerJoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _SURFACE_FACET_DRAWER_JAVA_MAPPER_HXX_
#define _SURFACE_FACET_DRAWER_JAVA_MAPPER_HXX_

#include "DrawableObjectJavaMapper.hxx"
#include "SurfaceFacetDrawerGL.hxx"

namespace sciGraphics
{

class SurfaceFacetDrawerJavaMapper : public virtual DrawableObjectJavaMapper
{
public:

  SurfaceFacetDrawerJavaMapper(void);

  virtual ~SurfaceFacetDrawerJavaMapper(void);

  /*----------------------------------------------------------------------*/
  // Inherited From DrawableObjectJavaMapper
  virtual void display(void);

  virtual void initializeDrawing(int figureIndex);
  virtual void endDrawing(void);

  virtual void show(int figureIndex);

  virtual void destroy(int figureIndex);
  /*----------------------------------------------------------------------*/
  // specific for surface facet drawer
  virtual void setDefaultColors(int defaultColor, int hiddenColor);

  virtual void setSurfaceType(int typeOfSurface, int colorFlag);

  virtual void drawSurface(double xCoords[], int sizeX,
                           double yCoords[], int sizeY,
                           double zCoords[], int sizeZ,
                           int nbFacets);

  virtual void setFacetsColors(double colors[], int colorsSize);

  /*----------------------------------------------------------------------*/

protected:

  /**
   * Giws generated wrapper.
   */
  org_scilab_modules_renderer_surfaceDrawing::SurfaceFacetDrawerGL * m_pJavaObject;


};

}

#endif /* _SURFACE_FACET_DRAWER_JAVA_MAPPER_HXX_ */
