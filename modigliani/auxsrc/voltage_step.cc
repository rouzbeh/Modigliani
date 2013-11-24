/**
 * @file voltage_step.cc
 * Performs voltage step experiments on a given Lua channel
 *
 * Copyright (C) 2010, 2011 Mohammad Ali Neishabouri
 *
 * @section LICENSE
 * This file is part of Modigliani.
 *
 * Modigliani is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Modigliani is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Modigliani.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <modigliani_core/aux_func.h>
#include <boost/program_options.hpp>

///
/// \brief Activation voltage step
///
/// \param channel_file_name Lua code for the channel
/// \param alg Simulation algorithm
/// \param step_length How long to record at each step
/// \param num_trials
/// \param output_folder
int activation(string channel_file_name, int alg,
               modigliani_base::Real step_length,
               modigliani_base::Size num_trials, string output_folder) {
  using modigliani_base::Real;
  using modigliani_core::Lua_based_stochastic_voltage_gated_channel;
  using modigliani_core::Lua_based_deterministic_voltage_gated_channel;
  using modigliani_core::Voltage_gated_ion_channel_current;
  using modigliani_core::StochasticType;
  using std::endl;
  using std::cout;

  string timed_output_folder = modigliani_core::CreateOutputFolder(
      output_folder);
  ofstream time_file, log_file, data_file;
  modigliani_core::OpenOutputFile(timed_output_folder, "Time", time_file);
  modigliani_core::OpenOutputFile(timed_output_folder, "log", log_file, ".log");
  modigliani_core::OpenOutputFile(timed_output_folder, "data", data_file,
                                  ".txt");

  Real time_step = 0.001;
  int samp_n = 1;
  Real V_hold = -110;  // holding potential
  Real V_resting = -60;

  for (modigliani_base::Size iTrials = 0; iTrials < num_trials; iTrials++) {
    for (int aim = V_hold + 5; aim <= V_resting + 50; aim += 5) {
      Voltage_gated_ion_channel_current *current_p = 0;
      if (alg == 1) {
        current_p = new Lua_based_deterministic_voltage_gated_channel
            (100,  // Area
            20,  // density,
            20,  // conductance
            -85,  // reversal potential
            time_step, 24, channel_file_name);

        current_p->set_simulation_mode(
            modigliani_core::StochasticType::DETERMINISTIC);
      } else {
        current_p = new Lua_based_stochastic_voltage_gated_channel(100,  // Area
            20,  // density,
            20,  // conductance
            -85,  // reversal potential
            time_step, 24, channel_file_name);
        switch (alg) {
          case 2:
            current_p->set_simulation_mode(StochasticType::SINGLECHANNEL);
            break;
          default:
            current_p->set_simulation_mode(StochasticType::BINOMIALPOPULATION);
            break;
        }
      }

      // Shuffle channel states
      for (int t = 0; t < 10 / time_step; t++) {
        current_p->Step(0);
      }
      for (int t = 0; t < 10 / time_step; t++) {
        current_p->Step(30);
      }
      // Normalise channel at resting potential for 100 ms
      for (int t = 0; t < 100 / time_step; t++) {
        current_p->Step(V_resting);
      }

      // establish holding potential and hold for 50 ms
      for (int t = 0; t < 50 / time_step; t++)
        current_p->Step(V_hold);
      if (current_p->current() < -1e-2 || current_p->current() > 1e-2)
        cerr << "holding potential not established. " << current_p->current()
             << endl;

      // record 5 ms at holding potential
      for (int t = 0; t < 5 / time_step; t++) {
        current_p->Step(V_hold);
        if (t % static_cast<int>(samp_n) == 0) {
          data_file << current_p->current() << " " << current_p->OpenChannels()
                    << " " << V_hold << endl;
        }
      }

      // step to new voltage, record for step_length ms
      for (int t = 0; t < step_length / time_step; t++) {
        current_p->Step(aim);

        // print to file
        if (t % static_cast<int>(samp_n) == 0) {
          data_file << current_p->current() << " " << current_p->OpenChannels()
                    << " " << aim << endl;
        }
      }
      delete current_p;
      current_p = 0;
    }  // main loop
  }  // iTrials

  data_file.close();

  return (0);
}

int main(int argc, char** argv) {
  using modigliani_base::Real;
  using modigliani_base::Size;
  namespace po = boost::program_options;
  // Declare the supported options.
  po::options_description desc("Allowed options");
  desc.add_options()("channel", po::value<string>(), "which channel to use")(
      "algorithm,a", po::value<int>(), "set algorithm")("trials,t",
                                                        po::value<Size>(),
                                                        "set number of trials")(
      "step-length,s", po::value<Real>(), "set length of step (in ms)")(
      "output-folder,o", po::value<string>(), "set output folder");

  po::positional_options_description p;
  p.add("channel", -1);

  po::variables_map vm;
  po::store(
      po::command_line_parser(argc, argv).options(desc).positional(p).run(),
      vm);
  po::notify(vm);

  int alg = 4;
  if (vm.count("algorithm")) {
    alg = vm["algorithm"].as<int>();
    cout << "Algorithm was set to " << alg << ".\n";
  } else {
    cout << "Algorithm was not set. Using binomial population by default.\n";
  }

  // Variables needed during trials
  Size num_trials = 100;
  if (alg == 1) {
    num_trials = 1;
    cout << "Deterministic simulation. Only 1 trial.\n";
  } else if (vm.count("trials")) {
    num_trials = vm["trials"].as<Size>();
    cout << "Number of trials was set to " << num_trials << ".\n";
  } else {
    cout << "100 trials by default.\n";
  }

  Real step_length = 20;
  if (vm.count("step-length")) {
    step_length = vm["step-length"].as<Real>();
  }
  cout << "step length was set to " << step_length << ".\n";

  string output_folder = "/tmp/";
  if (vm.count("output-folder")) {
    output_folder = vm["output-folder"].as<string>();
  }
  cout << "Output folder set to " << output_folder << ".\n";

  cout << vm["channel"].as<string>() << endl;
  return (activation(vm["channel"].as<string>(), alg, step_length, num_trials,
                     output_folder));
}
