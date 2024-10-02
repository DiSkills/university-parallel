#include <iostream>

#include "array.hpp"
#include "matrix.hpp"


int main(int argc, char **argv)
{
    int n;
    std::cin >> n;

    srand(time(NULL));

    int **matrix = matrix_generate(n),
        *array = array_generate(n);

#ifdef DEBUG
    std::cout << std::endl;
    matrix_print(matrix, n);

    std::cout << std::endl;
    array_print(array, n);
#endif

    parallel_matrix_product(matrix, array, n);

    matrix_delete(matrix, n);
    delete[] array;
    return 0;
}
