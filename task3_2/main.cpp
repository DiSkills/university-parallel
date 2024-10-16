#include <iostream>

#include "array.hpp"


int main()
{
    int n;
    std::cin >> n;

    srand(time(NULL));

    int *array = array_generate(n);
#ifdef DEBUG
    std::cout << std::endl;
    array_print(array, n);
#endif

    parallel_sort(array, n);
#ifdef DEBUG
    std::cout << std::endl;
    array_print(array, n);
#endif

    delete[] array;
    return 0;
}
