#!/bin/bash

script_name=${1-"first.sh"}
pid=$(ps -o pid -C first.sh | grep -v PID)

kill $pid