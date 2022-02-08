#! /bin/bash

make clean all

echo "\\n* Put 单位: 纳秒/操作 \\n"
echo "| Put(ns\op) | 500 | 5000 | 50000 | 500000 | 5000000 |"
echo "| ---------- | --- | ---- | ----- | ------ | ------- |"
./bin/main 4 
./bin/main 2
./bin/main 3
./bin/main 5
./bin/main 1

echo "\\n* Get 单位: 纳秒/操作\\n"
echo "| Get(ns\op) | 500 | 5000 | 50000 | 500000 | 5000000 |"
echo "| ---------- | --- | ---- | ----- | ------ | ------- |"
./bin/main 4_1 
./bin/main 2_1
./bin/main 3_1
./bin/main 5_1
./bin/main 1_1










