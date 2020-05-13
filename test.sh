#!/bin/bash

make

./VectorizeSSE 1024 -s
./VectorizeSSE 2048 -s
./VectorizeSSE 4096 -s
./VectorizeSSE 8192 -s
./VectorizeSSE 16384 -s
./VectorizeSSE 32768 -s
./VectorizeSSE 65536 -s
./VectorizeSSE 131072 -s
./VectorizeSSE 262144 -s
./VectorizeSSE 524287 -s
./VectorizeSSE 1048576 -s