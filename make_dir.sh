#!/bin/bash

top_dirs=("problems" "solutions")
category=("1-day" "Cryptography" "Forensic" "Hardware" "Misc" "Pwnable" "Reversing" "Webhacking" )

Difficulty=("Easy" "Beginner" "Medium" "Hard")

for top in "${top_dirs[@]}"; do
  mkdir -p "$top"
  for sub in "${category[@]}"; do
    mkdir -p "$top/$sub"
    for difficulty in "${Difficulty[@]}"; do
      mkdir -p "$top/$sub/$difficulty"
    done
  done
done
