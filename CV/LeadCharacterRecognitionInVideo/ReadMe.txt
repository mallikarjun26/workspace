This Project aims:
1. To detect all the faces in the video -> Done using Haar features.
2. To categorize the faces into different clusters which might represent different people -> Feature creation using SIFT. Clustering using k-means.
3. To find the lead characters -> Done thresholding the number of members of clusters.

Usage:
main.o ~/src/opencv/opencv-2.4.9/data/haarcascades ~/Desktop/test.avi

