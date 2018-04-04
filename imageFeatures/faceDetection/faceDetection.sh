#!/bin/sh

#Author: Laura Wendlandt (wenlaura@umich.edu)
#NOTE: must be run on gpu for objects_detection command to work
#NOTE: can only have image files in the directories or this won't work
#
#Acceptable error messages:
#
#/bin/mv: cannot stat `2016_05*': No such file or directory
#
#[libprotobuf ERROR google/protobuf/message_lite.cc:123] Can't parse message of type "doppia_protobuf.DetectorModel" because it is missing required fields: detector_type
#
#libprotobuf ERROR google/protobuf/text_format.cc:245] Error parsing text-format doppia_protobuf.DetectorModel: 2:2: Message type "doppia_protobuf.DetectorModel" has no field named "E".

# Change to work with your setup
################################################################################################
PATH="/scratch/mihalcea_flux/wenlaura/UT_Images_2016/data_preprocessed/" #path to preprocessed data
OBJECTS_DETECTION_PATH="/home/wenlaura/local/doppia/src/applications/objects_detection" #path to doppia's object detection code
OLD_RECORDINGS_PATH="/scratch/mihalcea_flux/wenlaura/doppia_oldRecordings/" #store old recordings here
OBJECTS_DETECTION_TOOL="/home/wenlaura/local/doppia/tools/objects_detection" #path to doppia's object detection tool
PYTHON_PATH="/sw/lsa/centos7/python-anaconda2/created-20170424/bin/python"
CURRENT_DATE="2018_01*" #change to current year and month (how doppia organizes output)
################################################################################################

run=0
for userDir in $PATH*/;
do
	echo $userDir

	if [ $run -eq 0 ]
	then
		echo 'skip'
		continue
	fi

	#remove all non-jpg images from folders
	cd $userDir
	for file in *;
	do
		extension="${file##*.}"
		filename="${file%.*}"

		if [ $extension == "jpg" ];
		then
			: #no-op
		else
			/bin/rm $file
		fi
	done

	#write config file
	cd $OBJECTS_DETECTION_PATH
	/bin/rm -f config.ini
	echo "save_detections = false" >> config.ini
	echo "process_folder = "$userDir >> config.ini
	echo "additional_border = 200" >> config.ini
	echo "[objects_detector]" >> config.ini
	echo "method = gpu_channels" >> config.ini
	echo "model = ../../../data/trained_models/face_detection/headhunter.proto.bin" >> config.ini
	echo "score_threshold = 0.05" >> config.ini #0.05 recommended for visualization
	echo "x_stride = 0.00001" >> config.ini
	echo "y_stride = 0.00001" >> config.ini
	echo "non_maximal_suppression_method = greedy" >> config.ini
	echo "minimal_overlap_threshold=0.3" >> config.ini
	echo "min_scale = 0.325" >> config.ini
	echo "max_scale = 6" >> config.ini
	echo "num_scales = 80" >> config.ini
	echo "min_ratio = 1" >> config.ini
	echo "max_ratio = 1" >> config.ini
	echo "num_ratios = 1" >> config.ini
	echo "[video_input]" >> config.ini
	echo "source = directory" >> config.ini
	echo "[preprocess]" >> config.ini
	echo "unbayer = false" >> config.ini
	echo "undistort = false" >> config.ini
	echo "rectify = true" >> config.ini
	echo "smooth = false" >> config.ini
	echo "residual = false" >> config.ini
	echo "specular = false" >> config.ini

	/bin/mv $CURRENT_DATE $OLD_RECORDINGS_PATH
	
	basename=$(/bin/basename $userDir)
	echo "processing "$basename

	#run program
	./objects_detection -c config.ini --gui.disable true > output/output_$basename.txt

	recordingsFile=""
	for file in $(/bin/ls | /bin/egrep $CURRENT_DATE);
	do
		recordingsFile=$file
	done
	detectionsFile=$OBJECTS_DETECTION_PATH$recordingsFile"/detections.data_sequence"
	echo $detectionsFile

	#count detections for each image in the directory (5 images total)
	cd $OBJECTS_DETECTION_TOOL
	#If you get the error "python: command not found", make sure that the following path is correct (module load python-anaconda2, then type "which python" to determine the correct path)
	$PYTHON_PATH detections_count.py -i $detectionsFile >> results.txt

	cd $OBJECTS_DETECTION_PATH

	#move old recordings to separate folder
	/bin/mv $CURRENT_DATE $OLD_RECORDINGS_PATH

done
