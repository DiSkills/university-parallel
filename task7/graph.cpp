#include <cstdlib>
#include <iostream>

#include "graph.hpp"

void graph_init(struct graph *g, int n)
{
    g->size = n;
    g->list = new std::set<int>[n];

    for (int i = 0; i < g->size; i++) {
        int a, b;

        a = rand() % g->size;
        b = rand() % g->size;

        g->list[a].insert(b);
        g->list[b].insert(a);
    }
}

void graph_del(struct graph *g)
{
    delete[] g->list;
    g->list = nullptr;
    g->size = 0;
}

void graph_print(const struct graph *g)
{
    for (int i = 0; i < g->size; i++) {
        std::cout << i << ": ";
        for (int j: g->list[i]) {
            std::cout << j << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

static void graph_rename_vertices(struct graph *g, int a, int b)
{
    concurrency::parallel_for(0, g->size, [a, b, g](int i) {
        bool a_exists, b_exists;

        a_exists = g->list[i].count(a);
        b_exists = g->list[i].count(b);
        if (a_exists == b_exists) {
            return;
        }

        if (a_exists) {
            g->list[i].erase(a);
            g->list[i].insert(b);
        } else {
            g->list[i].erase(b);
            g->list[i].insert(a);
        }
    });

    std::set<int> tmp = g->list[a];
    g->list[a] = g->list[b];
    g->list[b] = tmp;
}


void graph_isomorphic(const struct graph *g, struct graph *h)
{
    h->size = g->size;
    h->list = new std::set<int>[h->size];
    for (int i = 0; i < h->size; i++) {
        h->list[i] = std::set<int>(g->list[i]);
    }

    for (int i = 0; i < h->size; i++) {
        int a, b;

        a = rand() % h->size;
        b = rand() % h->size;
        if (a != b) {
            graph_rename_vertices(h, a, b);
        }
    }
}
