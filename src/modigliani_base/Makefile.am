noinst_LIBRARIES = libmodigliani_base.a

libmodigliani_base_a_SOURCES = sequential_statistics.cc \
obj.cc \
aux_math_func.cc

EXTRA_DIST= $(libmodigliani_base_a_SOURCES) dmatrix.h\
aux_math_func.h\
obj.h\
sequential_statistics.h\
timing.h\
types.h\
multidim_array.h\
ring_buffer.h\
physical_constants.h

AM_CXXFLAGS = $(all_includes) -I$(abs_top_srcdir)/src -ansi -pedantic -Wshadow -Wno-vla -std=c++11
