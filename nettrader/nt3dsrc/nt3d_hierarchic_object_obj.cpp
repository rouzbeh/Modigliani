/* \file nt3d_hierarchic_object_obj.cpp - NT3D hierarchic object implementation
 * by Ahmed A. Faisal &copy; created  21.4.1999 
 * part of the NT3D package
 *
 * NetTrader - visualisation, finance management, analysis and simulation system
 * Version:  0.3
 * Copyright (C)  Ahmed Aldo Faisal 1998,1999
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


/* $Id: 
 * $Log: 
 * 
 */

#include "nt3d_hierarchic_object_obj.h"

#include "nt_error_obj.h"

#include <GL/gl.h>

/* Constructor/Destructor */

/** Sets a unrotated 3D object in the origin */
NT3D_hierarchic_object_o::NT3D_hierarchic_object_o()
:
  NT3D_object_o()
 { 
 bHaveParent = NT_FALSE;
 bHaveChild = NT_FALSE;
 uiChildCounter = 0;
 pParent = NULL;
 }

NT3D_hierarchic_object_o::NT3D_hierarchic_object_o(const NT_vector3_o & oNewTranslation)
 :
 NT3D_object_o(oNewTranslation)
 {
 bHaveParent = NT_FALSE;
 bHaveChild = NT_FALSE;
 uiChildCounter = 0;
 pParent = NULL;
 }

/** The entire children tree is copied (recreated!) but no parent is
set, otherwise the parent of the original object has to be informed of
a new child which is generally not desired.  */
NT3D_hierarchic_object_o::NT3D_hierarchic_object_o(const NT3D_hierarchic_object_o & original)
:
NT3D_object_o(original)
{ 
  oTranslation = original.oTranslation;
  oRotation = original.oRotation;
  bHaveParent = NT_FALSE;
  bHaveChild = original.bHaveChild;
  uiChildCounter = original.uiChildCounter;
  pParent = NULL;
  opChildren = original.opChildren;
}

 
NT3D_hierarchic_object_o::~NT3D_hierarchic_object_o()
{
  bHaveParent = NT_FALSE;
  bHaveChild = NT_FALSE;
}

/* Public */


/** Draws the rotated and translated object and the entire children
  tree.  The matrix previous to this methods call is stored and
  restored after the children tree is drawn as last action.
 */
void
NT3D_hierarchic_object_o::Draw() 
{
  glPushMatrix();
  glTranslatef(oTranslation.x, oTranslation.y, oTranslation.z);
  glRotatef(oRotation.x, 1.0, 0.0, 0.0);
  glRotatef(oRotation.y, 0.0, 1.0, 0.0);
  glRotatef(oRotation.z, 0.0, 0.0, 1.0);
  DrawObj();
  DrawChild();
  glPopMatrix();
}



/** Used to weakly check the consistency of the NT3D object. It is used mainly to ensure that the pointers to children and parent objects in the render tree really point to a NT3D object */
NTreturn
NT3D_hierarchic_object_o::NT3DObjectCheck() const
{
	if (NT_TRUE == bHaveChild){
		if (opChildren.size() == uiChildCounter) return NT_SUCCESS;
		else return NT_FAIL;
	} else if (NT_FALSE == bHaveChild){
	return NT_SUCCESS;
	}	
	else return NT_FAIL;
	
	
}

/** @return{ the id of the child in the object list} */
NTuint
NT3D_hierarchic_object_o::AddChild( NT3D_hierarchic_object_o * pNewChild)
{
	if ( pNewChild == this ) cerr << "NT3D_hierarchic_object_o::SetChild - ERROR : trying to set self as child !" << endl;
	else if ( pNewChild == NULL ) cerr << "NT3D_hierarchic_object_o::SetChild - ERROR : trying to set NULL as child !" << endl;
	else if ( pNewChild->NT3DObjectCheck() != NT_SUCCESS ) cerr << "NT3D_hierarchic_object_o::SetChild - ERROR : child object not ok !" << endl;

  opChildren.push_back(pNewChild);
  uiChildCounter++;
  bHaveChild = NT_TRUE;
  
  pNewChild->SetParent(this);
  NT_ASSERT_POSTCOND( opChildren.size() == uiChildCounter );
  return uiChildCounter;
}

/** remove child by providing the child´s pointer  
@return NT_SUCCESS if child was removed succesfully. Otherwise
    NT_FAIL. */
/* has to be reimplemented
NTreturn
NT3D_hierarchic_object_o::RemoveChild( NT3D_hierarchic_object_o * pOldChild)
{
  if (uiChildCounter < 1) return NT_FAIL;
  else {  
    NTint iChildId = -1;*/
    /* Search for child */
  /*  for (NTuint ll=0; ll < uiChildCounter; ll++){
      if (pOldChild == opChildren[ll]) iChildId = ll;
    }    
*/    
/* Remove child if possible */
 /*   if (-1 == iChildId) return NT_FAIL;
    else {
      opChildren.remove(iChildId);
      uiChildCounter--;
      NT_ASSERT_POSTCOND( opChildren.size() == uiChildCounter );
      if (0 == uiChildCounter) bHaveChild = NT_FALSE;
      return NT_SUCCESS;
    }
  }
}
*/

/** remot the child which is at position childId in the child leaf
  list.  @return{ NT_SUCCESS if child was removed
  succesfully. Otherwise NT_FAIL.} */
/* has to be reimplemented
NTreturn
NT3D_hierarchic_object_o::RemoveChild( NTuint childId)
{*/
	/* Remove child ,if possible */
/*    if (childId > uiChildCounter) return NT_FAIL;
    else {
      opChildren.remove(childId);
      uiChildCounter--;
      NT_ASSERT_POSTCOND( opChildren.length() == uiChildCounter );
      return NT_SUCCESS;
    }
}
*/ 

/* Private */

/** Draw each child with an depth first move thru the child tree */
void
NT3D_hierarchic_object_o::DrawChild() const
{

 NT3D_ChildrenQue::const_iterator tmpIterator;
  for (tmpIterator = opChildren.begin();
 		tmpIterator != opChildren.end();
 		++tmpIterator)
  {
    (*tmpIterator)->Draw();
  }
}

void 
NT3D_hierarchic_object_o::ControlImpl(NT3Dcontroller cntrl , NTint value)
{
 NT3D_ChildrenQue::iterator tmpIterator;

 ControlObj( cntrl , value);

 for (tmpIterator = opChildren.begin();
 		tmpIterator != opChildren.end();
 		++tmpIterator)
  {
    (*tmpIterator)->Control(cntrl , value);
  }
}

void 
NT3D_hierarchic_object_o::ControlImpl(NT3Dcontroller cntrl , NTreal value)
{
 NT3D_ChildrenQue::iterator tmpIterator;

ControlObj( cntrl , value);

 for (tmpIterator = opChildren.begin();
 		tmpIterator != opChildren.end();
 		++tmpIterator)
  {
    (*tmpIterator)->Control(cntrl , value);
  }

}

void 
NT3D_hierarchic_object_o::ControlImpl(NT3Dcontroller cntrl , NT_vector_o value)
{
  NT3D_ChildrenQue::iterator tmpIterator;
  ControlObj( cntrl ,  value);


 for (tmpIterator = opChildren.begin();
 		tmpIterator != opChildren.end();
 		++tmpIterator)
  {
    (*tmpIterator)->Control(cntrl , value);
  }


}


  
void
NT3D_hierarchic_object_o::SetParent(NT3D_hierarchic_object_o * pNewParent)
{
	if ( pNewParent == this ) cerr << "NT3D_hierarchic_object_o::SetParent - ERROR : trying to set self as parent !" << endl;
	else if ( pNewParent == NULL ) cerr << "NT3D_hierarchic_object_o::SetParent - ERROR : trying to set NULL as parent !" << endl;
	else if ( pNewParent->NT3DObjectCheck() != NT_SUCCESS ) cerr << "NT3D_hierarchic_object_o::SetParent - ERROR : parent object not ok !" << endl;
	
	bHaveParent = NT_TRUE;	
	pParent = pNewParent;
}











