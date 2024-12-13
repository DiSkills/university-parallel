#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <ppl.h>
#include <vector>

#include "graph.hpp"

typedef std::map<int, std::set<int>> step_t;

static bool graph_equals(const struct graph *g, const struct graph *h)
{
    if (h->size != g->size) {
        return false;
    }

    for (int i = 0; i < g->size; i++) {
        if (h->list[i] != g->list[i]) {
            return false;
        }
    }
    return true;
}

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

static void rename_set(step_t &step, int a, int b)
{
    int a_index = -1, b_index = -1;

    for (auto &i: step) {
        for (int k: i.second) {
            if (k == a) {
                a_index = i.first;
            } else if (k == b) {
                b_index = i.first;
            }
        }
    }
    step[a_index].erase(a);
    step[a_index].insert(b);

    step[b_index].erase(b);
    step[b_index].insert(a);
}

static void graph_copy(const struct graph *g, struct graph *h)
{
    h->size = g->size;
    h->list = new std::set<int>[h->size];
    for (int i = 0; i < h->size; i++) {
        h->list[i] = std::set<int>(g->list[i]);
    }
}

void graph_isomorphic(const struct graph *g, struct graph *h)
{
    graph_copy(g, h);

    for (int i = 0; i < h->size; i++) {
        int a, b;

        a = rand() % h->size;
        b = rand() % h->size;
        if (a != b) {
            graph_rename_vertices(h, a, b);
        }
    }
}

static void graph_rename_pendants(struct graph *g, step_t &step1,
                                  step_t &step2)
{
    std::set<int> s;

    for (auto &i: step2) {
        if (i.second.size() == 1) {
            int a, b;

            a = *i.second.begin();
            b = *step1[i.first].begin();
            s.insert(i.first);

            if (a != b) {
                graph_rename_vertices(g, a, b);
                rename_set(step2, a, b);
            }
        }
    }
    for (int i: s) {
        step2.erase(i);
    }
}

static int first(std::set<int> &a, std::set<int> &b)
{
    for (int i: b) {
        if (!a.count(i)) {
            return i;
        }
    }
    return -1;
}

static bool next_permutation(std::vector<int> &v, std::vector<int> &a)
{
    for (int i = 1; i < a.size(); i++) {
        if (next_permutation(next(v.begin(), a[i - 1]),
                             next(v.begin(), a[i])))
        {
            return true;
        }
        std::sort(next(v.begin(), a[i - 1]), next(v.begin(), a[i]));
    }
    return false;
}

static bool run_permutations(const struct graph *h, const struct graph *a,
                             step_t &step)
{
    std::vector<int> v1, v2, v3 = { 0 };

    for (auto &i: step) {
        copy(i.second.begin(), i.second.end(), back_inserter(v1));
        v3.push_back(i.second.size() + v3.back());
    }
    copy(v1.begin(), v1.end(), back_inserter(v2));

    do {
        std::vector<int> currver;
        copy(v2.begin(), v2.end(), back_inserter(currver));

        struct graph b;
        graph_copy(a, &b);

        std::set<int> ing;
        for (int i = 0; i < v1.size(); i++) {
            if (!(v1[i] == currver[i] || ing.count(v1[i]))) {
                graph_rename_vertices(&b, v1[i], currver[i]);
                currver[std::find(currver.begin(), currver.end(), v1[i])
                        - currver.begin()] = currver[i];
                currver[i] = v1[i];
            }
            ing.insert(v1[i]);
        }
        if (graph_equals(h, &b)) {
            return true;
        }
    } while (next_permutation(v1, v3));
    return false;
}

bool graph_is_isomorphic(struct graph *g, struct graph *h)
{
    if (h->size != g->size) {
        return false;
    }

    struct graph a;
    graph_copy(h, &a);

    step_t step1, step2;
    for (int i = 0; i < g->size; i++) {
        step1[g->list[i].size()].insert(i);
    }
    for (int i = 0; i < g->size; i++) {
        step2[a.list[i].size()].insert(i);
    }
    graph_rename_pendants(&a, step1, step2);

    for (auto i = step2.begin(); i != step2.end();) {
        std::set<int> &xset = i->second,
                      &yset = step1[i->first];
        int x = first(xset, yset);
        if (x == -1) {
            i++;
            continue;
        }
        int	y = first(yset, xset);
        graph_rename_vertices(&a, x, y);
        rename_set(step2, x, y);
    }
    step2.erase(0);

    if (step2.size()) {
        return run_permutations(g, &a, step2);
    }
    return false;
}
