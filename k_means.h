#ifndef K_MEANS_H_
#define K_MEANS_H_

#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

#define POINT_IMPLEMENTATION
#include "point.h"

#define SHOW_DATA(dt_Point, samples) _show_data(dt_Point, samples)

Point *_init_model(float *data, size_t samples);
Point *_random_selection(Point p[], size_t samples, size_t k);

float _calculate_cost(Point *p, Point *centroids, size_t samples, size_t k);

void _apply_k_means(Point *p, Point centroids[], size_t samples, size_t k);
void _show_data(Point* dt_Point, size_t samples);
void _get_idx_of_clusters(Point *p, Point centroids[] ,size_t samples, size_t k);
void _update_centroids(Point p[], Point *centroids, size_t samples, size_t k);

#endif //K_MEANS_H_

#ifdef K_MEANS_IMPLEMENTATION

Point* _init_model(float *data, size_t samples) {
    return GET_DATA(data, samples); //INIT_POINT(4.f, 3.f);
}

void _show_data(Point* dt_Point, size_t samples) {
    printf("------------------------------------------------------------------------------------\n");
    for (size_t i = 0; i < samples; i++) {
        SHOW_POINT(dt_Point[i]);
    }
    printf("------------------------------------------------------------------------------------\n");
}

Point *_random_selection(Point p[], size_t samples, size_t k) {
    Point *centroids = malloc(k * sizeof(Point));
    POINT_ASSERT(centroids != NULL);

    shuffle(p, samples);
    for (size_t i = 0; i < k; ++i) {
        centroids[i] = p[i];
        centroids[i].idx_cluster = i;
    }
    return centroids;
}

Point *get_centroids(Point *P, size_t samples, size_t k) {
    return _random_selection(P, samples, k);
}

void _get_idx_of_clusters(Point *p, Point centroids[] ,size_t samples, size_t k) {
    for (size_t i = 0; i < samples; i++) {
        float min_dist = FLT_MAX;
        for (size_t j = 0; j < k; j++) {
            float dist = DISTANCE(p[i], centroids[j]);
            if (dist < min_dist) {
                min_dist = dist;
                p[i].idx_cluster = j;
            }
        }
    }
}

void _update_centroids(Point p[], Point *centroids, size_t samples, size_t k) {
    for (size_t i = 0; i < k; i++) { 
        Point sum_Of_points = ORIGIN;
        size_t count_cluster_idx = 0;
        for (size_t j = 0; j < samples; j++) {
            if (p[j].idx_cluster == i) {
                count_cluster_idx += 1;
                ADD_POINT(&sum_Of_points, &p[j]);
            }
        }
        centroids[i].x = sum_Of_points.x / count_cluster_idx;
        centroids[i].y = sum_Of_points.y / count_cluster_idx;
    }
}

float _calculate_cost(Point *p, Point *centroids, size_t samples, size_t k) {
    float cost = 0.0;
    for (size_t i = 0; i < samples; i++) {
        int cluster = p[i].idx_cluster;
        if (cluster >= 0 && cluster < k) {
            float dx = p[i].x - centroids[cluster].x;
            float dy = p[i].y - centroids[cluster].y;
            cost += dx * dx + dy * dy; // Sum of squared Euclidean distances
        }
    }   
    cost /= samples;
    return cost;
}

void _apply_k_means(Point *p, Point *centroids, size_t samples, size_t k) {
    _get_idx_of_clusters(p, centroids , samples, k);
    _update_centroids(p, centroids, samples, k);
}

#endif // K_MEANS_IMPLEMENTATION