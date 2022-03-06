# COP290 Assignment1 

## Part1

- main.cpp contains code for Part1
- To compile the code use command

```
make 
```

- Commands to execute in terminal 

```
./yourcode.out fullyconnected inputmatrix.txt weightmatrix.txt biasmatrix.txt outputmatrix.txt
```
```
./yourcode.out activation relu inputmatrix.txt outputmatrix.txt
```
```
./yourcode.out activation tanh inputmatrix.txt outputmatrix.txt
```
### stride in command below is integer value
```
./yourcode.out pooling max inputmatrix.txt stride outputmatrix.txt
```
### stride in command below is integer value
```
./yourcode.out pooling average inputmatrix.txt stride outputmatrix.txt
```
```
./yourcode.out probability softmax inputvector.txt outputvector.txt
```
```
./yourcode.out probability sigmoid inputvector.txt outputvector.txt
```
- To remove the loader files which are main.o and yourcode.out, use command
```
make clean
```

## Part3

- To compile the code use following command

```
export MKL_LIB=<<path to mkl libraries>>
```

```
export MKL_INC=<<path to mkl include>>
```

```
export LD_LIBRARY_PATH=<<current working directory>>
```

```
make all
```
- Commands to execute in terminal , arguement 1 has to be defined by user
```
./yourcode.out mfcc_features/ca4d5368_nohash_5.txt out.txt
```
