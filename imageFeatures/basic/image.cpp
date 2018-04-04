/**
    @author     Wendlandt Laura <wenlaura@umich.edu>
*/

#include "image.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/xfeatures2d/nonfree.hpp"

using cv::imread;
using cv::Scalar;
using cv::Vec4i;
using cv::Vec3f;
using cv::Rect;
using cv::Size;
using cv::MatIterator_;
using cv::Vec3b;
using cv::Point;
using cv::Ptr;
using cv::Feature2D;
using cv::xfeatures2d::SIFT;

#include <iostream>
using std::cout;
using std::endl;

Image::Image(string imagePath, string name): imagePath(imagePath), name(name) {
	image = imread(imagePath);
	if(image.rows == 0 || image.cols == 0) {
		throw string("Something went wrong: " + imagePath + " has loaded as an empty image");
	}
	grayImg = image;
    cvtColor(image, grayImg, CV_BGR2GRAY);
}

vector<double> Image::brightnessAndSaturation() {
    cv::Mat hsvImage;
    cvtColor(image, hsvImage, CV_BGR2HSV);
    Scalar mean, stdDev;
    meanStdDev(hsvImage, mean, stdDev);
    
    return {mean[2],stdDev[2],mean[1],stdDev[1]};
}

vector<double> Image::pleasureArousalDominance() {
	vector<double> brightSat = brightnessAndSaturation();
	double pleasure = 0.69*brightSat[2] + 0.22*brightSat[1];
	double arousal = -0.31*brightSat[2] + 0.60*brightSat[1];
	double dominance = 0.76*brightSat[2] + 0.32*brightSat[1];

	return {pleasure,arousal,dominance};
}

vector<double> Image::rgbPercentages() {
    Scalar amts;
    amts = sum(image);
    double total = amts[0] + amts[1] + amts[2];
    double redAmt = 0, blueAmt = 0, greenAmt = 0;
    if(total > 0) {
        redAmt = amts[2] / total;
        blueAmt = amts[0] / total;
        greenAmt = amts[1] / total;
    }
    
    return {redAmt,blueAmt,greenAmt};
}

vector<double> Image::lines(int threshold, double minLineLength, double maxLineGap, double deviation) {
	detectedLines.clear();
	
	cv::Mat dst, cdst;
    Canny(image, dst, 50, 200, 3);
    cvtColor(dst,cdst,CV_GRAY2BGR);
    
    HoughLinesP(dst,detectedLines,1,CV_PI/180,threshold,minLineLength,maxLineGap);
    double staticLineLength = 0, dynamicLineLength = 0; //both distances squared
    for(size_t i=0; i<detectedLines.size(); ++i) {
        Vec4i l = detectedLines[i];
        
        bool staticLine = false;
        if(abs(l[2]-l[0]) < .00001 || abs(l[3]-l[1]) < .00001) {
            staticLine = true;
            staticLineLength += (l[2]-l[0])*(l[2]-l[0]) + (l[3]-l[1])*(l[3]-l[1]);
        } else {
            double tanTheta = (double)(l[3]-l[1])/(l[2]-l[0]); //(by-ay)/(bx-ax)
            double theta = atan(tanTheta);
            if((theta > -deviation && theta < deviation) || (theta > (CV_PI/2.0 - deviation) && theta < (CV_PI/2.0 + deviation))) { //it's a static line
                staticLine = true;
                staticLineLength += (l[2]-l[0])*(l[2]-l[0]) + (l[3]-l[1])*(l[3]-l[1]);
            } else { //it's a dynamic line
                staticLine = false;
                dynamicLineLength += (l[2]-l[0])*(l[2]-l[0]) + (l[3]-l[1])*(l[3]-l[1]);
            }
        }
    }
    
    double staticPercent = 0, dynamicPercent = 0;
    if(staticLineLength > 0 || dynamicLineLength > 0) {
        staticPercent = staticLineLength / (staticLineLength + dynamicLineLength);
        dynamicPercent = dynamicLineLength / (staticLineLength + dynamicLineLength);
    }
    
    return {staticPercent,dynamicPercent};
}

vector<double> Image::drawLines(string outputFile, int threshold, double minLineLength, double maxLineGap, double deviation) {
    vector<double> results = lines(threshold,minLineLength,maxLineGap,deviation);
    
    cv::Mat imageCopy =  image.clone();
    for(size_t i=0; i<detectedLines.size(); ++i) {
        Vec4i l = detectedLines[i];
        if(abs(l[2]-l[0]) < .00001 || abs(l[3]-l[1]) < .00001) {
            line(imageCopy,Point(l[0],l[1]),Point(l[2],l[3]),Scalar(255,0,255),3,CV_AA);
        } else {
            double tanTheta = (double)(l[3]-l[1])/(l[2]-l[0]); //(by-ay)/(bx-ax)
            double theta = atan(tanTheta);
            if((theta > -deviation && theta < deviation) || (theta > (CV_PI/2.0 - deviation) && theta < (CV_PI/2.0 + deviation))) { //it's a static line
                line(imageCopy,Point(l[0],l[1]),Point(l[2],l[3]),Scalar(255,0,255),3,CV_AA);
            } else { //it's a dynamic line
                line(imageCopy,Point(l[0],l[1]),Point(l[2],l[3]),Scalar(0,255,0),3,CV_AA);
            }
        }
    }

	cout << "Writing line visualization to " << outputFile << endl;
    imwrite(outputFile, imageCopy);

	return results;
}


int Image::circles(double minDist, double param1, double param2) {
	detectedCircles.clear();

    cv::Mat blurredGray;
	GaussianBlur(grayImg, blurredGray, Size(9, 9), 2, 2);
    HoughCircles(blurredGray, detectedCircles, CV_HOUGH_GRADIENT, 1, blurredGray.rows/minDist,param1,param2,0,0);
    
    return detectedCircles.size();
}

int Image::drawCircles(string outputFile, double minDist, double param1, double param2) {
    int result = circles(minDist,param1,param2);
    
    cv::Mat imageCopy = image.clone();
    for(size_t i = 0; i < detectedCircles.size(); i++)
    {
        Point center(cvRound(detectedCircles[i][0]), cvRound(detectedCircles[i][1]));
        int radius = cvRound(detectedCircles[i][2]);
        // circle center
        circle(imageCopy, center, 3, Scalar(0,255,0), -1, 8, 0);
        // circle outline
        circle(imageCopy, center, radius, Scalar(0,0,255), 3, 8, 0);
    }
    
	cout << "Writing circle visualization to " << outputFile << endl;
    imwrite(outputFile, imageCopy);

	return result;
}
