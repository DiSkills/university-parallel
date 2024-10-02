#ifdef DEBUG
#include <iostream>
#endif
#include <thread>

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


static void matrix_product(int *result, int **matrix, int *array, int size,
        int start, int step)
{
    for (; start < size; start += step) {
        result[start] = array_scalar(matrix[start], array, size);
    }
}


void parallel_matrix_product(int **matrix, int *array, int size)
{
    int *result = new int[size];

    int thrdcnt = std::thread::hardware_concurrency();
    if (!thrdcnt) {
        thrdcnt = 1;
    }
    std::thread *thrds = new std::thread[thrdcnt];

    for (int i = 0; i < thrdcnt; i++) {
        thrds[i] = std::thread(matrix_product, result, matrix, array, size,
                i, thrdcnt);
    }

    for (int i = 0; i < thrdcnt; i++) {
        thrds[i].join();
    }

#ifdef DEBUG
    array_print(result, size);
#endif
    delete[] thrds;
    delete[] result;
}
