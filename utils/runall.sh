#!/bin/bash

if [ $# -eq 0 ]; then
  echo "Usage: $0 <lg(array_size)> <num_threads>"
  exit 0
fi

for i in bin/*/*; do 
  if [ -x "$i" ]; then
    echo -ne "$i\t\t"
    $i -n $1 -t $2
  fi
done

