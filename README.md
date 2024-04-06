Overview
This project implements the classic Producer-Consumer problem in the context of an operating system environment. The Producer-Consumer problem involves two processes, a producer and a consumer, who share a common buffer or table. The producer generates items and puts them into the buffer, while the consumer consumes these items from the buffer.

Files
producer.c: Contains the code for the producer process.
consumer.c: Contains the code for the consumer process.

Requirements
C/C++ compiler
pthread and rt libraries

Compilation
Compile the programs using the following commands:
gcc producer.c -pthread -lrt -o producer
gcc consumer.c -pthread -lrt -o consumer

Execution
Run the producer and consumer programs simultaneously using:
./producer & ./consumer &

Example Output
Upon successful execution, you should see output indicating the items being produced and consumed by the producer and consumer, respectively.

Produced: 42
Consumed: 42
Produced: 67
Produced: 15
Consumed: 67
Produced: 89
Consumed: 15
Consumed: 89
Produced: 34
...
