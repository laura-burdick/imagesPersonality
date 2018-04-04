/**
    @author     Wendlandt Laura <wenlaura@umich.edu>
*/

#ifndef __FEATURES_FILE_H
#define __FEATURES_FILE_H

#include "image.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using std::ofstream;
using std::vector;

class FeaturesFile {
public:
    /**
        Creates a new feature csv file
     
        @throws string
            When there is an error creating the file
     
        @param fileName
            The name of the file to be created
    */
    FeaturesFile(string fileName);

	/*
	 * Set the parameters for calculating static and dynamic lines
	 */
	void setLineThreshold(int threshold) { this->threshold = threshold; }
	void setLineMinLineLength(double minLineLength) { this->minLineLength = minLineLength; }
	void setLineMaxLineGap(double maxLineGap) { this->maxLineGap = maxLineGap; }
	void setLineDeviation(double deviation) { this->deviation = deviation; }
    
	/*
	 * Set the parameters for calculating circles
	 */
	void setCirclesMinDist(double minDist) { this->minDist = minDist; }
	void setCirclesParam1(double param1) { this->param1 = param1; }
	void setCirclesParam2(double param2) { this->param2 = param2; }

	/*
	 * Turn on (or off) visualizations
	 */
	void setVisualizeLines(bool v = true) { visualizeLines = v; }
	void setVisualizeCircles(bool v = true) { visualizeCircles = v; }

    /**
        Turn off certain features (don't calculate them for the file)
    */
    void unsetBrightness() { brightness = false; }
    void unsetSaturation() { saturation = false; }
    void unsetRgb() { rgb = false; }
    void unsetLines() { lines = false; }
    void unsetCircles() { circles = false; }
	void unsetPleasureArousalDominance() { pleasureArousalDominance = false; }
    
    /**
	 	If root is a directory, loads all images in that directory.
		If root is an image, it only loads that image.
     
        @throws string
            If an image throws an exception
     
        @param root
            The path of the root directory
    */
    void outputImagesInDirectory(string& root);
    
    /**
        Process an image and add it to the features file
     
        @param img
            The image to be processed
    */
    void outputImage(Image& img);
    
    /**
        Closes the feature file
    */
    ~FeaturesFile();
private:
    ofstream fout;
    bool brightness, saturation, rgb, lines, circles, pleasureArousalDominance;
    string root;
    bool initialized;

	//parameters for static and dynamic lines
	int threshold;
    double minLineLength, maxLineGap, deviation;
    
	//parameters for circles
	double minDist, param1, param2;

	//visualization variables
	bool visualizeLines, visualizeCircles;

    /**
        Output the initial header line of the features file
    */
    void init();
};

#endif
