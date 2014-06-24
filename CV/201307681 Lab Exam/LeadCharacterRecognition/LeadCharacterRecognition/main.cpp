#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <boost/lexical_cast.hpp>
#include <sys/dir.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>

#include "LBP.hpp"
#include "/home/mallikarjun/src/opencv/opencv-2.4.9/modules/flann/include/opencv2/flann/flann.hpp"


#define FEATURE_DIMENSION 256
#define CLUSTERCOUNT 5

using namespace cv;
using namespace std;
using namespace lbp;

float normDistance (float a[], float b[] ) {
    float dist=0.0;
    for(int i=0; i<FEATURE_DIMENSION; i++) {
        //cout << "a[" << i << "]:" << a[i] << " b[" << i << "]:" << b[i] << endl;
        dist = dist + ( (a[i] - b[i]) * (a[i] - b[i]) );
    }
    //cout << "dist:" << dist << endl;
    dist = sqrt(dist);
    //cout << "dist:" << dist << endl;
    return dist;
}

int main(int argc, const char *argv[]) {


    cout << "LeadCharacterRecognition " << endl;
    int frameNo=0, noOfFaces=0;
    int currentMatrixRows = 1000;
    Mat points;

    map<int, vector<float> > featureVector;
    map<int, vector<int> >   membersInCluster;

    int leadFaceClusters[2], leadFaceNo[2];

    int noOfMembersInCluster[CLUSTERCOUNT];
    Mat labels, centers;

    string outputLocation = "../../images";
    string str;

    string folderName =  "../../images/faces";
    mkdir(folderName.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    for(int i=0; i<CLUSTERCOUNT; i++) {
        folderName =  outputLocation + "/cluster" + boost::lexical_cast<std::string>(i);
        mkdir(folderName.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }

    cout << "LeadCharacterRecognition 1" << endl;

    // Check for valid command line arguments, print usage
    // if no arguments were given.
    if(argc != 3){
        cout << "usage: " << argv[0] << " </path/to/haar_cascade> </path/to/video>" << endl;
        cout << "\t </path/to/haar_cascade> -- Path to the Haar Cascade for face detection." << endl;
        cout << "\t </path/to/video> -- Path to the video for face detection." << endl;
        exit(1);
    }

    // Get the path to your CSV:
    string fn_haar = string(argv[1]);
    string fn_video = string(argv[2]);
//    string fn_haar = string("/home/mallikarjun/src/opencv/opencv-2.4.9/data/haarcascades/haarcascade_frontalface_default.xml");
//    string fn_video = string("/home/mallikarjun/Documents/CV/Lab Exam/Video/test3.avi");

    CascadeClassifier haar_cascade;
    haar_cascade.load(fn_haar);

    cout << "LeadCharacterRecognition 2" << endl;

    VideoCapture myCapt(fn_video.c_str());
    if(!myCapt.isOpened())  // check if we succeeded
        return -1;

    cout << "Video File: " << fn_video.c_str() << endl;

    Mat frame;
    myCapt >> frame;
    Mat original = frame.clone();


    // Face Detection
    while ((original.channels()==3) || (original.channels()==4) ) {
        //Get frame from the video
        //cout << "Frame No:" << frameNo <<endl;
        Mat gray;
        cvtColor(original, gray, CV_BGR2GRAY);

        imshow("Debug 0", gray);
        cvWaitKey(50);
        vector< Rect_<int> > faces;
        haar_cascade.detectMultiScale(gray, faces);

        for(int i = 0; i < faces.size(); i++) {

            Rect face_i = faces[i];
            Mat face = gray(face_i).clone();
            //cout<<"displaying image" << endl;
            imshow("test", face);
            cvWaitKey(50);

            string fileName =  outputLocation + "/faces/" + boost::lexical_cast<std::string>(noOfFaces) + ".jpg";
            imwrite( fileName, face );

            face.convertTo( face, CV_64F );
            //cout << "image w/h = " << face.rows << "/" << face.cols << " (" << face.rows*face.cols << ")" << endl;

            // Create an LBP instance of type HF using 8 support points
            LBP lbp( 8, LBP_MAPPING_NONE );

            // Calculate the descriptor
            lbp.calcLBP( face );

            // Calculate Fourier tranformed histogram
            vector<double> hist = lbp.calcHist().getHist( false );

            // Print out the histogram values
            double sum = 0;
            //cout << "hist = [";
            for( int j = 0; j < hist.size(); j++ ) {
                //cout << hist[j] << ", ";
                //sum += hist[j];
                //points.at<float>(noOfFaces,j) = hist[j];
                featureVector[noOfFaces].push_back(hist[j]);
            }
            //cout << "]; " << endl;
            //cout << "hist sum=" << sum << endl;
            noOfFaces++;
        }

        frameNo++;
        myCapt >> frame;
        original = frame.clone();
    }


    cout<<"Number of Faces Detected" << noOfFaces << "\n";
    cout<<"featureVector.size():"<< featureVector.size() << "\n";
    cout<<"featureVector[0][1]:"<< featureVector[0][1] << "\n";

    // feature vector creation
    points.create(featureVector.size(), FEATURE_DIMENSION, CV_32F);

    for(int i=0; i<featureVector.size(); i++) {
        for(int j=0; j<FEATURE_DIMENSION; j++) {
            points.at<float>(i,j) = featureVector[i][j];
        }
    }

    cout << "DEBUG AAA 0" << endl;

    //Clustering the faces.
    kmeans(points, CLUSTERCOUNT, labels, TermCriteria( CV_TERMCRIT_EPS+CV_TERMCRIT_ITER, 50, 1.0), 8, KMEANS_PP_CENTERS, centers);

    for( int i = 0; i < noOfFaces; i++ )
    {
        int clusterIdx = labels.at<int>(i);
        //cout << "Face: " << i << " belongs to label: " << clusterIdx << endl;
        noOfMembersInCluster[clusterIdx]++;
        membersInCluster[clusterIdx].push_back(i);

        String filename_t = outputLocation + "/faces/" + boost::lexical_cast<std::string>(i) + ".jpg";
        Mat face_t = imread( filename_t, CV_LOAD_IMAGE_COLOR);
        filename_t = outputLocation + "/cluster" + boost::lexical_cast<std::string>(clusterIdx) + "/" + boost::lexical_cast<std::string>(i) + ".jpg";
        imwrite(filename_t, face_t);
    }

    cout << "DEBUG AAA" << endl;

    // Finding the lead characters cluster
    leadFaceClusters[1] = 0;
    for( int i=0; i<CLUSTERCOUNT; i++) {
         if(noOfMembersInCluster[i] < noOfMembersInCluster[leadFaceClusters[1]]) {
            leadFaceClusters[1] = i;
         }
    }
    leadFaceClusters[0] = 0;
    int temp_sec = 0;
    for( int i=0; i<CLUSTERCOUNT; i++) {
        if(noOfMembersInCluster[i] > noOfMembersInCluster[leadFaceClusters[0]]) {
            leadFaceClusters[0] = i;

        }
        if( (noOfMembersInCluster[i] < noOfMembersInCluster[leadFaceClusters[0]]) && (noOfMembersInCluster[i] >= noOfMembersInCluster[leadFaceClusters[1]]) ) {
            leadFaceClusters[1] = i;
        }
    }




    cout << "DEBUG BBB" << endl;

    // leadFaceNo
    // membersInCluster
    // leadFaceClusters
    // noOfMembersInCluster
//
//    for(int i=0; i<CLUSTERCOUNT; i++) {
//        cout << " Center A: ";
//        for(int j=0; j< FEATURE_DIMENSION; j++) {
//            cout << centers.at<float>(j,i) << " ";
//        }
//        cout << endl;
//    }

    // Finding the best match lead in the cluster
    for( int i=0; i<2; i++) {
        float dist = std::numeric_limits<float>::max();
        leadFaceNo[i] = membersInCluster[leadFaceClusters[i]][0];
        for( int j=1; j<noOfMembersInCluster[leadFaceClusters[i]]; j++) {
            float A[FEATURE_DIMENSION];
            float B[FEATURE_DIMENSION];
            int i_temp = membersInCluster[leadFaceClusters[i]][j];
            for(int k=0; k<FEATURE_DIMENSION; k++) {
                A[k] = centers.at<float>(leadFaceClusters[i],k);
                B[k] = featureVector[i_temp][k];
            }

            //cout << "normDistance bw cluster:" << leadFaceClusters[i] << "and face No" << i_temp << "is" << normDistance(A,B) << endl;
            if( dist > normDistance(A, B)) {
                leadFaceNo[i] =  membersInCluster[leadFaceClusters[i]][j];
                dist = normDistance(A, B);
            }
        }
    }

    cout << "DEBUG CCC" << endl;

    cout << "First Lead Cluster Number:" << leadFaceClusters[0] << " First Lead Number:" << leadFaceNo[0] << endl;
    cout << "Second Lead Cluster Number:" << leadFaceClusters[1] << " First Lead Number:" << leadFaceNo[1] << endl;

    String filename_t = outputLocation + "/faces/" + boost::lexical_cast<std::string>(leadFaceNo[0]) + ".jpg";
    cout << "filename 1 " << filename_t << endl;
    Mat face_t = imread( filename_t);
    filename_t = outputLocation + "/FirstLead.jpg";
    imwrite(filename_t, face_t);
    imshow("First Lead Character", face_t);

    filename_t = outputLocation + "/faces/" + boost::lexical_cast<std::string>(leadFaceNo[1]) + ".jpg";
    cout << "filename 2 " << filename_t << endl;
    Mat face_t1 = imread( filename_t);
    filename_t = outputLocation + "/SecondLead.jpg";
    imwrite(filename_t, face_t1);
    imshow("Second Lead Character", face_t1);

    cvWaitKey(0);

    cout << "Centers size:" << centers.size() << endl;
    cout << "original size:" << original.size() << endl;
    cout << "Detecting faces done" << endl;
    return 0;
}




