## How to run

### Running C++ Tests
* Clean up command: `make clean`
* To run all the required C++ tests: `make run`
* To run the standard matrix multiplication test: `make run_mult_standard`
* To run the line-by-line matrix multiplication test: `make run_mult_line`
* To run the block matrix multiplication test: `make run_mult_block`
* To run the line-by-line matrix multiplication test with parallelization (Version 1): `make run_mult_line_parallel_v1`
* To run the line-by-line matrix multiplication test with parallelization (Version 2): `make run_mult_line_parallel_v2`
* To run the block algorithm with parallelization: `make run_mult_block_parallel`

### Running Java Tests

* Give execution permission to the shell script : `chmod +x JavaRun.sh`
* Run the Java tests: `./JavaRun.sh`

## Results

You can find all results in csv format in `output/` folder.
* `aggregated_results_cpp.csv` C++ results
* `aggregated_results_java.csv` Java results


To run the script 
* for Java version: `python3 analyse.py java`
* for C++ version: `python3 analyse.py cpp`

```
Available PAPI preset and user defined events plus hardware information.
--------------------------------------------------------------------------------
PAPI version             : 7.1.0.0
Operating system         : Linux 6.5.0-15-generic
Vendor string and code   : GenuineIntel (1, 0x1)
Model string and code    : Intel(R) Core(TM) i7-9700 CPU @ 3.00GHz (158, 0x9e)
CPU revision             : 13.000000
CPUID                    : Family/Model/Stepping 6/158/13, 0x06/0x9e/0x0d
CPU Max MHz              : 4700
CPU Min MHz              : 800
Total cores              : 8
SMT threads per core     : 1
Cores per socket         : 8
Sockets                  : 1
Cores per NUMA region    : 8
NUMA regions             : 1
Running in a VM          : no
Number Hardware Counters : 10
Max Multiplex Counters   : 384
Fast counter read (rdpmc): yes
--------------------------------------------------------------------------------

================================================================================
  PAPI Preset Events
================================================================================
    Name        Code    Deriv Description (Note)
PAPI_L1_DCM  0x80000000  No   Level 1 data cache misses
PAPI_L1_ICM  0x80000001  No   Level 1 instruction cache misses
PAPI_L2_DCM  0x80000002  Yes  Level 2 data cache misses
PAPI_L2_ICM  0x80000003  No   Level 2 instruction cache misses
PAPI_L1_TCM  0x80000006  Yes  Level 1 cache misses
PAPI_L2_TCM  0x80000007  No   Level 2 cache misses
PAPI_L3_TCM  0x80000008  No   Level 3 cache misses
PAPI_CA_SNP  0x80000009  No   Requests for a snoop
PAPI_CA_SHR  0x8000000a  No   Requests for exclusive access to shared cache line
PAPI_CA_CLN  0x8000000b  No   Requests for exclusive access to clean cache line
PAPI_CA_ITV  0x8000000d  No   Requests for cache line intervention
PAPI_L3_LDM  0x8000000e  No   Level 3 load misses
PAPI_TLB_DM  0x80000014  Yes  Data translation lookaside buffer misses
PAPI_TLB_IM  0x80000015  No   Instruction translation lookaside buffer misses
PAPI_L1_LDM  0x80000017  No   Level 1 load misses
PAPI_L1_STM  0x80000018  No   Level 1 store misses
PAPI_L2_LDM  0x80000019  No   Level 2 load misses
PAPI_L2_STM  0x8000001a  No   Level 2 store misses
PAPI_PRF_DM  0x8000001c  No   Data prefetch cache misses
PAPI_MEM_WCY 0x80000024  No   Cycles Stalled Waiting for memory writes
PAPI_STL_ICY 0x80000025  No   Cycles with no instruction issue
PAPI_FUL_ICY 0x80000026  Yes  Cycles with maximum instruction issue
PAPI_STL_CCY 0x80000027  No   Cycles with no instructions completed
PAPI_FUL_CCY 0x80000028  No   Cycles with maximum instructions completed
PAPI_BR_UCN  0x8000002a  Yes  Unconditional branch instructions
PAPI_BR_CN   0x8000002b  No   Conditional branch instructions
PAPI_BR_TKN  0x8000002c  Yes  Conditional branch instructions taken
PAPI_BR_NTK  0x8000002d  No   Conditional branch instructions not taken
PAPI_BR_MSP  0x8000002e  No   Conditional branch instructions mispredicted
PAPI_BR_PRC  0x8000002f  Yes  Conditional branch instructions correctly predicted
PAPI_TOT_INS 0x80000032  No   Instructions completed
PAPI_LD_INS  0x80000035  No   Load instructions
PAPI_SR_INS  0x80000036  No   Store instructions
PAPI_BR_INS  0x80000037  No   Branch instructions
PAPI_RES_STL 0x80000039  No   Cycles stalled on any resource
PAPI_TOT_CYC 0x8000003b  No   Total cycles
PAPI_LST_INS 0x8000003c  Yes  Load/store instructions completed
PAPI_L2_DCA  0x80000041  No   Level 2 data cache accesses
PAPI_L3_DCA  0x80000042  Yes  Level 3 data cache accesses
PAPI_L2_DCR  0x80000044  No   Level 2 data cache reads
PAPI_L3_DCR  0x80000045  No   Level 3 data cache reads
PAPI_L2_DCW  0x80000047  Yes  Level 2 data cache writes
PAPI_L3_DCW  0x80000048  No   Level 3 data cache writes
PAPI_L2_ICH  0x8000004a  No   Level 2 instruction cache hits
PAPI_L2_ICA  0x8000004d  No   Level 2 instruction cache accesses
PAPI_L3_ICA  0x8000004e  No   Level 3 instruction cache accesses
PAPI_L2_ICR  0x80000050  No   Level 2 instruction cache reads
PAPI_L3_ICR  0x80000051  No   Level 3 instruction cache reads
PAPI_L2_TCA  0x80000059  Yes  Level 2 total cache accesses
PAPI_L3_TCA  0x8000005a  No   Level 3 total cache accesses
PAPI_L2_TCR  0x8000005c  Yes  Level 2 total cache reads
PAPI_L3_TCR  0x8000005d  Yes  Level 3 total cache reads
PAPI_L2_TCW  0x8000005f  Yes  Level 2 total cache writes
PAPI_L3_TCW  0x80000060  No   Level 3 total cache writes
PAPI_SP_OPS  0x80000067  Yes  Floating point operations; optimized to count scaled single precision vector operations
PAPI_DP_OPS  0x80000068  Yes  Floating point operations; optimized to count scaled double precision vector operations
PAPI_VEC_SP  0x80000069  Yes  Single precision vector/SIMD instructions
PAPI_VEC_DP  0x8000006a  Yes  Double precision vector/SIMD instructions
PAPI_REF_CYC 0x8000006b  No   Reference clock cycles
--------------------------------------------------------------------------------
Of 59 available events, 18 are derived.
```
