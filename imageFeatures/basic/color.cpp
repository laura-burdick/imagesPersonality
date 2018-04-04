/**
    @author     Wendlandt Laura <wenlaura@umich.edu>
*/

#include "color.h"
#include <cstdlib>

Color::Color(double r, double g, double b) : r(r), g(g), b(b) {
    //empty
}

bool Color::isGrayscale(int deviation) {
    if(abs(r-g) < deviation && abs(r-b) < deviation && abs(g-b) < deviation) {
        return true;
    }
    return false;
}

int Color::closestColor() {
    int start = 4;
    int end = NUM_COLORS;
    if(isGrayscale()) {
        start = 0;
        end = 3;
    }
    
    double nearest = 0;
    int nearestColor = 0;
    for(int i=start; i<end; ++i) {
        double distanceSquared = (double)(r-COLOR_VALUES[i].r)*(r-COLOR_VALUES[i].r) + (double)(g-COLOR_VALUES[i].g)*(g-COLOR_VALUES[i].g) + (double)(b-COLOR_VALUES[i].b)*(b-COLOR_VALUES[i].b);
        if(i==start || distanceSquared < nearest) {
            nearest = distanceSquared;
            nearestColor = i;
        }
    }
    return nearestColor;
}
