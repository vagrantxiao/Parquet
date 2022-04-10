#/**************************************************************************
#***
#*** Copyright (c) 1995-2000 Regents of the University of Michigan,
#***               Saurabh Adya and Igor L. Markov
#***
#***  Contact author(s): sadya@eecs.umich.edu, imarkov@eecs.umich.edu
#***  Original Affiliation:   UoM, Electrical Engineering and Computer Science
#                             Department, Ann Arbor, MI 48109 USA
#***
#***  Permission is hereby granted, free of charge, to any person obtaining
#***  a copy of this software and associated documentation files (the
#***  "Software"), to deal in the Software without restriction, including
#***  without limitation
#***  the rights to use, copy, modify, merge, publish, distribute, sublicense,
#***  and/or sell copies of the Software, and to permit persons to whom the
#***  Software is furnished to do so, subject to the following conditions:
#***
#***  The above copyright notice and this permission notice shall be included
#***  in all copies or substantial portions of the Software.
#***
#*** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
#*** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
#*** OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
#*** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
#*** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
#*** OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
#*** THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#***
#***
#***************************************************************************/

.SUFFIXES:        .o .cxx .exe .exeP .exeS

# ##################
# standard variables
# ##################

OPTFLAGS        = -g -DABKDEBUG -DHGDEBUG -DPARTDEBUG -DCAPODEBUG
CCFLAGS          = -G -PIC +p    # comment +w2 when compiling with optimization 
DEPENDOPT        = -xM1
CC               = CC $(OPTFLAGS) $(CCFLAGS)
LD               = CC -G
LDFINAL          = CC
CC4LIBS          = CC6
# could be g++
AR               = ar cr
SODIRKEY         = -R
SONAMEKEY        = -R # change to -h if you need "immobile"dynamic libraries.
                      # LD_LIBRARY_PATH overrides -h
                      # Make sure there is no conflict
             
UCLA     = ./
IMPORT   = NOT_USED
LIBBASE  = ./
SLIBBASE         = $(LIBBASE)
STL              = 

ARNAME           = lib$(THISLIB).a
SONAME           = lib$(THISLIB).so
SODIR            = $(PWD)
SOFULL           = $(SODIR)/$(SONAME)
SOLABEL          = $(SONAMEKEY)$(SOFULL)
TESTNAME         = $(THISLIB)Test


# #########################################################
# standard variables that need modification in each project
# #########################################################

THISLIB = ParquetFP
INCLUDES= $(STL) -I$(UCLA)

LIBPATHS         = $(SODIRKEY)$(LIBBASE)
LIBS             =-L. $(SODIRKEY). -l$(THISLIB) $(LIBPATHS) -L$(LIBBASE)
SLIBS            =-L$(SLIBBASE) -L$(PWD) -l$(THISLIB)

OBJS        =    PlToSP.o CommandLine.o FPcommon.o Node.o Nodes.o Net.o Nets.o DB.o ClusterDB.o SeqPair.o SPeval.o AnalSolve.o Annealer.o SolveMulti.o

UTIL_OBJS   = UTIL/abkassert.o UTIL/abkio.o UTIL/infolines.o UTIL/paramproc.o UTIL/pathDelims.o UTIL/platfDepend.o UTIL/timer.o

SRCS             =  $(OBJS:%.o=%.cxx)

PURIFY  = purify -cache_dir=cache -always_use_cache_dir\
	  -windows=no -log_file=purify.log

# The four definitions below are for use with the PRINT_VERSION_INFO macro
# defined in ABKCommon/abkversion.h
ABKPACKAGE =`pwd | sed 's/.*\///g'`
ABKSYSTEM  ="`uname -n` running `uname -sr | sed 's/ /-/g'`"
ABKUSER    =`whoami`@`uname -n`
ABKSYMBOLS =-D__ABKPACKAGE__=\"$(ABKPACKAGE)\" -D__ABKSYSTEM__=\"$(ABKSYSTEM)\"\
        -D__ABKUSER__=\"$(ABKUSER)\" -D__ABKLIBBASE__=\"$(LIBBASE)\"
# add -(ABKSYMBOLS) to the CC call defined by the .cxx.o rule

# ################
# standard targets
# ################

all:        lib 

lib: $(SOFULL) $(ARNAME)

UTIL/libutil.a:
	cd $(PWD)/UTIL; make PWD=$(PWD)/UTIL

$(SOFULL):        $(OBJS) UTIL/libutil.a
	/bin/rm -f $(SOFULL)
	$(LD) $(INCLUDES) -o $(SOFULL) $(OBJS) $(UTIL_OBJS) $(SOLABEL)

$(ARNAME):        $(OBJS)  UTIL/libutil.a
	/bin/rm -f $(ARNAME)
	$(AR) $(ARNAME) $(OBJS) $(UTIL_OBJS)

.cxx.o:
	$(CC) $(INCLUDES) -c $*.cxx

.cxx.exe:
	/bin/rm -f $*.exe
	$(CC) $(INCLUDES) -c $*.cxx
	$(LDFINAL) -o $*.exe $*.o $(LIBS)

.cxx.exeS:
	/bin/rm -f $*.exeS
	$(CC) $(INCLUDES) -c $*.cxx
	$(LDFINAL) -o $*.exeS $*.o -static $(SLIBS)

.cxx.exeP:
	/bin/rm -f $*.exeP
	$(CC) $(INCLUDES) -c $*.cxx
	$(PURIFY) $(LDFINAL) -o $*.exeP $*.o $(LIBS)

depend:
	/bin/rm -f depend.mk

depend.mk:
	$(CC) $(DEPENDOPT) $(INCLUDES) $(SRCS) *.cxx >depend.mk

clean:        
	rm -rf *.o *.exe *.exe? $(SOFULL) $(ARNAME) core ir.out\
	$(TEST_OUT) $(TEST_DIFF) lib*pure* cache SunWS_cache *notime new.out *~
	cd $(PWD)/UTIL; make PWD=$(PWD)/UTIL clean

test:  Parquet.exe FPEvalTest1.exe FPEvalTest2.exe FPEvalTest3.exe FPEvalTest4.exe

testP: Parquet.exeP FPEvalTest1.exeP FPEvalTest2.exeP FPEvalTest3.exeP FPEvalTest4.exeP

testS: Parquet.exeS FPEvalTest1.exeS FPEvalTest2.exeS FPEvalTest3.exeS FPEvalTest4.exeS

.PHONY: depend, new, clean, test, testP, lib

# Uncomment for automatic handling of dependencies
# (use make depend to [re-]build dependencies)

# include depend.mk

