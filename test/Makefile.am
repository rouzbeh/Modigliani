AM_CPPFLAGS = -I../src -std=c++11

bin_PROGRAMS = tests

tests_SOURCES = main.cc
tests_LDADD = $(top_builddir)/src/modigliani/libmodigliani.la

AM_CXXFLAGS = $(all_includes) -I$(abs_top_srcdir)/src -ansi -pedantic -Wshadow -Wno-vla -std=c++11
