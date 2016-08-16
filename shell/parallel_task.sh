#!/bin/bash

function task() {
    pid=$! # $! is Last backgroud pid $$ is current pid
    echo "Task:$1 "
    curl 'https://ultimate010.com' -H 'Cookie:_no_cache'   1> /dev/null 2> /dev/null
}

usage() {
    echo "Usage:"
    echo -e "\t$0 batch_count concurrency_count"
}

if [[ $# -lt 2 ]]; then
    usage
    exit
fi

TOTAL_TASK_COUNT=$(($1 * $2))
CONCURRENCY_THREAD_NUM=$2
tmp_fifofile="/tmp/$$.fifo" 

echo "Create $tmp_fifofile"
mkfifo "$tmp_fifofile" 
exec 6<>"$tmp_fifofile" 

for((i=0; i < $CONCURRENCY_THREAD_NUM; i++)); do 
    echo 
done >&6 

for i in `seq 1 $TOTAL_TASK_COUNT`; do 
    read -u6 
    { 
        task $i 
        echo >&6 
    } & 
done 

wait 
exec 6>&-
rm -f $tmp_fifofile
echo "Done"
echo "Delete $tmp_fifofile"
exit 0
