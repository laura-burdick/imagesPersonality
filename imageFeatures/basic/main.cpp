/**
    @author     Wendlandt Laura <wenlaura@umich.edu>
*/

#include "featuresFile.h"

#include <string>
#include <sstream>
#include <getopt.h>

using std::stringstream;
using std::string;
using std::cout;
using std::endl;
using cv::getTickCount;
using cv::getTickFrequency;

static const struct option longopts[] = {
	{"help", no_argument, nullptr, 'h'},
	{"file", required_argument, nullptr, 'f'},
	{"visualize_lines", no_argument, nullptr, 'v'},
	{"visualize_circles", no_argument, nullptr, 'c'},
	{"threshold", required_argument, nullptr, 't'},
	{"minlinelength", required_argument, nullptr, 'l'},
	{"maxlinegap", required_argument, nullptr, 'g'},
	{"deviation", required_argument, nullptr, 'd'},
	{"minDist", required_argument, nullptr, 'm'},
	{"param1", required_argument, nullptr, 'p'},
	{"param2", required_argument, nullptr, 'w'}
};

int main(int argc, char** argv) {
    //start timing
    double start = (double) getTickCount();

	FeaturesFile featuresFile("image_features.csv");

	int optionIndex = 0;
	char c;

	double dTemp;
	int iTemp;

	string filePath = "";
	
	while((c = getopt_long(argc, argv, "hf:vct:l:g:d:m:p:w:", longopts, &optionIndex)) != -1){
		stringstream ss;
		switch(c) {
			case 'h':
				cout << "Command line options:" << endl;
				cout << "--help, -h: Show this help message" << endl;
				cout << "--file, -f: Either the full path of a single image or the path to a directory containing only images" << endl;
				cout << "--visualize_lines, -v: (optional) Visualize the static and dynamic lines (will copy each image and save it to a new file in the same directory with visualizations)" << endl;
				cout << "--visualize_circles, -c: (optional) Visualize the detected circles (will copy each image and save it to a new file in the same directory with visualizations)" << endl << endl;
				cout << "--threshold, -t: (optional) Parameter for line calculations: accumulator threshold parameter; only those lines are returned that get enough votes (default: 50)" << endl;
				cout << "--minlinelength, -l: (optional) Parameter for line calculations: minimum line length; line segments shorter than this are rejected (default: 50.0)" << endl;
				cout << "--maxlinegap, -g: (optional) Parameter for line calculations: maximum allowed gap between points on the same line to link them (default: 10.0)" << endl;
				cout << "--deviation, -d: (optional) Parameter for line calculations: the number of radians that a line can deviate from a perfectly straight line and still be considered static (default: PI/12)" << endl << endl;
				cout << "--minDist, -m: (optional) Parameter for circle calculations: the minimum distance between circle centers = numRows / minDist (default: 8.0)" << endl;
				cout << "--param1, -p: (optional) Parameter for circle calculations: the first method-specific parameter used in OpenCV's HoughCircles (default: 170.0)" << endl;
				cout << "--param2, -w: (optional) Parameter for circle calculations: the second method-specific parameter used in OpenCV's HoughCircles (default: 30.0)" << endl;
				break;
			case 'f':
				ss << optarg;
				ss >> filePath;
				break;
			case 'v':
				featuresFile.setVisualizeLines();
				break;
			case 'c':
				featuresFile.setVisualizeCircles();
				break;
			case 't':
				ss << optarg;
				ss >> iTemp;
				featuresFile.setLineThreshold(iTemp);
				cout << "Threshold set to " << iTemp << endl;
				break;
			case 'l':
				ss << optarg;
				ss >> dTemp;
				featuresFile.setLineMinLineLength(dTemp);
				cout << "MinLineLength set to " << dTemp << endl;
				break;
			case 'g':
				ss << optarg;
				ss >> dTemp;
				featuresFile.setLineMaxLineGap(dTemp);
				cout << "MaxLineGap set to " << dTemp << endl;
				break;
			case 'd':
				ss << optarg;
				ss >> dTemp;
				featuresFile.setLineDeviation(dTemp);
				cout << "Deviation set to " << dTemp << endl;
				break;
			case 'm':
				ss << optarg;
				ss >> dTemp;
				featuresFile.setCirclesMinDist(dTemp);
				cout << "MinDist set to " << dTemp << endl;
				break;
			case 'p':
				ss << optarg;
				ss >> dTemp;
				featuresFile.setCirclesParam1(dTemp);
				cout << "Param1 set to " << dTemp << endl;
				break;
			case 'w':
				ss << optarg;
				ss >> dTemp;
				featuresFile.setCirclesParam2(dTemp);
				cout << "Param2 set to " << dTemp << endl;
				break;
		}
	}

	if(filePath == "") {
		cout << "Error: must provide an argument to the --file argument (see --help for more information)" << endl;
		exit(2);
	}

	featuresFile.outputImagesInDirectory(filePath);

	//end timing
    double end = (double) getTickCount();
    cout << "execution time: " << (end-start)/getTickFrequency() << " seconds\n";

    return 0;
}
