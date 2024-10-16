#include <cstdlib>
#include <iostream>
#include <mutex>
#include <thread>

#include "array.hpp"


static int scalar = 0;
static std::mutex mtx;


int *array_generate(int size)
{
    int *array = new int[size];

    for (int i = 0; i < size; i++) {
        array[i] = rand() % 100 - 50;
    }

    return array;
}


static void multiply(int *a, int *b, int size, int start, int step)
{
    int result = 0;
    for (; start < size; start += step) {
        result += a[start] * b[start];
    }

    mtx.lock();
    scalar += result;
    mtx.unlock();
}


void parallel_scalar(int *a, int *b, int size)
{
    int thrdcnt = std::thread::hardware_concurrency();
    if (!thrdcnt) {
        thrdcnt = 1;
    }

    std::thread *thrds = new std::thread[thrdcnt];
    for (int i = 0; i < thrdcnt; i++) {
        thrds[i] = std::thread(multiply, a, b, size, i, thrdcnt);
    }

    for (int i = 0; i < thrdcnt; i++) {
        thrds[i].join();
    }
    delete[] thrds;

    std::cout << scalar << std::endl;
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
