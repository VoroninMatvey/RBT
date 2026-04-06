# Implementation of the range query algorithm in a binary search tree
## Information about project:
In this project range query was implemented using the method of ordinal statistics. This allows us to find the count of elements in a range in logarithmic rather than linear time.
## Building the project:
```
git clone git@github.com:VoroninMatvey/RBT.git
cd RBT
cmake -S . -B build
cmake --build build
```
Run the executable file:
```
./build/range_query
```
The program expects the following sequence of requests as input (in any order)
```
k <key1> k <key2> q <left1> <right1> ... k <key_n> q <left_m> <right_m>
```
## Test generation
```
bash scripts/generate_tests.sh <tests_number> <keys_number> <min_value> <max_value>
```
There min_value and max_value - minimum and maximum key value.
In directory data/tests are generated tests and answers.
## Running tests
```
bash scripts/run_generated_tests.sh

<img width="2379" height="1144" alt="1" src="https://github.com/user-attachments/assets/1c3fd54b-0ca0-46df-9218-44be0022d141" />

