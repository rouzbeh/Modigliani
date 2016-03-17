bin_PROGRAMS = show_binary_file \
generate_white_noise \
modigliani

if WITH_PLPLOT
  MAYBE_PLPLOT = -lplplotcxxd -lplplotd
  PLPLOTCPPFLAGS = -DWITH_PLPLOT
endif

LDADD = $(MAYBE_GSL) -lm
show_binary_file_SOURCES = show_binary_file.cc
show_binary_file_LDADD = $(BOOST_LINKS) -lstdc++
generate_white_noise_SOURCES = generate_white_noise.cc
generate_white_noise_LDADD = $(BOOST_LINKS) -lstdc++ $(top_builddir)/src/modigliani_base/libmodigliani_base.a
modigliani_SOURCES = modigliani.cc
modigliani_LDADD = ../modigliani_core/libmodigliani_core.la $(top_builddir)/src/modigliani_base/libmodigliani_base.a -lm @LUA_LIBS@ $(BOOST_FILESYSTEM_LIB) $(BOOST_PROGRAM_OPTIONS_LIB) $(BOOST_SYSTEM_LIB) $(MAYBE_PLPLOT) -lstdc++

AM_CXXFLAGS = $(all_includes) -I$(abs_top_srcdir)/src -ansi -pedantic -Wshadow -Wno-vla -std=c++11 @LUA_CFLAGS@ $(BOOST_CPPFLAGS)
