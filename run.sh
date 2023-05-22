#! /usr/bin/bash

echo compiling...
g++ cellular_automata.cpp -pthread

echo -e "compiled\nprogram starting..."
./a.out

