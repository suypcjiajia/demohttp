#!/bin/sh

while [ true ]
do

#sleep 1
CPU=`top -d 1 -n 2|grep %Cpu|awk 'NR==2{print $2}'`

conn=`netstat -antp|wc -l`
thread=`ps -XH|wc -l`
totalMem=`free|grep Mem:|awk 'NR==1{print $2}'`
UseMem=`free|grep Mem:|awk 'NR==1{print $3}'`
MemPer=` echo "scale=2; ($UseMem / $totalMem)*100 " | bc `

echo "conn:"$conn "thread:"$thread "CPU:"$CPU"%" "MEM:"$MemPer"%"

done
