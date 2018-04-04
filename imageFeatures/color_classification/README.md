# Color Classification Module

This module calculates the color name for each pixel in an image. The module
takes a color image(s) as input and returns the most likely color for each pixel
from a set of eleven color names (black, blue, brown, gray, green, orange, pink,
purple, red, white, and yellow). The mapping from RGB values to color names has
been learned from Google images, as described in the reference below.

## Contents
- **calculateColors.m**: function that takes either an image (or a directory path)
	and outputs a CSV file containing color pixel counts 
- **w3c.mat**: prelearned matrix of mappings from RGB to color names
- **im2c.m**: function that takes an image and outputs color names

## Quick Start
In bash:
```
$ matlab
$ calculateColors('someDirectory/myImage.jpg')
$ calculateColors('someDirectory')
```

The calculateColors function takes one argument, either the full path of a
single image or the path to a directory containing only images.

## Error Handling
If an image is not in the right colorspace (Matlab does not support JPEG images
with CMYK colorspaces), an error will be thrown, but the program will continue
executing. The errors will be stored in a file called errors.txt. To convert an
image to the right colorspace, use ImageMagick:
```
$ convert file1.jpg -colorspace RGB file1.jpg.
```

## Output
Results will be stored in features_color.csv, which has the following headers:
file, black, blue, brown, grey, green, orange, pink, purple, red, white, yellow

	file - name of file without extension (or path)
	black,...,yellow - number of pixels in the image identified as that color
		name

Errors will be recorded in errors.txt.

## Timing
6m,6s for approximately 7765 images

## References
Van De Weijer, Joost, et al. "Learning color names for real-world applications."
Image Processing, IEEE Transactions on 18.7 (2009): 1512-1523.
