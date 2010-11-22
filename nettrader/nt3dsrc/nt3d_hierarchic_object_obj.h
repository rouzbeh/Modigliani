/* \file nt3d_hierarchic_object_obj.h - NT3D hierrchic object definition
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

#ifndef _nt3d_hierarchic_object_h_
#define _nt3d_hierarchic_object_h_

#include "nt_main.h"
#include "nt_types.h"

#include "nt_vector_obj.h"
#include "nt_vector3_obj.h"
#include <deque>
//#include "nt_data_obj.h"

#include "nt3d_obj.h"
#include "nt3d_object_obj.h"


  
//
//	Class name : NT3D_hierarchic_object_o
//
/**	NetTrader 3D object visualisation superclass.
 
	The "main axis" of an object in space,  
	should be constructed along the X-Axis, as methods
	will assume this.
 */
class NT3D_hierarchic_object_o : public NT3D_object_o {
public:
  NT3D_hierarchic_object_o();
  NT3D_hierarchic_object_o(const NT_vector3_o & oNewPosition);
  NT3D_hierarchic_object_o(const NT3D_hierarchic_object_o & original);
  /** 2DO: Destroy or not destroy parent child objects ? */  
  ~NT3D_hierarchic_object_o();
  /*                         */  
 

 /** The Draw() method is the invocation method to use for users.
    It handles the drawing of the object, its correct positioning and the drawing of the child objects. However, it should not be overridden!*/
  void Draw();
  
  /** Draws the unrotated and untranslated object.
    DrawObj() is method should be overridden by derived classes 
    to implement to pure unrotated untranslated object. 
    @warning Note, it has to be declard virtual here, 
    such that upon invocation via Draw() the child method is 
    exectuded and not the parent method !*/
  


  NTuint   AddChild( NT3D_hierarchic_object_o * pNewChild ); 
  NTreturn RemoveChild( NT3D_hierarchic_object_o * pOldChild );  
  NTreturn RemoveChild( NTuint OldChild );  

  protected:
  virtual void DrawObj() {cerr <<"NT3D_hierarchic_object_o::DrawObj() - Error : This method should be overriden by derived classes"<<endl;};

  NTreturn NT3DObjectCheck() const;
  /** ControlImpl overwrites the NT3D_object_o controllers,
      and are not to be overwritten themselves by derived classes,
      derived classes implement/overwrite ControlObj */    
  void ControlImpl(NT3Dcontroller cntrl , NTint value);
  void ControlImpl(NT3Dcontroller cntrl , NTreal value); 
  void ControlImpl(NT3Dcontroller cntrl , NT_vector_o value);
  /** ControlObj is to overwritten by derived classes to
      handle controller calls */
  virtual void ControlObj(NT3Dcontroller cntrl , NTint value){NT_CERR(3,"NT3D_hierarchic_objet_o::ControlObj(...,NTint) - Warning : Not overridden be derived object.");};
  virtual void ControlObj(NT3Dcontroller cntrl , NTreal value){}; 
  virtual void ControlObj(NT3Dcontroller cntrl , NT_vector_o value){};
  private:
  void DrawChild() const;
  void SetParent(NT3D_hierarchic_object_o * pNewParent);
  /*                         */ 	
  //  NT3D_rainbow_o oColor;
  NTbool bHaveParent;
  NTbool bHaveChild;
  NTuint uiChildCounter;
  NT3D_hierarchic_object_o * pParent;
  deque<NT3D_hierarchic_object_o *> opChildren;
//  NT_data_o<NT3D_hierarchic_object_o *> opChildren;  
};

typedef deque<NT3D_hierarchic_object_o *> NT3D_ChildrenQue;

#endif /* _nt3d_hierarchic_object_h_ */


