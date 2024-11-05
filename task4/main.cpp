#include <ctime>
#include <iostream>

#include "mushroomer.hpp"


int main()
{
    srand(time(NULL));

    unsigned speed,
             time;

    std::cin >> time >> speed;

    start(1000 * time, speed);
    return 0;
}
