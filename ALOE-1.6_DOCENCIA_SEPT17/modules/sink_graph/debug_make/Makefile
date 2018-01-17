
# Change cpp and linker flags here
CPPFLAGS=-D_GNU_SOURCE -D__USE_GNU -O0 -g3 -Wall -Wno-format-extra-args -std=c99 -Winline -I../src -I../aloe -I../src -I../interfaces -fPIC
LDFLAGS=-lrt -lm

#Add here more .c files required by the module DSP function
MODSRC=../src/sink_graph.c 

#Matlab root path
MATLAB_ROOT=/path/to/matlab
MATLAB_INC=-D_COMPILE_MEX_INCLUDE

#Comment the next line if you have matlab to avoid mexPrintf warnings
MATLAB_INC=



# DO NOT NEED TO MODIFY BELOW HERE

# Name of the module 
NAME=sink_graph

MATLAB_INCLUDE=$(MATLAB_ROOT)/extern/include
CC=gcc
LIBNAME=lib$(NAME).a
COMSRC=../aloe/params.c
SASRC=../standalone/test_main.c ../src/test_generate.c ../standalone/gnuplot_i.c 
SAOBJ=$(SASRC:.c=.o)
COMOBJ=$(COMSRC:.c=.o)
MODOBJ_M=$(MODSRC:.c=.om)
MODOBJ=$(MODSRC:.c=.o)

.PHONY: all clean distclean

all: $(SOURCES) $(NAME) matlab

matlab: $(MODOBJ_M) $(COMOBJ)
	ar rcs $(LIBNAME) $(MODOBJ_M) $(COMOBJ)

$(MODOBJ_M):
	$(CC) $(CPPFLAGS) -I$(MATLAB_INCLUDE) $(MATLAB_INC) -D_COMPILE_MEX -c $(MODSRC) -o $(MODOBJ_M)

$(NAME): $(MODOBJ) $(SAOBJ) $(COMOBJ)
	$(LINK.cc) $(MODOBJ) $(COMOBJ) $(SAOBJ) $(LDFLAGS) -o $@

clean:
	@- $(RM) $(LIBNAME) $(NAME)
	@- $(RM) $(MODOBJ) $(COMOBJ) $(MODOBJ_M) $(SAOBJ)
	
	
	
