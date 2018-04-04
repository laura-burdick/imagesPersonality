from scipy.stats import pearsonr
import pandas as pd
from random import shuffle
import numpy as np
import time
import math
import sys

# Input formats
# 
# personalityFeaturesFile is assumed to have ground truth personality data for each user
# Specifically, this code looks for the columns 'bfi_o' (Openness), 'bfi_c' (Conscientiousness),
# 'bfi_e' (Extraversion), 'bfi_a' (Agreeableness), and 'bfi_n' (Neuroticism), 'id' (user id)
#
# imageFeaturesFile is assumed the have various features for each user
# Specifically, this code looks for the column 'userid' (user id)

#ADJUST THESE VARIABLES
correlationsRoot = "../data2015/correlations/" #Need slash at end
pearsonValuesFile = correlationsRoot + "features_objects_wordnet_domains_pearsonValues.csv" #raw pearson values for these features
imageFeaturesFile = correlationsRoot + "../image_features/features_objects_wordnet_domains.csv" #features file
finalPearsonValuesFile = "features_objects_wordnet_domains_pearsonValues_significant" #where to save the final results
personalityFeaturesFile = correlationsRoot + "../data_analytics_cleaned.csv" #ground truth data

#POSSIBLY EDIT imageFeaturesLabels BELOW

lenArgs = len(sys.argv)
if lenArgs < 3:
    print("This program takes two arguments: a start index (inclusive) and an end index (exclusive). It will process all of the image features between the start and the end indices. If you want to process all of the features, make the start index 0 and the end index -1.")
    sys.exit(2)

args = sys.argv
startIndex = int(args[1])
endIndex = int(args[2])

pearsonValues = pd.read_csv(pearsonValuesFile)
imageFeatures = pd.read_csv(imageFeaturesFile)
imageFeatures = imageFeatures.drop("Unnamed: 0",axis=1)
personalityFeatures = pd.read_csv(personalityFeaturesFile)
personalityFeatures = personalityFeatures.drop("Unnamed: 0",axis=1)
personalityFeatures = personalityFeatures[['userid','bfi_e','bfi_a','bfi_c','bfi_o','bfi_n']]
imageAndPersonalityFeatures = imageFeatures.merge(personalityFeatures,left_on='id',right_on='userid')

#The features in question
imageFeaturesLabels = [i for i in imageAndPersonalityFeatures if i[:4] == 'all_']
personalityFeaturesLabels = ['bfi_e','bfi_a','bfi_c','bfi_n','bfi_o']

#Drop unnecessary values
imageFeatures = imageFeatures.drop(imageFeaturesLabels[:startIndex],axis=1)
if endIndex >= 0:
    imageFeatures = imageFeatures.drop(imageFeaturesLabels[endIndex:],axis=1)
    imageFeaturesLabels = imageFeaturesLabels[startIndex:endIndex]
else:
    imageFeaturesLabels = imageFeaturesLabels[startIndex:]

#create new table to store new pearson values in (some are zeroed out)
newPearsonValues = pd.DataFrame([],columns=imageFeaturesLabels,index=personalityFeaturesLabels)

numShuffles = 10000
rValues = np.zeros((len(personalityFeaturesLabels),len(imageFeaturesLabels),numShuffles))

start = time.time()
for i in range(numShuffles):
    if(i % 1 == 0):
        print(i)
        end = time.time()
        print("elapsed",end-start)
        start = time.time()
    shuffled = imageAndPersonalityFeatures.apply(np.random.permutation,axis=0)
    for (personalityIndex,personalityFeature) in enumerate(personalityFeaturesLabels):
        for (imageIndex,imageFeature) in enumerate(imageFeaturesLabels):
            clean = shuffled[[personalityFeature,imageFeature]].dropna()
            pearson, p = pearsonr(clean[[personalityFeature]],clean[[imageFeature]])
            rValues[personalityIndex][imageIndex][i] = pearson
    end = time.time()

lowerBound = math.floor(numShuffles*.025) #2.5%
upperBound = numShuffles - math.floor(numShuffles*.025) #97.5%
if lowerBound < 0:
    lowerBound = 0
if upperBound >= numShuffles:
    upperBound = numShuffles - 1

lowerBound = math.floor(numShuffles*.025) #2.5%
upperBound = numShuffles - math.floor(numShuffles*.025) #97.5%
if upperBound >= numShuffles:
    upperBound = numShuffles - 1

for (personalityIndex,personalityFeature) in enumerate(personalityFeaturesLabels):
    for (imageIndex,imageFeature) in enumerate(imageFeaturesLabels):
        print(personalityFeature,imageFeature)
        rValues[personalityIndex][imageIndex].sort()
        lowerValue = rValues[personalityIndex][imageIndex][lowerBound]
        upperValue = rValues[personalityIndex][imageIndex][upperBound]
        current = list(pearsonValues[pearsonValues['Unnamed: 0']==personalityFeature][imageFeature])
        print("lower: %3f, upper: %3f" % (lowerValue, upperValue))
        if current[0] < lowerValue or current[0] > upperValue: #valid pearson
            print("%3f is statistically significant" % current[0])
            newPearsonValues.loc[personalityFeature,imageFeature] = current[0]
        else: #not statistically significant
            print("%3f is not statistically significant" % current[0])
            newPearsonValues.loc[personalityFeature,imageFeature] = 0.0
    
    if endIndex >= 0:
        newPearsonValues.to_csv(correlationsRoot + finalPearsonValuesFile + '_'+str(startIndex)+'_'+str(endIndex)+'.csv')
    else:
        newPearsonValues.to_csv(correlationsRoot + finalPearsonValuesFile + '.csv')

if endIndex >= 0:
    newPearsonValues.to_csv(correlationsRoot + finalPearsonValuesFile + '_'+str(startIndex)+'_'+str(endIndex)+'.csv')
else:
    newPearsonValues.to_csv(correlationsRoot + finalPearsonValuesFile + '.csv')
