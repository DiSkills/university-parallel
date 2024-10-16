#include <iostream>

#include "array.hpp"


int main()
{
    int n;
    std::cin >> n;

    srand(time(NULL));

    int *a = array_generate(n),
        *b = array_generate(n);

    parallel_scalar(a, b, n);

#ifdef DEBUG
    std::cout << std::endl;
    array_print(a, n);

    std::cout << std::endl;
    array_print(b, n);
#endif

    delete[] a;
    delete[] b;
    return 0;
}
