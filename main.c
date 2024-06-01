#include <stdio.h>
#include "raylib.h"
#include <float.h>
#include "train.c"

#define K_MEANS_IMPLEMENTATION
#include "k_means.h"


#define IMG_FACTOR 100
#define SCREEN_WIDTH (16 * IMG_FACTOR)
#define SCREEN_HEIGHT (9 * IMG_FACTOR)

float origin_shift_x(float offset, float scale, float xy_point, float min_xy) {
    return offset + scale * (xy_point - min_xy);
}

float origin_shift_y(float offset, float scale, float xy_point, float min_xy) {
    return offset - scale * (xy_point - min_xy);
}

// Function to find the min and max values in the dataset
void find_min_max(Point *P, size_t samples, float *min_x, float *max_x, float *min_y, float *max_y) {
    *min_x = *min_y = FLT_MAX;
    *max_x = *max_y = FLT_MIN;
    for (size_t i = 1; i < samples; i++) {
        if (P[i].x < *min_x) *min_x = P[i].x;
        if (P[i].x > *max_x) *max_x = P[i].x;
        if (P[i].y < *min_y) *min_y = P[i].y;
        if (P[i].y > *max_y) *max_y = P[i].y;
    }
}

int main(int argc, char const *argv[]) {
    Color clusterColors[] = { GRAY, MAGENTA, MAROON, BLUE, GREEN, PINK, GOLD, RED, PURPLE, LIME };
    size_t samples = sizeof(train) / (2 * sizeof(float));
    Point *P = _init_model(gen(), samples);

    size_t k = 10;
    Point * centroids = get_centroids(P, samples, k);
    size_t epoch = 0;
    size_t max_epoch = 20;

    float min_x, max_x, min_y, max_y;
    find_min_max(P, samples, &min_x, &max_x, &min_y, &max_y);

    float padding = 50;
    float scale_x = (SCREEN_WIDTH - 2*padding) / (max_x - min_x);
    float scale_y = (SCREEN_HEIGHT - 2*padding) / (max_y - min_y);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "K-Means");
    SetTargetFPS(1);

    float curr_cost;

    int offset_x = padding;  // Padding around the graph
    int offset_y = SCREEN_HEIGHT - padding;  // Adjust y offset since y-coordinates are inverted

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw x and y axes
        DrawLine(0, offset_y, offset_x + SCREEN_WIDTH, offset_y, BLACK); // X axis
        DrawLine(offset_x, 0, offset_x, offset_y + 50, BLACK); // Y axis

        float prev_cost = FLT_MAX;
        if (epoch < max_epoch) {
            _apply_k_means(P, centroids, samples, k);
            curr_cost = _calculate_cost(P, centroids, samples, k);
            printf("Iteration %d, Cost: %f\n", epoch, curr_cost);
        
            if (fabs(prev_cost - curr_cost) < 10) {
                break;
            }
            prev_cost = curr_cost;
            epoch += 1;
        }

        // Draw points
        for (size_t i = 0; i < samples; i++) {
            float x_point = origin_shift_x(offset_x, scale_x, P[i].x, min_x);
            float y_point = origin_shift_y(offset_y, scale_y, P[i].y, min_y);

            if (P[i].idx_cluster == -1) {
                DrawCircle(x_point, y_point, 5, RED);
            } else if (P[i].idx_cluster >= 0 && P[i].idx_cluster < sizeof(clusterColors) / sizeof(clusterColors[0])) {
                DrawCircle(x_point, y_point, 5, clusterColors[P[i].idx_cluster]);
            }
        }

        int squareSize = 15;
        for (size_t i = 0; i < k; i++) {
            float x_centroids = origin_shift_x(offset_x, scale_x, centroids[i].x, min_x);
            float y_centroids = origin_shift_y(offset_y, scale_y, centroids[i].y, min_y);

            if (centroids[i].idx_cluster >= 0 && centroids[i].idx_cluster < sizeof(clusterColors) / sizeof(clusterColors[0])) {
                DrawRectangle(x_centroids - squareSize / 2, y_centroids - squareSize / 2, squareSize, squareSize, clusterColors[centroids[i].idx_cluster]);
            }
        }

        EndDrawing();
    }

    CloseWindow(); // Close the window and unload resources
    return 0;
}
