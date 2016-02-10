bin_PROGRAMS = show_binary_file \
generate_white_noise \
modigliani \
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
show_binary_file_LDADD = $(BOOST_LINKS) -lstdc++
generate_white_noise_SOURCES = generate_white_noise.cc
generate_white_noise_LDADD = $(BOOST_LINKS) -lstdc++ $(top_builddir)/modigliani/modigliani_base/libmodigliani_base.a
modigliani_SOURCES = modigliani.cc
modigliani_LDADD = $(top_builddir)/modigliani/modigliani_core/libmodigliani_core.a $(top_builddir)/modigliani/modigliani_base/libmodigliani_base.a $(MAYBE_GSL) -lm -llua $(BOOST_LINKS) $(MAYBE_PLPLOT) -lstdc++
network_SOURCES = modigliani.cc
network_LDADD = $(top_builddir)/modigliani/modigliani_core/libmodigliani_core.a $(top_builddir)/modigliani/modigliani_base/libmodigliani_base.a $(MAYBE_GSL) -lm -llua $(BOOST_LINKS) $(MAYBE_PLPLOT) -lstdc++

AM_CXXFLAGS = $(all_includes) -I$(abs_top_srcdir)/modigliani -march=native -mtune=native -ansi -pedantic -Wshadow -Wno-vla -malign-double -std=c++11
