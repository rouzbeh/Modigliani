bin_PROGRAMS = show_binary_file \
generate_white_noise \
axon_simulator \
resting_potential \
single_compartment_simulator \
voltage_step \
single_compartment_resting_potential\
network

if WITH_PLPLOT
  MAYBE_PLPLOT = -lplplotcxxd -lplplotd
  PLPLOTCPPFLAGS = -DWITH_PLPLOT
endif
if WITH_GSL
  MAYBE_GSL = -lgsl -lgslcblas
  GSLCPPFLAGS = -DWITH_GSL
endif

if MINGW
  BOOST_LINKS = -lboost_filesystem-mt -lboost_system-mt -lboost_program_options-mt
else
  BOOST_LINKS = -lboost_filesystem -lboost_system -lboost_program_options
endif

LDADD = $(MAYBE_GSL) -lm

show_binary_file_SOURCES = show_binary_file.cc

generate_white_noise_SOURCES = generate_white_noise.cc
generate_white_noise_LDADD = $(top_builddir)/modigliani/modigliani_base/libmodigliani_base.a

axon_simulator_SOURCES = axon_simulator.cc
single_compartment_simulator_SOURCES = single_compartment_simulator.cc
resting_potential_SOURCES = resting_potential.cc
single_compartment_resting_potential_SOURCES = single_compartment_resting_potential.cc

axon_simulator_LDADD = $(top_builddir)/modigliani/modigliani_core/libmodigliani_core.a $(top_builddir)/modigliani/modigliani_base/libmodigliani_base.a $(MAYBE_GSL) -lm -llua $(BOOST_LINKS) $(MAYBE_PLPLOT)
single_compartment_simulator_LDADD = $(top_builddir)/modigliani/modigliani_core/libmodigliani_core.a $(MAYBE_GSL) -lm $(top_builddir)/modigliani/modigliani_base/libmodigliani_base.a $(BOOST_LINKS) -llua
axon_simulator_CPPFLAGS = $(PLPLOTCPPFLAGS)
resting_potential_CPPFLAGS = $(PLPLOTCPPFLAGS)
resting_potential_LDADD = $(single_compartment_simulator_LDADD)
single_compartment_resting_potential_CPPFLAGS = $(single_compartment_simulator_CPPFLAGS)
single_compartment_resting_potential_LDADD = $(single_compartment_simulator_LDADD)
voltage_step_SOURCES = voltage_step.cc
voltage_step_LDADD = $(single_compartment_simulator_LDADD)
network_SOURCES = network.cc
network_LDADD = $(single_compartment_simulator_LDADD)

AM_CXXFLAGS = $(all_includes) -I$(abs_top_srcdir)/modigliani -march=native -mtune=native -ansi -pedantic -Wshadow -Wno-vla -malign-double -std=c++11
