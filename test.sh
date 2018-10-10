#!/bin/bash

for file in $(ls test); do
  test=${file%.*}
  objs=`ls *.o | grep -v main.o`
  cc -o test/${test}.exe ${objs} test/${test}.o
  ./test/${test}.exe
done

echo OK
