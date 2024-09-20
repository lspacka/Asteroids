#include <stdio.h>
#include <stdlib.h>
#include <time.h>

float RandSpeed(float a, float b) {
    float randValue = (float)rand() / (float)RAND_MAX;
    float scaledValue = randValue * (b - a);
    float finalValue = a + scaledValue;

    // Debugging prints
    printf("rand() = %d, RAND_MAX = %d\n", rand(), RAND_MAX);
    printf("randValue = %f\n", randValue);
    printf("scaledValue = %f\n", scaledValue);
    printf("finalValue = %f\n", finalValue);

    return finalValue;
}

int main(void) {
    srand(time(NULL));  // Seed the random number generator

    for (int i = 0; i < 5; i++) {
        float speed = RandSpeed(-1.5f, 1.5f);
        printf("Random Speed: %.2f\n", speed);
    }

    return 0;
}
