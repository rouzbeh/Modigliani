noinst_LIBRARIES = libauxsrc.a

libauxsrc_a_SOURCES = measure_mutual_information_in_binary_file.cpp generate_white_noise.cpp find_spikewords_in_binary_file.cpp test_ring_buffer.cpp test_binary_io.cpp show_binary_file.cpp print_interspike_intervals_in_binary_file.cpp make_nt_class_files.cpp make_histogram_from_txt_file.cpp find_spikes_in_binary_file.cpp compute_derivative_from_column.cpp axon_simulator.cpp resting_potential.c++

bin_PROGRAMS = show_binary_file make_nt_class_files compute_derivative_from_column find_spikes_in_binary_file make_histogram_from_txt_file \
				test_ring_buffer find_spikewords_in_binary_file generate_white_noise measure_mutual_information_in_binary_file \
				axon_simulator resting_potential
				
LDADD = -lm

show_binary_file_SOURCES = show_binary_file.cpp

make_nt_class_files_SOURCES = make_nt_class_files.cpp

compute_derivative_from_column_SOURCES = compute_derivative_from_column.cpp

find_spikes_in_binary_file_SOURCES = find_spikes_in_binary_file.cpp

find_spikewords_in_binary_file_SOURCES = find_spikewords_in_binary_file.cpp

make_histogram_from_txt_file_SOURCES = make_histogram_from_txt_file.cpp

test_ring_buffer_SOURCES = test_ring_buffer.cpp
test_ring_buffer_LDADD = $(top_builddir)/modigliani/mbase/libmbase.a

generate_white_noise_SOURCES = generate_white_noise.cpp
generate_white_noise_LDADD = $(top_builddir)/modigliani/mbase/libmbase.a

measure_mutual_information_in_binary_file_SOURCES = measure_mutual_information_in_binary_file.cpp
measure_mutual_information_in_binary_file_LDADD = $(top_builddir)/modigliani/mbase/libmbase.a

if WITH_PLPLOT
  MAYBE_PLPLOT = -lplplotcxxd -lplplotd
  PLPLOTCPPFLAGS = -DWITH_PLPLOT
endif

axon_simulator_SOURCES = axon_simulator.cpp

if MINGW
  axon_simulator_LDADD = $(top_builddir)/modigliani/mcore/libmcore.a $(top_builddir)/modigliani/mbase/libmbase.a -lboost_filesystem-mt -lboost_system-mt -ljsoncpp -lm -llua $(MAYBE_PLPLOT)
else
  axon_simulator_LDADD = $(top_builddir)/modigliani/mcore/libmcore.a $(top_builddir)/modigliani/mbase/libmbase.a -lboost_filesystem -lboost_system -ljson -lm -llua -ldl $(MAYBE_PLPLOT)
endif
axon_simulator_CPPFLAGS = $(PLPLOTCPPFLAGS)

resting_potential_SOURCES = resting_potential.c++
resting_potential_LDADD = $(top_builddir)/modigliani/mcore/libmcore.a $(top_builddir)/modigliani/mbase/libmbase.a -ljson -llua -ldl  -lboost_filesystem -lboost_system

# set the include path found by configure
INCLUDES= $(all_includes)  -I$(top_srcdir)/modigliani


