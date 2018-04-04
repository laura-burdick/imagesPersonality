% Laura Wendlandt
% wenlaura@umich.edu

% filePath can be either a single image or a directory
function calculateColors(filePath)

tic;

load('w2c.mat')

fileID = fopen('features_color.csv','w');
errorID = fopen('errors.txt','w');

images = {};

if isdir(filePath)
	dataFolder = dir(filePath);
	
	for i = 1: length(dataFolder);
		if strcmp(dataFolder(i).name,'.')==0 && strcmp(dataFolder(i).name,'..')==0
			images{end+1} = [filePath '/' dataFolder(i).name];
		end
	end
else %filePath is a single image
	images{end+1} = filePath;
end

fprintf(fileID,'file, black, blue, brown, grey, green, orange, pink, purple, red, white, yellow\n');

index = 0
for i = 1: length(images)
	[pathstr,name,ext] = fileparts(images{i}); 
	name

	try
		im=double(imread(images{i}));
	catch ME
		ME.identifier
		images{i}
		fprintf(errorID,'%s\n',images{i})
		fprintf(errorID,'Something is wrong loading this image; possibly that JPEG images with CMYK colorspaces are not currently supported in Matlab\n\n')
		continue
	end

	if size(im,3) ~= 3 %not an rgb image
		im = cat(3,im,im,im);
	end
	out=im2c(im,w2c,0);
	fprintf(fileID,'%s, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\n',name,sum(out(:)==1),sum(out(:)==2),sum(out(:)==3),sum(out(:)==4),sum(out(:)==5),sum(out(:)==6),sum(out(:)==7),sum(out(:)==8),sum(out(:)==9),sum(out(:)==10),sum(out(:)==11));
	index = index + 1
end

fclose(fileID);
fclose(errorID);

timeSpent = toc
