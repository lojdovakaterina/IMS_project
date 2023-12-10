#!/bin/bash

if  [ "$1" = "-b" ]; then
    if [ ! -d "base_out" ]; then
        mkdir base_out
    fi

    for k in {1..10}; do
        for i in {1..4}; do
            for n in {1..6}; do
                
                output_filename="base_$i$n.out" # output file
                
                ./main -i $i -n $n -b >> base_out/$output_filename
            done
        done
        sleep 1
    done
elif [ "$1" = "-d" ]; then
    if [ ! -d "line_out" ]; then
        mkdir line_out
    fi

    for k in {1..10}; do
        for r in {1..3}; do
            for l in {1..3}; do
                
                output_filename="line_$l$r.out" # output file
                n=$((r+l)) 
                ./main -i 2 -n $n -d -l $l -r $r >> line_out/$output_filename
                ### ###### this $i
            done
        done
        sleep 1
    done
else
    echo "[-b] pro základní model"
    echo "[-d] pro model rozdělení fronty"

fi


