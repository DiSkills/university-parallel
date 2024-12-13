#ifndef GRAPH_H
#define GRAPH_H

#include <set>

struct graph {
    int size;
    std::set<int> *list;
};

void graph_init(struct graph *g, int n);
void graph_del(struct graph *g);
void graph_print(const struct graph *g);
void graph_isomorphic(const struct graph *g, struct graph *h);

#endif
