#include <cstdlib>

#ifdef DEBUG
#include <iostream>
#endif

#include "array.hpp"


int *array_generate(int size)
{
    int *array = new int[size];

    for (int i = 0; i < size; i++) {
        array[i] = rand() % 100 - 50;
    }

    return array;
}


int array_scalar(int *a, int *b, int size)
{
    int r = 0;
    for (; size; size--) {
        r += a[size - 1] * b[size - 1];
    }
    return r;
}


#ifdef DEBUG
void array_print(int *array, int size)
{
    for (int i = 0; i < size; i++) {
        std::cout << array[i] << ' ';
    }
    std::cout << std::endl;
}
#endif
