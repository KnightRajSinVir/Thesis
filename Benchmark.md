Benchmark Data

StacK Size Limitation 1024 bytes

1 main kernel -> 3000 Bytes

Splitting into 3 kernels 

No unrolling 
Running the kernel once
Timestamp of when the last u is written
6588800 ps 
6588800 / 1000 = 6588.8 ns
6588800 / 1000000 = 6.5888 us
800 ns / multiplication
No pipelining 
Total Simulation Time
14088984 ps


Get more data 
Run it 1000/10000/100 times --  Baseline
pipelined nicely if balanced


Unrolling 

Kernel 1
Cannot be fully unrolled because when we do stack usage is 1408 Bytes
When I just unroll the smallest mac which is the 24 usage is 1184
Cannot be unrolled in this condition

Register Spilling not enough registers s goes into stack 


Splitting k1 into 2 lower 13 


//      //

//      //      // 

// /// //

am009 register file 