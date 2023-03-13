#!/bin/bash

for i in {0..999}
do 
    ./serial | awk '{ print $7 }' | tee -a initial-serial-times.dat
    #./pthread | awk '{ print $7 }' | tee -a initial-serial-times-two.dat
done 
