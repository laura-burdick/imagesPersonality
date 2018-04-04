# Basic Image Features Module #

This module calculates a number of basic image features:

1. Brightness and saturation - mean and standard deviation
2. Pleasure, arousal, and dominance scores. These scores are based on
	   brightness and saturation and are calculated as defined in (Valdez and
	   Mehrabian 1994).
```
Pleasure = 0.69B + 0.22S
Arousal = -0.31B + 0.60S
Dominance = 0.76B + 0.32S
(where B is the average brightness and S is the average saturation)
```
3. Percentages of red, green, and blue
4. The percentage of dynamic v. static lines. This is calculated using the
	   Hough transform. Static lines are lines that are reasonably close to
	   horizontal or vertical, while dynamic lines are lines that are slanted.
5. The number of circles. This is calculated using the Hough transform.

## Contents
- **imageProcessing**: executable that takes either an image (or a directory path)
	and outputs a CSV file containing the features
- **color.h**: C++ class interface for color functions
- **color.cpp**: C++ class implementation for color functions
- **image.h**: C++ class interface for working with images
- **image.cpp**: C++ class implementation for working with images
- **featuresFile.h**: C++ class interface for creating CSV files
- **featuresFile.cpp**: C++ class implementation for creating CSV files
- **main.cpp**: function that calculates features and creates a CSV file
- **Makefile**: instructions for building and linking the executable

## Dependencies
To run executable:
<NONE>

To build from code:
- OpenCV (installation instructions can be found at
  http://docs.opencv.org/2.4/doc/tutorials/introduction/table_of_content_introduction/table_of_content_introduction.html)
- Boost (installation instructions can be found at
  http://www.boost.org/doc/libs/1_61_0/more/getting_started/index.html)

## Quick Start
To run executable, in bash:
```
$ ./brightness_saturation <flags>
```

The executable takes a number of flags that control the execution of the
program. The acceptable flags are:
```
--help, -h: Show a help message
--file, -f: Either the full path of a single imgae or the path to a
	  directory containing only images
--visualize_lines, -v: (optional) Visualize the static and dynamic lines
	  (will copy each image and save it to a new file in the same directory with
	  visualizations)
--visualize_circles, -c: (optional) Visualize the detected circles (will
	  copy each image and save it to a new file in the same directory with
	  visualizations)
--threshold, -t: (optional) Parameter for line calculations: accumulator
	  threshold parameter; only those lines are returned that get enough votes
	  (default: 50)
--minlinelength, -l: (optional) Parameter for line calculations: minimum
	  line length; line segments shorter than this are rejected (default: 50.0)
--maxlinegap, -g: (optional) Parameter for line calculations: maximum
	  allowed gap between points on the same line to link them (default: 10.0)
--deviation, -d: (optional) Parameter for line calculations: the number of
	  radians that a line can deviate from a perfectly straight line and still
	  be considered static (default: PI/12)
--minDist, -m: (optional) Parameter for circle calculations: the minimum
	  distance between circles centers = numRows / minDist (default: 8.0)
--param1, -p: (optional) Parameter for circle calculations: the first
	  method-specific parameter used in OpenCV's HoughCircles (default: 170.0)
--param2, -p: (optional) Parameter for circle calculations: the second
	  method-specific parameter used in OpenCV's HoughCircles (default: 30.0)
```

To build from code, first edit lines 1-19 of the Makefile and make sure that the
paths to OpenCV and Boost are set up correctly. Then, in bash:
```
$ make
```

## Output
Results will be stored in image_features.csv, which has the following headers:
file, brightness_mean, brightness_stddev, saturation_mean, saturation_stddevi,
pleasure, arousal, dominance, red_%, blue_%, green_%, static_lines_%,
dynamic_lines_%, num_circles

	file - name of file without extension (or path)
	brightness_mean - the average brightness of the image
	brightness_stddev - the standard deviation of the brightness of the image
	saturation_mean - the average saturation of the image
	saturation_stddev - the standard deviation of the saturation of the image
	pleasure - the pleasure score for the image
	arousal - the arousal score for the image
	dominance - the dominance score for the image
	red_% - the percentage of red in the image
	blue_% - the percentage of blue in the image
	green_% - the percentage of green in the image
	static_lines_% - the percentage of lines that are static in the image
	dynamic_lines_% - the percentage of lines that are dynamic in the image
	num_circles - the number of circles in the image

If the visualize_lines flag is set, then for each image in the directory, a new
image will be created (in the same directory) with the static lines drawn on the
image in pink and the dynamic lines drawn on the image in green.

If the visualize_circles flag is set, then for each image in the directory, a
new image will be created (in the same directory) with the circles drawn on the
image.

## References
Valdez, Patricia, and Albert Mehrabian. "Effects of color on emotions." Journal
of experimental psychology: General 123.4 (1994): 394.
