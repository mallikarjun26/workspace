#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "ExtractFaces.hpp"
#include "FilterFaces.hpp"
#include "BuildListOfSelectedFaces.hpp"
#include "ConstructGraph.hpp"

void testing() {
    // Create an IplImage object *image
    Mat src= cvLoadImage("/home/mallikarjun/Documents/CV/ExpressionsSequencer/output/AllFaces/set_1/3669_0.jpg");
    Mat dst;
    Size sizeT(200,200);

    resize(src, dst, sizeT);
    const char* SRC_DISPLAY = "SRC";
    namedWindow(SRC_DISPLAY, CV_WINDOW_AUTOSIZE);
    const char* DST_DISPLAY = "DST";
    namedWindow(SRC_DISPLAY, CV_WINDOW_AUTOSIZE);
    namedWindow(DST_DISPLAY, CV_WINDOW_AUTOSIZE);
    imshow(SRC_DISPLAY, src);
    imshow(DST_DISPLAY, dst);

    cvWaitKey(2000000);

}


int main(int argc, char *argv[])
{

    if(argc != 5) {
        cout << "Not enough parameters" << endl;
        return -1;
    }

    const string videoLocation       = argv[1];
    const string outputLocation      = argv[2];
    const string haarCascadeXML      = argv[3];
    const string finalFacesLocation  = argv[4];

    // Extract faces from the movie
    // extractFaces(videoLocation, outputLocation, haarCascadeXML);

    // Filter faces in a given range of size and make them of same size
    //filterFaces(finalFacesLocation);

    // Pose alignment of the faces


    // Build the list of all selected faces
    //buildListOfSelectedFaces(finalFacesLocation);

    // Build the graph out of all the facesBuildListOfSelectedFaces.cpp
    constructGraph(outputLocation);

    // Find the interesting routes to come up with interesting sequences


    // Stage 2: Build a cube out of one particular expression sequence and find how another actor looks if he gives similar sort of sequence of expressions

    waitKey(50);
    return 0;

}



