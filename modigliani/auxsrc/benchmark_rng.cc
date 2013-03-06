/*
 * benchmark_rng.cc
 *
 *  Created on: Mar 1, 2013
 *      Author: man210
 */
#include "modigliani_base/uniform_rnd_dist.h"
#include "modigliani_base/binomial_rnd_dist.h"
#include <time.h>
#ifdef WITH_GSL
extern "C" {
#include <gsl/gsl_randist.h>
}
#endif
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/binomial_distribution.hpp>
#include <boost/program_options.hpp>

int main(int argc, char* argv[]) {
  namespace po = boost::program_options;
  // Declare the supported options.
  po::options_description desc("Allowed options");
  desc.add_options()("help,h", "produce help message")("write,w",
                                                       "write out values")(
      "algorithm,a", po::value<std::string>(), "set algorithm");

  int test;
  po::positional_options_description p;
  p.add("algorithm", -1);
  po::variables_map vm;
  po::store(
      po::command_line_parser(argc, argv).options(desc).positional(p).run(),
      vm);
  po::notify(vm);

  auto iterations = 100000000;
  if (vm.count("write")) iterations = 10000;
  if (!vm.count("algorithm")) return (1);

  if (std::string("aldo") == vm["algorithm"].as<std::string>()) {
    modigliani_base::Binomial_rnd_dist binomRnd(0.0, 100);
    for (int i = 0; i < iterations; i++) {
      if (vm.count("write")) std::cout << binomRnd.Binomial(0.5, 100)
                                       << std::endl;
      else test += binomRnd.Binomial(0.5, 100);
    }

    return (test + 1);
  }

#ifdef WITH_GSL
  if (std::string("gsl") == vm["algorithm"].as<std::string>()) {
    std::cout << "%Using gnu science library" << std::endl;
    long seed;
    gsl_rng *rng;  // random number generator

    rng = gsl_rng_alloc(gsl_rng_rand48);// pick random number generator
    seed = time(NULL);
    gsl_rng_set(rng, seed);// set seed
    for (int i = 0; i < iterations; i++) {
      if (vm.count("write")) std::cout << gsl_ran_binomial(rng, 0.5, 100)
      << std::endl;
      else test += gsl_ran_binomial(rng, 0.5, 100);  // get a random number
    }
    gsl_rng_free(rng);                       // dealloc the rng
    return (test + 1);
  }
#endif

  if (std::string("boost") == vm["algorithm"].as<std::string>()) {
    boost::random::mt19937 rng;
    boost::random::binomial_distribution<> bin(100, 0.5);
    for (int i = 0; i < iterations; i++) {
      bin.param(
          boost::random::binomial_distribution<>::param_type { 100, 0.5 });
      if (vm.count("write")) std::cout << bin(rng) << std::endl;
      else test += bin(rng);  // get a random number
    }

    return (int(test) + 1);
  }
}
