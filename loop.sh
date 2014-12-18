#!/bin/sh


for p in {1..4}
do
   for t in {1..4}
   do
      val="(0"
      cat main.c | sed "37s/.*/         #pragma omp parallel for num_threads($t)/" > src/main.c
      make clean && make
      for repeat in {1..30}
      do
         val="$val + `mpirun -np $p ./run`"
      done
      val="$val ) / 30"
#      echo $val
      val=`echo $val | bc -l`
      echo "P:$p T:$t == $val"
   done

done
