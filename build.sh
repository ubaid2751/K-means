#!/bin/sh

gcc train.c -o train
gcc main.c -o main -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 