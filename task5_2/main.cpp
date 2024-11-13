#include <iostream>
#include <omp.h>

#include "array.hpp"
#include "matrix.hpp"


int main()
{
    double n, **matrix, *array;

    srand(time(NULL));

    std::cin >> n;

    matrix = matrix_generate(n);
    array = array_generate(n);

#ifdef DEBUG
    std::cout << std::endl;
    matrix_print(matrix, n);

    std::cout << std::endl;
    array_print(array, n);
    std::cout << std::endl;
#endif

    double *result = calculate(matrix, array, n);

#ifdef DEBUG
    std::cout << std::endl;
    array_print(result, n);
    std::cout << std::endl;
#endif

    matrix_delete(matrix, n);
    delete[] array;
    delete[] result;
    return 0; 
}
