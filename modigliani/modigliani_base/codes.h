/**
 * @file codes.h
 * Defintions of codes
 * @author Ahmed A. Faisal, 22. 5. 1998(c)
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

#ifndef MODIGLIANI_MODIGLIANI_BASE_CODES_H_
#define MODIGLIANI_MODIGLIANI_BASE_CODES_H_

#include <string>
#include "modigliani_base/main.h"

namespace modigliani_base {
/**
 *  CLASS CODES
 */
  enum ClassCode {
    /** M class */
    MMATH_O = 1,
    /** M3D classes */
    M3D_O = 1000000
  };

/** image/bitmap related  CODES */
  enum ImageFileType {
    AUTO,
    RAW,
    M = 1,
    PPM,
    BMP,
    TGA,
    GIF,
    JPG,
    PNG,
    UNKNOWN
  };
  enum ImageDataFormat {
    SINGLE_CHANNEL = 1,
    RGB = 3,
    RGBA = 4,
    RED,
    GREEN,
    BLUE,
    ALPHA
  };

/** COUMRY CODES == intl. phone code */
  enum CountryCode {
    EU = -1, USA = 1, ITALY = 39, UK = 44, GERMANY = 49
  };

/* *** CURRENCY NAMES *** */
  const std::string EURO_CURRENCY = "Euro";
  const std::string USA_CURRENCY = "US-Dollar";
  const std::string ITALY_CURRENCY = "Lira Italiane";
  const std::string GERMANY_CURRENCY = "Deutsche Mark";

  enum StockExchangeId {
    STKEXC_UNDEF = 0,
    /* USA */
    STKEXC_NYSE = 1000, STKEXC_NASDAQ = 2000,
    /* EUROPE */
    STKEXC_FTSE = 10000, STKEXC_FSE = 20000
  };

  const int UNDEF = 0;
  const int CURRENCY = 100;
  const int STOCK_VALUE = 200;

}  // namespace modigliani_base
#endif  // MODIGLIANI_MODIGLIANI_BASE_CODES_H_
