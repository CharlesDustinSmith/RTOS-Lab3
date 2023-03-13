#!/bin/bash

for i in {0..100}
do 
    make JOBS=${i} WORK_PER_JOB=100 clean serialpt2 && ./serialpt2 >> serialpt2.csv
done 

for i in {0..100}
do 
    make JOBS=${i} WORK_PER_JOB=100 clean pthreadspt2 && ./pthreadspt2 >> pthreadspt2.csv
done 

for i in {0..100}
do 
    make JOBS=${i} WORK_PER_JOB=100`` clean libdispatchpt2 && ./libdispatchpt2 >> libdispatchThreadpt2.csv
done 
