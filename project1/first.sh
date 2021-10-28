#!/bin/bash


for ((i=0; i < 100; i++))
do
echo "This is the $i execution."
./Project1 -i test_small -q query_small -o out_small
echo -e '\n'
done
