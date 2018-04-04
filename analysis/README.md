# Analysis Module

This module contains code for two types of analysis:
1. Use a t-test to determine which attributes are significantly different between men and women.
2. Use significant correlation testing to get correlations between personality traits and attributes.

## Contents
t-Tests.ipynb --------------------- Code for finding significant differences between men and women
Unsignificant Correlations.ipynb -- Get raw correlations (might not be significant) between personality traits and attributes
calculateSignificance.py ---------- Calculate significance of raw correlations calculated using Unsignificant Correlations.ipynb

## Dependencies
- Jupyter Notebook (http://jupyter.org/)
- Python Scipy (https://www.scipy.org/)
- Python Pandas (https://pandas.pydata.org/)

## Quick Start
For t-tests, take a look at t-Tests.ipynb. For correlation testing, first take a look at Unsignificant Correlations.ipynb. Then, once you have generated the raw correlation numbers, use the python script calculateSignificance.py (make sure to adjust variables at the top of the script). This script takes two arguments: a start index (inclusive) and an ende index (exclusive). It will calculate significance for all of the features between the start and the end indices. If you want to process all of the features, make the start index 0 and the end index -1.

## References
Yoder, P.J., Blackford, J.U., Waller, N.G., Kim, G.: Enhancing power while controlling
family-wise error: An illustration of the issues using electrocortical studies. Journal of Clinical and Experimental Neuropsychology 26(3), 320–331 (2004)
