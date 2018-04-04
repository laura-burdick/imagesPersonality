# Scene Classification Module

This module loads all of the images and sends them through the MIT PlacesCNN, which returns the softmax output of classifying the image into 204 scene classes.

## Contents
- **classifyScene.py**: script that loads the images, sends them through the CNN, and returns the softmax output in a CSV file

## Dependencies
- Python numpy library (http://www.numpy.org/)
- Python pandas library (http://pandas.pydata.org/)
- Caffe version 0.9999 (this won't work with newer versions of Caffe)
- PlacesCNN model (downloaded from https://github.com/BVLC/caffe/wiki/Model-Zoo#places-cnn-model-from-mit)

##Quick Start
This program must be run on a GPU.

First, open up classifyScene.py and make sure that caffe is being imported from the correct location (lines 13-14).

Then, in bash:
```
$ python classifyScene.py someDirectory/myImage.jpg
$ python classifyScene.py someDirectory
```

The classifyScene script takes one argument, either the full path of a single image or the path to a directory containing only images.

## Output
Results will be stored in features_scene.csv, which has the following headers:
file ,abbey , ..., yard

	file - name of file without extension (or path)
	abbey , ..., yard - the probability that this image took place in this scene (total: 204 scene categories)

## Timing
13m,49s for approximately 7765 images

## References
Zhou, Bolei, et al. "Learning deep features for scene recognition using places database." Advances in neural information processing systems. 2014.
