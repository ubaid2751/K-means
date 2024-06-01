#include <time.h>
#include <stdlib.h>

#define SAMPLES 500
float train[SAMPLES*2];

float rand_float(){
    return (float)rand() / (float)RAND_MAX;
}

float high = 50;
float low = 0;

float* gen() {
    srand(time(0));
    for (size_t i = 0; i < 2*SAMPLES; i++) {
        train[i] = rand_float() * (high - low) + low;
    }
    return train;
}
