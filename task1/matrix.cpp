#ifdef DEBUG
#include <iostream>
#endif

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
    for (int i = 0; i < size; i++) {
        int result = 0;
        for (int j = 0; j < size; j++) {
            result += matrix[i][j] * array[j];
        }
#ifdef DEBUG
        std::cout << result << std::endl;
#endif
    }
}
