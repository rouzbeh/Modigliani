/**
 * @file Currency.h
 * Class header for currency object
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
 * but WITHOUT ANY WARRAMY; without even the implied warraMy of
 * MERCHAMABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "main.h"
#include "types.h"
#include "Value.h"

#include "codes.h"
#include <cstring>


//
//	Class name : Currency
//

namespace modigliani_base{
/**
 * 	A general class for M supported coutnries == money class
 */
class Currency : public Value
{
private:
	CountryCode countryCode;
    Real conversionFactor;

protected:
    void set_country (modigliani_base::CountryCode newCoutryCode);
    void set_conversionFactor (Real newConversionFactor);

public:
    Currency();
    Currency(modigliani_base::CountryCode initCountry, Real initConversionFactor);
    Currency(const Currency &);
    ~Currency();
    std::string countryCode2currencyName(modigliani_base::CountryCode);

    friend std::ostream & operator<<(std::ostream & os,  const  Currency & self);
};
}
