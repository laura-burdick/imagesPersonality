import numpy as np
from skimage.io import imread
from skimage.feature import greycomatrix, greycoprops
from skimage.color import rgb2gray
import os
import re
from PIL import Image
import sys
import time

start = time.time()

lenArgs = len(sys.argv)
if lenArgs < 2:
	print("This program takes one argument: either the full path of a single \
	image or the path to a directory containing only images")
	sys.exit(2)

args = sys.argv
filePath = args[1]

features = open('features_texture.csv','w')
features.write('file, contrast, correlation, energy, homogeneity\n')

#Load images to be classified
image_files = []
if os.path.isdir(filePath):
	for subdir, dirs, files in os.walk(filePath):
		for file in files:
			image_files.append(os.path.join(subdir,file))
else: #filePath is a single image
	image_files.append(filePath)

progress = 0

for image_file in image_files:
	image = np.asarray(Image.open(image_file))
	if len(image.shape)==3:
		image = rgb2gray(image)

	glcm = greycomatrix(image,[1],[0])

	features.write(os.path.split(image_file)[1][:os.path.split(image_file)[1].find('.')]+', ')
	features.write(str(greycoprops(glcm, 'contrast')[0,0]) + ', ')
	features.write(str(greycoprops(glcm, 'correlation')[0,0]) + ', ')
	features.write(str(greycoprops(glcm, 'energy')[0,0]) + ', ')
	features.write(str(greycoprops(glcm, 'homogeneity')[0,0]) + '\n')

	progress += 1
	print('progress: ',progress)

features.close()

end = time.time()
print("Completed program time: %f seconds" % (end-start))
