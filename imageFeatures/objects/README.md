# Object Detection Module

This module generates object detections for each image.

## Contents
- **boundingBoxes_csv.m**: Bounding box extraction script
- **detect.py**: Object detection script
- **imagenet_synset_words.txt**: Manual mapping between WordNet synsets and ImageNet categories
- **Tweaking Object Detection.ipynb**: Clean up object detections (thresholding, etc.)
- **WordNet Domains.ipynb**: Map fine-grained object detections into coarser grained WordNet domains
- **WordNet Supersenses.ipynb**: Map fine-grained object detection into coarser grained WordNet supersenses

## Dependencies
- Matlab
- Piotr's Computer Vision Matlab Toolbox (https://pdollar.github.io/toolbox/)
- Structured Edge Detection Toolbox V3.0 (https://github.com/pdollar/edges)
- Caffe
- Segmentation as Selective Search for Object Recognition (https://github.com/sergeyk/selective_search_ijcv_with_python)
- Jupyter Notebook (http://jupyter.org/)
- Python's Numpy (http://www.numpy.org/)
- Python's Pandas (https://pandas.pydata.org/)
- WordNet Domains (http://wndomains.fbk.eu/)

## Quick Start
First, you need to generate candidate bounding boxes for each image. Go to boundingBoxes_csv.m and adjust the parameters at the top of the script. Then run this script in Matlab. 

Generating EdgeBoxes will potentially have some errors. These are normally output to errors.txt (though you can change this output file in boundingBoxes_csv.m). To fix this, for each error, run:
```
$ convert file.jpg -colorspace RGB file.jpg
```

There are too many edge boxes to send through the detector at once. So split them up:
```
$ split -l 200000 window_file_all.csv
```

After you split the files, there's a couple of things that you need to do before they're reading to send through the detector:
1. Rename each file so that it has a .csv file ending.
2. Make sure that each file has the correct heading (the heading should be correct in the first file; copy it from there).

Now, you need to detect the objects in these bounding boxes. We will do this through Caffe. First, change to the Caffe directory. Copy detect.py to Caffe's folder python/detect.py. Then, run the following command in bash (with the appropriate path references for your setup):
```	
$ python python/detect.py --crop_mode=list --pretrained_model=models/bvlc_reference_caffenet/bvlc_reference_caffenet.caffemodel --model_def=models/bvlc_reference_caffenet/deploy.prototxt --gpu --raw_scale=255 /home/wenlaura/image-processing-pipeline/objects/window_file.csv /home/wenlaura/image-processing-pipeline/objects/output.csv
```

Once the object detection is finished, to clean up the features CSV, use Tweaking Object Detection.ipynb (Change variables at the top of the file).

This gives you all of the fine-grained image features (which are used for classification). For correlation purposes, we used more coarse-grained image features. To generate these, take a look at WordNet Domains.ipynb and WordNet Supersenses.ipynb.

## Timing
2h,36m,57s to calculate bounding boxes for approximately 7765 images

59m,51s to generate object detection predictions for 150,000 bounding boxes

## References
Jia, Y., Shelhamer, E., Donahue, J., Karayev, S., Long, J., Girshick, R., Guadarrama, S., Darrell, T.: Caffe: Convolutional architecture for fast feature embedding. In: Proceedings of the 22nd ACM International Conference on Multimedia. pp. 675–678 (2014)

Krizhevsky, A., Sutskever, I., Hinton, G.E.: ImageNet classification with deep convolutional neural networks. In: Advances in Neural Information Processing Systems. pp. 1097–1105 (2012)
M

Zitnick, C. Lawrence, and Piotr Dollár. "Edge boxes: Locating object proposals from edges." European Conference on Computer Vision. Springer, Cham, 2014.
