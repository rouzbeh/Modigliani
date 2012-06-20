///
/// @file voltage_step.cc
/// Performs voltage_step experiments on a given Lua channel
/// @version  1.0
/// @author Copyright (C) 2010, 2011 Mohammad Ali Neishabouri
///
/// @section LICENSE
/// This library is free software; you can redistribute it and/or
/// modify it under the terms of the GNU Library General Public
/// License as published by the Free Software Foundation; either
/// version 3 of the License, or (at your option) any later version.
///
/// This library is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
/// Library General Public License for more details.
///
/// You should have received a copy of the GNU Library General Public
/// License along with this library; if not, write to the Free
/// Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
///

#include <modigliani_core/aux_func.h>

///
/// Activation voltage step
/// @param channel_file_name Lua code for the channel
/// @param alg Simulation algorithm
int activation(string channel_file_name, int alg) {
  using modigliani_base::Real;
  using modigliani_core::Lua_based_stochastic_multi_current;
  using modigliani_core::Lua_based_deterministic_multi_current;
  using modigliani_core::Multi_current;
  using std::endl;
  using std::cout;

  string timed_output_folder = modigliani_core::createOutputFolder(
      string("/lab/voltage_step/"));
  ofstream time_file, log_file, data_file;
  modigliani_core::openOutputFile(timed_output_folder, "Time", time_file);
  modigliani_core::openOutputFile(timed_output_folder, "log", log_file, ".log");
  modigliani_core::openOutputFile(timed_output_folder, "data", data_file,
                                  ".bin");

  /* Variables needed during trials */
  Real num_trials = 100;
  if (alg == 1) {
    num_trials = 1;
  }
  Real time_step = 0.001;
  int samp_n = 10;

  /* *** Trials loop *** */
  for (int iTrials = 0; iTrials < num_trials; iTrials++) {
    log_file << "\nTrial " << iTrials + 1 << " of " << num_trials << ":"
             << endl;

    /*******************************************************************/
    /* *************************  Main ******************************* */
    cout << "Main experiment started" << endl;

    Real V_hold = -100;  // holding potential relative to resting potential
    Real V_resting = -60;

    /*the loop */
    for (int aim = V_hold + 5; aim <= V_resting + 100; aim += 5) {
      Multi_current *current_p = 0;
      if (alg == 1) {
        current_p = new Lua_based_deterministic_multi_current(10,  // Area
            20,  // density,
            20,  // conductance
            -85,  // reversal potential
            0.001, 24, channel_file_name);

        current_p->SetSimulationMode(
            modigliani_core::StochasticType::DETERMINISTIC);
      } else {
        current_p = new Lua_based_stochastic_multi_current(10,  // Area
            20,  // density,
            20,  // conductance
            -85,  // reversal potential
            0.001, 24, channel_file_name);

        switch (alg) {
          case 2:
            current_p->SetSimulationMode(
                modigliani_core::StochasticType::SINGLECHANNEL);
            break;
          default:
            current_p->SetSimulationMode(
                modigliani_core::StochasticType::BINOMIALPOPULATION);
            break;
        }
      }

      /* Normalise channel at resting potential for 100 ms */
      for (int t = 0; t < 100 / time_step; t++)
        current_p->Step(V_resting);

      /* establish holding potential and hold for 50 ms */
      for (int t = 0; t < 50 / time_step; t++)
        current_p->Step(V_hold);
      if (current_p->current() < -1e-2 || current_p->current() > 1e-2)
        cerr << "holding potential not established. " << current_p->current()
             << endl;

      cout << "Stepping to " << aim << "." << endl;

      /* record 5 ms at holding potential */
      for (int t = 0; t < 5 / time_step; t++) {
        current_p->Step(V_hold);
        if (t % (int) samp_n == 0) data_file << current_p->current() << endl;
      }

      /* step to new voltage, record for 20 ms */
      for (int t = 0; t < 20 / time_step; t++) {
        current_p->Step(aim);

        /* print to file */
        if (t % (int) samp_n == 0) {
          data_file << current_p->current() << endl;
        }
      }
    }  //main loop
  }  //iTrials

  data_file.close();
  cout << "\n\nRun complete." << endl;

  return (0);
}

int main(int argc, char** argv) {
  if(argc!=3){
    cout << "voltage_step <channel_file> <algorithm>" << endl;
  }
  return (activation(string(argv[1]), atoi(argv[2])));
}
