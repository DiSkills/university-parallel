#ifndef MATRIX_HPP
#define MATRIX_HPP

int **matrix_generate(int size);
void matrix_delete(int **matrix, int size);
void matrix_print(int **matrix, int size);
void matrix_product(int **matrix, int *array, int size);

#endif
