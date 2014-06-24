#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

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

    CvCapture *myCapt;
    myCapt = cvCreateFileCapture(fn_video.c_str());

    cout << "Video File: " << fn_video.c_str() << endl;

    if(!myCapt)
    {
        printf("!!! cvCapture failed (file not found?)\n");
        return -1;
    }

    while (1) {
        //Get frame from the video
        cout << "Frame No:" << frameNo <<endl;
        cvGrabFrame(myCapt);
        Mat frame;
        frame = cvQueryFrame(myCapt);
        //frame = cvRetrieveFrame(myCapt);
        Mat original = frame.clone();

        Mat gray;
        cvtColor(original, gray, CV_BGR2GRAY);
        vector< Rect_<int> > faces;
        haar_cascade.detectMultiScale(gray, faces);

        for(int i = 0; i < faces.size(); i++) {

            Rect face_i = faces[i];
            Mat face = gray(face_i);
            cout<<"displaying image" << endl;
            imshow("test", face);
            waitKey(0);
            //imshow(face)
        }
        frameNo++;
    }

    cvReleaseCapture(&myCapt);


    return 0;
}
