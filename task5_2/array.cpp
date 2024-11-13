#include <cstdlib>

#ifdef DEBUG
#include <iostream>
#endif

#include "array.hpp"


double *array_generate(int size)
{
    double *array = new double[size];

    for (int i = 0; i < size; i++) {
        array[i] = rand() % 100 - 50;
    }

    return array;
}


double array_scalar(double *a, double *b, int size)
{
    double r = 0;
    for (; size; size--) {
        r += a[size - 1] * b[size - 1];
    }
    return r;
}


#ifdef DEBUG
void array_print(double *array, int size)
{
    for (int i = 0; i < size; i++) {
        std::cout << array[i] << ' ';
    }
    std::cout << std::endl;
}
#endif
