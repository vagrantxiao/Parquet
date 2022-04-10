
# Created by Igor Markov

#CC = g++ -g -Wall -Wno-deprecated
CC     = g++ -O3 -Wall -Wno-deprecated

.SUFFIXES:      .o .cxx .exe

OBJS = abkassert.o abkio.o pathDelims.o infolines.o platfDepend.o paramproc.o timer.o

all : $(OBJS)
#ar cr libutil.a  $(OBJS)

clean :
	rm -rf *.o *.a

.cxx.o: 
	$(CC) $(INCLUDES) -c $*.cxx 

