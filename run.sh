#! /bin/bash

make clean all

echo "\\n* Put\\n"
echo "| Put(测试名\数据量) | 500 | 5000 | 50000 | 500000 | 5000000 |"
echo "| ---------------- | --- | ---- | ----- | ------ | ------- |"
./bin/main 4
./bin/main 2
./bin/main 3
./bin/main 5
./bin/main 1

echo "\\n* Get\\n"
echo "| Get(测试名\数据量) | 500 | 5000 | 50000 | 500000 | 5000000 |"
echo "| ---------------- | --- | ---- | ----- | ------ | ------- |"
./bin/main 4_1
./bin/main 2_1
./bin/main 3_1
./bin/main 5_1
./bin/main 1_1










