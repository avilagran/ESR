AUTOMAKE_OPTIONS = subdir-objects

# name of the program
bin_PROGRAMS = bin/sink_graph

# sources.
# ADD here extra source files
bin_sink_graph_SOURCES = ../aloe/skeleton.c ../aloe/params.c ../src/gnuplot_i.c ../src/sink_graph_functions.c  ../src/sink_graph.c 

#pre-processor flags, e.g. includes
bin_sink_graph_CPPFLAGS = -D_COMPILE_ALOE -I../interfaces -I../src/ -I../aloe -I../../typetools/include/ -I../../../sw_api/include
#linker flags: library paths (do not modify)
bin_sink_graph_LDFLAGS = -L../../typetools/lnx_make/lib/ -L../../../hw_api/lnx/lib -L../../../sw_api/lnx_make/lib
#libraries to include (do not modify)
bin_sink_graph_LDADD =  -lskeleton $(hwapi) $(swapi) -lm -lfftw3 -ldl

# include these extra files in the distribution
# IMPORTANT: Add ../src/*.h files if you include headers in the src/ directory
EXTRA_DIST = ../include/*.h ../doc/ ../src/*.h
