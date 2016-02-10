/**
 * @file obj.cc
 * @brief Obj class implementation
 *
 * Copyright (C) 1998 Ahmed Aldo Faisal
 *
 * @section LICENCE
 * This file is part of Modigliani.
 *
 * Modigliani is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Modigliani.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "modigliani_base/obj.h"

namespace modigliani_base {
/* static data member declaration in C++ part 2, see declaration */
Long Obj::runnId = 0;

Obj::Obj() {
  uniqId = 0;
#ifdef M_DEBUG_OBJECT_ID
  runnId++;
  uniqId = runnId;
#endif
}

Obj::Obj(Obj const __attribute__((unused)) & right) {
  uniqId = 0;
#ifdef M_DEBUG_OBJECT_ID
  runnId++;
  uniqId = runnId;
#endif
}

Obj::~Obj() {
}

inline Long Obj::_runnId() const {
  return (runnId);
}

inline Long Obj::_uniqId() const {
  return (uniqId);
}

}  // namespace modigliani_base
