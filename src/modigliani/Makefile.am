modiglianidir = $(includedir)/modigliani

lib_LTLIBRARIES = libmodigliani.la

libmodigliani_la_SOURCES = \
leak_current.cc\
ion_channels.cc\
lua_based_deterministic_voltage_gated_channel.cc\
file_based_stochastic_voltage_gated_channel.cc\
lua_based_stochastic_voltage_gated_channel.cc\
membrane_compartment.cc\
membrane_current.cc\
membrane_patch.cc\
voltage_gated_ion_channel_current.cc\
aux_func.cc\
custom_cylindrical_compartment.cc\
cylindrical_compartment.cc\
membrane_compartment_sequence.cc\
membrane.cc\
object.cc\
transition_rate_matrix.cc\
spherical_compartment.cc\
synaptic_background.cc\
network_synapse.cc\
sequential_statistics.cc \
obj.cc \
aux_math_func.cc


libmodigliani_la_LIBADD = @LUA_LIBS@\
$(BOOST_FILESYSTEM_LIB)\
$(BOOST_SYSTEM_LIB)

modigliani_HEADERS = \
aux_func.h\
leak_current.h\
ion_channels.h\
lua_based_deterministic_voltage_gated_channel.h\
file_based_stochastic_voltage_gated_channel.h\
lua_based_stochastic_voltage_gated_channel.h\
membrane_compartment.h\
membrane_current.h\
membrane_patch.h\
voltage_gated_ion_channel_current.h\
aux_func.h\
custom_cylindrical_compartment.h\
cylindrical_compartment.h\
membrane_compartment_sequence.h\
membrane.h\
object.h\
transition_rate_matrix.h\
spherical_compartment.h\
synaptic_background.h\
network_synapse.h\
dmatrix.h\
aux_math_func.h\
obj.h\
sequential_statistics.h\
timing.h\
types.h\
multidim_array.h\
physical_constants.h

AM_CXXFLAGS = $(all_includes) -I$(abs_top_srcdir)/src -pedantic -std=c++11 @LUA_CFLAGS@ $(BOOST_CPPFLAGS) -lstdc++
