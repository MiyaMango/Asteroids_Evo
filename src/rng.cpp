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
    // Generate a random integer in a safe range, e.g., 0 to 1,000,000
    int precision = 1000000; // adjust for desired fractional precision
    int randInt = GetRandomValue(0, precision);

    // Scale it to the desired range
    float result = min + (max - min) * ((float)randInt / precision);
    return result;
}

vector<double> random_double_vector(int size, double min, double max){
    vector<double> output(size);
    for(int i = 0; i < size; i++){
        output[i] = get_random_double(min, max);
    }
    return output;
}