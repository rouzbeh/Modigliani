bin_PROGRAMS = 
nettrader_SOURCES = 
nettrader_LDADD   = ./ntsrc/libntsrc.a ./nt3dsrc/libnt3dsrc.a ./ntgsrc/libntgsrc.a ./ntbpsrc/libntbpsrc.a ./auxsrc/libauxsrc.a -L/usr/X11R6/lib -lGL -lGLU -lm -lXext -lX11

SUBDIRS = ntsrc ntgsrc ntbpsrc tnt auxsrc 
if MINGW
	SUBDIRS += nt3dsrc
endif
# set the include path found by configure
INCLUDES= $(all_includes)
 
# the library search path.
LDFLAGS = $(all_libraries) 
                                                                                
