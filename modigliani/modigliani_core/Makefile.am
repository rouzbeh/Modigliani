noinst_LIBRARIES = libmodigliani_core.a

libmodigliani_core_a_SOURCES = file_based_stochastic_multi_current.cc\
leak_current.cc\
ion_channels.cc\
lua_based_deterministic_multi_current.cc\
lua_based_stochastic_multi_current.cc\
membrane_compartment.cc\
membrane_current.cc\
membrane_patch.cc\
multi_current.cc\
aux_func.cc\
custom_cylindrical_compartment.cc\
cylindrical_compartment.cc\
membrane_compartment_sequence.cc\
membrane.cc\
object.cc\
transition_rate_matrix.cc\
spherical_compartment.cc\
synaptic_background.cc

EXTRA_DIST = $(libmodigliani_core_a_SOURCES) file_based_stochastic_multi_current.h\
leak_current.h\
ion_channels.h\
lua_based_deterministic_multi_current.h\
lua_based_stochastic_multi_current.h\
membrane_compartment.h\
membrane_current.h\
membrane_patch.h\
multi_current.h\
aux_func.h\
custom_cylindrical_compartment.h\
cylindrical_compartment.h\
membrane_compartment_sequence.h\
membrane.h\
object.h\
transition_rate_matrix.h\
spherical_compartment.h\
synaptic_background.h

# set the include path found by configure
AM_CPPFLAGS= $(all_includes) -I$(top_srcdir)/modigliani


noinst_HEADERS = aux_func.h

