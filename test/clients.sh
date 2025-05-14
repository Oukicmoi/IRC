#! /bin/bash

for (( c=1; c<=$2; c++ ))
do  
  nc localhost $1 &
done