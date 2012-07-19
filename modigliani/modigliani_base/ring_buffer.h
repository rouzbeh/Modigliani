/**
 * @file ring_buffer.h
 * Ring_buffer class header
 * @author Ahmed Aldo Faisal &copy; created 8.10.2001
 *
 * NetTrader - visualisation, scientific and financial analysis and simulation system
 * @version  0.5
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
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

#ifndef _mbase_ring_buffer_h_
#define _mbase_ring_buffer_h_

/* M core includes */
#include "main.h"
#include "types.h"
#include "obj.h"
/* M includes */
#include "error.h"

/* other includes */

#include <vector>

namespace modigliani_base {
/** @short Ring_buffer class
 \bug unknown
 \warning unknown
 */
template<class T>
class Ring_buffer {
public:
    /***   Constructors, Copy/Assignment and Destructor  ***/
    Ring_buffer(Size newRingLength = 2) {
        Init(newRingLength);
    }
    Ring_buffer(const Ring_buffer & original);
    const Ring_buffer & operator=(const Ring_buffer & right);
    virtual ~Ring_buffer() {
    }
    /* ***  Methods              ***/
    Size _ringLength() const {
        return (ringLength);
    }
    /** after resize states are reset to scratch */
    ReturnEnum Resize(Size newRingLength) {
        Init(newRingLength);
        return (ReturnEnum::SUCCESS);
    }
    void Push(T value) {
        bufferVec[IncreaseCounter()] = value;
    }
    void Show() {
        std::vector<T> tmpVec = Buffer();
        for (Size ll = 0; ll < _ringLength(); ll++)
            std::cout << tmpVec[ll] << " ";
        std::cout << std::endl;
    }
    /* highest index is newest entry */
    std::vector<T> Buffer() {
        std::vector<T> tmpVec(ringLength);
        for (Size ll = 0; ll < _ringLength(); ll++)
            tmpVec[ll] = bufferVec[(_counter() + ll + 1) % ringLength];
        return (tmpVec);
    }
    /* ***  Data                 ***/
protected:
    /* ***  Methods              ***/
    Size IncreaseCounter() {
        counter = (counter + 1) % ringLength;
        return (counter);
    }
    Size _counter() {
        return (counter);
    }
    /* ***  Data                 ***/
    Size ringLength;
private:
    /* ***  Methods              ***/
    std::vector<T> bufferVec;
    Size counter;
    void Init(Size newRingLength) {
        counter = 0;
        ringLength = newRingLength;
        bufferVec.resize(ringLength);
    }
    /* ***  Data                 ***/
};
}
#endif /* _mbase_ring_buffer_h_ */