/**
    @author     Wendlandt Laura <wenlaura@umich.edu>
*/

#ifndef __COLOR_H
#define __COLOR_H

const int NUM_COLORS = 16;
enum CommonColor {black,gray,silver,white,maroon,red,olive,yellow,green,lime,teal,aqua,navy,blue,purple,fuchsia};

class Color {
public:
    /**
        @param r
            The red value of the color (between 0 and 256)
        @param g
            The green value of the color (between 0 and 256)
        @param b
            The blue value of the color (between 0 and 256)
    */
    Color(double r, double g, double b);
    
    /**
        Determines if the color is a grayscale color
     
        @param deviation
            How far apart the colors are allowed to be and still be considered a grayscale
     
        @returns true if the color is a grayscale color
    */
    bool isGrayscale(int deviation = 10.0);
    
    /**
        Determines which of the colors in the enum CommonColor is the closest to the current color
     
        @returns the index (in the enum CommonColor) of the closest color
    */
    int closestColor();
    
private:
    double r,g,b;

};

const Color COLOR_VALUES[NUM_COLORS] = {Color(0,0,0),Color(128,128,128),Color(192,192,192),Color(255,255,255),Color(128,0,0),Color(255,0,0),Color(128,128,0),Color(255,255,0),Color(0,128,0),Color(0,255,0),Color(0,128,128),Color(0,255,255),Color(0,0,128),Color(0,0,255),Color(128,0,128),Color(255,0,255)};

#endif
