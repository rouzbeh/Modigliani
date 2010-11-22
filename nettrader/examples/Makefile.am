####### kdevelop will overwrite this part!!! (begin)##########
noinst_LIBRARIES = libexamples.a

libexamples_a_SOURCES = analyse_binary_data_for_isi_with_histogram.cpp basic_glx_demo.cpp channel_density_vs_noise_variance.cpp channel_density_vs_noise_variance_second_version.cpp channel_density_vs_noise_variance_third_version.cpp channel_spike_trigger_2.cpp channel_spike_trigger.cpp diffusion_signalling.cpp diffusion_vs_wave_signalling.cpp dynamic_flight.cpp flying_demo.cpp flying_through.cpp gaussian_isi_spikes_on_deterministic_axon.cpp gaussian_isi_spikes_on_langevin_axon.cpp hh_sodium_channel_model.cpp hranvier_sodium_channel_model.cpp ion_channels_and_open_state_variance.cpp MembraneFluctuationsStHH.cpp merge_text_file_columns.cpp modigliani_core.cpp multi_compartment_neuron.cpp nt3d_drv_test.cpp nt3d_glx_test.cpp ntg_grandom_graph_efficiency_study.cpp ntg_ring_graph_test.cpp ntg_test.cpp patlak_sodium_channel_model.cpp performance_current_classes_comparison.cpp planetary_demo.cpp resting_potential_noise.cpp saltatory_conduction.cpp single_compartment_spiker.cpp somasimulator-v1.cpp somasimulator-v1-SGA.cpp spheric_flying_demo.cpp stochastic_ion_channels.cpp template.cpp test_code.cpp test_graph_distance_stats.cpp test_membrane_currents.cpp tunnel_flight_demo.cpp WhiteNoise1.0e-8StHH.cpp WhiteNoiseMultiCOmpartmentSImulationWithCompartmentVoltageHistogramming.cpp myelinaxonsimulator-v1.cpp

#EXTRA_DIST = AxonConduction.cpp MembraneFluctuationsStHH.cpp WhiteNoise1.0e-8StHH.cpp WhiteNoiseMultiCOmpartmentSImulationWithCompartmentVoltageHistogramming.cpp analyse_binary_data_for_isi_with_histogram.cpp basic_glx_demo.cpp channel_density_vs_noise_variance.cpp channel_density_vs_noise_variance_second_version.cpp channel_density_vs_noise_variance_third_version.cpp dynamic_flight.cpp flying_demo.cpp flying_through.cpp gaussian_isi_spikes_on_deterministic_axon.cpp gaussian_isi_spikes_on_langevin_axon.cpp ion_channels_and_open_state_variance.cpp multi_compartment_neuron.cpp nt3d_demo_glut_drv_obj.h nt3d_drv_test.cpp nt3d_glut_drv_test.h nt3d_glx_test.cpp ntg_grandom_graph_efficiency_study.cpp ntg_ring_graph_test.cpp ntg_test.cpp planetary_demo.cpp resting_potential_noise.cpp saltatory_conduction.cpp single_compartment_spiker.cpp spheric_flying_demo.cpp stochastic_ion_channels.cpp template.cpp test_graph_distance_stats.cpp test_membrane_currents.cpp tunnel_flight_demo.cpp merge_text_file_columns.cpp axon_simulation.cpp diffusion_vs_wave_signalling.cpp diffusion_signalling.cpp compute_joint_pdf_from_binary_data.cpp growing_graphs.cpp test_code.cpp deterministic_axon_simulator_experiment_aac.cpp stochastic_axon_simulator_experiment_aaf.cpp stochastic_axon_simulator_experiment_aae.cpp stochastic_axon_simulator_experiment_aaa.cpp stochastic_axon_simulator_experiment_aab.cpp stochastic_axon_simulator_experiment_aad.cpp stochastic_axon_simulator_experiment_aah.cpp stochastic_axon_simulator_experiment_aag.cpp stochastic_axon_simulator_experiment_aah_fine.cpp stochastic_axon_simulator_experiment_aal.cpp stochastic_axon_simulator_experiment_aak.cpp stochastic_axon_simulator_experiment_aai.cpp stochastic_axon_simulator_experiment_aam.cpp stochastic_axon_simulator_experiment_aan.cpp growing_graph.h Makefile.am axonsimulator-v1.cpp stochastic_axon_simulator_experiment_aanr5.cpp channel_spike_trigger.cpp channel_spike_trigger_2.cpp patlak_sodium_channel_model.cpp hh_sodium_channel_model.cpp hranvier_sodium_channel_model.cpp axonsimulator-v1-hranvier.cpp performance_current_classes_comparison.cpp axonsimulator-v1-colbert-axonal.cpp colbert_axonal_sodium_channel_model.cpp colbert_axonal_potassium_channel_model.cpp somasimulator-v1-SGA.cpp somasimulator-v1.cpp modigliani_core.cpp modigliani_model_format_grammar_t.h 

install-data-local:
	$(mkinstalldirs) ./
	$(INSTALL_DATA) $(srcdir)/basic_glx_demo.cpp ./basic_glx_demo

uninstall-local:
	-rm -f ./basic_glx_demo

####### kdevelop will overwrite this part!!! (end)############

bin_PROGRAMS = analyse_binary_data_for_isi_with_histogram basic_glx_demo channel_density_vs_noise_variance channel_spike_trigger diffusion_signalling diffusion_vs_wave_signalling dynamic_flight flying_demo flying_through gaussian_isi_spikes_on_deterministic_axon gaussian_isi_spikes_on_langevin_axon hh_sodium_channel_model hranvier_sodium_channel_model ion_channels_and_open_state_variance MembraneFluctuationsStHH merge_text_file_columns multi_compartment_neuron nt3d_drv_test nt3d_glx_test ntg_grandom_graph_efficiency_study ntg_ring_graph_test ntg_test patlak_sodium_channel_model performance_current_classes_comparison planetary_demo resting_potential_noise saltatory_conduction single_compartment_spiker somasimulator-v1 somasimulator-v1-SGA spheric_flying_demo stochastic_ion_channels test_code test_graph_distance_stats test_membrane_currents tunnel_flight_demo WhiteNoise1.0e-8StHH WhiteNoiseMultiCOmpartmentSImulationWithCompartmentVoltageHistogramming myelinaxonsimulator-v1
			   	
basic_glx_demo_SOURCES = basic_glx_demo.cpp
basic_glx_demo_LDADD   = ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

planetary_demo_SOURCES = planetary_demo.cpp
planetary_demo_LDADD   = ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a  -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

tunnel_flight_demo_SOURCES = tunnel_flight_demo.cpp
tunnel_flight_demo_LDADD   = ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a  -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

analyse_binary_data_for_isi_with_histogram_SOURCES = analyse_binary_data_for_isi_with_histogram.cpp
analyse_binary_data_for_isi_with_histogram_LDADD   = ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a  -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

ntg_grandom_graph_efficiency_study_SOURCES = ntg_grandom_graph_efficiency_study.cpp
ntg_grandom_graph_efficiency_study_LDADD   =  ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a  ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

test_membrane_currents_SOURCES = test_membrane_currents.cpp
test_membrane_currents_LDADD = ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

dynamic_flight_SOURCES = dynamic_flight.cpp
dynamic_flight_LDADD = ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

gaussian_isi_spikes_on_deterministic_axon_SOURCES = gaussian_isi_spikes_on_deterministic_axon.cpp
gaussian_isi_spikes_on_deterministic_axon_LDADD = ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

gaussian_isi_spikes_on_langevin_axon_SOURCES = gaussian_isi_spikes_on_langevin_axon.cpp
gaussian_isi_spikes_on_langevin_axon_LDADD = ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

MembraneFluctuationsStHH_SOURCES =  MembraneFluctuationsStHH.cpp
MembraneFluctuationsStHH_LDADD = ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

ion_channels_and_open_state_variance_SOURCES = ion_channels_and_open_state_variance.cpp
ion_channels_and_open_state_variance_LDADD = ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

multi_compartment_neuron_SOURCES = multi_compartment_neuron.cpp
multi_compartment_neuron_LDADD = ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

channel_density_vs_noise_variance_SOURCES = channel_density_vs_noise_variance.cpp
channel_density_vs_noise_variance_LDADD = ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

nt3d_drv_test_SOURCES = nt3d_drv_test.cpp
nt3d_drv_test_LDADD = ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

nt3d_glx_test_SOURCES = nt3d_glx_test.cpp
nt3d_glx_test_LDADD = ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

test_graph_distance_stats_SOURCES = test_graph_distance_stats.cpp
test_graph_distance_stats_LDADD = ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

resting_potential_noise_SOURCES = resting_potential_noise.cpp
resting_potential_noise_LDADD = ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

saltatory_conduction_SOURCES = saltatory_conduction.cpp
saltatory_conduction_LDADD = ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

WhiteNoiseMultiCOmpartmentSImulationWithCompartmentVoltageHistogramming_SOURCES = WhiteNoiseMultiCOmpartmentSImulationWithCompartmentVoltageHistogramming.cpp
WhiteNoiseMultiCOmpartmentSImulationWithCompartmentVoltageHistogramming_LDADD = ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

myelinaxonsimulator_v1_SOURCES = myelinaxonsimulator-v1.cpp
myelinaxonsimulator_v1_LDADD = ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

single_compartment_spiker_SOURCES = single_compartment_spiker.cpp
single_compartment_spiker_LDADD = ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

spheric_flying_demo_SOURCES = spheric_flying_demo.cpp
spheric_flying_demo_LDADD = ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

stochastic_ion_channels_SOURCES = stochastic_ion_channels.cpp
stochastic_ion_channels_LDADD = ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

WhiteNoise1_0e_8StHH_SOURCES = WhiteNoise1.0e-8StHH.cpp
WhiteNoise1_0e_8StHH_LDADD = ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

flying_through_SOURCES = flying_through.cpp
flying_through_LDADD = ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

flying_demo_SOURCES = flying_demo.cpp
flying_demo_LDADD = ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

ntg_test_SOURCES = ntg_test.cpp
ntg_test_LDADD =  ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

ntg_ring_graph_test_SOURCES = ntg_ring_graph_test.cpp
ntg_ring_graph_test_LDADD =  ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

merge_text_file_columns_SOURCES = merge_text_file_columns.cpp
merge_text_file_columns_LDADD =  ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

diffusion_vs_wave_signalling_SOURCES = diffusion_vs_wave_signalling.cpp
diffusion_vs_wave_signalling_LDADD = ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

diffusion_signalling_SOURCES = diffusion_signalling.cpp
diffusion_signalling_LDADD = ../nt3dsrc/libnt3dsrc.a ../ntgsrc/libntgsrc.a ../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a -L/usr/X11R6/lib -lGL -L/usr/X11R6/lib -lGLU -lm -lXext -lX11

test_code_SOURCES = test_code.cpp
test_code_LDADD = ../ntsrc/libntsrc.a

channel_spike_trigger_SOURCES = channel_spike_trigger.cpp
channel_spike_trigger_LDADD =../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a  -lm

patlak_sodium_channel_model_SOURCES = patlak_sodium_channel_model.cpp
patlak_sodium_channel_model_LDADD =../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a  -lm

hh_sodium_channel_model_SOURCES = hh_sodium_channel_model.cpp
hh_sodium_channel_model_LDADD =../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a  -lm

hranvier_sodium_channel_model_SOURCES = hranvier_sodium_channel_model.cpp
hranvier_sodium_channel_model_LDADD =../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a  -lm

performance_current_classes_comparison_SOURCES = performance_current_classes_comparison.cpp
performance_current_classes_comparison_LDADD =../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a  -lm

somasimulator_v1_SGA_SOURCES =  somasimulator-v1-SGA.cpp
somasimulator_v1_SGA_LDADD =../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a  -lm

somasimulator_v1_SOURCES =  somasimulator-v1.cpp
somasimulator_v1_LDADD =../ntbpsrc/libntbpsrc.a ../ntsrc/libntsrc.a  -lm

# set the include path found by configure
INCLUDES= $(all_includes)

# the library search path.
AM_LDFLAGS = $(all_libraries)




