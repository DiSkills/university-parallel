#include <iostream>
#include <vector>

#include "field.hpp"


static const char line[] = "--------+-------+--------";


bool field_is_solved(int **field, int size)
{
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (field[i][j] == 0) {
                return false;
            }
        }
    }
    return true;
}


int **field_input(int size)
{
    int **field = new int*[size];
    for (int i = 0; i < size; i++) {
        field[i] = new int[size];

        for (int j = 0; j < size; j++) {
            std::cin >> field[i][j];
        }
    }
    return field;
}


void field_print(int **field, int size)
{
    std::cout << line << std::endl;

    for (int i = 0; i < size; i++) {
        std::cout << "| ";

        for (int j = 0; j < size; j++) {
            std::cout << field[i][j] << " ";

            if (j % 3 == 2) {
                std::cout << "| ";
            }
        }
        std::cout << std::endl;

        if (i % 3 == 2) {
            std::cout << line << std::endl;
        }
    }
}


void field_del(int **field, int size)
{
    for (int i = 0; i < size; i++) {
        delete[] field[i];
    }
    delete[] field;
}


static void fill(std::vector<bool> &f, int **field, int size, int i, int j)
{
    for (int k = 0; k < size; k++) {
        if (field[i][k] > 0) {
            f[field[i][k] - 1] = true;
        }

        if (field[k][j] > 0) {
            f[field[k][j] - 1] = true;
        }
    }

    for (int k = i / 3 * 3; k < i / 3 * 3 + 3; k++) {
        for (int l = j / 3 * 3; l < j / 3 * 3 + 3; l++) {
            if (field[k][l] > 0) {
                f[field[k][l] - 1] = true;
            }
        }
    }
}


void field_solve(int **field, int size, int start, int stop)
{
    while (!field_is_solved(field, size)) {
        for (int i = start; i < stop; i++) {
            for (int j = 0; j < size; j++) {
                if (field[i][j] > 0) {
                    continue;
                }

                std::vector<bool> f(size, false);
                fill(f, field, size, i, j);

                int sum = 0;
                for (int k = 0; k < size; k++) {
                    sum += f[k];
                }

                if (sum == 8) {
                    for (int k = 0; k < size; k++) {
                        if (!f[k]) {
                            field[i][j] = k + 1;
                        }
                    }
                }
            }
        }
    }
}
