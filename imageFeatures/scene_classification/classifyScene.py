# Laura Wendlandt
# wenlaura@umich.edu
#
# Classify all of the UTAustin images using placesCNN (http://places.csail.mit.edu/)

import numpy as np
import pandas as pd
import os
import re
import time

import sys
sys.path.insert(0,"/usr/cac/rhel6/caffe/0.9999/python")
import caffe

start = time.time()

lenArgs = len(sys.argv)
if lenArgs < 2:
	print("This program takes one argument: either the full path of a single image or the path to a directory containing only images")
	sys.exit(2)

args = sys.argv
filePath = args[1]

#Load PlacesCNN
MODEL_FILE = 'placesCNN/places205CNN_deploy_upgraded.prototxt'
PRETRAINED = 'placesCNN/places205CNN_iter_300000_upgraded.caffemodel'
MEAN_FILE = 'placesCNN/places205CNN_mean.npy'
LABELS_FILE = 'placesCNN/categoryIndex_places205.csv'

labelsFile = pd.read_csv(LABELS_FILE,header=None)
labels = labelsFile.as_matrix()
labels = [i[0].split(' ')[0][3:] for i in labels]

#Load images to be classified
image_files = []
if os.path.isdir(filePath):
	for subdir, dirs, files in os.walk(filePath):
		for file in files:
			image_files.append(os.path.join(subdir,file))
else: #filePath is a single image
	image_files.append(filePath)

features = open('features_scene.csv','w')
features.write('file, ')
for i in range(len(labels)):
	features.write(labels[i])
	if i != len(labels)-1:
		features.write(', ')
features.write('\n')

net = caffe.Classifier(MODEL_FILE, PRETRAINED,
                       mean=np.load(MEAN_FILE),
					   channel_swap=(2,1,0),
					   raw_scale=255,
					   image_dims=(227, 227))

net.set_phase_test()
net.set_mode_gpu()

#Classify in batches (to avoid going over Flux memory limits)
BATCH_SIZE = 300

index = 0
loaded = 0
while index < len(image_files):
	batchStart = time.time()

	input_images = []
	for i in range(index,index+BATCH_SIZE):
		if i >= len(image_files):
			break
		try:
			input_images.append(caffe.io.load_image(image_files[i]))
		except:
			print("Failed to open %s" % os.path.join(image_files[i]))
		loaded += 1
		print("loading progress: %d / %d" % (loaded, len(image_files)))

	print("predicting...")
	#predict returns (N x C) ndarray of class probabilities
	#for N images and C classes.
	prediction = net.predict(input_images)

	print("writing to file...")
	imageIndex = 0
	for i in range(index,index+BATCH_SIZE):
		if i >= len(image_files):
			break
		features.write(os.path.split(image_files[i])[1][:os.path.split(image_files[i])[1].find('.')] + ', ')
		for j in range(205): #output probability of each image class
			features.write(str(prediction[imageIndex][j]))
			if j != 204:
				features.write(', ')
		features.write('\n')
		imageIndex += 1

	index += BATCH_SIZE

	batchEnd = time.time()
	print("Completed batch time: %f seconds" % (batchEnd-batchStart))

end = time.time()
print("Completed program time: %f seconds" % (end-start))
