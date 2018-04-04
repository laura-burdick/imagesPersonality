#!/bin/sh

# Author: Laura Wendlandt (wenlaura@umich.edu)
#
# Preprocess images (start with a copy of the original data):
# Remove all non-images,
# Convert all images to jpg files,
# Resize to 700x700 (necessary for face detection)

# Change to work with your setup
#########################################################################
#WARNING: Need slash on the end of file path for script to work properly
PATH="/home/wenlaura/image-processing-pipeline/sample_obj/"
#########################################################################

index=0
for userDir in $PATH*/;
do
	echo $userDir
	cd $userDir

	for file in *;
	do
		extension="${file##*.}"
		filename="${file%.*}"

		#remove all files not starting in "file"
		if [ ${filename:0:4} != "file" ];
		then
			/bin/rm $file
		fi
	done

	for file in *;
	do
		extension="${file##*.}"
		filename="${file%.*}"

		#Convert all images to jpg files and resize to 700x700
		/usr/bin/convert $file -resize 700x700\> $filename".jpg" 
	done

	/bin/rm *.dat

	index=$((index+1))
	echo $index
done
