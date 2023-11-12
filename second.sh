#!/bin/bash

arr=$(ps -o pid -C sleep | grep -v PID | head -n3)

for pid in ${arr[*]};
do
kill $pid
done