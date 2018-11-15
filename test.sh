#!/bin/bash

for file in $(ls test); do
  test=${file%.*}
  objs=`ls *.o | grep -v main.o`
  cc -o test/${test}.exe ${objs} test/${test}.o &> /dev/null
  ./test/${test}.exe
  out=$?
  if [ "${out}" != 0 ]; then
    echo "${test}: NG"
  fi
done
