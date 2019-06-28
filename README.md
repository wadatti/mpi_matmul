# Matrix multiplication using MPI
C++ implementation of Matrix Multiplication in MPI. 

## Description
This program calculate square matrix multiplication using Intel MPI.(C=A×B)

Matrix size is defined in source code.
Default matrix size is 4096×4096.

Matrix A is divided each process.
Matrix B is broadcasted to all process.

## Compiling
```
$ mpiicpc -O3 mat_matrix.cpp
```

## Executing
```
$ mpirun -np 4 ./a.out
```
