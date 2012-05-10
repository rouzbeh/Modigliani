/**\file nt3d_glx_drv_obj.cpp - NT3D_glx_drv_o class implementation 
 * by Ahmed Aldo Faisal &copy; created 9.5.1999  
 */
/* NetTrader - visualisation, scientific and financial analysis and simulation system
 * Version:  0.3
 * Copyright (C) 1998,199 Ahmed Aldo Faisal                            
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

/* $Id: nt3d_glx_drv_obj.cpp,v 1.1.1.1 2004/12/16 01:38:36 face Exp $
 * $Log: nt3d_glx_drv_obj.cpp,v $
 * Revision 1.1.1.1  2004/12/16 01:38:36  face
 * Imported NetTrader 0.5 source from flyeye02.zoo.cam.ac.uk repository
 *
 * Revision 1.2  2003/06/20 13:25:57  face
 * *** empty log message ***
 *
 * Revision 1.1  2001/06/29 13:16:55  face
 * *** empty log message ***
 *
 * Revision 1.10  2000/10/03 08:29:59  face
 * *** empty log message ***
 *
 * Revision 1.9  2000/07/06 08:06:10  face
 * *** empty log message ***
 *
 * Revision 1.8  2000/06/13 20:10:25  face
 * *** empty log message ***
 *
 * Revision 1.7  2000/04/26 17:10:34  face
 * minor bug fixed
 * and upgrades:
 *
 * Revision 1.6  1999/12/04 13:43:04  face
 * *** empty log message ***
 *
 * Revision 1.5  1999/12/03 16:36:08  face
 * the error lies in the initilisation
 * sequence nt3d_glx_drv
 *
 * Revision 1.4  1999/12/03 14:53:44  face
 * *** empty log message ***
 *
 * Revision 1.3  1999/07/20 01:15:01  face
 * general debugging and clean up
 *
 * Revision 1.2  1999/07/16 15:05:05  face
 * added code::^
 *
 * Revision 1.1.1.1  1999/07/03 15:17:20  face
 * Switching from RCS to CVS to improve
 * the handling of the new src code subdirectory
 * structure. aaf
 *
 *
 * Revision 1.3  1999/06/26 16:49:02  face
 * *** empty log message ***
 *
 * Revision 1.2  1999/06/19 16:15:49  face
 * *** empty log message ***
 *
 * Revision 1.1  1999/05/13 11:34:53  face
 * Initial revision
 *
 * Revision 1.1  1999/04/22 11:37:49  face
 * Initial revision
 *
 *
 */

#include "nt3d_glx_drv_obj.h"

#include <iostream>
#include <sstream>

#include <stdio.h>
#include <X11/Xatom.h>

//2DO complete this 
NTint NT3D_glx_drv_o::initCount = 0; /* 0 means no windows open */
NTint NT3D_glx_drv_o::windowCount = 0;
NTint NT3D_glx_drv_o::commonCtxCount = 0;
Display * NT3D_glx_drv_o::dpy = NULL;
GLXContext NT3D_glx_drv_o::uniqueCommonCtx = NULL;

/* ***      CONSTRUCTORS	***/
/** standard constructor which on default opens a 200 X 200 window */
NT3D_glx_drv_o::NT3D_glx_drv_o(mbase::Msize windowWidth, mbase::Msize windowHeight,
		bool useCommonContext) {
	useUniqueCommonCtx = useCommonContext;

	if (1 > initCount) {
		dpy = XOpenDisplay(NULL);
		if (NULL == dpy) {
			cerr
					<< "NT3D_glx_drv_o::Constructor failed - error : XOpenDisplay failed."
					<< std::endl;
			return;
		}
	}

	CreateWindow(windowWidth, windowHeight);
	NT3D_glx_drv_o::initCount++;

	stringstream title;
	title << "GLX drv win no. " << windowNumber;
	windowTitle = title.str();
	SetWindowTitle(windowTitle);
}

const NT3D_glx_drv_o&
NT3D_glx_drv_o::operator=(const NT3D_glx_drv_o & right) {
	if (this == &right)
		return *this; // Gracefully handle self assignment
	// add assignment code here
	return *this;
}

/* ***      DESTRUCTOR		***/
NT3D_glx_drv_o::~NT3D_glx_drv_o() {
	if (mbase::M_FAIL == Close()) {
		cerr << "NT3D_glx_drv_o::Close - error : call to Close failed !"
				<< std::endl;
		return;
	}
	NT3D_glx_drv_o::initCount--;

	if (initCount < 1)
		XCloseDisplay(dpy);
}

/* ***  PUBLIC                                    ***   */
mbase::Mreturn NT3D_glx_drv_o::Open(NT3D_vp_spec_o newWinSpec) {
	if (0 != CreateWindow(newWinSpec.width, newWinSpec.height))
		return mbase::M_SUCCESS;
	else
		return mbase::M_FAIL;
}

mbase::Mreturn NT3D_glx_drv_o::Close() {
	glXDestroyContext(dpy, ctx);
	XDestroyWindow(dpy, win);
//	NT3D_glx_drv_o::windowCount--;   
	if (true == useUniqueCommonCtx)
		commonCtxCount--;

	return mbase::M_SUCCESS;
}

mbase::Mreturn NT3D_glx_drv_o::Resize(mbase::Msize windowWidth, mbase::Msize windowHeight) {
	width = windowWidth;
	height = windowHeight;

	glViewport(0, 0, width, height);
	XResizeWindow(dpy, win, width, height);

	return mbase::M_SUCCESS;
}

/** @short   The OLD method of choice to open a rendering window.  It opens a
 window of size windowWidth and windowHeight, sets the rendering
 pipelin up as SMOOTH shaded, black clear render. Next it will provid a
 window which set as current rendering context, swaps ones the
 drawing buffers and is clears  the color and depth buffer.
 @return The windowNumber identifier of the the added window.

 @param      NTint windowWidth, NTint windowHeight
 @return     returns the internal windowNumber and 0 if failed
 \warning    unknown
 \bug        unknown
 */
NTint NT3D_glx_drv_o::CreateWindow(NTint windowWidth, NTint windowHeight) {
	width = windowWidth;
	height = windowHeight;

	windowNumber = 0; // in case it fails

	if (MakeRGBDBWindow(width, height) != mbase::M_SUCCESS) {
		cerr << "NT3D_glx_drv_o::CreateWindow - error : MakeRBGDBWindow failed."
				<< std::endl;
		return 0;
	}

	XMapWindow(dpy, win);
	glXMakeCurrent(dpy, win, ctx); /* necessary for the first time */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST); /* enable depth buffering */
	glDepthFunc(GL_LESS); /* pedantic, GL_LESS is the default */
	glClearDepth(1.0); /* pedantic, 1.0 is the default */
	glClearColor(0.0, 0.0, 0.0, 0.0); /* frame buffer clears should be to black */

	glXSwapBuffers(dpy, win);

	windowCount++;
	windowNumber = windowCount;

	return windowNumber; // return 0 if failed !
}

mbase::Mreturn NT3D_glx_drv_o::SetWindowTitle(string newTitle) {

	windowTitle = newTitle;
	XTextProperty textprop;

	XSizeHints * szHints;
	szHints = XAllocSizeHints();
	szHints->x = (int) width;
	szHints->y = (int) height;
	XWMHints *wmHints;
	wmHints = XAllocWMHints();
	wmHints->initial_state = NormalState;
	wmHints->flags = StateHint;

	/* Setup ICCCM properties. */
	textprop.value = (unsigned char *) windowTitle.c_str();

	textprop.encoding = XA_STRING;
	textprop.format = 8;
	textprop.nitems = windowTitle.length();

	XSetWMProperties(dpy, win, &textprop, &textprop, NULL, 0, szHints, wmHints,
			NULL);

	XFree(wmHints);
	delete szHints;
	return mbase::M_SUCCESS;
}

NT3D_bitmap_o NT3D_glx_drv_o::View2Bitmap() {
	NT3D_bitmap_o oTmp;
	oTmp.GetBitmap(0, 0, width, height, NT_RGB);
	return oTmp;
}

NT3D_bitmap_o *
NT3D_glx_drv_o::View2BitmapPtr(NT3D_bitmap_o * bmpPtr) {
	if (NULL == bmpPtr) {
		NT3D_bitmap_o * oTmp = new NT3D_bitmap_o;
		oTmp->GetBitmap(0, 0, width, height, NT_RGB);
		return oTmp;
	} else {
		bmpPtr->GetBitmap(0, 0, width, height, NT_RGB);
		return bmpPtr;
	}
}

/* ***  PROTECTED                         ***   */
/* ***  PRIVATE                           ***   */
mbase::Mreturn NT3D_glx_drv_o::MakeRGBDBWindow(NTint windowWidth,
		NTint windowHeight) {
	int attrib[] = { GLX_RGBA, GLX_DEPTH_SIZE, 16, GLX_DOUBLEBUFFER, None };
	int scrnum;
	XSetWindowAttributes attr;
	unsigned long mask;
	Window root;
	XVisualInfo *visinfo;

	scrnum = DefaultScreen( dpy );
	root = RootWindow( dpy, scrnum );

	visinfo = glXChooseVisual(dpy, scrnum, attrib);
	if (!visinfo) {
		cerr
				<< "NT3D_glx_drv_o::MakeRGBDBWindow - error: couldn't get an RGB, Double-buffered visual."
				<< std::endl;
		return mbase::M_FAIL;
	}

	/* window attributes */
	attr.background_pixel = 0;
	attr.border_pixel = 0;
	attr.colormap = XCreateColormap(dpy, root, visinfo->visual, AllocNone);
	/*** Attribute EVENT_MASK ***/
	attr.event_mask = StructureNotifyMask | ExposureMask; // | ButtonPressMask |KeyPressMask;
	mask = CWBackPixel | CWBorderPixel | CWColormap | CWEventMask;

	win = XCreateWindow(dpy, root, 0, 0, windowWidth, windowHeight, 0,
			visinfo->depth, InputOutput, visinfo->visual, mask, &attr);

	/* 2DO hacked code here BEWARE of side effects, e.g. deletion of original context object ... */
	GLXContext shared_context;
	if (useUniqueCommonCtx == true) {
		cerr << "Using unique common context." << std::endl;
		if (commonCtxCount < 1) {
			cerr << "First init of common context." << std::endl;
			shared_context = NULL;
		} else {
			cerr << "Using used context." << std::endl;
			shared_context = uniqueCommonCtx;
		}
	} else {
		cerr << "Using individual context." << std::endl;
		shared_context = NULL;
	}

	ctx = glXCreateContext(dpy, visinfo, shared_context, (Bool) False);
	if (NULL == ctx) {
		cerr
				<< "NT3D_glx_drv_o::MakeRGBDBWindow - error: could not create a rendering context."
				<< std::endl;
		return mbase::M_FAIL;
	}

	if (useUniqueCommonCtx == true) {
		commonCtxCount++;
		if (commonCtxCount < 2) {
			cerr << "First time assignment of common context." << std::endl;
			uniqueCommonCtx = ctx;
		}
	}

	return mbase::M_SUCCESS;

}

/* File skeleton generated by GenNTObj version 0.3. */
