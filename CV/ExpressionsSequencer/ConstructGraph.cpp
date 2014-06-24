#include <iostream>
#include <fstream>
#include <boost/algorithm/string.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>

using namespace std;
using namespace boost;

struct FaceVertex
{
    string faceID;
};

struct SimilarityEdge
{
    float similarityScore;
};

typedef boost::property<boost::edge_weight_t, float> DistanceProperty;
typedef boost::adjacency_list<vecS, vecS, undirectedS, no_property, DistanceProperty> UndirectedGraph;
//typedef boost::adjacency_list<listS, vecS, undirectedS, FaceVertex, SimilarityEdge> UndirectedGraph;

void constructGraph(const string outputLocation){

    string line;
    vector<string> stringVector;
    string edgeWeightsTxtName = outputLocation + "/AllFaces/edgeWeights.txt";
    UndirectedGraph graphHandle;
    DistanceProperty edgeProperty;
    int noOfVertices, vertexA, vertexB;
    float edgeWeight;

    ifstream edgeWeightPtr(edgeWeightsTxtName.c_str());

    if(edgeWeightPtr.is_open()) {
        // Line 1 contains, number of vertices.
        if(getline(edgeWeightPtr, line)) {
            try {
                noOfVertices = lexical_cast<int>(line);
            }
            catch(bad_lexical_cast const&) {
                cout << "Error: input string was not valid" << endl;
            }
        }

        while(getline(edgeWeightPtr, line)) {
            cout << line << endl;
            split(stringVector, line, boost::is_any_of("-="));
            // Adding edges and weights
            try {
                vertexA = lexical_cast<int>(stringVector[0].c_str());
                vertexB = lexical_cast<int>(stringVector[1].c_str());
                edgeWeight = lexical_cast<float>(stringVector[2].c_str());

            }
            catch(bad_lexical_cast const&) {
                cout << "Error: input string was not valid" << endl;
            }

            boost::graph_traits<UndirectedGraph>::vertex_descriptor u, v;
            u = vertex(vertexA, graphHandle);
            v = vertex(vertexB, graphHandle);
            edgeProperty.m_value = edgeWeight;
            add_edge(u, v, edgeProperty, graphHandle);

        }
    }
    else {
        cout << "Couldn't open " << edgeWeightsTxtName << endl;
    }

}
