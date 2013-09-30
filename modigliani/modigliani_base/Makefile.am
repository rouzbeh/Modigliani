noinst_LIBRARIES = libmodigliani_base.a

libmodigliani_base_a_SOURCES = two_dim_histogram.cc \
multidim_histogram.cc \
binomial_rnd_dist.cc \
value.cc \
uniform_rnd_dist.cc \
sequential_statistics.cc \
rnd_dist.cc \
obj.cc \
id_management.cc \
histogram.cc \
gui_drv.cc \
gaussian_rnd_dist.cc \
error.cc \
aux_math_func.cc

EXTRA_DIST= $(libmodigliani_base_a_SOURCES) dmatrix.h\
aux_math_func.h\
codes.h\
error.h\
gaussian_rnd_dist.h\
gui_drv.cc gui_drv.h\
header_string.h\
histogram.h\
id_management.h\
main.h\
moonmap_image_data.h\
obj.h\
rnd_dist.h\
sequential_statistics.h\
stock_value.h\
timing.h\
types.h\
uniform_rnd_dist.h\
value.h\
binomial_rnd_dist.h\
multidim_array.h\
multidim_histogram.h\
ring_buffer.h\
two_dim_histogram.h\
mtrand.h\
physical_constants.h

AM_CXXFLAGS = $(all_includes) -I$(abs_top_srcdir)/modigliani -ansi -pedantic -Wshadow -Wno-vla -malign-double -std=c++11
