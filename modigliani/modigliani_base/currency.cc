/** @file currency.cc
 * Class implementation for currency object
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

#include "currency.h"

using namespace modigliani_base;

//
//    Method name : nt_currency_obj
//
//    Description : standard constructor
//    Input :
//    Output :
//
Currency::Currency()
{
}

//
//    Method name : Currency
//
//    Description : elaborate constructor
//    Input :
//    Output :
//
Currency::Currency(CountryCode Initcountry, Real InitconversionFactor)
{
    set_amount(0);
    set_id(modigliani_base::CURRENCY);
    set_name(countryCode2currencyName(Initcountry)); // 2DO implement conversion routine
    set_value(0.0);
    countryCode = Initcountry;
    conversionFactor = InitconversionFactor;
}




//
//    Method name : ~Currency
//
//    Description : standard destructor
//    Input :
//    Output :
//
Currency::~Currency()
{

}

//
//    Method name : code2country(McoutnryCode)
//
/**    Description : convert a country code into the apropriate currency name
    Input : valid country code (see nt_codes.h)
    Output : deletable M_std::string (pointer to Mchar[M_STRING_LENGTH)
*/
std::string Currency::countryCode2currencyName(CountryCode code)
{
    std::string country;
    switch (code) {
    case EU :
        country = EURO_CURRENCY;
        break;
    case USA :
        country = USA_CURRENCY;
        break;
    case ITALY :
        country =  ITALY_CURRENCY;
        break;
    case GERMANY :
        country =  GERMANY_CURRENCY;
        break;
    default :
        country =  "UNKOWN CURRENCY";
        break;
    }
    return (country);
}


//* NOTE : const is omitted to access the element functions of the base class  */
std::ostream & modigliani_base::operator<<(std::ostream & os,  const   Currency & self)
{
    std::string osName = self._name();
    os << "Currency"  << M_SEPARATOR << self._id() << M_SEPARATOR
    << self._amount() << M_SEPARATOR  <<  osName << M_SEPARATOR
    << self._value() << M_SEPARATOR << (int) self.countryCode << M_SEPARATOR
    << self.conversionFactor << std::endl;

    return (os);
}
