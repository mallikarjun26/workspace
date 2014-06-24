#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include "lbp.hpp"
#include "histogram.hpp"

using namespace cv;
using namespace std;

int main(int argc, const char *argv[]) {

    int frameNo=0;

    // Check for valid command line arguments, print usage
    // if no arguments were given.
    if(argc != 3){
        cout << "usage: " << argv[0] << " </path/to/haar_cascade> </path/to/video>" << endl;
        cout << "\t </path/to/haar_cascade> -- Path to the Haar Cascade for face detection." << endl;
        cout << "\t </path/to/video> -- Path to the video for face detection." << endl;
        exit(1);
    }

    // Get the path to your CSV:
    //string fn_haar = string(argv[1]);
    //string fn_video = string(argv[2]);
    string fn_haar = string("/home/mallikarjun/src/opencv/opencv-2.4.9/data/haarcascades/haarcascade_frontalface_default.xml");
    string fn_video = string("/home/mallikarjun/Desktop/test2.avi");


    // These vectors hold the images and corresponding labels:
    ///      vector<Mat> images;
    ///      vector<int> labels;

    CascadeClassifier haar_cascade;
    haar_cascade.load(fn_haar);

    VideoCapture myCapt(fn_video.c_str());
    if(!myCapt.isOpened())  // check if we succeeded
        return -1;

    cout << "Video File: " << fn_video.c_str() << endl;

    Mat frame;
    myCapt >> frame;
    Mat original = frame.clone();

    while ((original.channels()==3) || (original.channels()==4) ) {
        //Get frame from the video
        //cout << "Frame No:" << frameNo <<endl;
        Mat gray;
        cvtColor(original, gray, CV_BGR2GRAY);

        //imshow("Debug 0", gray);
        //cvWaitKey(50);
        vector< Rect_<int> > faces;
        haar_cascade.detectMultiScale(gray, faces);

        for(int i = 0; i < faces.size(); i++) {
            Rect face_i = faces[i];
            Mat face = gray(face_i).clone();
            //cout<<"displaying image" << endl;
            //imshow("test", face);
            //cvWaitKey(50);
        }

        frameNo++;

        myCapt >> frame;
        original = frame.clone();
    }

    cout<< "Detecting faces done" << endl;
    return 0;
}
