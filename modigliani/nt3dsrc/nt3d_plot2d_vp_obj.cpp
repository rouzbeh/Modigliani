/**\file nt3d_plot2d_vp_obj.cpp - NT3D_plot2d_vp_o class implementation 
 * by Ahmed Aldo Faisal &copy; created 11.5.2001  
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.5
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal    
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* $Id: nt3d_plot2d_vp_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $ 
 * $Log: nt3d_plot2d_vp_obj.cpp,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.2  2002/03/04 18:08:33  face
 * added checking for NaN and Inf values in find_spikes_in_binary_file-
 *
 * Revision 1.1  2001/06/29 13:16:55  face
 * *** empty log message ***
 *

 */
#include "nt3d_plot2d_vp_obj.h" 

/* ***      CONSTRUCTORS	***/
/** Create a NT3D_plot2d_vp_o */
NT3D_plot2d_vp_o::NT3D_plot2d_vp_o() :
		NT3D_plot_vp_o() {
	autoRange = false;
	drawCoord = true;
	dataXMin = 0.0;
	dataXMax = 0.0;
	dataYMin = 0.0;
	dataYMax = 0.0;
	xMin = -1.0;
	xMax = 1.0;
	yMin = -1.0;
	yMax = 1.0;
}

/* ***      COPY AND ASSIGNMENT	***/
NT3D_plot2d_vp_o::NT3D_plot2d_vp_o(const NT3D_plot2d_vp_o & original) :
		NT3D_plot_vp_o() {
	autoRange = original.autoRange;
	drawCoord = original.drawCoord;
	dataXMin = original.dataXMin;
	dataXMax = original.dataXMax;
	dataYMin = original.dataYMin;
	dataYMax = original.dataYMax;
	xMin = original.xMin;
	xMax = original.xMax;
	yMin = original.yMin;
	yMax = original.yMax;
}

const NT3D_plot2d_vp_o&
NT3D_plot2d_vp_o::operator=(const NT3D_plot2d_vp_o & right) {
	if (this == &right)
		return *this; // Gracefully handle self assignment
	// add assignment code here
	return *this;
}

/* ***      DESTRUCTOR		***/
NT3D_plot2d_vp_o::~NT3D_plot2d_vp_o() {
}

/* ***  PUBLIC                                    ***   */
/** @short       
 @param      none
 @return     none
 \warning    unknown
 \bug        unknown
 */
void NT3D_plot2d_vp_o::Draw() {
	MakeCurrent();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (true == autoRange) {
		xMin = dataXMin;
		xMax = dataXMax;
		yMin = dataYMin;
		yMax = dataYMax;
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(xMin, xMax, yMin, yMax);
	if (drawCoord == true) {
		DrawCoordinateSystem();
	}
	DrawData();
	SwapBuffer();
}

void NT3D_plot2d_vp_o::DrawCoordinateSystem(/* to be optionalised */) {
	mbase::Mreal x, y;
	mbase::Mreal xLen = fabs(xMax - xMin);
	mbase::Mreal yLen = fabs(yMax - yMin);

	mbase::Mreal minorXTickLength = 0.01 * yLen;
	mbase::Mreal majorXTickLength = 0.02 * yLen;
	mbase::Mreal minorXTickSpacing = 1.0;
	mbase::Msize minor2MajorXTickRatio = 10.0;
	mbase::Mreal minorYTickLength = 0.01 * xLen;
	mbase::Mreal majorYTickLength = 0.02 * xLen;
	mbase::Mreal minorYTickSpacing = 1.0;
	mbase::Msize minor2MajorYTickRatio = 10.0;

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_POINTS);
	glVertex2f(0, 0);
	glEnd();
	glBegin(GL_LINES);
	/* x axis */
	glVertex2f(xMin, yMin);
	glVertex2f(xMax, yMin);
	/* y axis */
	glVertex2f(xMin, yMin);
	glVertex2f(xMin, yMax);

	/* x axis ticks */
	y = yMin;
	mbase::Msize ll;
	for (ll = 0; ll < (float) xLen / minorXTickSpacing; ll++) {
		x = ll * minorXTickSpacing
				+ minorXTickSpacing * floor(xMin / minorXTickSpacing);
		glVertex2f(x, y);
		if (ll % minor2MajorXTickRatio == 0)
			glVertex2f(x, y + majorXTickLength);
		else
			glVertex2f(x, y + minorXTickLength);
	}

	/* y axis ticks */
	x = xMin;
	for (ll = 0; ll < (float) yLen / minorYTickSpacing; ll++) {
		y = ll * minorYTickSpacing
				+ minorYTickSpacing * floor(yMin / minorYTickSpacing);
		glVertex2f(x, y);
		if (ll % minor2MajorYTickRatio == 0)
			glVertex2f(x + majorYTickLength, y);
		else
			glVertex2f(x + minorYTickLength, y);
	}

	glEnd();
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */

/* File skeleton generated by GenNTObj version 0.7. */
/**  */
