bin_PROGRAMS = show_binary_file make_nt_class_files \
compute_derivative_from_column \
find_spikes_in_binary_file \
make_histogram_from_txt_file \
test_ring_buffer \
find_spikewords_in_binary_file \
generate_white_noise \
measure_mutual_information_in_binary_file \
axon_simulator \
resting_potential \
single_compartment_simulator \
voltage_step
				
LDADD = -lm

show_binary_file_SOURCES = show_binary_file.cc

make_nt_class_files_SOURCES = make_nt_class_files.cc

compute_derivative_from_column_SOURCES = compute_derivative_from_column.cc

find_spikes_in_binary_file_SOURCES = find_spikes_in_binary_file.cc

find_spikewords_in_binary_file_SOURCES = find_spikewords_in_binary_file.cc

make_histogram_from_txt_file_SOURCES = make_histogram_from_txt_file.cc

test_ring_buffer_SOURCES = test_ring_buffer.cc
test_ring_buffer_LDADD = $(top_builddir)/modigliani/modigliani_base/libmodigliani_base.a

generate_white_noise_SOURCES = generate_white_noise.cc
generate_white_noise_LDADD = $(top_builddir)/modigliani/modigliani_base/libmodigliani_base.a

measure_mutual_information_in_binary_file_SOURCES = measure_mutual_information_in_binary_file.cc
measure_mutual_information_in_binary_file_LDADD = $(top_builddir)/modigliani/modigliani_base/libmodigliani_base.a

if WITH_PLPLOT
  MAYBE_PLPLOT = -lplplotcxxd -lplplotd
  PLPLOTCPPFLAGS = -DWITH_PLPLOT
endif

axon_simulator_SOURCES = axon_simulator.cc
single_compartment_simulator_SOURCES = single_compartment_simulator.cc
resting_potential_SOURCES = resting_potential.cc

if MINGW
  BOOST_LINKS = -lboost_filesystem-mt -lboost_system-mt -lboost_program_options-mt
  JSON_LINKS = -ljsoncpp
else
  BOOST_LINKS = -lboost_filesystem -lboost_system -lboost_program_options
  JSON_LINKS = -ljson
endif

axon_simulator_LDADD = $(top_builddir)/modigliani/modigliani_core/libmodigliani_core.a $(top_builddir)/modigliani/modigliani_base/libmodigliani_base.a -lm -llua $(BOOST_LINKS) $(MAYBE_PLPLOT) $(JSON_LINKS)
single_compartment_simulator_LDADD = $(top_builddir)/modigliani/modigliani_core/libmodigliani_core.a $(top_builddir)/modigliani/modigliani_base/libmodigliani_base.a $(BOOST_LINKS) $(JSON_LINKS) -lm -llua
axon_simulator_CPPFLAGS = $(PLPLOTCPPFLAGS)
resting_potential_CPPFLAGS = $(PLPLOTCPPFLAGS)

resting_potential_LDADD = $(single_compartment_simulator_LDADD)

voltage_step_SOURCES = voltage_step.cc
voltage_step_LDADD = $(single_compartment_simulator_LDADD) 


# set the include path found by configure
INCLUDES= $(all_includes)  -I$(top_srcdir)/modigliani


