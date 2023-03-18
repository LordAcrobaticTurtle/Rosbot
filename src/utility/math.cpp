#include <utility/math.h>

double floatMap(
    double value, double preMin, double preMax, 
    double postMin, double postMax) 
{
    return (postMax - postMin)/(preMax - preMin) * (value - preMin) + postMin;
}