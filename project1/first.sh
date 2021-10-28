#!/bin/bash


for ((i=0; i < 100; i++))
do
echo "This is the $i execution."
sleep 0.5
./Project1 -i input_small_id -q query_small -o out_small
echo -e '\n'
sleep 1
done
