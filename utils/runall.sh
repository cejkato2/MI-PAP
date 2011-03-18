#!/bin/bash

for i in sekvencni/bmerge paralelni/shearsort paralelni/eomergesort ./eomerge paralelni/bitonicsort; do 
  echo -ne "$i\t\t"
  $i
done

