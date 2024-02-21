#!/bin/bash

# Compile the C program
gcc -O3 hit_time.c -o hit_time

# Check if compilation was successful
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

# Ensure results.txt is overwritten on the first run
for i in {1..1000}; do
   if [ $i -eq 1 ]; then
      ./hit_time > results.txt  # Overwrite file on the first iteration
   else
      ./hit_time >> results.txt # Append to file on subsequent iterations
   fi
done

echo "Execution finished."
