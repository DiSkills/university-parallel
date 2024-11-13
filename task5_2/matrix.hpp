#ifndef MATRIX_HPP
#define MATRIX_HPP

double **matrix_generate(int size);
void matrix_delete(double **matrix, int size);
void matrix_print(double **matrix, int size);
double *calculate(double **matrix, double *array, int size);

#endif
