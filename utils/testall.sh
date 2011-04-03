#!/bin/bash

find . -name q.sh |
while read process; do
(
echo $(dirname $process)
cd $(dirname $process)
qsub q.sh
)
done

