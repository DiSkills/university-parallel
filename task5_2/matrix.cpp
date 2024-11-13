#ifdef DEBUG
#include <iostream>
#endif
#include <omp.h>

#include "array.hpp"
#include "matrix.hpp"


int **matrix_generate(int size)
{
    int **matrix = new int*[size];

    for (int i = 0; i < size; i++) {
        matrix[i] = array_generate(size);
    }
    return matrix;
}


void matrix_delete(int **matrix, int size)
{
    for (int i = 0; i < size; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}


#ifdef DEBUG
void matrix_print(int **matrix, int size)
{
    for (int i = 0; i < size; i++) {
        array_print(matrix[i], size);
    }
}
#endif


void matrix_product(int **matrix, int *array, int size)
{
    int *result = new int[size];

#pragma omp parallel for
    for (int i = 0; i < size; i++) {
        result[i] = array_scalar(matrix[i], array, size);
    }

#ifdef DEBUG
    array_print(result, size);
#endif
    delete[] result;
}
