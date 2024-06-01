#ifndef POINT_H_
#define POINT_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

typedef struct {
    float x;
    float y;
    float origin_dist;
    size_t idx_cluster;
} Point;

#define ORIGIN                              (Point){0.0, 0.0, 0.0, (size_t)-1} 
#define INIT_POINT(x, y)                    _init_point(x, y)                               // It initializes a point
#define SHOW_POINT(a)                       _display_point(a)                               // It shows the coordinates of a particular point
#define DISTANCE(a,b)                       _calc_dist(a, b)                                // It calculates the euclidean distance between two points
#define GET_DATA(data, samples)             get_data(data, samples)                         // It shows the data provided
#define POINT_ASSERT                        assert                                          // It asserts values.
#define ADD_POINT(a, b)                     _add_point(a, b)

float _calc_dist(Point a, Point b);

Point _init_point(float x, float y);
Point *get_data(float *data, size_t size);

void swap(Point *a, Point *b);
void _display_point(Point a);
void shuffle(Point *p, size_t samples);
void _copy_data(Point *dest, const Point *src, size_t samples);
void _add_points(Point *dest, Point *src);

void swap(Point* a, Point* b) {
    Point temp = *a;
    *a = *b;
    *b = temp;
}

void shuffle(Point *p, size_t samples) {
    srand(time(0));
    if (samples > 1) {
        for (size_t i = 0; i < samples - 1; i++) {
            size_t j = i + rand() / (RAND_MAX / (samples - i) + 1);
            swap(&p[i], &p[j]);
        }
    }
}

#endif //POINT_H_

#ifdef POINT_IMPLEMENTATION
float _calc_dist(Point a, Point b) {
    return (float) sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

Point _init_point(float x, float y) {
    Point a;
    a.x = x;
    a.y = y;
    a.origin_dist = _calc_dist(a, ORIGIN);
    a.idx_cluster = -1;
    return a;
}

void _display_point(Point a) {
    printf("x: %f,\t y: %f\n", a.x, a.y); 
}

Point* get_data(float *data, size_t samples) {
    Point* points = (Point*) malloc(samples * sizeof(Point));
    POINT_ASSERT(points != NULL);
    for (int i = 0; i < samples; ++i) {
        points[i] = INIT_POINT(data[2 * i], data[2 * i + 1]);
    }
    return points;
}

void _copy_data(Point *dest, const Point *src, size_t samples) {
    for (size_t i = 0; i < samples; i++) {
        dest[i] = src[i];
    }
}

void _add_point(Point *dest, Point *src) {
    dest->x += src->x;
    dest->y += src->y;
    dest->origin_dist = _calc_dist(*dest, ORIGIN);
}

#endif //POINT_IMPLEMENTATION