/**
 * @file obj.h
 * Master object declaration
 * @author Ahmed A. Faisal, 22. 5. 1998(c)
 * NetTrader - finance management, analysis and simulation system
 * @version  0.1
 * Copyright (C) 1998 Ahmed Aldo Faisal
 *
 * @section LICENSE
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


#ifndef _mbase_H_
#define _mbase_H_

#include "types.h"

namespace modigliani_base{
/**
 * @class Obj
 * @short Master class for all M objects.
 * It provides object accounting, and a unique identifier (uniqId) for
 * each object.  The unique id might become ambigous when uniqId gets
 * greater LONG_MAX. However, the initialisation of each M object with
 * this class might help in synchronisation of a threaded version.
 */
class Obj {
public:
    Obj(); // 2DO : the constructor is not thread safe !
    Obj(const Obj &);
    virtual ~Obj();

    //protected:
    Long _runnId() const;
    Long _uniqId() const;



private:
    /** unique Id number of the object
      static data member,  see implementation */
    Long uniqId;
    /** uniqId of the last created Obj */
    static Long runnId;
};
}

#endif /* _mbase_H_ */