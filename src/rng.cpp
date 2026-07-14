#include <raylib.h>
#include "rng.hpp"

using namespace std;

double get_random_double(double min, double max) {
    if(max < min){
        double aux = max;
        max = min;
        min = aux;
    }
    
    // Generate a random integer in a safe range, e.g., 0 to 1,000,000
    int precision = 1000000; // adjust for desired fractional precision
    int randInt = GetRandomValue(0, precision);

    // Scale it to the desired range
    double result = min + (max - min) * ((double)randInt / precision);
    return result;
}

float get_random_float(float min, float max) {
    if(max < min){
        float aux = max;
        max = min;
        min = aux;
    }
    
    // Generate a random integer in a safe range, e.g., 0 to 1,000,000
    int precision = 1000000; // adjust for desired fractional precision
    int randInt = GetRandomValue(0, precision);

    // Scale it to the desired range
    float result = min + (max - min) * ((float)randInt / precision);
    return result;
}