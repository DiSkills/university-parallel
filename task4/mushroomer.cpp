#include <cmath>
#include <cstdlib>
#include <future>
#include <iostream>
#include <mutex>

#include "mushroomer.hpp"

typedef struct {
    double x, y;
} point_t;

typedef struct {
    point_t position;
    
    unsigned steps,
             time;
    
    unsigned mushrooms;
    
    double d, f;
} mushroomer_t;


static std::mutex mtx;
static std::promise<point_t> prms[2];


static int random(int max)
{
    return (int)(1.0 * max * rand() / (RAND_MAX + 1.0));
}


static double distance(point_t *point, double x, double y)
{
    return sqrt(pow(point->x - x, 2) + pow(point->y - y, 2));
}


static void mushroomer_init(mushroomer_t *mushroomer, unsigned time)
{
    mushroomer->position.x = 0;
    mushroomer->position.y = 0;
    
    mushroomer->steps = 0;
    mushroomer->time = time;
    
    mushroomer->mushrooms = 0;
    mushroomer->d = 0;
    mushroomer->f = 0;
}


static void mushroomer_move(mushroomer_t *mushroomer,
        unsigned max_speed, int i)
{
    int j = sizeof(prms) - 1 - i;
    unsigned angle, speed, time, path;

    angle = random(360 - 1);
    speed = random(max_speed);
    time = random(mushroomer->time);

    path = time * speed;

    mushroomer->position.x += cos(angle) * path;
    mushroomer->position.y += sin(angle) * path;

    mushroomer->steps++;
    mushroomer->time -= time;
    mushroomer->mushrooms += random(20);

    mushroomer->d += distance(&mushroomer->position, 0, 0);

    mtx.lock();
    prms[i] = std::promise<point_t>();
    prms[i].set_value(mushroomer->position);
    mtx.unlock();

    point_t point = prms[j].get_future().get();

    mtx.lock();
    prms[j] = std::promise<point_t>();
    prms[j].set_value(point);
    mtx.unlock();

    mushroomer->f += distance(&mushroomer->position, point.x, point.y);
}


static void mushroomer_start(mushroomer_t *mushroomer,
        unsigned max_speed, int i)
{
    while (mushroomer->time > 0) {
        mushroomer_move(mushroomer, max_speed, i);
    }
}


void start(unsigned time, unsigned max_speed)
{
    mushroomer_t m1, m2;

    mushroomer_init(&m1, time);
    mushroomer_init(&m2, time);

    prms[0] = std::promise<point_t>();
    prms[0].set_value(m1.position);

    prms[1] = std::promise<point_t>();
    prms[1].set_value(m2.position);

    std::thread t1(mushroomer_start, &m1, max_speed, 0);
    std::thread t2(mushroomer_start, &m2, max_speed, 1);

    t1.join();
    t2.join();

    std::cout << (m1.d + m2.d) / 2 << ' ' << (m1.f + m2.f) / 2 << std::endl;
}
