#!/bin/bash

n=${1-10}

for (( i = 0; i < n; i++ ))
do
sleep 60 &
pids[${i}]=$!
done

for pid in ${pids[*]};
do
wait $pid
codes[${pid}]=$?
done

correctly_completed=0
SIGTERM=143

for code in ${codes[*]};
do
if [ $code -ne $SIGTERM ]; then
    (( correctly_completed++ ))
fi
done

echo $correctly_completed > result.txt

sleep 3600