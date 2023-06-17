#! /usr/bin/bash

echo compiling...
javac DiamondSquare.java
g++ -o diamond_square diamond_square_driver.cpp

echo -e "compiled\nprogram starting..."
./diamond_square

