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


void array_sort(int *array, int left, int right)
{
    int middle = array[(left + right) / 2];

    int l = left,
        r = right;

    while (l <= r) {
        while (array[l] < middle) {
            l++;
        }

        while (array[r] > middle) {
            r--;
        }

        if (l <= r) {
            int tmp = array[l];
            array[l] = array[r];
            array[r] = tmp;
            l++;
            r--;
        }
    }

    if (left < r) {
        array_sort(array, left, r);
    }
    if (l < right) {
        array_sort(array, l, right);
    }
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
