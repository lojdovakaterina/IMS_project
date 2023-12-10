#!/bin/bash

if [ ! -d "out" ]; then
    mkdir out
fi

# Projdi všechny kombinace -i a -n
for i in {1..10}; do
    for i in {1..4}; do
        for n in {1..6}; do
            # Vytvoř název výstupního souboru
            output_filename="base_$i$n.out"

            # Spusť příkaz s odpovídajícími přepínači
            ./main -i $i -n $n -b >> out/$output_filename
        done
    done
    sleep 1
done
