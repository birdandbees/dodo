DEMO_OBJS = OP.o 
OBJS = IOWorker.o 
CC = g++
DEBUG = -g
CFLAGS = -std=c++11 -Wall -c -I $(EIGEN)
LFLAGS = $(DEBUG) -lboost_regex -I $(EIGEN)

#etl_test : $(DEMO_OBJS)
#	    $(CC) $(LFLAGS) $(DEMO_OBJS) etl_test.cpp -o etl_test
#
main : $(OBJS) 
			$(CC) $(CFLAGS) $(LFLAGS) $(OBJS) main.cpp -o main

OP.o : OP.hpp 
	    $(CC) $(CFLAGS) OP.cpp
Parser.o : Parser.hpp DepRule.hpp OP.hpp
	    $(CC) $(LFLAGS) $(CFLAGS) Parser.cpp
SmartSplit.o : SmartSplit.hpp
	    $(CC) $(CFLAGS) SmartSplit.cpp
IOWorker.o : IOWorker.hpp Worker.hpp MemWorker.hpp DepRule.hpp MungeBits.hpp
	    $(CC) $(CFLAGS) IOWorker.cpp MemWorker.cpp
MemWorker.o : MemWorker.hpp Worker.hpp DepRule.hpp MungeBits.hpp
	    $(CC) $(CFLAGS) MemWorker.cpp

clean:
	    \rm -f *.o etl_test main

