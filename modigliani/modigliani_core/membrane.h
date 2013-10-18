/**
 * @file membrane.h
 * @brief Membrane class header
 *
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal    
 * Copyright (C) 2013 Mohammad Ali Neishabouri
 * @section LICENSE
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

#ifndef MODIGLIANI_MODIGLIANI_CORE_MEMBRANE_H_
#define MODIGLIANI_MODIGLIANI_CORE_MEMBRANE_H_

#include "modigliani_core/object.h"

namespace modigliani_core {
/**
 * @brief Membrane class
 */
class Membrane: public Object {
  public:
    Membrane();
    Membrane(const Membrane & original) = delete;
    Membrane & operator=(const Membrane & right) = delete;
    virtual ~Membrane();
};
}  // namespace modigliani_core
#endif  // MODIGLIANI_MODIGLIANI_CORE_MEMBRANE_H_
