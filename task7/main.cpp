#include <cstdlib>
#include <ctime>
#include <iostream>

#include "graph.hpp"

int main()
{
    srand(time(NULL));

    for (int i = 4; i < 15; i += 2) {
        struct graph g, h;

        graph_init(&g, i);
        graph_isomorphic(&g, &h);

        graph_print(&g);
        graph_print(&h);
        if (!graph_is_isomorphic(&g, &h)) {
            std::cout << i << " fail" << std::endl;
            return 1;
        }
        graph_del(&g);
        graph_del(&h);
    }
    return 0;
}
