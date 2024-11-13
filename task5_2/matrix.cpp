#ifdef DEBUG
#include <iostream>
#endif
#include <omp.h>

#include "array.hpp"
#include "matrix.hpp"


double **matrix_generate(int size)
{
    double **matrix = new double*[size];

    for (int i = 0; i < size; i++) {
        matrix[i] = array_generate(size);
    }
    return matrix;
}


void matrix_delete(double **matrix, int size)
{
    for (int i = 0; i < size; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}


#ifdef DEBUG
void matrix_print(double **matrix, int size)
{
    for (int i = 0; i < size; i++) {
        array_print(matrix[i], size);
    }
}
#endif


static void matrix_product(double *result, double **matrix, double *array, int size)
{
#pragma omp parallel for
    for (int i = 0; i < size; i++) {
        result[i] = array_scalar(matrix[i], array, size);
    }
}


static double **matrix_submatrix(double **matrix, int size, int row, int col)
{
    double **submatrix = new double*[size - 1];

    for (int i = 0, ni = 0; i < size; i++) {
        if (i == row) {
            continue;
        }

        submatrix[ni] = new double[size - 1];
        for (int j = 0, nj = 0; j < size; j++) {
            if (j == col) {
                continue;
            }

            submatrix[ni][nj] = matrix[i][j];
            nj++;
        }
        ni++;
    }

    return submatrix;
}


static double matrix_calculate_determinant(double **matrix, int size)
{
    if (size == 1) {
        return matrix[0][0];
    }
    if (size == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }

    double result = 0;
    for (int i = 0, sign = 1; i < size; i++, sign = -sign) {
        double **submatrix = matrix_submatrix(matrix, size, i, 0);

        result += sign * matrix[i][0] * 
            matrix_calculate_determinant(submatrix, size - 1);

        matrix_delete(submatrix, size - 1);
    }
    return result;
}


static double **matrix_reverse(double **matrix, int size)
{
    double **reverse = new double*[size];
    for (int i = 0; i < size; i++) {
        reverse[i] = new double[size];
    }

    double determinant = matrix_calculate_determinant(matrix, size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            double **submatrix = matrix_submatrix(matrix, size, i, j);

            reverse[j][i] = matrix_calculate_determinant(submatrix, size - 1)
                / determinant;
            if ((i + j) % 2 != 0) {
                reverse[j][i] = -reverse[j][i];
            }

            matrix_delete(submatrix, size - 1);
        }
    }

    return reverse;
}


double *calculate(double **matrix, double *array, int size)
{
    double *result = new double[size];
    double **reverse = matrix_reverse(matrix, size);

    matrix_product(result, reverse, array, size);

    matrix_delete(reverse, size);
    return result;
}
