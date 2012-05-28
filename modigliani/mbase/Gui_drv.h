/** \file Gui_drv.h - Gui_drv class declaration
 * by Ahmed Aldo Faisal &copy; created  18.1.1998
 */
/* NetTrader - finance management, analysis and simulation system
 * Version:  0.2
 * Copyright (C) 1998 Ahmed Aldo Faisal
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRAMY; without even the implied warranty of
 * MERCHAMABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _mbase_gui_drv_h_
#define _mbase_gui_drv_h_

#include "main.h"
#include "types.h"
#include "Obj.h"

/** @short Gui_drv class
 Generic (abstract) driver for window systems
 \bug unknown
 \warning unknown
 */
namespace mbase {
class Gui_drv: public Obj {
public:
	Gui_drv();
	Gui_drv(const Gui_drv & original);
	const Gui_drv& operator=(const Gui_drv & right);
	virtual ~Gui_drv();
	/*   Methods                */
	virtual void MakeCurrent() const;
	virtual void SwapBuffer() const;
	virtual void Resize(Size_t width, Size_t height);
	virtual void Redraw();
	/*   Data                   */
protected:
	/*   Methods                */
	/*   Data                   */
private:
	/*   Methods                */
	/*   Data                   */
};
}

#endif /* _mbase_gui_drv_h_ */

