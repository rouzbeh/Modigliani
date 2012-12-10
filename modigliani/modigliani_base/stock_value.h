/**
 * @file stock_value.h
 * Base class for stock exchangable values
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

#include "value.h"
#include "date.h"

namespace modigliani_base{

/**
 *     Bbase class for values traded at a stock exchange
 */
class Stock_value : public Value {

private:
    Mnum stockExchangeId; // where the value is traded
    Real currentBuyingPrice;
    Real currentSellingPrice;
    Date emissionDate;

protected:

public:
    Stock_value();
    Stock_value(const Stock_value &);
    ~Stock_value();

    friend std::ostream & operator<<(std::ostream & os, const Stock_value & self);
};
}

