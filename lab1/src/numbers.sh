#!/bin/bash

num_count=150
touch numbers.txt

for i in $(seq 1 $num_count)
do
    random_num=$(od -An -N2 -i /dev/random)
    echo $random_num >> numbers.txt
done