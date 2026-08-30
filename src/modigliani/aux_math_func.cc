/**
 * @file aux_math_func.cc
 * @brief Auxiliary functions implementation
 *
 * Copyright (C) 1998,1999,2000 Ahmed Aldo Faisal
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

#include "modigliani/aux_math_func.h"

#include <chrono>
#include <iostream>
#include <random>

using modigliani::Real;

namespace {
/**
 * A cheap 32-bit avalanche (splitmix style). Seeding several Mersenne
 * twisters with consecutive integers correlates their initial output, so
 * spread the counter across the whole word before handing it out.
 */
unsigned int Mix(unsigned int value) {
  value += 0x9e3779b9u;
  value = (value ^ (value >> 16)) * 0x21f0aaadu;
  value = (value ^ (value >> 15)) * 0x735a2d97u;
  return value ^ (value >> 15);
}

bool base_chosen = false;
unsigned int base_seed = 0;
unsigned int seed_counter = 0;
}  // namespace

void modigliani::SetSeedBase(unsigned int base) {
  base_seed = base;
  base_chosen = true;
  seed_counter = 0;
}

unsigned int modigliani::SeedBase() {
  if (!base_chosen) {
    // std::random_device is permitted to be a deterministic fallback, so
    // mix in a high resolution timestamp: processes started in the same
    // second must still get independent streams.
    std::random_device device;
    unsigned int now = static_cast<unsigned int>(
      std::chrono::high_resolution_clock::now().time_since_epoch().count());
    base_seed = device() ^ Mix(now);
    base_chosen = true;
    seed_counter = 0;
    std::cout << "Modigliani RNG base seed = " << base_seed
              << " (repeat this run with --seed " << base_seed << ")"
              << std::endl;
  }
  return base_seed;
}

unsigned int modigliani::NextSeed() {
  return Mix(SeedBase() + seed_counter++);
}

Real modigliani::ComputePLogP(Real prob) {
    if (0.0 == prob)
        return (0);
    else if (prob < 0.0) {
        std::cerr
            << "ComputeEntropyElement - Error :"
            << " Negative probability encountered."
                << std::endl;
        return (0);
    } else {
        return (prob * log(prob) / 0.69314718055994530942 /* log(2) */);
    }
}

Real modigliani::Limit01(Real value) {
    if (value > 1.0)
        return (1.0);
    else if (value < 0.0)
        return (0.0);
    else
        return (value);
}

Real modigliani::Round(Real value) {
    return ((Real) (floor(value + 0.5)));
}

Real modigliani::LogGammaFunction(Real arg) {
    double x, tmp, ser;
    static double cof[6] = { 76.18009173, -86.50532033, 24.01409822,
            -1.231739516, 0.120858003e-2, -0.536382e-5 };
    int j;

    x = arg - 1.0;
    tmp = x + 5.5;
    tmp -= (x + 0.5) * log(tmp);
    ser = 1.0;
    for (j = 0; j <= 5; j++) {
        x += 1.0;
        ser += cof[j] / x;
    }
    return (-tmp + log(2.50662827465 * ser));
}

int modigliani::IsNAN(const Real value) {
  return (static_cast<int>(value != value));
}

int modigliani::IsInf(const Real x) {
    double y = x - x;
    int s = (y != y);

    if (s && x > 0)
        return (+1);
    else if (s && x < 0)
        return (-1);
    else
        return (0);
}
