#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "ExtractFaces.hpp"
#include "BuildListOfSelectedFaces.hpp"
#include "ConstructGraph.hpp"

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
    //extractFaces(videoLocation, outputLocation, haarCascadeXML);

    // Cluster the faces which are similar


    // Pose alignment of the faces


    // Build the list of all selected faces
    //buildListOfSelectedFaces(finalFacesLocation);

    // Find edgeWeights
    //findEdgeWeights(outputLocation);

    // Build the graph out of all the facesBuildListOfSelectedFaces.cpp
    constructGraph(outputLocation);

    // Find the interesting routes to come up with interesting sequences


    // Stage 2: Build a cube out of one particular expression sequence and find how another actor looks if he gives similar sort of sequence of expressions

    waitKey(50);
    return 0;

}



