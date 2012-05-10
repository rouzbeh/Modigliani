bin_PROGRAMS = 
nettrader_SOURCES = 
nettrader_LDADD   = ./mbase/libmbase.a ./nt3dsrc/libnt3dsrc.a ./ntgsrc/libntgsrc.a ./ntbpsrc/libmcore.a ./auxsrc/libauxsrc.a -L/usr/X11R6/lib -lGL -lGLU -lm -lXext -lX11

SUBDIRS = mbase ntgsrc mcore auxsrc 
if MINGW
	SUBDIRS += nt3dsrc
endif
# set the include path found by configure
INCLUDES= $(all_includes)
 
# the library search path.
LDFLAGS = $(all_libraries) 
                                                                                
