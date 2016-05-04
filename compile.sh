#!/bin/bash

if [ ! -d "bin" ]; then
  mkdir bin
fi

gcc -c -o bin/mcp2515.o *.c
ar rcs bin/libmcp2515.a bin/mcp2515.o
