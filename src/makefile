DEMO_OBJS = OP.o 
OBJS = IOWorker.o  MemWorker.o Parser.o OP.o main.o Worker.o SmartSplit.o
CC = g++
DEBUG = -g
CFLAGS = -std=c++11 -Wall -c -I $(EIGEN) -lboost_regex 
LFLAGS = $(DEBUG) -lboost_regex -lboost_iostreams -I $(EIGEN)

.PHONY: clean lint etl_test

#etl_test : $(DEMO_OBJS)
#	    $(CC) $(LFLAGS) $(DEMO_OBJS) etl_test.cpp -o etl_test
#
#NOTE: boost linking switch has to be the last
etl_test : $(OBJS) 
			$(CC) $(OBJS) -o etl_test $(LFLAGS)

main.o : Parser.hpp  IOWorker.hpp MemWorker.hpp SmartSplit.hpp
	    $(CC) $(CFLAGS) main.cpp
OP.o : OP.hpp 
	    $(CC) $(CFLAGS) OP.cpp
Parser.o : Parser.hpp DepRule.hpp OP.hpp
	    $(CC) $(LFLAGS) $(CFLAGS) Parser.cpp
SmartSplit.o : SmartSplit.hpp
	    $(CC) $(CFLAGS) SmartSplit.cpp
IOWorker.o : IOWorker.hpp Worker.hpp DepRule.hpp MungeBits.hpp
	    $(CC) $(CFLAGS) IOWorker.cpp 
MemWorker.o : MemWorker.hpp Worker.hpp DepRule.hpp MungeBits.hpp
	    $(CC) $(CFLAGS) MemWorker.cpp
Worker.o : Worker.hpp DepRule.hpp MungeBits.hpp
	    $(CC) $(CFLAGS) Worker.cpp

clean:
	    \rm -f *.o etl_test main

lint:
	  python ../scripts/lint.py dodo cpp ./ 

