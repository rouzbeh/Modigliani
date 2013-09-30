noinst_LIBRARIES = libmodigliani_base.a

libmodigliani_base_a_SOURCES = two_dim_histogram.cc \
multidim_histogram.cc \
value.cc \
sequential_statistics.cc \
obj.cc \
histogram.cc \
error.cc \
aux_math_func.cc

EXTRA_DIST= $(libmodigliani_base_a_SOURCES) dmatrix.h\
aux_math_func.h\
codes.h\
error.h\
histogram.h\
main.h\
obj.h\
sequential_statistics.h\
stock_value.h\
timing.h\
types.h\
value.h\
multidim_array.h\
multidim_histogram.h\
ring_buffer.h\
two_dim_histogram.h\
physical_constants.h

AM_CXXFLAGS = $(all_includes) -I$(abs_top_srcdir)/modigliani -ansi -pedantic -Wshadow -Wno-vla -malign-double -std=c++11
