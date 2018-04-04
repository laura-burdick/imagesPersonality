/**
    @author     Wendlandt Laura <wenlaura@umich.edu>
*/

#ifndef __IMAGE_H
#define __IMAGE_H

#include "color.h"

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <string>
#include <vector>

using std::vector;
using std::string;
using cv::CascadeClassifier;
using cv::Vec4i;
using cv::Vec3f;
using cv::Rect;
using cv::KeyPoint;

const int DEFAULT_LINES_THRESHOLD = 50;
const double DEFAULT_LINES_MINLINELENGTH = 50.0;
const double DEFAULT_LINES_MAXLINEGAP = 10.0;
const double DEFAULT_LINES_DEVIATION = CV_PI/12.0;

const double DEFAULT_CIRCLES_MINDIST = 8.0;
const double DEFAULT_CIRCLES_PARAM1 = 170.0;
const double DEFAULT_CIRCLES_PARAM2 = 30.0;

class Image {
public:
    /**
        Constructs an image
     
        @param imagePath
            The file path where the image to load is located

		@throws string
			When image loads as an empty image
    */
    Image(string imagePath, string name);
   
	string getImagePath() { return imagePath; }
	string getName() { return name; }

	/*
        Calculates brightness & saturation stats
     
        @returns {brightness mean, brightness std dev, saturation mean, saturation std dev}
    */
    vector<double> brightnessAndSaturation();

	/*
	 * Calculates pleasure, arousal, and dominance values based on scale from 
	 * "Effects of Color on Emotions", Valdex & Mehrabian, 1994
	 *
	 * @returns {pleasure, arousal, dominance}
	 */
	vector<double> pleasureArousalDominance();
    
    /**
        Calculates rgb percentages
    
        @returns {red percentage, blue percentage, green percentage}
    */
    vector<double> rgbPercentages();
    
    /**
        Calculates static and dynamic line percentages
    
		@param threshold
			Accumulator threshold parameter. Only those lines are returned that get enough votes (used in HoughLinesP)
		@param minLineLength
			Minimum line length. Line segments shorter than that are rejected (used in HoughLinesP)
		@param maxLineGap
			Maximum allowed gap between points on the same line to link them (used in HoughLinesP)
		@param deviation
			The number of radians that a line can deviate from a perfectly straight line and still be considered static

        returns {static line percentage, dynamic line percentage}
    */
    vector<double> lines(int threshold = DEFAULT_LINES_THRESHOLD, double minLineLength = DEFAULT_LINES_MINLINELENGTH, double maxLineGap = DEFAULT_LINES_MAXLINEGAP, double deviation = DEFAULT_LINES_DEVIATION);
    
    /**
        Finds static and dynamic lines and draws them on the image
     
        @param outputFile
            The path of the file to save the drawn-on image to
        @param threshold, minLineLength, maxLineGap, deviation
            Same as lines() function

		@returns {static line percentage, dynamic line percentage}
    */
 	vector<double> drawLines(string outputFile, int threshold = DEFAULT_LINES_THRESHOLD, double minLineLength = DEFAULT_LINES_MINLINELENGTH, double maxLineGap = DEFAULT_LINES_MAXLINEGAP, double deviation = DEFAULT_LINES_DEVIATION);
    
    /**
        Counts the number of circles
        
		@param minDist
			The minimum distance between circle centers = numRows / minDist (used in HoughCircles)
		@param param1
			The first method-specific parameter used in HoughCircles
		@param param2
			The second method-specific parameter used in HoughCircles

        @returns the number of circles
    */
    int circles(double minDist = DEFAULT_CIRCLES_MINDIST, double param1 = DEFAULT_CIRCLES_PARAM1, double param2 = DEFAULT_CIRCLES_PARAM2);
    
    /**
        Counts the number of circles and draws them on the image
     
        Note: these parameters are incredibly finicky; for the UTAustinData runs they were set to 8, 170, 30
        4, 200, 100 = no circles; 8, 170, 30 = too many circles
     
        @param outputFile
            The path of the file to save the drawn-on image to
        @param minDist, param1, param2
            Same as circles() function

		@returns the number of circles
    */
    int drawCircles(string outputFile, double minDist = DEFAULT_CIRCLES_MINDIST, double param1 = DEFAULT_CIRCLES_PARAM1, double param2 = DEFAULT_CIRCLES_PARAM2);
    
private:
    //saved for potential drawing
    vector<Vec4i> detectedLines;
    vector<Vec3f> detectedCircles;

	string imagePath;
	string name;
    
    cv::Mat image;
    cv::Mat grayImg;
};

#endif
