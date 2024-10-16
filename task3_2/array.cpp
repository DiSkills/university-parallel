#include <cstdlib>
#include <cstring>
#include <thread>

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


static void merge(int *array, int asz, int bsz)
{
    int *tmp = new int[asz + bsz];
    memcpy(tmp, array, (asz + bsz) * sizeof(int));

    int *a = tmp,
        *b = tmp + asz;

    int i = 0,
        j = 0,
        k = 0;

    while (i < asz && j < bsz) {
        if (a[i] < b[j]) {
            array[k] = a[i];
            i++;
        } else {
            array[k] = b[j];
            j++;
        }
        k++;
    }

    for (; i < asz; i++, k++) {
        array[k] = a[i];
    }

    for (; j < bsz; j++, k++) {
        array[k] = b[j];
    }

    delete[] tmp;
}


static void parallel_merge(int *array, int size,
        std::thread *thrds, int thrdcnt, int blksz)
{
    while (thrdcnt > 1) {
        thrdcnt /= 2;

        int start = 0;
        for (int i = 0; i < thrdcnt - 1; i++) {
            thrds[i] = std::thread(merge, array + start, blksz, blksz);
            start += 2 * blksz;
        }
        thrds[thrdcnt - 1] = std::thread(merge,
                array + start, blksz, size - start - blksz);

        for (int i = 0; i < thrdcnt; i++) {
            thrds[i].join();
        }
        blksz *= 2;
    }
}


void parallel_sort(int *array, int size)
{
    int thrdcnt = std::thread::hardware_concurrency();
    if (!thrdcnt) {
        thrdcnt = 1;
    }

    int blksz = size / thrdcnt;
    std::thread *thrds = new std::thread[thrdcnt];

    int start = 0,
        end = blksz - 1;
    for (int i = 0; i < thrdcnt - 1; i++) {
        thrds[i] = std::thread(array_sort, array, start, end);

        start += blksz;
        end += blksz;
    }
    thrds[thrdcnt - 1] = std::thread(array_sort, array, start, size - 1);

    for (int i = 0; i < thrdcnt; i++) {
        thrds[i].join();
    }

    parallel_merge(array, size, thrds, thrdcnt, blksz);

    delete[] thrds;
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
