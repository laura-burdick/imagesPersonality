% Laura Wendlandt
% wenlaura@umich.edu
%
% Edge boxes code adapted from /home/wenlaura/local/edges/edgeBoxesDemo.m

tic;

% Change to work with your setup
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
addpath(genpath('/home/wenlaura/local/pdollar_toolbox')) %Path to Piotr's Computer Vision Matlab Toolbox
addpath(genpath('/home/wenlaura/local/edges')) %Path to Structured Edge Detection Toolbox V3.0
homeDir = '/home/wenlaura/image-processing-pipeline/objects/errors';
model=load('/home/wenlaura/local/edges/models/forest/modelBsds'); model=model.model; %Path to Edge Boxes model
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

errorID = fopen('errors.txt','w');

dataFolder = dir(homeDir);

%filter out parent and current directories
tf = ismember({dataFolder.name},{'.','..','.htaccess'});
dataFolder(tf) = [];

%Set up edge boxes code
%% load pre-trained edge detection model and set opts (see edgesDemo.m)
model.opts.multiscale=0; model.opts.sharpen=2; model.opts.nThreads=4;
%% set up opts for edgeBoxes (see edgeBoxes.m)
opts = edgeBoxes;
opts.alpha = .65;     % step size of sliding window search
% Aim for IoU of .5 rather than .7
opts.beta  = .55;     % nms threshold for object proposals
opts.minScore = .01;  % min score of boxes to detect
opts.maxBoxes = 2000;%1e4;  % max number of boxes to detect

output = fopen('boundingBoxes_errors.csv','wt');
%output = fopen('/scratch/mihalcea_flux/wenlaura/VOC2012/VOCdevkit/objectDetection/boxes.csv','wt');
%output = fopen('/scratch/mihalcea_flux/wenlaura/boundingBoxes/window_file_fewer.csv','wt')
%output = fopen('window_file_cats_55.csv','wt');
fprintf(output,'filename,xmin,ymin,xmax,ymax,score\n');

index = 0
for i = 1: length(dataFolder)
	currentDir = dataFolder(i).name
	currentFolder = dir([homeDir '/' currentDir]);

	%filter out parent and current directories
	tf = ismember({currentFolder.name},{'.','..','.htaccess'});
	currentFolder(tf) = [];

	for j = 1: length(currentFolder)
		name = currentFolder(j).name;
		if 1
			filename = [homeDir '/' currentDir '/' name]

			try
				im=imread(filename);
				[rows cols channels] = size(im);
				if channels == 1 %if grayscale, convert to rgb and use that
					im = cat(3, im, im, im);
				end
			catch ME
				disp('error caught')
				ME.identifier
				filename
				fprintf(errorID,'%s\n',filename)
				fprintf(errorID,'Something is wrong loading this image; possibly that JPEG images with CMYK colorspaces are not currently supported in Matlab\n\n')
				continue
			end

			bbs=edgeBoxes(im,model,opts);
			numBoxes = size(bbs,1)
			for j = 1: numBoxes
				x = bbs(j,1);
				y = bbs(j,2);
				width = bbs(j,3);
				height = bbs(j,4);
				score = bbs(j,5);
		
				%only look at boxes greater than 3% of the image area
				if width*height > 0.03*rows*cols
					fprintf(output,'%s,%d,%d,%d,%d,%d\n',filename,x,y,x+width,y+height,score);
				end
			end

			index = index + 1
		end
	end
end

fclose(errorID);
fclose(output);

timeSpent = toc
