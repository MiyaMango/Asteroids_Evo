#ifndef RNG_H
#define RNG_H

#include <vector>

double get_random_double(double min, double max);
float get_random_float(float min, float max);
std::vector<double> random_double_vector(int size, double min, double max);

#endif