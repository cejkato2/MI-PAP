#!/bin/bash

#mkdir -p tests
for thread in 2 4 8 16 32; do
   # make tasks
   for task in bin/*; do
      for prog in $task/*; do 
        taskdir=tests/${thread}/$(basename $task)/$(basename $prog)
        mkdir -p $taskdir
        cp $prog $taskdir/prog
        if [ "$(basename $task)" == "sekvencni" ]; then
          cp utils/queue_4c_serial_job.sh $taskdir/q.sh
        else
          cp utils/queue_12_1slots_per_host_job.sh $taskdir/q.sh
        fi
      done
   done
done

#/opt/bin/qrun.sh [4c|12c|mix] #CPU FRONTA job_soubor 
#long    | 1 | 32 | 40 min
#serial  | 1 |  1 | 60 min

