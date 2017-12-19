#!/bin/bash
i=0
for run in {1..1000}
do
  echo "$i"
  let "i += 1"
  ./a.out
done
