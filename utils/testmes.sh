#!/bin/bash

#mkdir -p tests
for data in 10 16 20 24 26; do
for thread in 1 2 4 8 16 32; do
   # make tasks
   for task in bin/*; do
      for prog in $task/*; do 
        taskdir=tests/${data}/${thread}/$(basename $task)/$(basename $prog)
        mkdir -p $taskdir
        cp $prog $taskdir/prog
        if [ "$(basename $task)" == "sekvencni" ]; then
	  if [ $thread -eq 1 ]; then
          cp utils/queue_4c_serial_job.sh $taskdir/q.sh
	  echo "./prog -n${data} -t1" >> $taskdir/q.sh
	  fi
        else
	  if [ $thread -ne 1 ]; then
          cp utils/queue_12_1slots_per_host_job.sh $taskdir/q.sh
	  echo "./prog -n${data} -t${thread}" >> $taskdir/q.sh
	  fi
        fi
      done
   done
done
done

#/opt/bin/qrun.sh [4c|12c|mix] #CPU FRONTA job_soubor 
#long    | 1 | 32 | 40 min
#serial  | 1 |  1 | 60 min

