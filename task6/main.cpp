#include <omp.h>

#include "field.hpp"


int main()
{
    int size = 9;
    int **field = field_input(size);

#pragma omp parallel sections shared(field)
    {
#pragma omp section
        field_solve(field, size, 0, 3);
#pragma omp section
        field_solve(field, size, 3, 6);
#pragma omp section
        field_solve(field, size, 6, 9);
    }

    field_print(field, size);

    field_del(field, size);
    return 0;
}
