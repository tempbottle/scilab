/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre Koumar
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.arcDrawing;

import javax.media.opengl.GL;

import org.scilab.modules.renderer.utils.geom3D.Vector3D;
import org.scilab.modules.renderer.utils.glTools.GLTools;

/**
 * ArcLineTools
 * @author Sylvestre Koumar
 *
 */
public abstract class ArcLineTools extends ArcTools {

	/**
	 * Constructor
	 * @param center Vector3D
	 * @param semiMinorAxis Vector3D
	 * @param semiMajorAxis Vector3D
	 * @param startAngle double
	 * @param endAngle double
	 */
	protected ArcLineTools(Vector3D center, Vector3D semiMinorAxis,
			Vector3D semiMajorAxis, double startAngle, double endAngle) {
		super(center, semiMinorAxis, semiMajorAxis, startAngle, endAngle);
	}

	/**
	 * beginRendering
	 * @param gl GL
	 * @param lineStyle int
	 * @param thickness float
	 * @param color double[]
	 */
	protected void beginRendering(GL gl, int lineStyle, float thickness, double[] color) {
		
		GLTools.beginDashMode(gl, lineStyle, thickness);
		
		// set color
		gl.glColor3d(color[0], color[1], color[2]);
		
		// transform the ellipse so we can draw a circle
		gl.glPushMatrix();
		
		setCoordinatesToCircleGL(gl);		
	}
	
	/**
	 * endRendering
	 * @param gl GL
	 */
	protected void endRendering(GL gl) {
		gl.glPopMatrix();
        GLTools.endDashMode(gl);
	}

}