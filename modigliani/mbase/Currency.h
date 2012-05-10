/* Currency.h - class header for currency object */
/* by Ahmed A. Faisal, 22. 5. 1998(c) */


/*
 * NetTrader - finance managemeM, analysis and simulation system
 * Version:  0.1
 * Copyright (C) 1998 Ahmed Aldo Faisal
 *
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
/**	A general class for M supported coutnries == money class
 */
namespace mbase{
class Currency : public Value
{
private:
	CountryCode countryCode;
    Mreal conversionFactor;

protected:
    void set_country (mbase::CountryCode newCoutryCode);
    void set_conversionFactor (Mreal newConversionFactor);

public:
    Currency();
    Currency(mbase::CountryCode initCountry, Mreal initConversionFactor);
    Currency(const Currency &);
    ~Currency();
    std::string countryCode2currencyName(mbase::CountryCode);

    friend std::ostream & operator<<(std::ostream & os,  const  Currency & self);
};
}
