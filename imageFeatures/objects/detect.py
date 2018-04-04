#!/usr/bin/env python
"""
detector.py is an out-of-the-box windowed detector
callable from the command line.

By default it configures and runs the Caffe reference ImageNet model.
Note that this model was trained for image classification and not detection,
and finetuning for detection can be expected to improve results.

The selective_search_ijcv_with_python code required for the selective search
proposal mode is available at
	https://github.com/sergeyk/selective_search_ijcv_with_python

TODO:
- batch up image filenames as well: don't want to load all of them into memory
- come up with a batching scheme that preserved order / keeps a unique ID
"""
import numpy as np
import pandas as pd
import os
import argparse
import time

import caffe

CROP_MODES = ['list', 'selective_search']
COORD_COLS = ['ymin', 'xmin', 'ymax', 'xmax','score']

NUM_OUTPUT = 1000 #number of output classes

#from /home/wenlaura/local/caffe-rc3/examples/detections.ipynb
def nms_detections(dets, overlap=0.3):
	"""
	Non-maximum suppression: Greedily select high-scoring detections and
	skip detections that are significantly covered by a previously
	selected detection.

	This version is translated from Matlab code by Tomasz Malisiewicz,
	who sped up Pedro Felzenszwalb's code.

	Parameters
	----------
	dets: ndarray
		each row is ['xmin', 'ymin', 'xmax', 'ymax', 'score']
	overlap: float
		minimum overlap ratio (0.3 default)

	Output
	------
	dets: ndarray
		remaining after suppression.
	"""
	x1 = dets[:, 0]
	y1 = dets[:, 1]
	x2 = dets[:, 2]
	y2 = dets[:, 3]
	ind = np.argsort(dets[:, 4])

	w = x2 - x1
	h = y2 - y1
	area = (w * h).astype(float)

	pick = []
	while len(ind) > 0:
		i = ind[-1]
		pick.append(i)
		ind = ind[:-1]

		xx1 = np.maximum(x1[i], x1[ind])
		yy1 = np.maximum(y1[i], y1[ind])
		xx2 = np.minimum(x2[i], x2[ind])
		yy2 = np.minimum(y2[i], y2[ind])

		w = np.maximum(0., xx2 - xx1)
		h = np.maximum(0., yy2 - yy1)

		wh = w * h
		o = wh / (area[i] + area[ind] - wh)

		ind = ind[np.nonzero(o <= overlap)[0]]

	return dets[pick, :]

def main(argv):
	pycaffe_dir = os.path.dirname(__file__)

	parser = argparse.ArgumentParser()
	# Required arguments: input and output.
	parser.add_argument(
		"input_file",
		help="Input txt/csv filename. If .txt, must be list of filenames.\
		If .csv, must be comma-separated file with header\
		'filename, xmin, ymin, xmax, ymax'"
	)
	parser.add_argument(
		"output_file",
		help="Output h5/csv filename. Format depends on extension."
	)
	# Optional arguments.
	parser.add_argument(
		"--model_def",
		default=os.path.join(pycaffe_dir,
				"../models/bvlc_reference_caffenet/deploy.prototxt"),
		help="Model definition file."
	)
	parser.add_argument(
		"--pretrained_model",
		default=os.path.join(pycaffe_dir,
				"../models/bvlc_reference_caffenet/bvlc_reference_caffenet.caffemodel"),
		help="Trained model weights file."
	)
	parser.add_argument(
		"--crop_mode",
		default="selective_search",
		choices=CROP_MODES,
		help="How to generate windows for detection."
	)
	parser.add_argument(
		"--gpu",
		action='store_true',
		help="Switch for gpu computation."
	)
	parser.add_argument(
		"--mean_file",
		default=os.path.join(pycaffe_dir,
							 'caffe/imagenet/ilsvrc_2012_mean.npy'),
		help="Data set image mean of H x W x K dimensions (numpy array). " +
			 "Set to '' for no mean subtraction."
	)
	parser.add_argument(
		"--input_scale",
		type=float,
		help="Multiply input features by this scale to finish preprocessing."
	)
	parser.add_argument(
		"--raw_scale",
		type=float,
		default=255.0,
		help="Multiply raw input by this scale before preprocessing."
	)
	parser.add_argument(
		"--channel_swap",
		default='2,1,0',
		help="Order to permute input channels. The default converts " +
			 "RGB -> BGR since BGR is the Caffe default by way of OpenCV."

	)
	parser.add_argument(
		"--context_pad",
		type=int,
		default='16',
		help="Amount of surrounding context to collect in input window."
	)
	args = parser.parse_args()

	mean, channel_swap = None, None
	if args.mean_file:
		mean = np.load(args.mean_file)
		if mean.shape[1:] != (1, 1):
			mean = mean.mean(1).mean(1)
	if args.channel_swap:
		channel_swap = [int(s) for s in args.channel_swap.split(',')]

	if args.gpu:
		caffe.set_mode_gpu()
		print("GPU mode")
	else:
		caffe.set_mode_cpu()
		print("CPU mode")

	# Make detector.
	detector = caffe.Detector(args.model_def, args.pretrained_model, mean=mean,
			input_scale=args.input_scale, raw_scale=args.raw_scale,
			channel_swap=channel_swap,
			context_pad=args.context_pad)

	# Load input.
	t = time.time()
	print("Loading input...")
	if args.input_file.lower().endswith('txt'):
		with open(args.input_file) as f:
			inputs = [_.strip() for _ in f.readlines()]
	elif args.input_file.lower().endswith('csv'):
		inputs = pd.read_csv(args.input_file, sep=',', dtype={'filename': str})
		inputs.set_index('filename', inplace=True)
	else:
		raise Exception("Unknown input file type: not in txt or csv.")

	# Detect.
	# Unpack sequence of (image filename, windows).
	images_windows = [
		(ix, inputs.iloc[np.where(inputs.index == ix)][COORD_COLS].values)
		for ix in inputs.index.unique()
	]
	
	newOutput = {}
	 
	for image,windows in images_windows:
		print(image)   
		print("Running {} regions through CNN...".format(len(windows)))
		#startDetect = time.time()
		image_detections = []
		for i in range(0,len(windows),50):
			end = i + 50
			if end >= len(windows):
				end = len(windows)
			images_windows_small = [(image,windows[i:i+50])]
			detections_small = detector.detect_windows(images_windows_small)
			image_detections += detections_small
		#endDetect = time.time()
		#print("Detection time: %d"%(endDetect-startDetect))

		#print("Running non-maximum suppression...")
		#startSuppression = time.time()
		#for each class, run non-maximum suppression
		for classNum in range(NUM_OUTPUT):
			allWindows = []
			for dataItem in image_detections:
				#score = probability of bounding box * probability of object in box
				score = dataItem['window'][4]*dataItem['prediction'][classNum]
				#if score > 0.05: #only look at windows scored above a threshold
				window = [dataItem['window'][1],dataItem['window'][0],dataItem['window'][3],dataItem['window'][2],score] #[xmin,ymin,xmax,ymax,score]
				allWindows.append(window)

			if len(allWindows) <= 0: #nothing detected here
				continue

			#maxWindows = nms_detections(np.array(allWindows))

			for bbox in allWindows:
				#if this bounding box already exists in newOutput, just update the appropriate score
				if (image,bbox[0],bbox[1],bbox[2],bbox[3]) in newOutput:
					newOutput[(image,bbox[0],bbox[1],bbox[2],bbox[3])][classNum] = bbox[4]
					continue

				#otherwise, add a new row
				newOutput[(image,bbox[0],bbox[1],bbox[2],bbox[3])] = [0]*NUM_OUTPUT
				newOutput[(image,bbox[0],bbox[1],bbox[2],bbox[3])][classNum] = bbox[4]
		#endSuppression = time.time()
		#print("Suppression time: %d"%(endSuppression-startSuppression))

	print("Processed {} non-suppressed windows in {:.3f} s.".format(len(newOutput),time.time() - t))
	
	# Save results.
	t = time.time()
	if args.output_file.lower().endswith('csv'):
		# csv
		pdOutput = pd.DataFrame(data=newOutput).T
		pdOutput.reset_index(inplace=True)
		pdOutput.to_csv(args.output_file)
	else:
		# h5
		df.to_hdf(args.output_file, 'df', mode='w')
	print("Saved to {} in {:.3f} s.".format(args.output_file,
											time.time() - t))


if __name__ == "__main__":
	import sys
	main(sys.argv)
