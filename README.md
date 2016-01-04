Simple Distributed Munging in C++

src: C++ source

dags: airflow dag python scripts

to compile:
```
make clean
make 
```
Parser.[c|h]pp: parser to parse rules

SmartSplit.[c|h]pp: split columns based on dependencies and return the number of partitions

IOWorker.[c|h]pp: IO read data paritions and compute on cell basis.

MemWorker.[c|h]pp: mmap shared and load the partitions into eigen Matrix to compute

OP.hpp: operator class

DepRule.hpp: dependency rules

MungeBits.hpp: sample munging methods



