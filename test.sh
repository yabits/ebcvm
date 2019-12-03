#!/bin/bash
# SPDX-License-Identifier: MIT

passed=1
for file in $(ls test); do
  test=${file%.*}
  objs=`ls *.o | grep -v main.o`
  cc -o test/${test}.exe ${objs} test/${test}.o &> /dev/null
  ./test/${test}.exe
  out=$?
  if [ "${out}" != 0 ]; then
    passed=0
    echo "${test}: NG"
  fi
done

if [ "${passed}" == 1 ]; then
  echo "OK"
  exit 0
else
  echo "NG"
  exit 1
fi
