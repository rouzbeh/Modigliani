/***************************************************************************
                          modigliani_model_format_grammar_t.h  -  description
                             -------------------
    begin                : Mon May 19 2003
    copyright            : (C) 2003 by Ahmed Aldo Faisal
    email                : a.faisal@gmx.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _MODIGLIANI_MODEL_FORMAT_GRAMMAR_T_H_
#define _MODIGLIANI_MODEL_FORMAT_GRAMMAR_T_H_

#define BOOST_SPIRIT_DEBUG

#include <boost/spirit/core.hpp>
#include <boost/spirit/utility.hpp>
#include <boost/spirit/symbols.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stack>

#include <cmath>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <algorithm>

#include "ntbp_membrane_compartment_sequence_obj.h"
#include "ntbp_sga_cylindrical_compartment_obj.h"
#include "ntbp_custom_cylindrical_compartment_obj.h"

#include "nt_error_obj.h"
#include "nt_vector_obj.h"

#include "nt_histogram_obj.h"
#include "nt_sequential_statistics_obj.h"
#include "tnt/tnt.h"
#include "tnt/cmat.h"
#include "nt_uniform_rnd_dist_obj.h"
#include "nt_binomial_rnd_dist_obj.h"
#include "nt_gaussian_rnd_dist_obj.h"
#include "nt3d_plot2d_vec_vp_obj.h"
#include "nt3d_glx_drv_obj.h"



using namespace std;
using namespace boost::spirit;

stack <void*> modigliani_core_stack;


void BeginMakeMembraneCompartmentSequence()
{
    NTBP_membrane_compartment_sequence_o* tmpPtr =  new  NTBP_membrane_compartment_sequence_o;
    modigliani_core_stack.push(tmpPtr);
}

void EndMakeMembraneCompartmentSequence()
{
}


void BeginMakeCylindricalMembraneCompartment()
{
    NTBP_custom_cylindrical_compartment_o *tmpPtr =	 new NTBP_custom_cylindrical_compartment_o;
    modigliani_core_stack.push(tmpPtr);
}

void EndMakeCylindricalMembraneCompartment()
{
    NTBP_custom_cylindrical_compartment_o *tmpPtr	= static_cast(NTBP_custom_cylindrical_compartment_o*)	modigliani_core_stack.pop();
    static_cast<NTBP_membrane_compartment_sequence_o*>(modigliani_core_stack.top())->PushBak(tmpPtr);
}


void BeginMakeSphericalMembraneCompartment()
{
    cerr << "modigliani_core::MakeSphericalMembraneCompartment - Error : Not implemented." << endl;
    exit(1);
}
void EndMakeSphericalMembraneCompartment()
{
    cerr << "modigliani_core::MakeSphericalMembraneCompartment - Error : Not implemented." << endl;
    exit(1);
}


/*
         	  tmpNaPtr->SetSimulationMode(NTBP_DETERMINISTIC);
  				  tmpNaPtr->SetSimulationMode(NTBP_LANGEVIN);
  				  tmpNaPtr->SetSimulationMode(NTBP_BINOMIALPOPULATION);
*/
void BeginMakeIonChannel(NTid channelTypeNa)
{
    NTBP_membrane_current_o *tmpNaPtr;
    switch (channelTypeNa) {
    case 1:
        /* Stochastic active ion-channel currents */
        tmpNaPtr =
            new NTBP_multi_sodium_current_o (static_cast<NTBP_membrane_compartment_o*>(modigliani_core_stack.top())->_area(),
                                             60, 20e-9	/* mSiemens per channel */
                                             break;
                                         default: cerr <<"Somasimulator-v1 - Error  : Invalid Na channel type specified. Exiting."; exit(NT_PARAM_OUT_OF_RANGE); break;
                                             }
                                         modigliani_core_stack.push(tmpNaPtr);
}

void EndMakeIonChannel()
{
    NTBP_membrane_current_o *tmpPtr = static_cast<NTBP_membrane_current_o*>(modigliani_core_stack.pop());
    static_cast<NTBP_membrane_compartment_o*>(modigliani_core_stack.top())->AttachCurrent (tmpNaPtr, NTBP_IONIC);
}



struct ModiglianiFormatSkipGrammar_t : public grammar<ModiglianiFormatSkipGrammar_t>
{
    template <typename ScannerT>
    struct definition
    {
        definition(ModiglianiFormatSkipGrammar_t const& /*self*/)
        {
            skip =            "//" >> *(anychar_p - '\n') >> '\n'     // C++ comment
                              | "/*" >> *(anychar_p - "*/") >> "*/";     // C comment

        }
        rule<ScannerT> skip;

        rule<ScannerT> const&
        start() const {
            return skip;
        }
    };
};


struct ModiglianiModelFormatGrammar_t : public grammar< ModiglianiModelFormatGrammar_t >
{
    template <typename ScannerT>

    struct definition
    {
        definition(ModiglianiModelFormatGrammar_t const& /* self */)
        {
            // NOTE: additional tokes are ion_channel_model names and algorithm names defined in place the respective rules

            keywords = "NEURON","SEQUENCE","TREE","SPHERE","CYLINDER","SPECIFIC_RESISTANCE","SPECIFIC_CAPACITANCE",
                       "RADIUS","LENGTH","TEMPERATURE","ION_CHANNEL","CHANNEL_CONDUCTANCE","Q10","BASE_TEMPERATURE",
                       "DENSITY","ALGORITHM","IDENTIFIER";

            // highlevel tokens
            NEURON   = strlit<>("NEURON");
            SEQUENCE = strlit<>("SEQUENCE");
            TREE     = strlit<>("TREE");

            // compartment tokens
            SPHERE   = strlit<>("SPHERE");
            CYLINDER = strlit<>("CYLINDER");
            SPECIFIC_RESISTANCE  = strlit<>("SPECIFIC_RESISTANCE");
            SPECIFIC_CAPACITANCE = strlit<>("SPECIFIC_CAPACITANCE");
            RADIUS      = strlit<>("RADIUS");
            LENGTH      = strlit<>("LENGTH");
            TEMPERATURE = strlit<>("TEMPERATURE");
            RADIUS      = strlit<>("RADIUS");

            // ion channel tokens
            ION_CHANNEL = strlit<>("ION_CHANNEL");
            CHANNEL_CONDUCTANCE = strlit<>("CHANNEL_CONDUCTANCE");
            Q10 =              strlit<>("Q10");
            BASE_TEMPERATURE = strlit<>("BASE_TEMPERATURE");
            DENSITY     = strlit<>("DENSITY");
            ALGORITHM   = strlit<>("ALGORITHM");

            // C-style variable names
            IDENTIFIER =  lexeme_d[
                              ((alpha_p | '_' | '$') >> *(alnum_p | '_' | '$'))
                              - (keywords >> anychar_p - (alnum_p | '_' | '$'))
                          ];


            // root symbol
            expression_ = (network_ | neuron_);

            network_ = NEURON >> ch_p('{') >> (*(uint_p >> neuron_ >> (*( uint_p >> uint_p))))  >> ch_p('}'); //2DO NOT IMPLEMENTED

            neuron_    = NEURON >> ch_p('{') >> membrane_ >> ch_p('}');

            membrane_  = (membrane_compartment_sequence_ | membrane_compartment_tree_ | membrane_compartment_ );

            membrane_compartment_sequence_ = SEQUENCE >> ch_p('{') >> membrane_compartment_ >> (* membrane_compartment_) >> ch_p('}');

            membrane_compartment_tree_ = TREE >>  ch_p('{') >> (*(uint_p >> membrane_compartment_ >> (*(uint_p)) )) >> ch_p('}');

            membrane_compartment_ = membrane_compartment_type_ >> ch_p('{') >> (* (membrane_compartment_param_ | ion_channel_)) >> ch_p('}');

            membrane_compartment_type_ = SPHERE | CYLINDER;

            membrane_compartment_param_ = (membrane_resistance_ | membrane_capacitance_ | radius_ | length_ | temperature_);

            membrane_resistance_ = SPECIFIC_RESISTANCE >> real_p;

            membrane_capacitance_ = SPECIFIC_CAPACITANCE >> real_p;

            radius_ =  (RADIUS >> real_p);

            length_ =  LENGTH >> real_p;

            temperature_ = TEMPERATURE >> real_p;

            ion_channel_ = ION_CHANNEL >> ion_channel_model_ >> ch_p('{')  >> (*(ion_channel_param_)) >> ch_p('}');

            ion_channel_param_ = ( ion_channel_algorithm_ |
                                   single_channel_conductance_ |
                                   q10_ |
                                   base_temperature_ |
                                   channel_density_
                                 );

            ion_channel_model_ =  ( str_p("NA_SGA_HH") |
                                    str_p("NA_PATLAK") |
                                    str_p("NA_COLBERT_AXONAL") |
                                    str_p("NA_HRANVIER")
                                  );


            ion_channel_algorithm_ = ALGORITHM >> (     str_p("DETERMINISTIC") |
                                     str_p("LANGEVIN") |
                                     str_p("SINGLE_CHANNEL") |
                                     str_p("BINOMIALPOPULATION") |
                                     str_p("GILLESPIE")      |
                                     str_p("NOISY_MEAN")
                                                  );

            single_channel_conductance_ = CHANNEL_CONDUCTANCE >> real_p;

            q10_ = Q10 >> real_p;

            base_temperature_ = BASE_TEMPERATURE >> real_p;

            channel_density_ = DENSITY >> real_p;
        }


        // keywords
        symbols<> keywords;

        // terminal symbols
        rule<ScannerT>  NEURON,SEQUENCE,TREE,SPHERE,CYLINDER,SPECIFIC_RESISTANCE,SPECIFIC_CAPACITANCE,
        RADIUS,LENGTH,TEMPERATURE,ION_CHANNEL,CHANNEL_CONDUCTANCE,Q10,BASE_TEMPERATURE,
        DENSITY,ALGORITHM,IDENTIFIER;


        // nonterminal symbols
        rule<ScannerT> expression_,
        network_, neuron_,
        membrane_,
        membrane_compartment_sequence_, membrane_compartment_tree_,
        membrane_compartment_, membrane_compartment_type_,
        ion_channel_,
        ion_channel_model_, ion_channel_param_, ion_channel_algorithm_,
        single_channel_conductance_, q10_, base_temperature_, channel_density_,
        membrane_compartment_param_,
        membrane_resistance_, membrane_capacitance_, radius_, length_, temperature_;

        rule<ScannerT> const& start() const {
            return neuron_;
        }

    };

};



template<typename GrammarT>
void
parse(GrammarT const& g, char const* filename)
{
    ifstream in(filename);

    if (!in)
    {
        cerr << "Could not open input file: " << filename << endl;
        return;
    }

    in.unsetf(ios::skipws); //  Turn of white space skipping on the stream

    vector<char> vec;
    std::copy(
        istream_iterator<char>(in),
        istream_iterator<char>(),
        std::back_inserter(vec));

    vector<char>::const_iterator start = vec.begin();
    vector<char>::const_iterator end = vec.end();

    ModiglianiFormatSkipGrammar_t skip;

    parse_info<vector<char>::const_iterator> result =
        parse(start, end, g, skip);

    if (result.full)
        cerr << filename << " Parses OK" << endl;
    else {
        cerr << filename << " Fails Parsing" << endl;
        for (int i = 0; i < 50; i++)
        {
            if (result.stop == end)
                break;
            cerr << *result.stop++;
        }
        cerr << endl;
    }
}



#endif /* _MODIGLIANI_MODEL_FORMAT_GRAMMAR_H_ */
