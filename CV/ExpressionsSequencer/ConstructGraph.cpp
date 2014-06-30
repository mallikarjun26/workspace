#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace boost;
using namespace cv;

int findNextVertex(int currentVertex, vector< list < pair<int, float> > > &adjacencyList, int &noOfVerticesLeft) {

    list< pair<int, float> >::iterator it=adjacencyList[currentVertex].begin();

    int nextVertex;
    float leastDist;

    while(adjacencyList[(*it).first].size()==0) {
        it++;
    }
    nextVertex   = (*it).first;
    leastDist  = (*it).second;

    it=adjacencyList[currentVertex].begin();
    for (; it!=adjacencyList[currentVertex].end(); ++it) {
        if(((*it).second <= leastDist) && (adjacencyList[(*it).first].size() > 0) ){
            nextVertex  = (*it).first;
            leastDist   = (*it).second;
        }
    }

    adjacencyList[currentVertex].clear();
    noOfVerticesLeft--;

    return(nextVertex);
}

void constructGraph(const string outputLocation){

    string line;
    vector<string> stringVector;
    string edgeWeightsTxtName = outputLocation + "/AllFaces/edgeWeights.txt";

    int                                                 noOfVertices, vertexA, vertexB;
    float                                               edgeWeight;
    vector< list < pair<int, float> > > adjacencyList;
    int noOfVerticesLeft=0;

    ifstream edgeWeightPtr(edgeWeightsTxtName.c_str());


    // Open the file to read all the edge weights.
    if(edgeWeightPtr.is_open()) {

        // Line 1 contains, number of vertices.
        if(getline(edgeWeightPtr, line)) {
            try {
                noOfVertices      = lexical_cast<int>(line);
                noOfVerticesLeft  = noOfVertices;
            }
            catch(bad_lexical_cast const&) {
                cout << "Error: input string was not valid" << endl;
            }
        }

        list< pair<int, float> > val;
        for(int i=0; i<noOfVertices; i++) {
            adjacencyList.push_back( val );
        }

        // Read each edge weights.
        while(getline(edgeWeightPtr, line)) {
            //cout << line << endl;
            split(stringVector, line, boost::is_any_of("-="));
            // Adding edges and weights
            try {
                //cout << "Debug 00:: " << stringVector[0].c_str() << " " << stringVector[1].c_str() << " " << stringVector[2].c_str() << endl;

                vertexA = lexical_cast<int>(stringVector[0].c_str());
                vertexB = lexical_cast<int>(stringVector[1].c_str());
                edgeWeight = lexical_cast<float>(stringVector[2].c_str());
            }
            catch(bad_lexical_cast const&) {
                cout << "Error: input string was not valid" << endl;
            }

            // Fill in the adjacency list
            //cout << "Debug 11:: Maybe here ::" << vertexA << " " << vertexB << " " << edgeWeight << "adjacencyList size:" << adjacencyList.size() << endl;
            adjacencyList[vertexA].push_back(make_pair(vertexB, edgeWeight));
            //cout << "Debug 22:: Maybe here" << endl;
            adjacencyList[vertexB].push_back(make_pair(vertexA, edgeWeight));
        }

    }
    else {
        cout << "Couldn't open " << edgeWeightsTxtName << endl;
        return;
    }

    // Traverse the graph
    list<int> traverseList;
    int currentVertex = 1, endVertex = 80, nextVertex;

    traverseList.push_back(currentVertex);

    nextVertex = findNextVertex(currentVertex, adjacencyList, noOfVerticesLeft);
    cout << "Debug 44:: Next Vertex" << nextVertex << "adjacencyList size:" << adjacencyList.size() << endl;

    int debugWhile = 0;
    while((noOfVerticesLeft!=1) && (nextVertex != endVertex)){
        cout << "Debug 55:: Inside while. Next Vertex:: " << nextVertex << " VerticesLeft:: " << noOfVerticesLeft << " debugWhile::" << debugWhile << endl;
        traverseList.push_back(nextVertex);
        currentVertex = nextVertex;
        nextVertex = findNextVertex(currentVertex, adjacencyList, noOfVerticesLeft);
        debugWhile++;
    }

    // Print the traverse route
    cout << "Final traversal of Vertices and size" << traverseList.size() << endl;
    for(list<int>::iterator it=traverseList.begin(); it!=traverseList.end(); it++) {
        cout << *it << " - ";
    }
    cout << endl;


    // Display the video
    cout << "Expression animation" << endl;
    string listOfFacesFileName = outputLocation + "/AllFaces/ListOfFaces.txt";
    ifstream listOfFacesFileNameHandle(listOfFacesFileName.c_str());
    vector<string> faceMap;

    // Collect the mapping
    cout << "Collecting the mapping" << endl;
    if(listOfFacesFileNameHandle.is_open()) {
        while(getline(listOfFacesFileNameHandle, line)) {
            split(stringVector, line, boost::is_any_of(" "));
            //cout << "DEBUG 66:: stringVector[0]=" << stringVector[0] << endl;
            faceMap.push_back(stringVector[0]);
        }
    }

    Mat faceMat, prevMat, midMat;
    const char* EXPRESSION_DISPLAY = "Expressions";
    namedWindow(EXPRESSION_DISPLAY, CV_WINDOW_AUTOSIZE);

    // Display the traversed faces and make a video of the same

    Size sizeT(200, 200);
    const string NAME = "Animation.avi";
    cout << "DEBUG 11: " << NAME << endl;

    VideoWriter outputVideo;
    //outputVideo.open(  , -1, 20, sizeT, true);
    outputVideo.open("/home/mallikarjun/Desktop/test.avi", CV_FOURCC('D','I','V','X'), 5, Size (200, 200), true );
    if (!outputVideo.isOpened())
    {
        perror("Could not open the output video for write");
    }

/*    Size sizeT(200, 200);
    CvVideoWriter *writer = cvCreateVideoWriter(
            "data4.avi",
            CV_FOURCC('M','J','P','G'),
            30,
            sizeT);

    cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE);
    cvMoveWindow("mainWin", 200, 200);
*/

    bool firstTime_bool = true;
    cout << "Displaying the traversed faces" << endl;
    for(list<int>::iterator it=traverseList.begin(); it!=traverseList.end(); it++) {
        int faceNumber = *it;
        //cout << "DEBUG 88:: faceMap[i]=" << faceMap[faceNumber] << endl;
        string strTemp = outputLocation + "/AllFaces/Sample Set/" +  faceMap[faceNumber];
        //cout << "DEBUG 77:: strTemp=" << strTemp << endl;
        //IplImage* img=cvLoadImage(strTemp.c_str());
        faceMat = imread(strTemp.c_str(), CV_LOAD_IMAGE_COLOR);
        if(!firstTime_bool){
            addWeighted(prevMat, 0.5, faceMat, 0.5, 0, midMat, -1);
            //putText(midMat, "Bridge Image", cvPoint(30,30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);
            outputVideo << midMat;
            putText(faceMat, faceMap[faceNumber].c_str(), cvPoint(30,30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);
            outputVideo << faceMat;
        }
        else{
            putText(faceMat, faceMap[faceNumber].c_str(), cvPoint(30,30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);
            outputVideo << faceMat;
            firstTime_bool = false;
        }
        prevMat = faceMat.clone();

        //cvShowImage("mainWin", img );
        //cvWriteFrame(writer,img);
        imshow(EXPRESSION_DISPLAY, faceMat);
        cvWaitKey(10);
    }
    //cvReleaseVideoWriter(&writer);
}


