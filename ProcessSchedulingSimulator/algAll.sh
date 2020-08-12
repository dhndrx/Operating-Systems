#!/bin/bash

for i in {1..30}
do
	./main 1 $i 0.06 0.01
done

for i in {1..30}
do
	./main 2 $i 0.06 0.01
done

for i in {1..30}
do
	./main 3 $i 0.06 0.01
done

for i in {1..30}
do
	./main 3 $i 0.06 0.2
done
