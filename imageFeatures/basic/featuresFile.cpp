/**
    @author     Wendlandt Laura <wenlaura@umich.edu>
*/

#include "featuresFile.h"

#include <boost/filesystem.hpp>
#include <iostream>
#include <sstream>

using std::stringstream;
using std::cout;
using std::to_string;
using std::endl;
using namespace boost::filesystem;

FeaturesFile::FeaturesFile(string fileName) : brightness(true), saturation(true), rgb(true), lines(true), circles(true), pleasureArousalDominance(true), initialized(false), threshold(DEFAULT_LINES_THRESHOLD), minLineLength(DEFAULT_LINES_MINLINELENGTH), maxLineGap(DEFAULT_LINES_MAXLINEGAP), deviation(DEFAULT_LINES_DEVIATION), minDist(DEFAULT_CIRCLES_MINDIST), param1(DEFAULT_CIRCLES_PARAM1), param2(DEFAULT_CIRCLES_PARAM2), visualizeLines(false), visualizeCircles(false) {
    fout.open(fileName);
    if(fout.fail()) {
        throw string("Failed to create new file: " + fileName + "\n");
    }
}

FeaturesFile::~FeaturesFile() {
    fout.close();
}

void FeaturesFile::init() {
    if(initialized) {
        return;
    } else {
        initialized = true;
    }
    
    vector<string> features;
    features.push_back("file");
    if(brightness) {
        features.push_back("brightness_mean");
        features.push_back("brightness_stddev");
    }
    if(saturation) {
        features.push_back("saturation_mean");
        features.push_back("saturation_stddev");
    }
	if(pleasureArousalDominance) {
		features.push_back("pleasure");
		features.push_back("arousal");
		features.push_back("dominance");
	}
    if(rgb) {
        features.push_back("red_%");
        features.push_back("blue_%");
        features.push_back("green_%");
    }
    if(lines) {
        features.push_back("static_lines_%");
        features.push_back("dynamic_lines_%");
    }
    if(circles) {
        features.push_back("num_circles");
    }
    
	for(int i=0; i<features.size(); ++i) {
        fout << features[i];
        if(i != features.size()-1) {
            fout << ", ";
        }
    }
    fout << endl;
}

void FeaturesFile::outputImage(Image& img) {
    init();
    
    vector<string> features;
    features.push_back(img.getName());
    
    if(brightness || saturation) {
        vector<double> results = img.brightnessAndSaturation();
        if(brightness) {
            features.push_back(to_string(static_cast<long double>(results[0]))); //throws compiler errors on Flux unless result is converted to long double
            features.push_back(to_string(static_cast<long double>(results[1])));
        }
        if(saturation) {
            features.push_back(to_string(static_cast<long double>(results[2])));
            features.push_back(to_string(static_cast<long double>(results[3])));
        }
    }
	if(pleasureArousalDominance) {
		vector<double> results = img.pleasureArousalDominance();
		for(int i=0; i<results.size(); ++i) {
			features.push_back(to_string(static_cast<long double>(results[i])));
		}
	}
    if(rgb) {
        vector<double> results = img.rgbPercentages();
        for(int i=0; i<results.size(); ++i) {
            features.push_back(to_string(static_cast<long double>(results[i])));
        }
    }
    if(lines) {
		vector<double> results;
		if(visualizeLines) {
			string outputFile = img.getImagePath().substr(0,img.getImagePath().find_last_of('/')+1) + img.getName() + "_lines" + img.getImagePath().substr(img.getImagePath().find_last_of('.'));
			results = img.drawLines(outputFile,threshold,minLineLength,maxLineGap,deviation);
		} else {
			results = img.lines(threshold,minLineLength,maxLineGap,deviation);
		}
        for(int i=0; i<results.size(); ++i) {
            features.push_back(to_string(static_cast<long double>(results[i])));
        }
    }
    if(circles) {
		int result;
		if(visualizeCircles) {
			string outputFile = img.getImagePath().substr(0,img.getImagePath().find_last_of('/')+1) + img.getName() + "_circles" + img.getImagePath().substr(img.getImagePath().find_last_of('.'));
			result = img.drawCircles(outputFile,minDist,param1,param2);
		} else {
			result = img.circles(minDist,param1,param2);
		}
        features.push_back(to_string(static_cast<long long int>(result)));
    }
    
	for(int i=0; i<features.size(); ++i) {
        fout << features[i];
        if(i != features.size()-1) {
            fout << ", ";
        }
    }
    fout << endl;
}

void FeaturesFile::outputImagesInDirectory(string& root) {
    this->root = root;

	if(!is_directory(root)) {
		try {
			string name = root.substr(root.find_last_of('/')+1,root.substr(root.find_last_of('/')).find('.')-1);
			Image img(root,name);
			outputImage(img);
		} catch(string s) {
			cout << "Problem with image " << root << endl;
			cout << s << endl;
			cout << "Skipping image and moving on" << endl;
		}
		return;
	}

	//Root is a directory
	int completed = 0;
    for(directory_iterator it = directory_iterator(root); it != directory_iterator(); ++it) {
		try {
			string name = it->path().filename().string().substr(0,it->path().filename().string().find('.'));
			Image img(it->path().string(),name);
			outputImage(img);
		} catch(string s) {
			cout << "Problem with image " << it->path().string() << endl;
			cout << s << endl;
			cout << "Skipping image and moving on" << endl;
		}

		completed += 1;
		cout << "progress: " << completed << "\n";
    }
}
