OBJS = OP.o
CC = g++
DEBUG = -g
CFLAGS = -std=c++11 -Wall -c $(DEBUG)
LFLAGS = -std=c++11 -Wall $(DEBUG)

etl_test : $(OBJS)
	    $(CC) $(LFLAGS) $(OBJS) etl_test.cpp -o etl_test

OP.o : OP.hpp
	    $(CC) $(CFLAGS) OP.cpp


clean:
	    \rm *.o etl_test

